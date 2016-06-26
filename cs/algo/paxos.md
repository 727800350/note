Paxos 分布式一致性协议
# intro
[paxos入门介绍](http://www.tudou.com/programs/view/e8zM8dAL6hM)

在Lamport 的The Part-Time Parliament 论文的[解说中](http://research.microsoft.com/en-us/um/people/lamport/pubs/pubs.html#lamport-paxos),
"at the heart of the algorithm is a three-phase consensus protocol", 而这个视频的讲述中, 说是一个两阶段的.

Byzantine failures

拜占庭将军问题是一个协议问题,拜占庭帝国军队的将军们必须全体一致的决定是否攻击某一支敌军. 问题是这些将军在地理上是分隔开来的,并且将军中存在叛徒.

叛徒可以任意行动以达到以下目标:
欺骗某些将军采取进攻行动,
促成一个不是所有将军都同意的决定,如当将军们不希望进攻时促成进攻行动,
或者迷惑某些将军,使他们无法做出决定.
如果叛徒达到了这些目的之一,则任何攻击行动的结果都是注定要失败的,只有完全达成一致的努力才能获得胜利.

拜占庭假设是对现实世界的模型化,由于硬件错误,网络拥塞或断开以及遭到恶意攻击,计算机和网络可能出现不可预料的行为.
拜占庭容错协议必须处理这些失效,并且这些协议还要满足所要解决的问题要求的规范.这些算法通常以其弹性t作为特征,t表示算法可以应付的错误进程数.

很多经典算法问题只有在t<n/3时才有解,如拜占庭将军问题,其中n是系统中进程的总数.

paxos 究竟在解决什么问题?

- paxos 用来确定一个不可变变量的取值
	- 取值可以是任意二进制数据
	- 一旦确定将不在更改, 并且可以被获取到(不可变性, 可读取性)

paxos 如何在分布式存储系统中应用?

- 数据本身可变, 采用多副本进行存储
- 多个副本的更新操作序列[Op1, Op2, ..., Opn] 是相同的, 不变的.
- 用paxos一次来确定不可变变量Opi的取值(即第i个操作是什么)
- 每确定完Opi之后, 让各个数据副本来执行Opi, 以此类推.
- Google 的Chubby, Megastore 和 Spanner 都采用了Paxos 来对数据副本的更新序列达成一致.

paxos 算法的核心思想是什么?

	- 第一阶段在做什么?
	- 第二阶段在做什么?

Paxos 希望解决的一致性问题

1. 设计一个系统, 来存储名称为var 的变量
	- 系统内部由多个Acceptor 组成, 负责存储和管理var变量
	- 外部有多个proposer 机器任意并发调用API, 向系统提交不同的var取值
	- var 的取值可以是任意的二进制数据
	- 系统对外的API库接口为: proposer(var, V) => <ok, f> or <error>
		- 若var 之前为空, 则var会被成功的更新, f 会等于V
		- 若var 之前已经有值了, 则返回的f 为之前已经确定的值
1. 系统需要保证var的取值满足一致性
	- 如果var 的取值还没有确定, 则var 的取值为null
	- 一旦var 的取值被确定, 则不可被更改. 并且可以一直获取到这个值
1. 系统需要满足容错特性
	- 可以容忍任意proposer机器出现故障
	- 可以容忍半数以下的acceptor故障
1. 为了讲解简单, 暂不考虑
	- 网络分化
	- acceptor故障丢失var的信息

确定一个不可变变量的难点

- 管理多个proposer 的并发执行
- 保证var变量的不可变性
- 容忍任意proposer机器的故障
- 容忍半数以下acceptor机器故障

# 分布式系统的一致性
## 方案1: 基于互斥锁的互斥访问权
### 原理
- 先考虑系统由单个acceptor组成, 通过类似互斥锁的机制, 来管理并发的proposer 运行
- proposer 首先想acceptor申请acceptor的互斥访问权, 再得到互斥访问权之后, 才能请求acceptor接收自己的取值
- acceptor给proposer发放互斥访问权, 谁申请到互斥访问权, 就接收谁提交的取值.
- 让proposer按照互斥访问权的顺序依次访问acceptor
- 一旦acceptor接收了某个proposer的取值, 则认为var取值被确定, 其他proposer不再更改.

### 实现
基于互斥访问权的acceptor的实现

- Acceptor保存变量var 和一个互斥锁lock
- Acceptor::prepare(): 加互斥锁, 给予var的互斥访问权, 并返回var 当前的取值f
- Acceptor::release(): 释放互斥锁, 收回var 的互斥访问权
- Acceptor::accept(var, V): 如果已经加锁, 并且bar没有取值, 则设置var 为V. 并且释放锁.


propose(var, V)的两阶段实现

- 第一截断: 通过Acceptor::prepare() 获取互斥访问权和当前var的取值
	- 如果不能, 说明锁被别人占用, 返回<error>
- 第二阶段: 根据当前var 的取值, 选择执行
	- 如果f 为null, 则通过Acceptor::accept(var, V) 提交数据V
	- 如果f 不为null, 则通过Acceptor::release() 释放访问权, 返回<ok, f>

### 思考
proposer 在释放互斥访问权之前发生故障, 会导致系统陷入死锁.
因此方案1不能容忍任意proposer机器故障.

## 方案2: 抢占式访问权
### 原理
- 引入抢占式访问权
	- acceptor 可以让某个proposer 获取到的访问权失效, 不在接收它的访问.
	- 之后, 可以将访问权发放给其他proposer, 让其他proposer访问acceptor.

- proposer 向acceptor 申请访问权时**指定编号epoch(越大的epoch越新)**, 获取到访问权之后, 才能向acceptor 提交取值.

- acceptor 采用**喜新厌旧**的原则
	- 一旦收到更大的新epoch 的申请, 马上让旧的 epoch 的访问权失效, 不在接受他们提交的取值
	- 然后给新的epoch 发放访问权, 只接收新epoch 提交的取值.

- 新epoch 可以抢占旧epoch, 让旧的epoch 的访问权失效. 旧的epoch 的 proposer 将无法运行, 新的epoch 的proposer 将开始运行.
	- **当一直有新的epoch 让旧的epoch 失效的话, 就会无休无止, 成为活锁**

- 为了保持一致性, 不同epoch 的proposer 之间采用**后者认同前者**的原则
	- 在肯定旧的epoch 无法生成确定性取值时, 新的epoch 会提交自己的value, 不会冲突
	- 一旦旧的epoch 形成确定性取值, 新的epoch可以获取到此取值, 并且会认同此取值, 不会破坏.

### 实现
基于抢占式访问权的acceptor 的实现

- acceptor 保存的状态
	- 当前var 的取值<accepted_epoch, accepted_value>, accepted_value 是在 accepted_epoch 时提交的值
	- 最新发放访问权的epoch(latest_prepared_epoch)

- Acceptor::prepare(epoch):
	- 只接收比 latest_prepared_epoch 更大的epoch, 并给予访问权
	- 记录latest_prepared_epoch = epoch: 返回当前var 的取值

- Acceptor::accept(var, prepared_epoch, V):
	- 验证 latest_prepared_epoch == prepared_epoch
	- 并设置var 的取值<accepted_epoch, accepted_value> = <prepared_epoch, v>


Propose(var, V)的两阶段实现

- 第一阶段: 获取epoch 轮次的访问权和当前var 的取值
	- 简单获取当前的时间戳为epoch, 通过acceptor::prepare(epoch) 获取epoch 轮次的访问权和当前var 的取值
	- 如果不能继续, 返回 <error>
- 第二阶段: 采用**后者认同前者**的原则选定取值, 进行提交
	- 如果var 的取值为空, 则肯定当前没有确定性取值, 则通过Acceptor::accept(var, epoch, V)提交数据V.
		- 成功后返回 <ok, V>
		- 失败返回<error>, 说明被新epoch 抢占或者acceptor 故障
	- 如果var 取值存在, 则此取值肯定是确定性取值, 此时认同它不在更改, 直接返回<ok, accepted_value>.

### 思考
- **仍需引入多个acceptor**, 单机模块acceptor的故障导致整个系统宕机, 无法提供服务.
- 活锁

## 方案3: Paxos
Acceptor

- paxos 在方案2的基础上引入多个acceptor
	- acceptor 的实现保持不变, 仍采用**喜新厌旧**的原则运行.
- 由于引入了多个acceptor, 采用**少数服从多数**的原则
- 一旦**某个epoch 的取值f 被半数以上acceptor接受**, 则认为此var取值被确定为f, 不再更改.

Proposer:

- Propose(var, V)第一阶段: 选定epoch, 获取epoch 访问权和对应的var 取值
	- 获取半数以上acceptor 的访问权和对应的一组var 取值
- Propose(var, V)第二阶段: 采用"后者认同前者"的原则执行
	- 如果获取的var 取值都为空, 则旧的epoch 无法形成确定性取值, 此时努力使<epoch, V>成为确定性取值.
		- 向epoch 对应的对应的所有acceptor提交取值<epoch, V>
		- 如果收到半数以上(应该是指总的acceptor)成功, 则返回<ok, V>
		- 否则, 则返回<error> (被新的epoch 抢占或者acceptor 故障)
	- 如果var 的取值存在, 认同**最大accpeted_epoch 对应的取值f**, 努力使<epoch, f> 成为确定性取值.
		- 如果f 出现半数以上, 则说明f 已经是确定性取值, 直接返回<ok, f>
		- 否则, 向epoch 对应的所有acceptor 提交取值<epoch, f>

例如: 
一共有100个acceptor, 在初始状态下, var 都为null.
然后, 某个proposer 要提交f1, 在获取了半数以上的acceptor(51个), 但是在成功提交到49个的时候(这时51个中剩下的两个的值仍然是null),
有新的proposer 到来, 将第一个proposer 还未提交生效的acceptor 的访问权抢占了过来, 连并剩下的49个组成了51个, 超过了半数. 且这个proposer 都提交成功了. 也就是现在的状态是有51个f2, 49个f1, f2 的epoch 大于f1 的epoch.
这时, 又有一个新的proposer 来了, 获取了f1 的49个 加上f2 的2个, 共51 个acceptor. 这个proposer 发现虽然有49个f1, 但是没有达到多数, 所以需要认同最大的accpeted_epoch 对应的取值f, 这里就是2个f2 的epoch, 然后这个proposer 会提交 <new_epoch, f2>
可是, 然后呢? 按照前面的规则, 因为这些acceptor 都有值, 好像不能提交成功啊???

### 思考
新轮次的抢占会让旧轮次停止运行, 如果每一轮次在第二阶段执行成功之前都被新一轮抢占, 则导致**活锁**, 怎么解决呢?


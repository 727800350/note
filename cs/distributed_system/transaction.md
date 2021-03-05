- [saga](#saga)
  - [why not 2PC(two phase commit)](#why-not-2pctwo-phase-commit)
  - [Saga的使用条件](#saga的使用条件)
  - [和TCC对比](#和tcc对比)
  - [Saga相关实现](#saga相关实现)

# saga
- [分布式事务:Saga模式](https://www.jianshu.com/p/e4b662407c66)
- [GOTO 2015 Applying the Saga Pattern Caitie McCaffrey](https://www.youtube.com/watch?v=xDuwrtwYHu8) and [pdf](
  http://gotocon.com/dl/goto-chicago-2015/slides/CaitieMcCaffrey_ApplyingTheSagaPattern.pdf)

1987年普林斯顿大学的Hector Garcia-Molina和Kenneth Salem发表了一篇Paper Sagas,讲述的是如何处理long lived transaction
(长活事务).Saga是一个长活事务可被分解成可以交错运行的子事务集合.其中每个子事务都是一个保持数据库一致性的真实事务.

Saga的组成

- 每个Saga由一系列sub-transaction Ti 组成
- 每个Ti 都有对应的补偿动作Ci,补偿动作用于撤销Ti造成的结果

可以看到,和TCC相比,Saga没有"预留"动作,它的Ti就是直接提交到库.

Saga的执行顺序有两种:

- T1, T2, T3, ..., Tn
- T1, T2, ..., Tj, Cj,..., C2, C1,其中0 < j < n

Saga定义了两种恢复策略:

- backward recovery,向后恢复,补偿所有已完成的事务,如果任一子事务失败.即上面提到的第二种执行顺序,其中j是发生错误的
  sub-transaction,这种做法的效果是撤销掉之前所有成功的sub-transation,使得整个Saga的执行结果撤销.
- forward recovery,向前恢复,重试失败的事务,假设每个子事务最终都会成功.适用于必须要成功的场景,执行顺序是类似于这样的:T1,
  T2, ..., Tj(失败), Tj(重试),..., Tn,其中j是发生错误的sub-transaction.该情况下不需要Ci.

显然,向前恢复没有必要提供补偿事务,如果你的业务中,子事务(最终)总会成功,或补偿事务难以定义或不可能,向前恢复更符合你的需求.

理论上补偿事务永不失败,然而,在分布式世界中,服务器可能会宕机,网络可能会失败,甚至数据中心也可能会停电.在这种情况下我们能做
些什么? 最后的手段是提供回退措施,比如人工干预.

## why not 2PC(two phase commit)
- does not scale well
- O(n^2) messages
- coordinator, single point of failure
- reduced throughput, holding all locks on these resources

## Saga的使用条件
Saga看起来很有希望满足我们的需求.所有长活事务都可以这样做吗?这里有一些限制:

1. Saga只允许两个层次的嵌套,顶级的Saga和简单子事务
1. 在外层,全原子性不能得到满足.也就是说,sagas可能会看到其他sagas的部分结果
1. 每个子事务应该是独立的原子行为
1. 在我们的业务场景下,各个业务环境(如:航班预订,租车,酒店预订和付款)是自然独立的行为,而且每个事务都可以用对应服务的数据库
  保证原子操作.

补偿也有需考虑的事项:

补偿事务从语义角度撤消了事务Ti的行为,但未必能将数据库返回到执行Ti时的状态.(例如,如果事务触发导弹发射, 则可能无法撤消此操
作) 但这对我们的业务来说不是问题.其实难以撤消的行为也有可能被补偿.例如,发送电邮的事务可以通过发送解释问题的另一封电邮来
补偿.

## 和TCC对比
Saga相比TCC的缺点是缺少预留动作, 导致补偿动作的实现比较麻烦: Ti 就是commit, 比如一个业务是发送邮件, 在TCC 模式下, 先保存
草稿(Try)再发送(Confirm),撤销的话直接删除草稿(Cancel)就行了.而Saga则就直接发送邮件了(Ti),如果要撤销则得再发送一份邮件说
明撤销(Ci),实现起来有一些麻烦.

如果把上面的发邮件的例子换成:A服务在完成Ti后立即发送Event到ESB(企业服务总线,可以认为是一个消息中间件),下游服务监听到这个
Event做自己的一些工作然后再发送Event到ESB,如果A服务执行补偿动作Ci,那么整个补偿动作的层级就很深.

不过没有预留动作也可以认为是优点:

- 有些业务很简单,套用TCC需要修改原来的业务逻辑,而Saga只需要添加一个补偿动作就行了.
- TCC最少通信次数为2n,而Saga为n(n=sub-transaction的数量).
- 有些第三方服务没有Try接口,TCC模式实现起来就比较tricky了,而Saga则很简单.
- 没有预留动作就意味着不必担心资源释放的问题,异常处理起来也更简单(请对比Saga的恢复策略和TCC的异常处理).

## Saga相关实现
Saga Log and Saga Execution Coordinator(SEC)

对于服务来说,实现Saga有以下这些要求:

1. Ti和Ci是幂等的.
1. Ci必须是能够成功的,如果无法成功则需要人工介入.
1. Ti - Ci和Ci - Ti的执行结果必须是一样的:sub-transaction被撤销了.

第一点要求Ti和Ci是幂等的,举个例子,假设在执行Ti的时候超时了,此时我们是不知道执行结果的,如果采用forward recovery策略就会再
次发送Ti,那么就有可能出现Ti被执行了两次,所以要求Ti幂等.如果采用backward recovery策略就会发送Ci,而如果Ci也超时了,就会尝试
再次发送Ci,那么就有可能出现Ci被执行两次,所以要求Ci幂等.

第二点要求Ci必须能够成功,这个很好理解,因为,如果Ci不能执行成功就意味着整个Saga无法完全撤销,这个是不允许的.但总会出现一些
特殊情况比如Ci的代码有bug,服务长时间崩溃等,这个时候就需要人工介入了.

第三点乍看起来比较奇怪,举例说明,还是考虑Ti执行超时的场景,我们采用了backward recovery,发送一个Ci,那么就会有三种情况:

1. Ti的请求丢失了,服务之前没有,之后也不会执行Ti
1. Ti在Ci之前执行
1. Ci在Ti之前执行

对于第1种情况,容易处理.对于第2,3种情况,则要求Ti和Ci是可交换的(commutative),并且其最终结果都是sub-transaction被撤销.


- [Partition tolerance 分区容错](#partition-tolerance-分区容错)
- [Consistency 一致性](#consistency-一致性)
- [Availability 可用性](#availability-可用性)
- [Consistency 和 Availability 的矛盾](#consistency-和-availability-的矛盾)
- [CAP定理应用](#cap定理应用)

在理论计算机科学中,CAP定理(CAP theorem),又被称作布鲁尔定理(Brewer's theorem),它指出对于一个分布式计算系统来说,不可能同时满足以下三点:

- 一致性(Consistency) (等同于所有节点访问同一份最新的数据副本)
- 可用性(Availability)(每次请求都能获取到非错的响应-但是不保证获取的数据为最新数据)
- 分区容错性(Partition tolerance)(以实际效果而言,分区相当于对通信的时限要求.系统如果不能在时限内达成数据一致性,就意味着发生了分区的情况,必须就当前操作在C和A之间做出选择[3].)

### Partition tolerance 分区容错
大多数分布式系统都分布在多个子网络.每个子网络就叫做一个区(partition).分区容错的意思是,区间通信可能失败.比如,一台服务器放在中国,另一台服务器放在美国,这就是两个区,它们之间可能无法通信.
系统设计的时候,必须考虑到这种情况.
一般来说,分区容错无法避免,因此可以认为 CAP 的 P 总是存在.
CAP 定理告诉我们,剩下的 C 和 A 无法同时做到.

### Consistency 一致性
意思是,写操作之后的读操作,必须返回该值.举例来说,某条记录是 v0,用户向 G1 发起一个写操作,将其改为 v1.
接下来,用户的读操作就会得到 v1.这就叫一致性.
问题是,用户有可能向 G2 发起读操作,由于 G2 的值没有发生变化,因此返回的是 v0.G1 和 G2 读操作的结果不一致,这就不满足一致性了.
为了让 G2 也能变为 v1,就要在 G1 写操作的时候,让 G1 向 G2 发送一条消息,要求 G2 也改成 v1.
这样的话,用户向 G2 发起读操作,也能得到 v1.

### Availability 可用性
意思是只要收到用户的请求,服务器就必须在**有限的时间内**, **返回结果**.
有限的时间内是在一个系统设计之处就设定好的系统运行指标, 通常不同的系统之间会有很大的不同.
用户可以选择向 G1 或 G2 发起读操作.不管是哪台服务器,只要收到请求,就必须告诉用户,到底是 v0 还是 v1,否则就不满足可用性.

### Consistency 和 Availability 的矛盾
一致性和可用性,为什么不可能同时成立?答案很简单,因为可能通信失败(即出现分区容错).
如果保证 G2 的一致性,那么 G1 必须在写操作时,锁定 G2 的读操作和写操作.只有数据同步后,才能重新开放读写.锁定期间,G2 不能读写,没有可用性.
如果保证 G2 的可用性,那么势必不能锁定 G2,所以一致性不成立.
综上所述,G2 无法同时做到一致性和可用性.系统设计时只能选择一个目标.如果追求一致性,那么无法保证所有节点的可用性,如果追求所有节点的可用性,那就没法做到一致性.

### CAP定理应用
[ref](https://blog.csdn.net/zhangyufeijiangxi/article/details/78286364)

放弃CAP定理

- 放弃P: 如果希望能够避免系统出现分区容错性问题, 一种较为简单的做法是将所有的数据(或者仅仅是那些与事务相关的数据)都放在一个分布式节点上.
  这样的做法虽然无法100%地保证系统不会出错, 但至少不会碰到由于网络分区带来的负面影响. 但同时需要注意的是,放弃P的同时也就意味着放弃了系统的可扩展性
- 放弃A: 相对于放弃"分区容错性"来说, 放弃可用性则正好相反, 其做法是一旦系统遇到网络分区或其他故障时,那么受到影响的服务需要等待一定的时间,因此在等待期间系统无法对外提供正常的服务,即不可用
- 放弃C: 这里所说的放弃一致性,并 不是完全不需要数据一致性, 如果真是这样的话, 那么系统的数据都是没有意义的,整个系统也是没有价值的.
  事实上, 放弃一致性指的是放弃数据的强一致性, 而保留数据的最终一致性. 这样的系统无法保证数据保持实时的一致性, 但是能够承诺的是, 数据最终会达到一个一致的状态.
  这就引入了一个时间窗口的概念,具体多久能够达到数据一致取决于系统的设计, 主要包括数据副本在不同节点之间的复制时间长短.

从CAP定理中我们可以看出, 一个分布式系统不可能同时满足一致性, 可用性和分区容错性这三个需求.

另一方面,需要明确的一点是,对于一个分布式系统而言,分区容错性可以说是一个最基本的要求.
为什么这样说,其实很简单,因为既然是一个分布式系统,那么分布式系统中的组件必然需要被部署到不同的节点,否则也就无所谓分布式系统了,因此必然出现子网络.
而对于分布式系统而言,网络问题又是一个必定会出现的异常情况,因此分区容错性也就成为了一个分布式系统必然需要面对和解决的问题.
因此系统架构设计师往往需要把精力花在如何根据业务特点在C(一致性)和A(可用性)之间寻求平衡.

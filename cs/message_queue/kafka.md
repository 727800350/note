# 幂等性
- [Kafka 事务性之幂等性实现](http://matt33.com/2018/10/24/kafka-idempotent/)
- [Exactly Once Delivery and Transactional Messaging in Kafka](
  https://docs.google.com/document/d/11Jqy_GjUGtdXJK94XGsEIK7CP1SnQGdp2eF0wSw9ra8)

在 0.11.0 之前,Kafka 通过 Producer 端和 Server 端的相关配置可以做到数据不丢,也就是 at least once,但是在一些情况下,可能会
导致数据重复,比如:网络请求延迟等导致的重试操作,在发送请求重试时 Server 端并不知道这条请求是否已经处理(没有记录之前的状态
信息),所以就会有可能导致数据请求的重复发送,这是 Kafka 自身的机制(异常时请求重试机制)导致的数据重复.

对于大多数应用而言,数据保证不丢是可以满足其需求的,但是对于一些其他的应用场景(比如支付数据等),它们是要求精确计数的,这时候
如果上游数据有重复,下游应用只能在消费数据时进行相应的去重操作,应用在去重时,最常用的手段就是根据唯一 id 键做 check 去重.

在这种场景下,因为上游生产导致的数据重复问题,会导致所有有精确计数需求的下游应用都需要做这种复杂的,重复的去重处理.
试想一下:如果在发送时,系统就能保证 exactly once,这对下游将是多么大的解脱.

```plain
at least once + 幂等 = exactly once
```

Kafka为了实现幂等性,它在底层设计架构中引入了ProducerID 和SequenceNumber. 那这两个概念的用途是什么呢?

- ProducerID: 在每个新的Producer初始化时,会被分配一个唯一的ProducerID,这个ProducerID对客户端使用者是不可见的.
- SequenceNumber: 对于每个ProducerID,Producer发送数据的每个Topic和Partition都对应一个从0开始单调递增的SequenceNumber值.

For a given PID, sequence numbers will start from zero and be monotonically increasing, with one sequence number per
topic partition produced to. The sequence number will be incremented for every message sent by the producer.
Similarly, the broker will increment the sequence number associated with the PID -> topic partition pair for every
message it commits for that topic partition.
Finally, the broker will reject a message from a producer unless its sequence number is exactly one greater than the
last committed message from that PID -> topic partition pair.

但是这里的幂等性是有条件的:

- 只能保证 Producer 在单个会话内不丟不重,如果Producer 出现意外挂掉再重启是无法保证的(幂等性情况下,是无法获取之前的状态信
  息,因此是无法做到跨会话级别的不丢不重);
- 幂等性不能跨多个Topic-Partition,只能保证单个partition 内的幂等性,当涉及多个Topic-Partition 时,这中间的状态并没有同步.
  如果需要跨会话,跨多个topic-partition 的情况,需要使用Kafka 的事务性来实现.

# 事务
[Kafka Exactly-Once 之事务性实现](http://matt33.com/2018/11/04/kafka-transaction/)

幂等性实现是事务性实现的基础,幂等性提供了单会话单Partition Exactly-Once 语义的实现,正是因为 Idempotent Producer 不提供跨
多个 Partition 和跨会话场景下的保证,因此,我们是需要一种更强的事务保证,能够原子处理多个Partition 的写入操作,数据要么全部
写入成功,要么全部失败,不期望出现中间状态.

为了实现这种机制,我们需要应用能提供一个唯一id,即使故障恢复后也不会改变,这个id 就是TransactionnalId(也叫txn.id,后面会详细
讲述).

当用户使用 Kafka 的事务性时,Kafka 可以做到的保证:

1. 跨会话的幂等性写入:即使中间故障,恢复后依然可以保持幂等性,
1. 跨会话的事务恢复:如果一个应用实例挂了,启动的下一个实例依然可以保证上一个事务完成(commit 或者 abort),
1. 跨多个 Topic-Partition 的幂等性写入,Kafka 可以保证跨多个 Topic-Partition 的数据要么全部写入成功,要么全部失败,不会出现
  中间状态.

上面是从 Producer 的角度来看,那么如果从 Consumer 角度呢?
Consumer 端很难保证一个已经 commit 的事务的所有 msg 都会被消费,有以下几个原因:

1. 对于compacted topic,在一个事务中写入的数据可能会被新的值覆盖,
1. 一个事务内的数据,可能会跨多个log segment,如果旧的segmeng 数据由于过期而被清除,那么这个事务的一部分数据就无法被消费到了
1. Consumer 在消费时可以通过 seek 机制,随机从一个位置开始消费,这也会导致一个事务内的部分数据无法消费,
1. Consumer 可能没有订阅这个事务涉及的全部 Partition.

## 事务性实现的关键
对于Kafka 的事务性实现,最关键的就是其事务操作原子性的实现.
对于一个事务操作而言,其会涉及到多个Topic-Partition 数据的写入,如果是一个 long transaction 操作,可能会涉及到非常多的数据,
如何才能保证这个事务操作的原子性(要么全部完成,要么全部失败)呢?

关于这点,最容易想到的应该是引用2PC 协议(它主要是解决分布式系统数据一致性的问题)中协调者的角色,它的作用是统计所有参与者的
投票结果,如果大家一致认为可以 commit,那么就执行 commit,否则执行 abort:

- 我们来想一下,Kafka 是不是也可以引入一个类似的角色来管理事务的状态,只有当 Producer 真正 commit 时,事务才会提交,否则事务
  会还在进行中(实际的实现中还需要考虑 timeout 的情况),不会处于完成状态,
- Producer 在开始一个事务时,告诉【协调者】事务开始,然后开始向多个 Topic-Partition 写数据,只有这批数据全部写完(中间没有出
  现异常),Producer 会调用 commit 接口进行 commit,然后事务真正提交,否则如果中间出现异常,那么事务将会被 abort(Producer 通
  过 abort 接口告诉【协调者】执行 abort 操作),
- 这里的协调者与2PC 中的协调者略有不同,主要为了管理事务相关的状态信息,这就是Kafka Server 端的TransactionCoordinator 角色

有了上面的机制,是不是就可以了?
很容易想到的问题就是 TransactionCoordinator 挂的话怎么办? TransactionCoordinator 如何实现高可用?

- TransactionCoordinator 需要管理事务的状态信息,如果一个事务的 TransactionCoordinator 挂的话,需要转移到其他的机器上,这里
  关键是在 事务状态信息如何恢复? 也就是事务的状态信息需要很强的容错性,一致性,
- 关于数据的强容错性,一致性,存储的容错性方案基本就是多副本机制,而对于一致性,就有很多的机制实现,其实这个在 Kafka 内部已经
  实现(不考虑数据重复问题),那就是 min.isr + ack 机制,
- 分析到这里,对于Kafka 熟悉的同学应该就知道,这个是不是跟`__consumer_offset` 这个内部的topic 很像,TransactionCoordinator
  也跟GroupCoordinator 类似,而对应事务数据(transaction log)就是 `__transaction_state` 这个内部 topic,所有事务状态信息都
  会持久化到这个topic, TransactionCoordinator 在做故障恢复也是从这个topic 中恢复数据,

有了上面的机制,就够了么?
我们再来考虑一种情况,我们期望一个 Producer 在 Fail 恢复后能主动 abort 上次未完成的事务(接上之前未完成的事务),然后重新开
始一个事务,这种情况应该怎么办?之前幂等性引入的PID 是无法解决这个问题的,因为每次Producer 在重启时,PID 都会更新为一个新值:

- Kafka 在 Producer 端引入了一个 TransactionalId 来解决这个问题,这个 txn.id 是由应用来配置的,
- TransactionalId 的引入还有一个好处,就是跟 consumer group 类似,它可以用来标识一个事务操作,便于这个事务的所有操作都能在
  一个地方(同一个 TransactionCoordinator)进行处理,

再来考虑一个问题,在具体的实现时,我们应该如何标识一个事务操作的开始,进行,完成的状态?
正常来说,一个事务操作是由很多操作组成的一个操作单元,对于TransactionCoordinator 而言,是需要准确知道当前的事务操作处于哪个
阶段,这样在容错恢复时,新选举的 TransactionCoordinator 才能恢复之前的状态:

- 这个就是事务状态转移,一个事务从开始,都会有一个相应的状态标识,直到事务完成,有了事务的状态转移关系之后,
  TransactionCoordinator 对于事务的管理就会简单很多,TransactionCoordinator 会将当前事务的状态信息都会缓存起来,每当事务需
  要进行转移,就更新缓存中事务的状态(前提是这个状态转移是有效的).

## consumer 端如何消费事务数据
在事务场景下,Consumer 的消费策略,Consumer 有一个 isolation.level 配置,有以下两种可选配置:

1. read_committed: only consume non-transactional messages or transactional messages that are already committed, in
  offset ordering.
1. read_uncommitted: consume all available messages in offset ordering. This is the default value.

简单来说就是,read_committed 只会读取 commit 的数据,而 abort 的数据不会向 consumer 显现,对于 read_uncommitted 这种模式,
consumer 可以读取到所有数据(control msg 会过滤掉),这种模式与普通的消费机制基本没有区别,就是做了一个 check,过滤掉 control
msg(也就是 marker 数据),这部分的难点在于 read_committed 机制的实现.

## 中间流程故障如何恢复
对于上面所讲述的一个事务操作流程,实际生产环境中,任何一个地方都有可能出现的失败:

1. Producer 在发送 beginTransaction() 时,如果出现 timeout 或者错误:Producer 只需要重试即可,
1. Producer 在发送数据时出现错误, Producer 应该 abort 这个事务,如果 Produce 没有 abort(比如设置了重试无限次,并且 batch
  超时设置得非常大),TransactionCoordinator 将会在这个事务超时之后 abort 这个事务操作,
1. Producer 发送 commitTransaction() 时出现 timeout 或者错误:Producer 应该重试这个请求,
1. Coordinator Failure:如果 Transaction Coordinator 发生切换(事务 topic leader 切换),Coordinator 可以从日志中恢复.如果发
  送事务有处于 PREPARE_COMMIT 或 PREPARE_ABORT 状态,那么直接执行 commit 或者 abort 操作,如果是一个正在进行的事务,
  Coordinator 的失败并不需要 abort 事务,producer 只需要向新的 Coordinator 发送请求即可.


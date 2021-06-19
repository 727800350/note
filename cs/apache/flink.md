Flink 就是 "Stateful Computations Over Streams".
Flink 是一个纯流式的计算引擎,它的基本数据模型是数据流.
流可以是无边界的无限流,即一般意义上的流处理.也可以是有边界的有限流,这样就是批处理

Spark 的技术理念是基于批来模拟流的计算.而 Flink 则完全相反,它采用的是基于流计算来模拟批计算.
从技术发展方向看,用批来模拟流有一定的技术局限性,并且这个局限性可能很难突破.而Flink基于流来模拟批,在技术上有更好的扩展性.

Flink 最区别于其他流计算引擎的,其实就是**状态管理**.
什么是状态?例如开发一套流计算的系统或者任务做数据处理,可能经常要对数据进行统计,如Sum,Count,Min,Max, 这些值是需要存储的.
因为要不断更新,这些值或者变量就可以理解为一种状态.如果数据源是在读取 Kafka,RocketMQ,可能要记录读取到什么位置,并记录
Offset,这些 Offset 变量都是要计算的状态.

Flink 会定期的将这些状态做checkpoint 持久化, 把checkpoint 存储在一个分布式的持久化系统中, 比如HDFS. 这样的话, 当Flink 的
任务出现任何故障时,它都会从最近的一次checkpoint将整个流的状态进行恢复,然后继续运行它的刘处理,对用户没有任何数据上的影响.

Flink 是如何做到在checkpoint 恢复过程中没有任何数据的丢失和数据的冗余? 来保证精准计算的?
这其中原因是 Flink 利用了一套非常经典的 Chandy-Lamport 算法,它的核心思想是把这个流计算看成一个流式的拓扑,定期从这个拓扑
的头部 Source 点开始插入特殊的 Barriers,从上游开始不断的向下游广播这个 Barriers.每一个节点收到所有的 Barriers,会将 State
做一次 Snapshot,当每个节点都做完 Snapshot 之后,整个拓扑就算完整的做完了一次 Checkpoint.接下来不管出现任何故障,都会从最近
的 Checkpoint 进行恢复.

所有消息到来的时间, 和它真正发生在源头, 在线系统log 当中的时间是不一致的, 在流处理中, 希望是按消息真正发生在源头的顺序进
行处理, Flink 提供了 Event Time 和 WaterMark 的一些先进技术来解决乱序的问题, 使得用户可以有序的处理这个消息.

Exactly-once 的

Flink 计算平台运行在开源的 Hadoop 集群之上.采用 Hadoop 的 YARN 做为资源管理调度,以 HDFS 作为数据存储.

# flink sql
- [Flink 最锋利的武器:Flink SQL 入门和实战](https://cloud.tencent.com/developer/article/1450865)
- [Apache Flink零基础入门(八):Flink SQL编程实践](https://www.infoq.cn/article/lsmqfcy-2bukbwf2vsip)


# introduction
[Introduction to the Apache Pulsar pub-sub messaging platform](https://streaml.io/blog/intro-to-pulsar)

Pulsar was built from the ground up to support multi-tenant use cases.
Pulsar supports two multi-tenancy-specific resources to enable multi-tenancy: **properties and namespaces**.

A property represents a tenant in the system. To give an example, imagine a Pulsar cluster deployed to support a wide
variety of applications (as was the case with Pulsar at Yahoo).
Within a Pulsar cluster, each property can represent a team in the enterprise, a core feature, or a product line, to
give just a few examples.

Each property, in turns, can contain several namespaces, for example one namespace for each application or use case.
A namespace can then contain any number of topics.

Authorization is managed at the property level, and the permissions are managed at the namespace level.

<img src="./pics/pulsar/hierarchy.png" alt="pulsar hierarchy" width="70%"/>

The namespace is the basic administrative unit in Pulsar.
At the namespace level, you can set permissions, fine-tune replication settings, manage geo-replication of message data
across clusters, control message expiry, and perform critical operations.
All topics in a namespace inherit the same settings,

As mentioned before, each namespace can have one or more topics;
each topic can have multiple subscriptions; and each subscription is set to retain and receive all messages published on
the topic.
To provide even more flexibility to application, Pulsar enables three different types of subscriptions that can coexist
on the same topic:

- **Exclusive subscription(streaming model)**: there can be only a single consumer at any given time.
- **Failover subscription(streaming model)**: multiple consumers are allowed to connect to a topic but only one consumer
  will receive messages at any given time.
- **Shared subscription(queuing model)**: multiple consumers can attach to the same subscription and each consumer will
  receive a fraction of the messages.
  The other consumers will start receiving messages only when the current receeiving consumer fails.

<img src="./pics/pulsar/subscription_types.png" alt="subscription_types" align="middle" width="60%"/>

A subscription in Pulsar is effectively the same as a consumer group in Apache Kafka.
Creating subscriptions is highly scalable and very cheap. You can create as many subscriptions as you need.
Different subscriptions on the same topic don't have to be of the same subscription type.

[Why Apache Pulsar? Part 1](https://streaml.io/blog/why-apache-pulsar)
Unlike other messaging systems, Pulsar allows scaling the number of active consumers beyond the number of partitions
within a topic.

Since Pulsar is using Apache BookKeeper as the stream storage, it also exposes the underlying log through a stream
Reader API, allowing applications to start consuming from any earlier positions.

When a Pulsar broker receives a message, it sends it to a set of bookie nodes. When message data is received by a
bookie, it saves a copy in memory and also writes the data to a WAL.
This log is forced to stable storage before the bookie sends an acknowledgment to the broker

[Why Apache Pulsar? Part 2](https://streaml.io/blog/why-apache-pulsar-part-2)
I/O isolation between reads and writes

In most messaging systems, consumer lag can produce general performance degradation.
If a consumer on a topic starts lagging, this can affect other consumers that might be going faster and staying at the
top of queue.
This happens because the slow consumer forces the messaging system to retrieve the data from the storage media, which
leads to I/O thrashing and results in very low throughput.
This slows down the consumers whose data needs to be brought into memory before it can be served. This happens because
reads and writes share a single path of execution.

Pulsar resolves this issue by using Apache BookKeeper as its message storage system.
By using BookKeeper, Pulsar is able to provide I/O isolation between reads and writes by using different paths of
execution for reads and writes.
Regular reads are served directly by Pulsar brokers, while durable writes are made to BookKeeper Write-ahead logs (WALs)
and catch-up reads are made from BookKeeper stable storage.

It's important to provide predictable latency for publishing applications under all circumstances.
With I/O isolation, you can achieve lower and more predictable publish latency even when disks are saturated due to
heavy read activity.

# Partitioned topics
Normal topics can be served only by a single broker, which limits the topic's maximum throughput.
Partitioned topics are a special type of topic that be handled by multiple brokers, which allows for much higher
throughput.

Behind the scenes, a partitioned topic is actually implemented as N internal topics, where N is the number of
partitions.
When publishing messages to a partitioned topic, each message is routed to one of several brokers. The distribution of
partitions across brokers is handled automatically by Pulsar.

每个Topic分区由Pulsar分配给某个Broker,该Broker称为该Topic分区的所有者(owner broker).
Puslar生产者和消费者连接到该Topic分区的owner broker,向其发送消息并消费消息.具体如下图所示.

<img src="./pics/pulsar/topic_partition.png" alt="pulsar topic partition" width="70%"/>

# Bookie消息存储层及其segment-oriented 存储机制
Apache Bookeeper是Pulsar的持久化存储层.Pulsar的每一个Topic分区事实上都是存储于Bookeeper中的分布式日志.
Pulsar的Topic分区在Bookeeper中存储结构如下图所示.

<img src="./pics/pulsar/bookie_segment.png" alt="segments in bookie" width="70%"/>

Pulsar的Topic分区数据会被分为一个个的segment.每个segment会作为Bookeeper中的一个Ledger,均匀分布并存储于Bookeeper集群中的
多个Bookie中(Bookeeper的存储节点).
如上图3所示的一个被分为x个segment的Topic分区,每个segment存储3个副本.所有的segment都分布并存储于Bookeeper集群中的3个
bookie中.

通过segment-oriented方式存储,Topic分区可以均匀平衡地分布于集群中的所有bookie节点中.这意味着Topic分区的大小不会像Kafka一
样受限于一个节点容量的限制,相反,它可以扩展到整个Bookeeper集群的总容量.

## Apache BookKeeper存储结构
Pulsar的底层消息数据以Ledger(对应到pulasr 里面的场景就是Segment)形式存储在多个BookKeeper上,Ledger是一个只追加的数据结构,
并且只有一个写入器, 这个写入器负责多个BookKeeper存储节点(就是Bookies)的写入.
Ledger的条目会被复制到多个bookies.同时bookkeeper会写入相关的数据来保证数据的一致性.

Bookeeper需要保存的数据包括:

- Journals
  这个journals文件里存储的相当于BookKeeper的事务log或者说是写前log, 在任何针对ledger的更新发生前,都会先将这个更新的描述
  信息持久化到这个journal文件中.
  Bookeeper提供有单独的sync线程根据当前journal文件的大小来作journal文件的rolling;
- EntryLogFile
  存储真正数据的文件,写入的时候Entry数据先缓存在内存buffer中,然后批量flush到EntryLogFile中;
  默认情况下,所有ledger的数据都是聚合然后顺序写入到同一个EntryLog文件中,避免磁盘随机写;
- Index文件
  所有Ledger的entry数据都写入相同的EntryLog文件中,为了加速数据读取,会作 ledgerId + entryId 到文件offset的映射,这个映射会
  缓存在内存中,称为IndexCache; IndexCache容量达到上限时,会被 Sync线程flush到文件;

三类数据文件的读写交互如下图:

<img src="./pics/pulsar/bookkeeper_write.png" alt="bookkeeper write" width="70%"/>

集群配置注意事项:

- journal, entrylog, index最好设置在不同磁盘上,避免IO竞争;
- journal 最好写在SSD等高速磁盘上,journal的IO性能有可能成为bookkeeper吞吐瓶颈.

数据一致性保证: LastLogMark

从上面的的讲述可知, 写入的EntryLog和Index都是先缓存在内存中,再根据一定的条件周期性的flush到磁盘,
这就造成了从内存到持久化到磁盘的时间间隔,如果在这间隔内BookKeeper进程崩溃,在重启后,我们需要根据journal文件内容来恢复,这
个LastLogMark就记录了从journal中什么位置开始恢复;

它其实是存在内存中,当IndexCache被flush到磁盘后其值会被更新,其也会周期性持久化到磁盘文件,供BookKeeper进程启动时读取来从
journal中恢复;

LastLogMark一旦被持久化到磁盘,即意味着在其之前的Index和EntryLog都已经被持久化到了磁盘,那么journal在这个LastLogMark之前的
数据都可以被清除了.

# Pulsar Comparison with Apache Kafka
Apache Kafka和Apache pulsar都有类似的消息概念.客户端通过Topic与消息系统进行交互.每个Topic都可以划分为多个分区.
然而,Apache Pulsar和Apache Kafka的根本区别有两点:

- Apache Puslar采用"服务-存储"分离的分层框架,而Apache Kafka的存储和服务都由Broker完成.
  Pulsar得益于其分层框架,可以独立扩展存储层和服务层,更具成本效益,而Kafka无论服务达到瓶颈还是存储达到瓶颈,都需对Broker进
  行整体扩容,且扩容需要迁移数据,扩容成本较高.
- Apache Kafka是partition-oriented的存储机制,而Pulsar是segment-oriented的.

<img src="./pics/pulsar/pulsar_vs_kafka.png" alt="pulsar vs kafka" width="100%"/>

上图显示了partition-oriented同segment-oriented之间的差异.
在Kafka中,分区只能存储在单个节点上并复制到其他节点.其存储容量受最小节点容量的限制.且Kafka扩容需要手动Rebalance分区,以平
衡已有Topic的数据和流量,扩容成本较高.

There are many techniques that Pulsar uses to improve performance. The most important technique is used to handle
tailing reads.
In a scenario where readers are only interested in the most recent data, the readers are served from an in-memory cache
in the serving layer (the Pulsar brokers),
and only catch-up readers end up having to be served from the storage layer (Apache BookKeeper). This approach is key to
improving the latency and throughput compared to systems such as Kafka.

- Concepts
  - Kafka: Producer-topic-consumer group-consumer
  - Pulsar: Producer-topic-subscription-consumer
- Consumption
  - Kafka: More focused on streaming, exclusive messaging on partitions. No shared consumption.
  - Pulsar: Unified messaging model and API. Streaming via exclusive, failover subscription, Queuing via shared
    subscription
- Acking
  - Kafka: Simple offset management, Prior to Kafka 0.8, offsets are stored in ZooKeeper, After Kafka 0.8, offsets are
    stored on offset topics
  - Pulsar: Unified messaging model and API. Streaming via exclusive, failover subscription, Queuing via shared
    subscription
- Retention 滞留
  - Kafka: Messages are deleted based on retention. If a consumer doesn't read messages before retention period, it will
    lose data.
  - Pulsar: Messages are only deleted after all subscriptions consumed them. No data loss even the consumers of a
    subscription are down for a long time.
    Messages are allowed to keep for a configured retention period time even after all subscriptions consume them.
- TTL: A message will automatically be marked as acknowledged if it is not consumed by any consumers within the
  configured TTL time period.
  The difference between message retention and message TTL is that message retention applies to messages that are marked
  as acknowledged and set to be deleted.
  - Kafka: No TTL support
  - Pulsar: Supports message TTL

If you would like to hear a short sentence about how Apache Pulsar differs from Apache Kafka in their respective
messaging models, here is mine:

Apache Pulsar combines high-performance streaming (which Apache Kafka pursues) and flexible traditional queuing (which
RabbitMQ pursues) into a unified messaging model and API.
Pulsar gives you one system for both streaming and queuing, with the same high performance, using a unified API.

[PERFORMANCE COMPARISON BETWEEN APACHE PULSAR AND KAFKA: LATENCY](
https://kafkaesque.io/performance-comparison-between-apache-pulsar-and-kafka-latency/)

## end-to-end latency
End-to-end latency is simply the time from when the message is sent by the producer to when it received by the consumer.
Where end-to-end latency gets complicated is with the clocks used to take those timestamp measurements.
When measuring end-to-end latency the clocks used for the timestamps must be synchronized. If they are not synchronized,
the difference between the clocks will impact your measurements.

## publishing latency
Publishing latency is the amount of time that passes from when the message is sent until the time an acknowledgment is
received from the messaging system.
The acknowledgment indicates that the messaging system has persisted the message and will guarantee its delivery.
Essentially, the acknowledgment indicates the responsibility for handling the message has been successfully passed from
the producing application to the messaging system.
Low, consistent publishing latency is good for applications. When the application is ready to hand off the delivery of a
message, the messaging system quickly accepts the message,
allowing the application to continue working on application-level concerns, such as business logic. This handoff of
responsibility for the message is a key feature of any messaging system.

In the benchmark tests, the publishing latency is measured from the time when calling the send method until the
acknowledgment callback is triggered.
These two timestamps are done in the producer, so clock synchronization is not a concern.
也就是只要mq 收到并给producer 回包确认收到就可以了.

## message replication
Kafka uses a leader-follower replication model.
One of the Kafka brokers is elected the leader for a partition. All messages are initially written to the leader, and
the followers read and replicate the messages from the leader.

Pulsar uses a quorum-vote replication model.
Multiple copies of the message (write quorum) are written in parallel. Once some number of copies have been confirmed
stored, then the message is acknowledged (ack quorum).


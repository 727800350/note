# Introduction
[开源实时数据处理系统Pulsar:一套搞定Kafka+Flink+DB](https://juejin.im/post/5af414365188256717765441)

在实时处理的系统中,系统的核心由消息,计算和存储三个子系统组成.
比如消息系统有 Kafka,RabbitMQ,Flume 等,计算系统有 Spark Streaming,Flink,Heron 等,存储系统有各种分布式的文件系统,DB,K/V store 等.

面对消息,存储和计算三个部分分隔的现状,Apache Pulsar 在这三个方面进行了很好的协调,抽象和统一.
具体到 Apache Pulsar 内部,消息部分由 Pulsar Broker 来负责,存储部分使用了 Apache BookKeeper,计算部分由 Pulsar Functions 来负责.

Apache Pulsar 在架构上最明显的优势是采用了消息服务和消息存储分层的策略.它包括了无状态的消息服务层(broker 节点)和消息存储层(BookKeeper 中 Bookie 是基本的存储节点).
这为系统带来了极好的扩展性和健壮性.在消息服务层和存储层,系统所关注的内容是不一样的:
在服务层更多的是对 Producer 和 Consumer 的支持,更关注用户接口和消息的服务质量,需要更好的 CPU 和网络带宽来支持消息的扇入扇出.
存储层更关注磁盘 IOPS 和存储容量,负责数据的持久化等.

分层的架构带为服务和存储两层都带来了线性,瞬时的扩展性.
如果需要增加和支持更多的 Producer 和 Consumer,只用对 broker 进行 Scale.
如果存储空间紧张,或者想要消息的时间保持的时间更长,可以单独增加存储节点 Bookie.

在服务层中,broker 不会有相关的数据被持久化保存,是无状态的.对 Topic 的服务可以很容易地迁移.如果 broker 失效,可以很容易地将 topic 迁移到健康的 broker.
在存储层(Bookie)也是一样.每个 topic 的数据被打散并均匀 partition 到多个 segment,每个 segment 的数据又被分散存储在 Bookie 集群中.
当想增加容量的时候,只需要添加新的 Bookie,数据会优先选择刚加入的 Bookie.同样当 broker 被 overloaded,添加新的 broker 之后,负载会被均衡地分配到新添加的 broker 之上.

## Message model
[Comparing Pulsar and Kafka: unified queuing and streaming](https://streaml.io/blog/pulsar-streaming-queuing)

In a modern real-time streaming architecture, messaging use cases can be separated into two categories: queuing and streaming.

### Queuing
Queuing is unordered or shared messaging.
With queuing messaging, multiple consumers are created to receive messages from a single point-to-point messaging channel.
When the channel delivers a message, any of the consumers could potentially receive it. The messaging system's implementation determines which consumer actually receives the message.

Queuing use cases are usually found in conjunction with stateless applications.
Stateless applications don't care about ordering but they do require the ability
to acknowledge or remove individual messages as well as the ability to scale consumption parallelism as much as possible.
Typical queuing-based messaging systems include RabbitMQ and RocketMQ.

### Streaming
In contrast, streaming is strictly ordered or exclusive messaging.
With streaming messaging, there is always only one consumer consuming the messaging channel.
The consumer receives the messages dispatched from the channel in the exactorder in which they were written.

Streaming use cases are usually associated with stateful applications.
Stateful applications care about ordering and their state.  The ordering of messages determines the state of a stateful application.
Ordering will impact the correctness of whatever processing logic the application needs to apply when out-of-order consumption occurs.

Both streaming and queuing are necessary in a microservices-oriented or event-driven architecture.

## The Pulsar Model
Apache Pulsar unifies queuing and streaming into a unified messaging model: producer-topic-subscription-consumer.
A topic(partition) is a named channel for sending messages. Each topic partition is backed by a distributed log stored in Apache BookKeeper.
Each published message is only stored once on a topic partition, replicated to be stored on multiple bookies (BookKeeper servers),and can be consumed as many times as necessary by consumers.

The topic is the source of truth for consumption. Although messages are only stored once on the topic partition, there can be different ways of consuming those messages.
Consumers are grouped together for consuming messages. Each group of consumers is a subscription on a topic.
Each consumer group can have its own way of consuming the messages—exclusively, shared, or failover—which can be different across consumer groups.
This combines queuing and streaming in one model and API, and it was designed and implemented with the goal of not impacting performance and introducing cost overhead,
while also providing a lot of flexibility to users to consume messages in a way that's best for the use case at hand.

# pulsar
## introduction
[Introduction to the Apache Pulsar pub-sub messaging platform](https://streaml.io/blog/intro-to-pulsar)

Pulsar was built from the ground up to support multi-tenant use cases.
Pulsar supports two multi-tenancy-specific resources to enable multi-tenancy: properties and namespaces.
A property represents a tenant in the system. To give an example, imagine a Pulsar cluster deployed to support a wide variety of applications (as was the case with Pulsar at Yahoo).
Within a Pulsar cluster, each property can represent a team in the enterprise, a core feature, or a product line, to give just a few examples.
Each property, in turns, can contain several namespaces, for example one namespace for each application or use case.
A namespace can then contain any number of topics.

Authorization is managed at the property level, and the permissions are managed at the namespace level.

<img src="./pics/pulsar/hierarchy.png" alt="pulsar hierarchy" align="middle" width="70%"/>

The namespace is the basic administrative unit in Pulsar.
At the namespace level, you can set permissions, fine-tune replication settings, manage geo-replication of message data across clusters, control message expiry, and perform critical operations.
All topics in a namespace inherit the same settings,

As mentioned before, each namespace can have one or more topics;
each topic can have multiple subscriptions; and each subscription is set to retain and receive all messages published on the topic.
To provide even more flexibility to application, Pulsar enables three different types of subscriptions that can coexist on the same topic:

- Exclusive subscription(streaming model): there can be only a single consumer at any given time.
- Failover subscription(streaming model): multiple consumers are allowed to connect to a topic but only one consumer will receive messages at any given time.
- Shared subscription(queuing model): multiple consumers can attach to the same subscription and each consumer will receive a fraction of the messages.
  The other consumers will start receiving messages only when the current receeiving consumer fails.

<img src="./pics/pulsar/subscription_types.png" alt="subscription_types" align="middle" width="60%"/>

A subscription in Pulsar is effectively the same as a consumer group in Apache Kafka.
Creating subscriptions is highly scalable and very cheap. You can create as many subscriptions as you need.
Different subscriptions on the same topic don't have to be of the same subscription type.

[Why Apache Pulsar? Part 1](https://streaml.io/blog/why-apache-pulsar)
Unlike other messaging systems, Pulsar allows scaling the number of active consumers beyond the number of partitions within a topic.

Since Pulsar is using Apache BookKeeper as the stream storage, it also exposes the underlying log through a stream Reader API, allowing applications to start consuming from any earlier positions.

When a Pulsar broker receives a message, it sends it to a set of bookie nodes. When message data is received by a bookie, it saves a copy in memory and also writes the data to a WAL.
This log is forced to stable storage before the bookie sends an acknowledgment to the broker

[Why Apache Pulsar? Part 2](https://streaml.io/blog/why-apache-pulsar-part-2)
I/O isolation between reads and writes

In most messaging systems, consumer lag can produce general performance degradation.
If a consumer on a topic starts lagging, this can affect other consumers that might be going faster and staying at the top of queue.
This happens because the slow consumer forces the messaging system to retrieve the data from the storage media, which leads to I/O thrashing and results in very low throughput.
This slows down the consumers whose data needs to be brought into memory before it can be served. This happens because reads and writes share a single path of execution.

Pulsar resolves this issue by using Apache BookKeeper as its message storage system.
By using BookKeeper, Pulsar is able to provide I/O isolation between reads and writes by using different paths of execution for reads and writes.
Regular reads are served directly by Pulsar brokers, while durable writes are made to BookKeeper Write-ahead logs (WALs) and catch-up reads are made from BookKeeper stable storage.

It's important to provide predictable latency for publishing applications under all circumstances.
With I/O isolation, you can achieve lower and more predictable publish latency even when disks are saturated due to heavy read activity.

## Partitioned topics
Normal topics can be served only by a single broker, which limits the topic's maximum throughput.
Partitioned topics are a special type of topic that be handled by multiple brokers, which allows for much higher throughput.

Behind the scenes, a partitioned topic is actually implemented as N internal topics, where N is the number of partitions.
When publishing messages to a partitioned topic, each message is routed to one of several brokers. The distribution of partitions across brokers is handled automatically by Pulsar.

## Pulsar Comparison with Apache Kafka
There are many techniques that Pulsar uses to improve performance. The most important technique is used to handle tailing reads.
In a scenario where readers are only interested in the most recent data, the readers are served from an in-memory cache in the serving layer (the Pulsar brokers),
and only catch-up readers end up having to be served from the storage layer (Apache BookKeeper). This approach is key to improving the latency and throughput compared to systems such as Kafka.

- Concepts
  - Kafka: Producer-topic-consumer group-consumer
  - Pulsar: Producer-topic-subscription-consumer
- Consumption
  - Kafka: More focused on streaming, exclusive messaging on partitions. No shared consumption.
  - Pulsar: Unified messaging model and API. Streaming via exclusive, failover subscription, Queuing via shared subscription
- Acking
  - Kafka: Simple offset management, Prior to Kafka 0.8, offsets are stored in ZooKeeper, After Kafka 0.8, offsets are stored on offset topics
  - Pulsar: Unified messaging model and API. Streaming via exclusive, failover subscription, Queuing via shared subscription
- Retention
  - Kafka: Messages are deleted based on retention. If a consumer doesn't read messages before retention period, it will lose data.
  - Pulsar: Messages are only deleted after all subscriptions consumed them. No data loss even the consumers of a subscription are down for a long time.
    Messages are allowed to keep for a configured retention period time even after all subscriptions consume them.
- TTL: A message will automatically be marked as acknowledged if it is not consumed by any consumers within the configured TTL time period.
  The difference between message retention and message TTL is that message retention applies to messages that are marked as acknowledged and set to be deleted. 
  - Kafka: No TTL support
  - Pulsar: Supports message TTL

If you would like to hear a short sentence about how Apache Pulsar differs from Apache Kafka in their respective messaging models, here is mine:

Apache Pulsar combines high-performance streaming (which Apache Kafka pursues) and flexible traditional queuing (which RabbitMQ pursues) into a unified messaging model and API.
Pulsar gives you one system for both streaming and queuing, with the same high performance, using a unified API.

[PERFORMANCE COMPARISON BETWEEN APACHE PULSAR AND KAFKA: LATENCY](https://kafkaesque.io/performance-comparison-between-apache-pulsar-and-kafka-latency/)

### END-TO-END LATENCY:
End-to-end latency is simply the time from when the message is sent by the producer to when it received by the consumer.
Where end-to-end latency gets complicated is with the clocks used to take those timestamp measurements.
When measuring end-to-end latency the clocks used for the timestamps must be synchronized. If they are not synchronized, the difference between the clocks will impact your measurements.

### PUBLISHING LATENCY
Publishing latency is the amount of time that passes from when the message is sent until the time an acknowledgment is received from the messaging system.
The acknowledgment indicates that the messaging system has persisted the message and will guarantee its delivery.
Essentially, the acknowledgment indicates the responsibility for handling the message has been successfully passed from the producing application to the messaging system.
Low, consistent publishing latency is good for applications. When the application is ready to hand off the delivery of a message, the messaging system quickly accepts the message,
allowing the application to continue working on application-level concerns, such as business logic. This handoff of responsibility for the message is a key feature of any messaging system.

In the benchmark tests, the publishing latency is measured from the time when calling the send method until the acknowledgment callback is triggered.
These two timestamps are done in the producer, so clock synchronization is not a concern.
也就是只要mq 收到并给producer 回包确认收到就可以了.

### MESSAGE REPLICATION
Kafka uses a leader-follower replication model.
One of the Kafka brokers is elected the leader for a partition. All messages are initially written to the leader, and the followers read and replicate the messages from the leader.
Pulsar uses a quorum-vote replication model.
Multiple copies of the message (write quorum) are written in parallel. Once some number of copies have been confirmed stored, then the message is acknowledged (ack quorum).


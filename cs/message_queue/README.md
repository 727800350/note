# introduction
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

# message model
[Comparing Pulsar and Kafka: unified queuing and streaming](https://streaml.io/blog/pulsar-streaming-queuing)

In a modern real-time streaming architecture, messaging use cases can be separated into two categories: queuing and streaming.

## queuing
Queuing is unordered or shared messaging.
With queuing messaging, multiple consumers are created to receive messages from a single point-to-point messaging channel.
When the channel delivers a message, any of the consumers could potentially receive it. The messaging system's implementation determines which consumer actually receives the message.

Queuing use cases are usually found in conjunction with stateless applications.
Stateless applications don't care about ordering but they do require the ability
to acknowledge or remove individual messages as well as the ability to scale consumption parallelism as much as possible.
Typical queuing-based messaging systems include RabbitMQ and RocketMQ.

## streaming
In contrast, streaming is strictly ordered or exclusive messaging.
With streaming messaging, there is always only one consumer consuming the messaging channel.
The consumer receives the messages dispatched from the channel in the exactorder in which they were written.

Streaming use cases are usually associated with stateful applications.
Stateful applications care about ordering and their state.  The ordering of messages determines the state of a stateful application.
Ordering will impact the correctness of whatever processing logic the application needs to apply when out-of-order consumption occurs.

Both streaming and queuing are necessary in a microservices-oriented or event-driven architecture.

## the pulsar model
Apache Pulsar unifies queuing and streaming into a unified messaging model: producer-topic-subscription-consumer.
A topic(partition) is a named channel for sending messages. Each topic partition is backed by a distributed log stored in Apache BookKeeper.
Each published message is only stored once on a topic partition, replicated to be stored on multiple bookies (BookKeeper servers),and can be consumed as many times as necessary by consumers.

The topic is the source of truth for consumption. Although messages are only stored once on the topic partition, there can be different ways of consuming those messages.
Consumers are grouped together for consuming messages. Each group of consumers is a subscription on a topic.
Each consumer group can have its own way of consuming the messages—exclusively, shared, or failover—which can be different across consumer groups.
This combines queuing and streaming in one model and API, and it was designed and implemented with the goal of not impacting performance and introducing cost overhead,
while also providing a lot of flexibility to users to consume messages in a way that's best for the use case at hand.


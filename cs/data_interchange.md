# Google Protocol Buffer
Protocol Buffers (a.k.a., protobuf) are Google language-neutral, platform-neutral, extensible mechanism for serializing structured data.
You can find [protobuf documentation on the Google Developers site](https://developers.google.com/protocol-buffers/).

[Google Protocol Buffer 的使用和原理](http://www.ibm.com/developerworks/cn/linux/l-cn-gpb/#ibm-pcon)

## usage
### 数据类型
double, float, int32, int64, uint32, uint64, sint32, sint64, fixed32, fixed64, sfixed32, sfixed64, bool, string, bytes

- sint32/64: Uses variable-length encoding. Signed int value. These more efficiently encode negative numbers than regular int32/64.
- fixed32: Always four bytes. More efficient than uint32 if values are often greater than 228.
- bytes: May contain any arbitrary sequence of bytes.
- map<key_type, value_type> map_field = N; where the key_type can be any integral or string type. The value_type can be any type.

### required / optional / repeated
Each field in the message definition has a unique numbered tag.
These tags are used to identify your fields in the message binary format, and should not be changed once your message type is in use.
Note that tags with values in the range 1 through 15 take one byte to encode, including the identifying number and the field type.
Tags in the range 16 through 2047 take two bytes.
So you should reserve the tags 1 through 15 for very frequently occurring message elements.
Remember to leave some room for frequently occurring elements that might be added in the future.

- required: a well-formed message must have exactly one of this field.
- optional: a well-formed message can have zero or one of this field (but not more than one).
- repeated: this field can be repeated any number of times (including zero) in a well-formed message. The order of the repeated values will be preserved.

`package foo.bar` 在C++ 中会被转换为 `foo::bar` namespace

### 编译使用
1. 生成类文件
	- for cpp: `protoc -I=./ --cpp_out=./ somename.proto`
	- for python: `protoc -I=./ --python_out=./ somename.proto`
1. `g++ my_program.cc my_proto.pb.cc -lpthread -lprotobuf`
	- `pkg-config --cflags --libs protobuf`: 可以看到具体的依赖库

## C++ API
- class static method
	- `static const Descriptor* descriptor()`: 返回类型的descripter, 可用于反射
- numeric field
	- `bool has_foo() const`: return true if the field foo is set
	- `int32 foo()`: return the current value or the default value if not set
	- `void set_foo(int32 value)`
	- `void clear_foo()`
- string or bytes field
	- `const string& foo() const`
	- `void set_foo(const string& value)`: 内存是怎么处理的
	- `void set_foo(const char* value)`
	- `string* mutable_foo()`
	- `void clear_foo()`
	- 对于repeated string fields, `add_foo(const string& value | const char* value)` 中内存是怎么处理的.
- enum fields
	- `bool has_foo()`
	- `Bar foo()`
	- `void set_foo(Bar value)`
	- `void clear_foo()`
- repeated numeric fields
	- `int foo_size()`
	- `int32 foo(int index) const`: 返回index 位置的value
	- `void set_foo(int index, int32 value)`
	- `void add_foo(int32 value)`
	- `void clear_foo()`
- map fields
	- proto def: `map<int32, int32> weight = 1;`
	- `const google::protobuf::Map<int32, int32> &weight()`: return an **immutable** map
	- `google::protobuf::Map<int32, int32>* mutable_weight()`: return a **mutable** map

## install
To install protobuf, you need to install the protocol compiler (used to compile .proto files) and the protobuf runtime for your chosen programming language.

apt-get install protobuf-compiler ## protocol compiler
apt-get install python-protobuf ## protobuf runtime for Python
apt-get install libprotobuf-dev ## protobuf runtime for C++

## Encoding
### varint
Varints are a method of serializing integers using one or more bytes.
Smaller numbers take a smaller number of bytes.
Varint 中的每个byte的第一个bit(the most significant bit(msb)) 如果为1, 表示后面的一个byte仍然是当前这个数据的一个byte, 如果为0, 就表示这个byte已经是最后一个了.
每个byte中剩下的7个bits就是实际的数据内容.

字节序(比如int a = 0x05060708)

- BIG-ENDIAN: 05 06 07 08
- LITTLE-ENDIAN: 08 07 06 05

剩下每个byte中的7个bits 是按照little endian 来组织的.

1. 原始数据`1010 1100 0000 0010`
1. 首先第一个byte的第一个bit为1, 表示后面的一个byte 仍然是数据的一部分; 第二个byte的第一个bit为0, 表示这个byte是数据的末尾.
	去掉数据的各个byte的第一个bit之后, 成为`010 1100 0000 010`
1. 由于数据是按照little endian组织的, 所以转换为一般的数据序列为: `0000 010 010 1100`, 也就是`1 0010 1100`, 等于 2^8 + 2^5 + 2^3 + 2^2 = 300

### message structure
A protocol buffer message is a series of **key-value pairs**.
The binary version of a message just uses the field number as the key. 所以对于decode一方, 需要有对应.proto 文件才能正确解析.

The "key" for each pair is actually a **variant** composing of two values

1. the field number from .proto file
1. a wire type that provides just enough information to find the length of the value.

```
key = (field_number << 3) | wire_type
```
也就是说key的最后3个bits 存储的是wire type.


6 种wire type

1. 0: Varint	int32, int64, uint32, uint64, sint32, sint64, bool, enum
1. 1: 64-bit	fixed64, sfixed64, double
1. 2: Length-delimited	string, bytes, embedded messages, packed repeated fields
1. 3: Start group	groups (deprecated)
1. 4: End group	groups (deprecated)
1. 5: 32-bit	fixed32, sfixed32, float

ex: 0x9601

1. `0x089601 = 0000 1000 1001 0110 0000 0001`
1. 第一个byte 为 `0000 1000`, 由于第一个bit 为0, 因此这个kv pair的key就是一个byte, 为`0000 1000`
1. key的最后3个bits 为 `000`, 因此这个kv pair的value 是一个varint 类型数据.
1. 从第二个byte开始为value部分, 然后第二个byte的第一个bit为1, 第三个byte的第一个bit 为0, 因此value 为 `1001 0110 0000 0001`.
1. 去掉value 的每个byte的第一个bit, 剩下 `001 0110 000 0001`
1. single endien 到一般序列, 结果为: 000 0001 001 0110 = 1001 0110 = 2^7 + 2^4 + 2^2 + 2^1 = 150

### string
A wire type of 2(length-delimited) means that the value is a **varint encoded length** followed by the specified number of bytes of data.
```
message Test2{
	required string b = 2;
}
```
setting the value of b to "testing" gives us: `0x 12 07 74 65 73 74 69 6e 67`

1. `12 = 0001 0010`, 最后三个bits为`010 = 2`, 表示wire type 等于2, `0001 0`是field number, 也是2
1. `07 = 0000 0111`, value 部分的第一个byte 的第一个bit为0, 意味着这个表示data length的varint类型数据只有一个byte, 值为7, 也就是说后面紧跟着的7个bytes为value的data部分
1. `74 65 73 74 69 6e 67` 分别为 `t e s t i n g` 7个字符

### Embedded Messages
```
message Test1{
	required int32 a = 1;
}
message Test3{
	required Test1 c = 3;
}
```
令c中的a 为 150, 会被编码为:`1a 03 08 96 01`

1. `1a = 0001 1010`, wire type = `010` = 2, field number = `0001 1` = 3
1. `03 = 0000 0011`, data lenght 就一个byte的varint, 值为3
1. `08 96 01` 为一个Test1 类型的message
1. `08 = 0000 1000`, wire type = 0, field number = 1
1. `96 01 = 1001 0110 0000 0001`, 去掉msb, 倒过来就是`000 0001 001 0110` = 150

### Optional and repeated elements
These repeated values do not have to appear consecutively, they may be interleaved with other fields.
The order of the elements with respect to each other is preserved when parsing.

# gRPC
In gRPC a client application can directly call methods on a server application on a different machine as if it was a local object,
making it easier for you to create distributed applications and services.
As in many RPC systems, gRPC is based around the idea of defining a service, specifying the methods that can be called remotely with their parameters and return types.
On the server side, the server implements this interface and runs a gRPC server to handle client calls.
On the client side, the client has a stub that provides exactly the same methods as the server.

[grpc concept](http://www.grpc.io/img/grpc_concept_diagram_00.png)

# Apache Thrift
[install Apache Thrift on debian](http://thrift.apache.org/docs/install/debian)
[doc](http://thrift.apache.org/tutorial/)

thrift -r --gen cpp/python <Thrift filename>

# Message Queue(MQ)
消息队列可驻留在内存或磁盘上, 队列存储消息直到它们被应用程序消费.

message queue的通讯模式

- 点对点通讯: 点对点方式是最为传统和常见的通讯方式,它支持一对一,一对多,多对多,多对一等多种配置方式,支持树状,网状等多种拓扑结构.
- 多点广播: 能够将消息发送到多个目标站点 (Destination List).可以使用一条 MQ 指令将单一消息发送到多个目标站点,并确保为每一站点可靠地提供信息.
- 发布/订阅(publish/subscribe)模式: 使消息按照特定的主题甚至内容进行分发,用户或应用程序可以根据主题或内容接收到所需要的消息
- 集群: 为了简化点对点通讯模式中的系统配置, MQ 提供 Cluster(群集)的解决方案.
	群集类似于一个域 (Domain),群集内部的队列管理器之间通讯时,不需要两两之间建立消息通道,而是采用群集 (Cluster) 通道与其它成员通讯,从而大大简化了系统配置.

- RabbitMQ是一个AMQP实现,传统的messaging queue系统实现,基于Erlang.老牌MQ产品了.AMQP协议更多用在企业系统内,对数据一致性,稳定性和可靠性要求很高的场景,对性能和吞吐量还在其次.
	不单单做到了简单的数据转发功能,还保证了单个队列上的数据有序,即便是有多个消费者和多个生产者.
	- RabbitMQ的消息应当尽可能的小,并且只用来处理实时且要高可靠性的消息.
	- 消费者和生产者的能力尽量对等,否则消息堆积会严重影响RabbitMQ的性能.
	- 集群部署,使用热备,保证消息的可靠性.
- Kafka是linkedin开源的MQ系统,主要特点是基于Pull的模式来处理消息消费,追求高吞吐量,一开始的目的就是用于日志收集和传输.
	0.8开始支持复制,不支持事务,适合产生大量数据的互联网服务的数据收集业务.
	- 应当有一个非常好的运维监控系统,不单单要监控Kafka本身,还要监控Zookeeper.
	- 对消息顺序不依赖,且不是那么实时的系统.
	- 对消息丢失并不那么敏感的系统.
- ZeroMQ只是一个网络编程的Pattern库, 将常见的网络请求形式(分组管理,链接管理,发布订阅等)模式化,组件化,简而言之socket之上,MQ之下.
	对于MQ来说,网络传输只是它的一部分,更多需要处理的是消息存储,路由,Broker服务发现和查找,事务,消费模式(ack,重投等),集群服务等.

## RabbitMQ
AMQP,即Advanced Message Queuing Protocol, 一个提供统一消息服务的应用层标准高级消息队列协议,是应用层协议的一个开放标准,为面向消息的中间件设计.
基于此协议的客户端与消息中间件可传递消息,并不受客户端/中间件不同产品,不同开发语言等条件的限制.
Erlang中的实现有 RabbitMQ等.

- $ sudo rabbitmqctl list_queues name messages_ready messages_unacknowledged
- $ sudo rabbitmqctl list_exchanges
- $ sudo rabbitmqctl list_bindings

### work queue
The main idea behind Work Queues (aka: Task Queues) is to avoid doing a resource-intensive task immediately and having to wait for it to complete.
Instead we schedule the task to be done later. We **encapsulate a task as a message and send it to the queue**.
A worker process running in the background will pop the tasks and eventually execute the job. When you run many workers the tasks will be shared between them.

可是如果producer 需要得到这个计算任务的结果呢?怎么办, 怎么把结果取回来?

By default, RabbitMQ will send each message to the next consumer, in sequence. On average every consumer will get the same number of messages.
This way of distributing messages is called round-robin.

#### 持久化
##### consumer dies
Message acknowledgments are turned on by default. In previous examples we explicitly turned them off via the `no_ack=True` flag.
`channel.basic_consume(callback, queue='hello', no_ack=True)`

1. 默认开启ack模式 `channel.basic_consume(callback, queue = 'hello')`
1. 在callback 函数主动发送一个ack `channel.basic_ack(delivery_tag = method.delivery_tag)`

##### rabbitmq server dies
mark queue durable
```
channel.queue_declare(queue='hello',
					  durable=True)
```

mark messages durable
```
channel.basic_publish(exchange='',
                      routing_key="task_queue",
                      body=message,
                      properties=pika.BasicProperties(
                         delivery_mode = 2, # make message persistent
                      ))
```

#### Fair dispatch
For example in a situation with two workers, when all odd messages are heavy and even messages are light,
one worker will be constantly busy and the other one will do hardly any work.

`channel.basic_qos(prefetch_count=1)`:
This tells RabbitMQ not to give more than one message to a worker at a time.
Or, in other words, do not dispatch a new message to a worker until it has processed and acknowledged the previous one.
Instead, it will dispatch it to the next worker that is not still busy.

If all the workers are busy, your queue can fill up.
You will want to keep an eye on that, and maybe add more workers, or have some other strategy.

### Exchange
The core idea in the messaging model in RabbitMQ is that the producer never sends any messages directly to a queue.
Instead, the **producer can only send messages to an exchange**.
An exchange is a very simple thing. On one side it receives messages from producers and the other side it pushes them to queues.
The exchange must know exactly what to do with a message it receives.
Should it be appended to a particular queue? Should it be appended to many queues? Or should it get discarded.
The rules for that are defined by the exchange type.

There are a few exchange types available: direct, topic, headers and fanout.

- '': nameless exchange, the default exchange, messages are routed to the queue with the name specified by routing_key, if it exists.
- fanout: very simple, it just broadcasts all the messages it receives to all the queues it knows
	[log example using fanout](http://www.rabbitmq.com/tutorials/tutorial-three-python.html)
	every running copy of the receiver program will get the messages. run one receiver and direct the logs to disk and run another receiver and see the logs on the screen.
- direct
	[log example using direct](http://www.rabbitmq.com/tutorials/tutorial-four-python.html)
	direct only critical error messages to the log file(to save disk space), while still being able to print all of the log messages on the console.
- topic
	[log example using topic](http://www.rabbitmq.com/tutorials/tutorial-five-python.html)
	subscribe to logs based on both severity and the source which emitted the log. eg: listen to just critical errors coming from 'cron' but also all logs from 'kern'
- headers

### RPC
[rpc example](http://www.rabbitmq.com/tutorials/tutorial-six-python.html)

## Apache Kafka


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

## Apache Kafka


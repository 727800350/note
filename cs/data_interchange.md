# Google Protocol Buffer
Protocol Buffers (a.k.a., protobuf) are Google language-neutral, platform-neutral, extensible mechanism for serializing structured data.
You can find [protobuf documentation on the Google Developers site](https://developers.google.com/protocol-buffers/).

[Google Protocol Buffer 的使用和原理](http://www.ibm.com/developerworks/cn/linux/l-cn-gpb/#ibm-pcon)

Each field in the message definition has a unique numbered tag.
These tags are used to identify your fields in the message binary format, and should not be changed once your message type is in use.
Note that tags with values in the range 1 through 15 take one byte to encode, including the identifying number and the field type.
Tags in the range 16 through 2047 take two bytes.
So you should reserve the tags 1 through 15 for very frequently occurring message elements.
Remember to leave some room for frequently occurring elements that might be added in the future.

- required: a well-formed message must have exactly one of this field.
- optional: a well-formed message can have zero or one of this field (but not more than one).
- repeated: this field can be repeated any number of times (including zero) in a well-formed message. The order of the repeated values will be preserved.

数据类型
double, float, int32, int64, uint32, uint64, sint32, sint64, fixed32, fixed64, sfixed32, sfixed64, bool, string, bytes

- sint32/64: Uses variable-length encoding. Signed int value. These more efficiently encode negative numbers than regular int32/64.
- fixed32: Always four bytes. More efficient than uint32 if values are often greater than 228.
- bytes: May contain any arbitrary sequence of bytes.

map<key_type, value_type> map_field = N;
where the key_type can be any integral or string type (so, any scalar type except for floating point types and bytes). The value_type can be any type.

To install protobuf, you need to install the protocol compiler (used to compile .proto files) and the protobuf runtime for your chosen programming language.

apt-get install protobuf-compiler ## protocol compiler
apt-get install python-protobuf ## protobuf runtime for Python
apt-get install libprotobuf-dev ## protobuf runtime for C++

protoc -I=./ --cpp_out=./ somename.proto
protoc -I=./ --python_out=./ somename.proto

`pkg-config --cflags --libs protobuf`
g++ my_program.cc my_proto.pb.cc -lpthread -lprotobuf

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


# glog
- `LOG(INFO), LOG(WARNING), LOG(ERROR), LOG(FATAL)`: info 是0, warning 是1, fatal 级别会出core
- `--minloglevel=0`: 打印级别大于0的日志, 也就是INFO 及以上都会打印
- `--v=0`: 默认不打印vlog, eg: `vlog(100) << "vlog"` 则需要 --v=100 才能打印出来, 还可以配合`--vmodule` 进行文件级别的控制

# gflags
- `DEFINE_bool`: boolean
- `DEFINE_int32`: 32-bit integer
- `DEFINE_int64`: 64-bit integer
- `DEFINE_uint64`: unsigned 64-bit integer
- `DEFINE_double`: double
- `DEFINE_string`: C++ string

# gtest

# gperftools
```C++
#include <gperftools/profiler.h>

void func1(){
	int i = 0;
	while(i < 100000){
		++i;
	}
}

void func2() {
	int i = 0;
	while(i < 200000){
		++i;
	}
}

int main(){
	ProfilerStart("my.prof");
	for(int i = 0; i < 1000; ++i){
		func1();
		func2();
	}
	ProfilerStop();
	return 0;
}

g++ -std=c++11 -g -Wall test.cpp -lprofiler
google-pprof --text ./a.out my.prof // 二进制的命令老版本为 pprof
google-pprof --pdf ./a.out my.prof >output.pdf
```

# Google Protocol Buffer
Protocol Buffers (a.k.a., protobuf) are Google language-neutral, platform-neutral, extensible mechanism for serializing structured data.
You can find [protobuf documentation on the Google Developers site](https://developers.google.com/protocol-buffers/).

[Google Protocol Buffer 的使用和原理](http://www.ibm.com/developerworks/cn/linux/l-cn-gpb/#ibm-pcon)

## 数据类型
double, float, int32, int64, uint32, uint64, sint32, sint64, fixed32, fixed64, sfixed32, sfixed64, bool, string, bytes

- sint32/64: Uses variable-length encoding. Signed int value. These more efficiently encode negative numbers than regular int32/64.
- fixed32: Always four bytes. More efficient than uint32 if values are often greater than 228.
- bytes: May contain any arbitrary sequence of bytes.
- `map<key_type, value_type> map_field = N`; where the key type can be any integral or string type. The value type can be any type.

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

### Service
如果想要将消息类型用在RPC(远程方法调用)系统中,可以在.proto文件中定义一个RPC服务接口,protocol buffer编译器将会根据所选择的不同语言生成服务接口代码及存根.
如,想要定义一个RPC服务并具有一个方法,该方法能够接收 SearchRequest并返回一个SearchResponse,此时可以在.proto文件中进行如下定义:
```C++
service SearchService{
	rpc Search(SearchRequest) returns (SearchResponse);
}
```

RPC的调用过程(Stub这个术语应该是借鉴了JavaRMI):

1. Client向ClientStub发送请求(Call);
1. ClientStub对请求参数进行封包(也叫Marshalling), 发出系统调用, OS向Server端发送消息;
1. Server端接收到消息后, 把封包消息传递给ServerStub, ServerStub解包(UnMarshalling);
1. ServerStub调用Server端的子程序, 处理完毕后, 以同样的方式向Client端发送结果. 注:ServerStub又叫Skeleton.

[Stub](https://en.wikipedia.org/wiki/Stub_(distributed_computing)) 是一段代码, 用来转换RPC过程中传递的参数. 处理内容包括不同OS之间的大小端问题. 另外,Client端一般叫Stub,Server端一般叫Skeleton.
生产方式:1)手动生成,比较麻烦,2)自动生成,使用IDL(InterfaceDescriptionLanguate),定义C/S的接口.

## Options
在定义.proto文件时能够标注一系列的options.
Options并不改变整个文件声明的含义,但却能够影响特定环境下处理方式.完整的可用选项可以在google/protobuf/descriptor.proto找到.

`cc_generic_services`, `py_generic_services`: 在C++, python中protocol buffer编译器是否应该基于服务定义产生抽象服务代码.由于历史遗留问题,该值默认是true
- `option cc_generic_services = true;`

## 编译使用
1. 生成类文件
	- for cpp: `protoc -I=./ --cpp_out=./ somename.proto`
	- for python: `protoc -I=./ --python_out=./ somename.proto`
1. `g++ my_program.cc my_proto.pb.cc -lpthread -lprotobuf`
	- `pkg-config --cflags --libs protobuf`: 可以看到具体的依赖库

## C++ API
- class static method
	- `static const Descriptor* descriptor()`: 返回类型的descripter, 可用于反射
- numeric field
- string or bytes field: 即使设置为`""`, `has_foo()`也会返回true;
- enum fields
	- `bool ImageFeatureID_IsValid(int value);`
	- `const ImageFeatureID ImageFeatureID_MIN = FEATURE_MIN;`
	- `const ImageFeatureID ImageFeatureID_MAX = FEATURE_MAX;`
	- `const int ImageFeatureID_ARRAYSIZE = ImageFeatureID_MAX + 1;`
	- `const ::std::string& ImageFeatureID_Name(ImageFeatureID value);` 将 enum 转换为名字
	- `bool ImageFeatureID_Parse(const ::std::string& name, ImageFeatureID* value);` 将名字转换为enum

- repeated numeric fields
- map fields
	- proto def: `map<int32, int32> weight = 1;`
	- `const google::protobuf::Map<int32, int32> &weight()`: return an **immutable** map
	- `google::protobuf::Map<int32, int32>* mutable_weight()`: return a **mutable** map

序列化与反序列化

- std::string 作为容器
	- `bool SerializeToString(string* output) const;`
	- `bool ParseFromString(const string& data);`
- C char array 作为容器
	- `bool	SerializeToArray(void * data, int size) const`: size 为 data 的capacity
	- `bool	ParseFromArray(const void * data, int size)`: size 必须为message 的actual size, 通过 `int pb.ByteSize() const` 得到, 在序列化的时候要保存下来
- C++ stream
	- `bool Message::SerializeToOstream(std::ostream * output) const`: `#include <fstream>`
	- `bool Message::ParseFromIstream(std::istream * input)`
- C Descriptor
	- `bool Message::SerializeToFileDescriptor(int file_descriptor) const`: 文件需open 打开, `#include <fcntl.h>, #include <unistd.h>`
	- `bool Message::ParseFromFileDescriptor(int file_descriptor)`

debug

- `string DebugString() const`: Generates a human readable form of this message, useful for debugging and other purposes.
- `string ShortDebugString() const`: Like DebugString(), but with less whitespace.

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

```C++
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
```C++
message Test2{
	required string b = 2;
}
```
setting the value of b to "testing" gives us: `0x 12 07 74 65 73 74 69 6e 67`

1. `12 = 0001 0010`, 最后三个bits为`010 = 2`, 表示wire type 等于2, `0001 0`是field number, 也是2
1. `07 = 0000 0111`, value 部分的第一个byte 的第一个bit为0, 意味着这个表示data length的varint类型数据只有一个byte, 值为7, 也就是说后面紧跟着的7个bytes为value的data部分
1. `74 65 73 74 69 6e 67` 分别为 `t e s t i n g` 7个字符

### Embedded Messages
```C++
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


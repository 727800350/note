# glog
- 当不对glog 初始化而直接使用LOG(level) 时, 日志会打印到stderr
- 当初始化, 日志默认打印到/tmp 目录下, 文件名格式为: `<program name>.<hostname>.<user name>.log.<severity level>.<date>.<time>.<pid>`.
	也可以使用设置临时环境变量的方式(给命令行参数加上 `GLOG_` 前缀),如 `GLOG_logtostderr=1 ./your_application` 将日志输出到 stderr
- 如果有配套gflags 使用, 可以通过 gflags 的形式指定参数

## 日志类型
- `LOG(INFO), LOG(WARNING), LOG(ERROR), LOG(FATAL)`: info 是0, warning 是1, fatal 级别会出core
- VLOG: 自定义日志, 需要通过 `--v` 选项来控制输出
- DLOG: DEBUG模式可输出的日志

## 条件日志
- `LOG_IF(INFO, num_cookies > 10) << "Got lots of cookies";`: 当条件满足时输出日志
- `LOG_EVERY_N(INFO, 10) << "Got the " << google::COUNTER << "th cookie";`: google::COUNTER 记录该语句被执行次数,从1开始,在第一次运行输出日志之后,每隔 10 次再输出一次日志信息
- `LOG_IF_EVERY_N(INFO, (size > 1024), 10) << "Got the " << google::COUNTER << "th big cookie";`是先每隔 10 次去判断条件是否满足,如果滞则输出日志,而不是当满足某条件的情况下,每隔 10 次输出一次日志信息.
- `LOG_FIRST_N(INFO, 20) << "Got the " << google::COUNTER << "th cookie";`: 当此语句执行的前 20 次都输出日志,然后不再输出

## 控制选项
- `--minloglevel=0`: 打印级别大于0的日志, 也就是INFO 及以上都会打印
- `--v=0`: Show all VLOG(m) messages for m less or equal the value of this flag. Overridable by --vmodule. eg: `vlog(100) << "vlog"` 则需要 --v >= 100 才能打印出来
- `--vmodule(string, default="")`: 分文件(不包括文件名后缀,支持通配符)设置自定义日志的可输出级别. 如:`--vmodule=server=2,client=3` 表示文件名为`server.*` 的只输出小于 2 的日志,文件名为 `client.*` 的只输出小于 3 的日志.
- `VLOG_IS_ON(n)`: "verbose level" condition macro. returns true when the --v is equal or greater than n
- `--logbuflevel(int, default=0)`: 缓存小于或等于这个级别的日志信息. (-1表示不缓存; 0表示只缓存INFO级别日志; ...)
- `--logbufsecs(int, default=30)`: 最多缓存这么多秒的日志信息.
- `--log_backtrace_at=server_status_manager.cc:177`: 可以看到指定行的调用堆栈信息

## CHECK 宏
当通过该宏指定的条件不成立的时候,程序会中止,并且记录对应的日志信息.功能类似于ASSERT,区别是 CHECK 宏不受 NDEBUG 约束,在 release 版中同样有效.
```C++
CHECK(port == 80) << "HTTP port 80 is not exit.";
```

- `CHECK_NOTNULL`
- `CHECK_EQ, CHECK_NE, CHECK_LE, CHECK_LT, CHECK_GE, and CHECK_GT`
- `CHECK_STREQ, CHECK_STRNE, CHECK_STRCASEEQ, and CHECK_STRCASENE`
- `CHECK_DOUBLE_EQ`

`glog/logging.h` 文件中以 `CHECK_` 开头的宏定义.

# gflags
- `DEFINE_bool`: boolean
- `DEFINE_int32`: 32-bit integer
- `DEFINE_int64`: 64-bit integer
- `DEFINE_uint64`: unsigned 64-bit integer
- `DEFINE_double`: double
- `DEFINE_string`: C++ string

# gtest
## Basic Assertions
These assertions do basic true/false condition testing.

Fatal assertion            | Nonfatal assertion         | Verifies
-------------------------- | -------------------------- | --------------------
`ASSERT_TRUE(condition);`  | `EXPECT_TRUE(condition);`  | `condition` is true
`ASSERT_FALSE(condition);` | `EXPECT_FALSE(condition);` | `condition` is false

Remember, when they fail, `ASSERT_*` yields a fatal failure and returns from the current function, while `EXPECT_*` yields a nonfatal failure, allowing the function to continue running.
In either case, an assertion failure means its containing test fails.

### Binary Comparison
This section describes assertions that compare two values.

Fatal assertion          | Nonfatal assertion       | Verifies
------------------------ | ------------------------ | --------------
`ASSERT_EQ(val1, val2);` | `EXPECT_EQ(val1, val2);` | `val1 == val2`
`ASSERT_NE(val1, val2);` | `EXPECT_NE(val1, val2);` | `val1 != val2`
`ASSERT_LT(val1, val2);` | `EXPECT_LT(val1, val2);` | `val1 < val2`
`ASSERT_LE(val1, val2);` | `EXPECT_LE(val1, val2);` | `val1 <= val2`
`ASSERT_GT(val1, val2);` | `EXPECT_GT(val1, val2);` | `val1 > val2`
`ASSERT_GE(val1, val2);` | `EXPECT_GE(val1, val2);` | `val1 >= val2`

浮点数

- `ASSERT_FLOAT_EQ` and `ASSERT_DOUBLE_EQ`
- `ASSERT_NEAR`: 浮点值接近(第3个参数为误差阈值)

# gmock
除宏或其它特别提到的之外所有Google Mock名称都位于testing命名空间之下
```C++
EXPECT_CALL(mock_object, method(matcher1, matcher2, ...))
	.With(multi_argument_matcher)
	.Times(cardinality)
	.InSequence(sequences)
	.After(expectations)
	.WillOnce(action)
	.WillRepeatedly(action)
	.RetiresOnSaturation();
```
- `EXPECT_CALL`声明一个调用期待,就是我们期待这个对象的这个方法按什么样的逻辑去执行.
- `mock_object`是我们mock的对象,上例中就是TestUser的一个对象.
- Method是mock对象中的mock方法,它的参数可以通过matchers规则去匹配.
- With是多个参数的匹配方式指定.
- Times表示这个方法可以被执行多少次.
- InSequence用于指定函数执行的顺序.它是通过同一序列中声明期待的顺序确定的.
- After方法用于指定某个方法只能在另一个方法之后执行.
- WillOnce表示执行一次方法时,将执行其参数action的方法.一般我们使用Return方法,用于指定一次调用的输出.
- WillRepeatedly表示一直调用一个方法时,将执行其参数action的方法.需要注意下它和WillOnce的区别,WillOnce是一次,WillRepeatedly是一直.
- RetiresOnSaturation用于保证期待调用不会被相同的函数的期待所覆盖

```C++
EXPECT_CALL(mock_user, Pay(_)).Times(3).WillOnce(Return(true)).WillOnce(Return(true)).WillRepeatedly(Return(false))
```
调用`mock_user`的Pay()方法, 这个方法会调用3次, 第1次被调用时返回true, 第2次被调用时返回true, 从第3次被调用返回false.
如果想至少调用3次`Times(Atleaste(3))`

`EXPECT_THAT`, `ASSERT_THAT` 可以在gtest 中丰富判断行为

- `EXPECT_THAT(value, matcher)`: Asserts that value matches matcher
- `ASSERT_THAT(value, matcher)`: Asserts that value matches matcher

```C++
ASSERT_THAT("hello world", testing::HasSubStr("world"));

::std::set<int> page_ids;
page_ids.insert(3);
page_ids.insert(1);
EXPECT_THAT(page_ids, testing::Contains(1));
EXPECT_THAT(page_ids, testing::Contains(testing::Gt(2)));
EXPECT_THAT(page_ids, testing::Not(testing::Contains(4)));

::std::map<int, size_t> page_lengths;
page_lengths[1] = 100;
EXPECT_THAT(page_lengths, testing::Contains(::std::pair<const int, size_t>(1, 100)));

const char* user_ids[] = { "joe", "mike", "tom" };
EXPECT_THAT(user_ids, testing::Contains(testing::Eq(::std::string("tom"))));
```

# gperftools
- `pprof --base=./base.prof --text ./a.out new.prof`: 以base.prof 为基准来看new.prof

## cpu profiler
You can change the sampling frequency with the `CPUPROFILE_FREQUENCY` environment variable. Default value: 100

## heap profiler
- Figuring out what is in the program heap at any given time
- Locating memory leaks
- Finding places that do a lot of allocation

下面是一个文字输出样例
```plain
[root@dev:git]$ ./a.out
Starting tracking the heap
Dumping heap profile to heap.0001.heap (loop 0)
Dumping heap profile to heap.0002.heap (loop 1)
...
Dumping heap profile to heap.0009.heap (loop 8)
Dumping heap profile to heap.0010.heap (loop 9)
```

```plain
[root@dev:git]$ google-pprof --text ./a.out heap.0010.heap
Using local file ./a.out.
Using local file heap.0010.heap.
Total: 20.0 MB
    10.0  50.0%  50.0%     10.0  50.0% f2
     5.0  25.0%  75.0%      5.0  25.0% f
     5.0  25.0% 100.0%     10.0  50.0% f1
     0.0   0.0% 100.0%     20.0 100.0% __libc_start_main
     0.0   0.0% 100.0%     20.0 100.0% _start
     0.0   0.0% 100.0%     20.0 100.0% main
```
- 第一列表示直接占用的, 第二列表示对应的比例
- 第四列表示所占用的所有内存, 直接的和间接的都包括在内, 第五列表示对应的比例
- 第三列为第二列的累积和

通过`--base` 可以把某个heap 文件作为基准, 这样可以去掉一些初始化情况下的内存占用, 从而可以更加聚焦.
```plain
[root@dev:git]$ google-pprof --text ./a.out --base heap.0001.heap heap.0010.heap
Using local file ./a.out.
Using local file heap.0010.heap.
Total: 18.0 MB
    10.0  55.6%  55.6%     10.0  55.6% f2
     4.0  22.2%  77.8%      4.0  22.2% f
     4.0  22.2% 100.0%      8.0  44.4% f1
     0.0   0.0% 100.0%     18.0 100.0% __libc_start_main
     0.0   0.0% 100.0%     18.0 100.0% _start
     0.0   0.0% 100.0%     18.0 100.0% main
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

[TextFormat](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.text_format)

- `bool ParseFromString(const string &input, Message *output)`: Parses a text-format protocol message from the given input string(格式与 DebugString() 的输出一样) to the given message object.
```C++
#include <google/protobuf/text_format.h>
// google::protobuf::Message 是一个abstract type
bool ret = google::protobuf::TextFormat::ParseFromString(content, &message);
```

[Protobuf reflection](https://stackoverflow.com/questions/29960871/protobuf-message-object-creation-by-name)

The C++ Protobuf library already maintains a pool of "descriptors" for all types compiled into the binary.
The library also maintains an object that can be used to construct instances of any compiled-in type, given its descriptor:
```C++
google::Protobuf::Descriptor* desc = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName("mypkg.MyType");
assert(desc != NULL);
google::protobuf::Message* message = google::protobuf::MessageFactory::generated_factory()->GetPrototype(desc)->New();
```

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


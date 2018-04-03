# client
Client指发起请求的一端,在brpc中没有对应的实体,取而代之的是brpc::Channel,它代表和一台或一组服务器的交互通道,Client和Channel在角色上的差别在实践中并不重要,你可以把Channel视作Client.

Channel可以被所有线程共用,你不需要为每个线程创建独立的Channel,也不需要用锁互斥.不过Channel的创建和Init并不是线程安全的,请确保在Init成功后再被多线程访问,在没有线程访问后再析构.

```C++
brpc::ChannelOptions options; // 包含了默认值
options.xxx = yyy;
...
channel.Init(server, load_balancer, &options);
```
注意Channel不会修改options,Init结束后不会再访问options.所以options一般就像上面代码中那样放栈上.也就是init 之后释放是ok 的

Supported naming service("protocol://service name"):

- `bns://<node-name>`: Baidu Naming Service
- `file://<file-path>`: load addresses from the file
- `list://addr1,addr2,...`: use the addresses separated by comma
- `http://<url>`: Domain Naming Service, aka DNS.

Supported load balancer:

- rr: round robin, choose next server
- random: randomly choose a server
- la: locality aware
- `c_murmurhash/c_md5`: consistent hashing with murmurhash3/md5
	- 一致性哈希,与简单hash的不同之处在于增加或删除机器时不会使分桶结果剧烈变化,特别适合cache类服务.
	- 发起RPC前需要设置`Controller.set_request_code()`,否则RPC会失败.`request_code`一般是请求中主键部分的32位哈希值,不需要和负载均衡使用的哈希算法一致.比如用`c_murmurhash`算法也可以用md5计算哈希值.
	src/brpc/policy/hasher.h中包含了常用的hash函数.如果用std::string key代表请求的主键,`controller.set_request_code(brpc::policy::MurmurHash32(key.c_str(), key.length()))`就正确地设置了request_code.
	- 另外也要注意padding问题,比如`struct Foo {int32_t a; int64_t b;}`在64位机器上a和b之间有4个字节的空隙,内容未定义,如果像`hash(&foo, sizeof(foo))`这样计算哈希值, 结果就是未定义的,得把内容紧密排列或序列化后再算.

- "" or NULL: treat `naming_service_url` as `server_addr_and_port`

发起访问
一般来说,我们不直接调用Channel.CallMethod,而是通过protobuf生成的桩`XXX_Stub`, 过程更像是"调用函数".
stub内没什么成员变量,建议在栈上创建和使用,而不必new,当然你也可以把stub存下来复用. `Channel::CallMethod`和stub访问都是线程安全的,可以被所有线程同时访问.比如:

```C++
XXX_Stub stub(&channel);
stub.some_method(controller, request, response, done);
```
or

```C++
XXX_Stub(&channel).some_method(controller, request, response, done);
```

应该重用brpc::Controller吗?
不用刻意地重用,但Controller是个大杂烩,可能会包含一些缓存,Reset()可以避免反复地创建这些缓存.
在大部分场景下,构造Controller 和重置Controller 的性能差异不大.

## http
在http 下怎么设置refer?

### post 请求参数设置
channel init 中的server 还是要用server 的地址, 和get 不一样.
例如 `cq01-c1-vis-dog1.cq01:8889/stuui/watermarkbvc?version=v2&_from_openapi=1&rpc=1`, server 设置为`cq01-c1-vis-dog1.cq01:8889`, `cntl.uri = "/stuui/watermarkbvc?version=v2&_from_openapi=1&rpc=1"`

`cntl.request_attachment().append("{\"message\":\"hello world!\"}")`
append 的参数必须是 `{"key1": "value1", "key2": "value2"}` 的结构, 可以通过 jsoncpp 来构造.
在 server 端, 获取post 内容时, 获取到raw data, 要用json decode.

```C++
#include <json/json.h>
Json::Value root;
root["key1"] = "value1";
root["key2"] = "value2";
Json::FastWriter writer;
cntl.http_request().set_content_type("application/x-www-form-urlencoded");
cntl.request_attachment().append(writer.write(root));
```

也可以用`cntl.request_attachment().append("key1=value1&key2=value2")` 的形式, 但是value 需要用url encode.
实际上, 应该任何格式的数据都可以, 只要server 端获得raw data, 然后按照商定好的格式解析就行了.

# server
```C++
#include "echo.pb.h"

class MyEchoService : public EchoService{
public:
	void Echo(::google::protobuf::RpcController* cntl_base, const ::example::EchoRequest* request, ::example::EchoResponse* response, ::google::protobuf::Closure* done){
		// 这个对象确保在return时自动调用done->Run()
		brpc::ClosureGuard done_guard(done);

		brpc::Controller* cntl = static_cast<brpc::Controller*>(cntl_base);

		// 填写response
		response->set_message(request->message());
	}
};
```
Service在插入brpc.Server后才可能提供服务.
当客户端发来请求时,Echo()会被调用.参数的含义分别是:

- controller: 在brpc中可以静态转为brpc::Controller(前提是代码运行brpc.Server中),包含了所有request和response之外的参数集合,具体接口查阅controller.h
- request: 请求,只读的,来自client端的数据包.
- response: 回复.需要用户填充,如果存在required字段没有被设置,该次调用会失败.
- done: server 端的done 由框架创建,递给服务回调,包含了调用服务回调后的后续动作,包括检查response正确性,序列化,打包,发送等逻辑.

### done detail
不管成功失败,`done->Run()`必须在请求处理完成后被用户调用一次.
为什么框架不自己调用`done->Run()`?这是为了允许用户把done保存下来,在服务回调之后的某事件发生时再调用,即实现异步Service.

强烈建议使用ClosureGuard确保`done->Run()`被调用,即在服务回调开头的那句: `brpc::ClosureGuard done_guard(done);`
不管在中间还是末尾脱离服务回调, 都会使`done_guard`析构, 其中会调用`done->Run()`.这个机制称为RAII(Resource Acquisition Is Initialization也称为"资源获取就是初始化").没有这个的话你得在每次return前都加上`done->Run()`,极易忘记.

在异步Service中,退出服务回调时请求未处理完成,`done->Run()`不应被调用,done应被保存下来供以后调用,乍看起来,这里并不需要用ClosureGuard.
但在实践中,异步Service照样会因各种原因跳出回调,如果不使用ClosureGuard,一些分支很可能会在return前忘记`done->Run()`,所以我们也建议在异步service中使用`done_guard`,
与同步Service不同的是,为了避免正常脱离函数时`done->Run()`也被调用,你可以调用`done_guard.release()`来释放其中的done.

一般来说,同步Service和异步Service分别按如下代码处理done

```C++
class MyFooService: public FooService  {
public:
	// 同步服务
	void SyncFoo(::google::protobuf::RpcController* cntl_base, const ::example::EchoRequest* request, ::example::EchoResponse* response, ::google::protobuf::Closure* done){
		 brpc::ClosureGuard done_guard(done);
		 ...
	}
 
	// 异步服务
	void AsyncFoo(::google::protobuf::RpcController* cntl_base, const ::example::EchoRequest* request, ::example::EchoResponse* response, ::google::protobuf::Closure* done){
		 brpc::ClosureGuard done_guard(done);
		 ...
		 done_guard.release();
	}
};
```
ClosureGuard 的实现

```C++
// RAII: Call Run() of the closure on destruction.
class ClosureGuard {
public:
	ClosureGuard();
	// Constructed with a closure which will be Run() inside dtor.
	explicit ClosureGuard(google::protobuf::Closure* done) : _done(done) {}
	
	// Call Run() of internal closure if it's not NULL.
	~ClosureGuard(){
	   if (_done) {
			_done->Run();
		}
	}
 
	// Call Run() of internal closure if it's not NULL and set it to `done'.
	void reset(google::protobuf::Closure* done){
		if (_done) {
			_done->Run();
		}
		_done = done;
	}
 
	// Set internal closure to NULL and return the one before set.
	google::protobuf::Closure* release(){
		google::protobuf::Closure* const prev_done = _done;
		_done = NULL;
		return prev_done;
	}
};
```

### 标记当前调用为失败
调用`Controller.SetFailed()`可以把当前调用设置为失败, 当发送过程出现错误时,框架也会调用这个函数.
用户一般是在服务的CallMethod里调用这个函数,比如某个处理环节出错,SetFailed()后确认`done->Run()`被调用了就可以跳出函数了(若使用了ClosureGuard,跳出函数时会自动调用done,不用手动).
Server端的done的逻辑主要是发送response回client,当其发现调用了`SetFailed()`后, 会把错误信息送回client.
client收到后, 它的`Controller::Failed()`会为true(成功时为false), `Controller::ErrorCode()` 和 `Controller::ErrorText()` 则分别是错误码和错误信息.

用户可以为http访问设置status-code,在server端一般是调用`controller.http_response().set_status_code()`,
标准的status-code定义在`http_status_code.h`中. 如果SetFailed了但没有设置status-code,框架会代为选择和错误码最接近的status-code,实在没有相关的则填`brpc::HTTP_STATUS_INTERNAL_SERVER_ERROR`(500错误)

### 获取地址
`controller->remote_side()`可获得发送该请求的client地址和端口,类型是`butil::EndPoint`.
如果client是nginx, `remote_side()`是nginx的地址. 要获取真实client的地址,可以在nginx里设置`proxy_header ClientIp $remote_addr;`, 在rpc中通过`controller->http_request().GetHeader("ClientIp")`获得对应的值.

### 关闭闲置连接
如果一个连接在`ServerOptions.idle_timeout_sec`对应的时间内没有读取或写出数据,则被视为"闲置"而被server主动关闭.默认值为-1,代表不开启
打开`-log_idle_connection_close`后关闭前会打印一条日志.

```
Name                        Value   Description                                     Defined At
log_idle_connection_close   false   Print log when an idle connection is closed     src/brpc/socket.cpp 
```

限制最大消息

为了保护server和client,当server收到的request或client收到的response过大时,server或client会拒收并关闭连接.此最大尺寸由`-max_body_size`控制,单位为字节, 默认64MB.

# bvar
当很多线程都在累加一个计数器时,每个线程只累加私有的变量而不参与全局竞争,在读取时累加所有线程的私有变量.
虽然读比之前慢多了,但由于这类计数器的读多为低频的记录和展现,慢点无所谓.
而写就快多了,极小的开销使得用户可以无顾虑地使用bvar监控系统,这便是我们设计bvar的目的.

build

1. `sh config_brpc.sh --headers=/usr/include --libs=/usr/lib`
1. `make -j 4`


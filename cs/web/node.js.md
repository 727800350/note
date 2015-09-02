# Intro
The main idea of Node.js: use **non-blocking, event-driven I/O** to remain lightweight and efficient 
in the face of **data-intensive real-time** applications that run across distributed devices.

In Node, **blocking operations are the root of all evil**—99% of Node misuses come as a direct consequence.

Node.js shines in real-time web applications employing push technology over websockets. 
After over 20 years of stateless-web based on the stateless request-response paradigm, 
we finally have web applications with **real-time, two-way connections**, 
 where both the client and server can initiate communication, allowing them to exchange data freely.

You definitely do not want to use Node.js for CPU-intensive operations; in fact, using it for heavy computation will annul nearly all of its advantages. 
Where Node really shines is in building fast, scalable network applications, 
as it is capable of handling a huge number of simultaneous connections with high throughput, which equates to high scalability.

使用 Node.js,你可以轻松地开发:

- 具有复杂逻辑的网站,
- 基于社交网络的大规模 Web 应用,
- Web Socket 服务器,
- TCP/UDP 套接字应用程序,
- 命令行工具,
- 交互式终端程序,
- 带有图形用户界面的本地应用程序,
- 单元测试工具,
- 客户端 JavaScript 编译器.

Node.js 内建了 HTTP 服务器支持,也就是说你可以轻而易举地实现一个网站和服务器的组合.

# IO
## 输出
- `console.log`, 支持C 的printf 语法, eg: `console.log('%s: %d', 'Hello', 25);`
- `console.info`
- `console.warn`
- `console.error`
- `process.stdout.write` 类似于`console.log`, 但是建议使用console
- `process.stderr.write`

统计运行时间
```
console.time('str');
... code ...
console.timeEnd('str'); // 这个字符串与前面的字符串要一样
```

## 输入
```
process.stdin.setEncoding('utf-8');
process.stdin.on('data', function(data){console.log(data);});  // 注册data事件监听
process.stdin.on('readable', function(){var data = process.stdin.read(); console.log(data);});  
// 注册readable 事件监听, 但是这个事件不会将用户输入的参数主动传递给回调函数
```

##  变量
global: `__dirname`, `__filename`

exit 事件
`process,on('exit', function(){console.log('program will exit');});`

SIGINT 事件 signal interrupted
`process,on('SIGINT', function(){console.log('a sigint signal'); process.exit()});`

退出程序 `process.exit()`

`process.argv`

## File
fs.readFile  接收了三个参数, 文件名, 编码方式(optional),回调函数.
```
var fs = require('fs')
fs.readfile('file', function(err, data){
	if(err){
		console.error(err);
	}
	else{
		console.log(data);
	}
	})
```
默认的读取方式是读入到全局变量buffer中, 里面是ASCII 表的十六进制

转换成字符可以通过以下两种方法
`console.log(data.toString());` 或者
`fs.readfile('file', 'utf-8', function(err, data)`

同步读取, 不需要使用回调函数
```
var data = fs.readFileSync('file','utf-8');
```

```
var path = require('path')
path.sep  // seperator
path.extname('path')  // 包括那个点, 'path'不用真实存在
```
**阻塞与非阻塞**  
一个类似的例子: 银行里取号之后, 然后我们就可以去干其他的事情, 等轮到我们的时候, 银行的叫号机就会发出通知.  
[文件读取的异步与同步方式比较](/demo/node.js/sync_async.js)

# 模块和包
## 模块
Node.js的模块分为两类,一类为原生(核心)模块,一类为文件模块.

在文件模块中,又分为3类模块.这三类文件模块以后缀来区分,Node.js会根据后缀名来决定加载方法.

- .js: 通过fs模块同步读取js文件并编译执行.
- .node: 通过C/C++进行编写的Addon.通过dlopen方法进行加载.
- .json: 读取文件,调用JSON.parse解析加载.

Node.提供了exports和require两个对象,
其中exports是模块公开的接口,
require用于从外部获取一个模块接口,即所获取模块的exports对象.

只有module.exports指向的对象才回被导出.
module.exports才是真正的接口,exports只不过是它的一个辅助工具. 
最终返回给调用的是module.exports而不是exports. 
所有的exports收集到的属性和方法,都赋值给了module.exports.

通过 `require` 引入模块, 例如: `require('./sum.js')` (可以不写后缀`.js`)

模块中的功能(如: 变量, 函数)通过赋值给exports 对象的某个属性提供给调用者使用.
例如:
[sum 模块](/demo/node.js/sum.js), 
[sum 模块的测试](/demo/node.js/sum_test.js)

http server 实例:
```
var http = require('http');
var server = http.createServer(function(request, response){
	response.end('node.js course.');	
});
// 当server 收到浏览器的一个请求时, 回调函数就会被执行

server.listen(8080); // 第二个参数是IP地址, 不写的话就是默认的本地

// 或者把两者写在一起
http.createServer(function(request, response){}).server.listen(8080);
```

[http server with url demo](/demo/node.js/http_server.js)

Node.js 只有在第一次引用到某部份时才会去解析脚本文件,以后都会直接访问内存,避免重复载入.
而 PHP 则总是重新读取并解析脚本(如果没有专门的优化配置).
Node.js的这种设计虽然有利于提高性能,却不利于开发调试,因为我们在开发过程中总是希望代码修改后立即看到效果,而不是每次都要终止进程并重启.
supervisor 可以帮助你实现这个功能,它会监视你对代码的改动,并自动重启 Node.js.

1. 使用 npm 安装 supervisor: `$ npm install -g supervisor`
1. 使用 supervisor 命令启动 app.js: `$ supervisor app.js`

## 包
特殊的文件夹  
通过package.json 来描述包的信息: 入口文件, 依赖的外部包等

- npm install package: 默认情况下是本地模式,即把包安装到当前目录的 node_modules 子目录下, -g 为全局模式
- npm remove package

# event
```
var EventEmitter = require('events').EventEmitter;
var event = new EventEmitter();
event.on('some_event', function(){
	console.log('some_event occured.');
});
setTimeout(function(){
	event.emit('some_event');
}, 1000);
```
运行这段代码,1秒后控制台输出了some_event occured.
其原理是 event 对象注册了事件 some_event 的一个监听器, 然后我们通过setTimeout在1000毫秒以后向event对象发送事件some_event,此时会调用 some_event 的监听器.

Node.js 程序由事件循环开始,到事件循环结束,所有的逻辑都是事件的回调函数,
所以 Node.js 始终在事件循环中,程序入口就是事件循环第一个事件的回调函数.
事件的回调函数在执行的过程中,可能会发出 I/O 请求或直接发射(emit)事件,执行完毕后再返回事件循环,事件循环会检查事件队列中有没有未处理的事件,直到程序结束.

与其他语言不同的是,Node.js 没有显式的事件循环.
Node.js 的事件循环对开发者不可见,由libev库实现. 
libev支持多种类型的事件,如`ev_io`, `ev_timer`, `ev_signal`, `ev_idle` 等,在 Node.js 中均被EventEmitter  封装. 


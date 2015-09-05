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
- `console.log`, 会自动添加换行符, 支持C 的printf 语法, eg: `console.log('%s: %d', 'Hello', 25);`
- `console.info`
- `console.warn`
- `console.error`
- `console.trace()`: 向标准错误流输出当前的调用栈
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

## File
fs.readFile 接收了三个参数, 文件名, 编码方式(optional),回调函数.
```
var fs = require('fs');
fs.readfile('file', function(err, data){
	if(err){
		console.error(err);
	}
	else{
		console.log(data);
	}
});
```
默认的读取方式是读入到全局变量buffer中, 里面是ASCII 表的十六进制

转换成字符可以通过以下两种方法
`console.log(data.toString());` 或者
`fs.readfile('file', 'utf-8', function(err, data)`

同步读取, 不需要使用回调函数 `var data = fs.readFileSync('file','utf-8');`

`fs.open(path, flags, [mode], [callback(err, fd)])`: 
flags 与 C语言的flags 一样, mode 为创建文件时的权限, 默认为 0666

`fs.read(fd, buffer, offset, length, position, [callback(err, bytesRead, buffer)])`
是 POSIX read 函数的封装,相比 fs.readFile 提供了更底层的接口.

从fd 的 position 位置读取 length 个bytes, 存入到 buffer 的offset 位置.
position 是文件读取的起始位置,如果 position 的值为 null ,则会从当前文件指针的位置读取.
回调函数传递bytesRead 和buffer ,分别表示读取的字节数和缓冲区对象.

fs.open 与 fs.read 示例
```
var fs = require('fs');
fs.open('content.txt', 'r', function(err, fd){
	if(err){
		console.error(err);
		return;
	}
	var buf = new Buffer(8);
	fs.read(fd, buf, 0, 8, null, function(err, bytesRead, buffer){
		if(err){
			console.error(err);
			return;
		}
	console.log('bytesRead: ' + bytesRead);
	console.log(buffer);
	})
});
```
一般来说,除非必要,否则不要使用这种方式读取文件,因为它要求你手动管理缓冲区和文件指针,
尤其是在你不知道文件大小的时候,这将会是一件很麻烦的事情.


```
var path = require('path')
path.sep  // seperator
path.extname('path')  // 包括那个点, 'path'不用真实存在
```
**阻塞与非阻塞**  
一个类似的例子: 银行里取号之后, 然后我们就可以去干其他的事情, 等轮到我们的时候, 银行的叫号机就会发出通知.  
[文件读取的异步与同步方式比较](/demo/node.js/sync_async.js)

# 变量
在浏览器 JavaScript 中,通常 `window` 是全局对象,
而 Node.js 中的全局对象是`global`, 所有全局变量(除了global本身以外)都是global对象的属性.
我们在 Node.js 中能够直接访问到对象通常都是 global 的属性,如 console , process等

## process
它用于描述当前 Node.js 进程状态的对象,提供了一个与操作系统的简单接口.通常在你写本地命令行程序的时候,少不了要和它打交道

process.argv 是命令行参数数组,第一个元素是 node, 第二个元素是脚本文件名(绝对路径), 从第三个元素开始每个元素是一个运行参数

process.stdout 是标准输出流,
通常我们使用的 console.log() 向标准输出打印字符,
而 process.stdout.write() 函数提供了更底层的接口

process.stdin 是标准输入流,初始时它是被暂停的,要想从标准输入读取数据,
你必须恢复流,并手动编写流的事件响应函数.
```
process.stdin.resume();
process.stdin.on('data', function(data){
	process.stdout.write('read from console: ' + data.toString());
});
```

process.nextTick(callback) 的功能是为事件循环设置一项任务, Node.js 会在下次事件循环调响应时调用 callback.
可以把复杂的工作拆散,变成一个个较小的事件

退出程序 `process.exit()`

除此之外 process 还具有 process.platform, process.pid, process.execPath, process.memoryUsage()

exit 事件: `process.on('exit', function(){console.log('program will exit');});`

SIGINT 事件 signal interrupted: `process.on('SIGINT', function(){console.log('a sigint signal'); process.exit()});`

## 闭包 Closure
函数内部声明变量的时候,一定要使用var命令.如果不用的话,你实际上声明了一个全局变量!
```
function f1(){
	n=999;
}
f1();
console.log(n); // 999
```
但是如果不调用f1, n 还是访问不到

出于种种原因,我们有时候需要得到函数内的局部变量.
但是,前面已经说过了,正常情况下,这是办不到的,只有通过变通方法才能实现.
那就是在函数的内部,再定义一个函数.
```
function f1(){
	var n=999;
	function f2(){
		console.log(n); // 999
	}
}
```
在上面的代码中,函数f2就被包括在函数f1内部,这时f1内部的所有局部变量,对f2都是可见的.
但是反过来就不行,f2内部的局部变量,对f1就是不可见的.
子对象会一级一级地向上寻找所有父对象的变量.所以,父对象的所有变量,对子对象都是可见的,反之则不成立.

既然f2可以读取f1中的局部变量,那么只要把f2作为返回值,我们不就可以在f1外部读取它的内部变量了吗!
```
function f1(){
	var n=999;
	function f2(){
		console.log(n);
	}
	return f2;
}

var result=f1();
result(); // 999
```

上一节代码中的f2函数,就是闭包.
由于在Javascript语言中,只有函数内部的子函数才能读取局部变量,因此可以把闭包简单理解成"定义在一个函数内部的函数".

### 闭包的用途
闭包可以用在许多地方.它的最大用处有两个,一个是前面提到的可以读取函数内部的变量,另一个就是让这些变量的值始终保持在内存中.
```
function f1(){
	var n=999;
	nAdd = function(){n+=1}
	function f2(){
		console.log(n);
	}
	return f2;
}
var result=f1();
result(); // 999
nAdd();
result(); // 1000
```
在这段代码中,result实际上就是闭包f2函数.它一共运行了两次,第一次的值是999,第二次的值是1000.
这证明了,函数f1中的局部变量n一直保存在内存中,并没有在f1调用后被自动清除.
为什么会这样呢?
原因就在于f1是f2的父函数,而f2被赋给了一个全局变量,这导致f2始终在内存中,而f2的存在依赖于f1,因此f1也始终在内存中,不会在调用结束后,被垃圾回收机制(garbage collection)回收.
nAdd是一个全局变量, nAdd的值是一个匿名函数(anonymous function),而这个匿名函数本身也是一个闭包,
所以nAdd相当于是一个setter,可以在函数外部对函数内部的局部变量进行操作, 而 result 也就是返回的f2 相当于一个 getter, 用于获取内部变量的状态.

# [this](https://github.com/alsotang/node-lessons/blob/master/lesson11/README.md)
在函数执行时,this 总是指向调用该函数的对象.要判断 this 的指向,其实就是判断 this 所在的函数属于谁.

在<javaScript语言精粹>这本书中,把 this 出现的场景分为四类,简单的说就是:

* 有对象就指向调用对象
* 没调用对象就指向全局对象
* 用new构造就指向新对象
* 通过 apply 或 call 或 bind 来改变 this 的所指.

1)函数有所属对象时:指向所属对象

函数有所属对象时,通常通过 `.` 表达式调用,这时 `this` 自然指向所属对象.比如下面的例子:

```js
var myObject = {value: 100};
myObject.getValue = function(){
  console.log(this.value);  // 输出 100

  console.log(this);
  // 输出 { value: 100, getValue: [Function] },
  // 其实就是 myObject 对象本身

  return this.value;
};

console.log(myObject.getValue()); // => 100
```
`getValue()` 属于对象 `myObject`,并由 `myOjbect` 进行 `.` 调用,因此 `this` 指向对象 `myObject`.

2) 函数没有所属对象:指向全局对象

```js
var myObject = {value: 100};
myObject.getValue = function(){
  var foo = function(){
    console.log(this.value) // => undefined
    console.log(this);// 输出全局对象 global
  };

  foo();

  return this.value;
};
console.log(myObject.getValue()); // => 100
```
在上述代码块中,`foo` 函数虽然定义在 `getValue` 的函数体内,但实际上它既不属于 `getValue` 也不属于 `myObject`.
`foo` 并没有被绑定在任何对象上,所以当调用时,它的 `this` 指针指向了全局对象 `global`.

据说这是个设计错误.

3) 构造器中的 this:指向新对象

js 中,我们通过 `new` 关键词来调用构造函数,此时 this 会绑定在该新对象上.

```js
var SomeClass = function(){
  this.value = 100;
}
var myCreate = new SomeClass();
console.log(myCreate.value); // 输出100
```
顺便说一句,在 js 中,构造函数,普通函数,对象方法,闭包,这四者没有明确界线.界线都在人的心中.

4) apply 和 call 调用以及 bind 绑定:指向绑定的对象

- apply() 方法接受两个参数第一个是函数运行的作用域,另外一个是一个参数数组(arguments).
- call() 方法第一个参数的意义与 apply() 方法相同,只是其他的参数需要一个个列举出来.

简单来说,call 的方式更接近我们平时调用函数,而 apply 需要我们传递 Array 形式的数组给它.它们是可以互相转换的.
```js
var myObject = {value: 100};

var foo = function(){
  console.log(this);
};

foo(); // 全局变量 global
foo.apply(myObject); // { value: 100 }
foo.call(myObject); // { value: 100 }

var newFoo = foo.bind(myObject);
newFoo(); // { value: 100 }
```

练习
```
var name = "The Window";
var object = {
	name : "My Object",
	getNameFunc : function(){
		var foo =  function(){
			return this.name;
		};
		return foo;
	}
};
console.log(object.getNameFunc()());
```
因为 `object.getNameFunc` 是一个function, 所以需要使用`getNameFunc()`, 而`getNameFunc()`的返回仍然是一个function, 所以需要继续使用 `getNameFunc()()`.  
getNameFunc 里面, this 是 object, 这点没有疑问, 但是getNameFunc 的里面的里面, this 就是没有绑定的, 所以是global, 而global 又没有name 这个property, 所以输出 `undefined`.  
如果把 `return foo` 换成 `return foo.bind(this)`, 则可以正确地输出 My Object. 因为这里将 object 绑定到了 foo, 也就是改变了foo 里面的this 指向.

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

http.Server 是一个基于事件的 HTTP 服务器.
http.request 则是一个HTTP 客户端工具

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


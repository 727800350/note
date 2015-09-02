# IO
## 输出
- `console.log`
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
[文件读取的异步与同步方式比较](../demo/node.js/sync_async.js)

# 模块和包
## 模块
通过 `require` 引入模块, 例如
```
require('./sum.js')
```
我们可以不写后缀`.js`

模块中的功能(如: 变量, 函数)通过赋值给exports 对象的某个属性提供给调用者使用.
例如:  
[sum 模块](../demo/node.js/sum.js)  
[sum 模块的测试](../demo/node.js/sum_test.js)

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

[http server with url demo](../demo/node.js/http_server.js)

## 包
特殊的文件夹  
通过package.json 来描述包的信息: 入口文件, 依赖的外部包等
```
npm install package
npm remove package
```

var http = require('http');
var url = require('url');

var server = http.createServer(function(request, response){
	var urlObj = url.parse(request.url);
	var pathname = urlObj.pathname;
	var query = urlObj.query;
	response.end('node.js course.');	
});
// 当server 收到浏览器的一个请求时, 回调函数就会被执行

server.listen(8080); // 第二个参数是IP地址, 不写的话就是默认的本地

// 或者
http.createServer(function(request, response){}).server.listen(8080);

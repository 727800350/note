/* *
 * demo 1
 * 先异步读取, 后同步读取
 * */
var fs = require('fs');

console.log('start async read.');
fs.readFile('sum.js', 'utf-8', function(err, data){
	if(err){
		console.log(err);
	}
	else{
		console.log('end async read.');
	}
});

console.log('start sync read.');
var data = fs.readFileSync('log.txt', 'utf-8');
console.log('end sync read.');

/**
output
start async read.
start sync read.
end sync read.
end async read.
可以看出异步读取文件, 不阻塞程序的继续运行
 **/


/* *
 * demo 2
 * 先同步读取, 后异步读取
 * */
var fs = require('fs');

console.log('start sync read.');
var data = fs.readFileSync('log.txt', 'utf-8');
console.log('end sync read.');

console.log('start async read.');
fs.readFile('sum.js', 'utf-8', function(err, data){
	if(err){
		console.log(err);
	}
	else{
		console.log('end async read.');
	}
});

/**
output
start sync read.
end sync read.
start async read.
end async read.
**/

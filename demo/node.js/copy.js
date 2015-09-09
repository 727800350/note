var fs = require('fs');
var path = require('path');
var out = process.stderr;

var readStream = process.stdin;
var writeStream = process.stdout;

var passedLength = 0;
var lastSize = 0;
var startTime = Date.now();

readStream.on('data', function(chunk){
    passedLength += chunk.length;
    if(writeStream.write(chunk) === false) {
        readStream.pause();
    }
});

var done = false;
readStream.on('end', function() {
	done = true;
});

// 上面的写法有一些问题,如果写入的速度跟不上读取的速度,有可能导致数据丢失.
// 正常的情况应该是,写完一段,再读取下一段,如果没有写完的话,就让读取流先暂停,等写完再继续,于是代码可以修改为:
writeStream.on('drain', function() {
    readStream.resume();
});

setTimeout(function show(){
    var size = Math.ceil(passedLength / 1000000);
    var diff = size - lastSize;
    lastSize = size;
    out.clearLine();
    out.cursorTo(0);
    out.write('Done' + size + 'MB, speed:' + diff * 2 + 'MB/s');
    if(done){
        var endTime = Date.now();
        console.error();
        console.error('It takes ' + (endTime - startTime) / 1000 + ' secs');
    }
	else{
        setTimeout(show, 500);
    }
}, 500);


#!/bin/bash

function add(){
	echo $1,$2
	return $(($1 + $2))
}

add 1 2
echo $?
## 这个例子说明, 定义的函数和普通的shell 命令没有区别, 用法是一样的

r=`add 3 4`
echo $?
echo $r
echo $r
函数返回值,只能通过$?系统变量获得,
直接通过=,获得是函数里面的echo 语句输出的结果
其实,我们按照上面一条理解,知道函数是一个命令,在shell获得命令返回值,都需要通过$?获得

r=$(add 5 6)
echo $?.
echo ${r}

## output:
## Press ENTER or type command to continue
## 1,2
## 3
## 7
## 3,4
## 3,4
## 11.
## 5,6


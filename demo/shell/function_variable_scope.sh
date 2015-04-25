#!/bin/bash
 
echo $(uname);
declare num=1000;
 
uname(){
	echo "in user defined function uname!";
	((num++));
	return 100;
}

testvar(){
	echo "in user defined function testvar!";
	local num=10;
	((num++));
	echo $num;
}
 
## Linux

uname
## in user defined function uname!
echo $?
## 100

echo $num
## 1001

testvar
## in user defined function testvar!
## 11

echo $num
## 1001


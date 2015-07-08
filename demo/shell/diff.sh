#!/bin/bash

<<COM
function:
cs1 flag1
cs1 flag2 (flag2 != flag1)
==> cs1
COM

last=""
sum=0
while read line
do
	this=`echo ${line} | awk '{print $1}'`
	value=`echo ${line} | awk '{print $2}'`
	if [ "x$last" = "x" ]
	then
		last=$this
		sum=$value
	else
		if [ "$this" = "$last" ]
		then
			sum=`expr $sum + $value`
		else
			last=$this
			sum=$value
		fi
	fi
done


#!/bin/bash

<<COM
function:
cs1 flag1
cs1 flag2 (flag2 != flag1)
==> cs1
COM

cs_old=""
flag_old=""
while read line
do
	cs=`echo ${line} | awk '{print $1}'`
	flag=`echo ${line} | awk '{print $2}'`
	if [ "x$cs_old" = "x" ]
	then
		cs_old=$cs
		flag_old=$flag
	else
		if [ "$cs" = "$cs_old" ]
		then
			if [ $flag -ne $flag_old ]
			then
				echo $cs
			fi
		else
			cs_old=$cs
			flag_old=$flag
		fi
	fi
done


#!/bin/bash
## supervise the running status of certain process

while [[ 1 ]]
do
	## num=`ps aux | grep wangchao34 | grep -v grep | wc -l | awk '{print $1}'`
	num=`ps aux | grep "bash run.sh" | grep -v grep | wc -l | awk '{print $1}'`
	if [ ${num} -eq 0 ]
	then
		echo "task over"
## 		poweroff
	else
		echo "task is still running, `date`"
		sleep 1800
	fi
done


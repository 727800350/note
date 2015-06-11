#!/bin/bash
## supervise the running status of certain process

while [[ 1 ]]
do
	num=`ps aux | grep "bash ./all_loop.sh" | grep -v grep | wc -l | awk '{print $1}'`
	if [ ${num} -eq 0 ]
	then
		echo "task over, `date`"
		sleep 60
		poweroff
	else
		echo "task is still running, `date`"
		sleep 1800
	fi
done


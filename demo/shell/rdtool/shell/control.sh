#########################################################################
# File Name: transfer.sh
# Author: Eric Wang
# Email: wangchaogo1990 at gmail.com
# Created Time: 2017-11-24 23:07:18
#########################################################################
#!/bin/bash
set -x

if [ $# -ne 3 ]
then
	echo "usage: $0 operation machine.list capacity" >&2
	exit 1
fi

op=$1
machine=$2
capacity=$3

[ ! -d status ] && mkdir status
rm -rf ./status/*.$op

>$op.err
i=0
while read host
do
	{
	scp -r -o StrictHostKeyChecking=no ./shell/$op.sh work@$host:/home/work/
	if [ $? -ne 0 ]
	then
		echo "scp to $host error" >&2
		exit 1
	fi

	ssh work@$host "bash /home/work/$op.sh" </dev/null
	if [ $? -ne 0 ]
	then
		echo "$op on $host error" >&2
		echo "$host" >>$op.err
		exit 1
	fi

	touch ./status/$host.$op
	} &

	i=`expr $i + 1`
	if [ $i -ge $capacity ]
	then
		wait
		i=0
	fi
done < $machine

wait

while read host
do
	if [ ! -f ./status/$host.$op ]
	then
		echo "$op on $host error" >&2
		exit 1
	fi
done < $machine

if [ ! -s $op.err ]
then
	rm $op.err
fi

exit 0


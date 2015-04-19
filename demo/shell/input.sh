#!/bin/bash
## redirect the input

input="input"
if [ ! -e $input ]
then
	echo "file $input does not exist, exiting..."
	exit
else
	echo "reading from file $input"
fi

exec 0< $input

count=1
while read line
do
	echo "Line $count: $line"
	count=$[ $count + 1 ]
done


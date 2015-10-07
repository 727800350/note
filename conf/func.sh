#!/bin/bash

## check return value
#FATAL $Msg
function FATAL(){
	echo "FATAL `date +"%F %T"` : $@" >&2
	exit -1;
}

#WARNING $Msg
function WARNING(){
	echo "WARNING `date +"%F %T"` : $@" >&2
}

#NOTICE $Msg
function NOTICE(){
	echo "NOTICE `date +"%F %T"` : $@" >&2
}

#CHK_RET $LEVEL $Msg
function CHK_RET(){
	if [ $? -ne 0 ]
	then
		$@
	fi
}

## ex:safe_run "${_hadoop_cmd} -copyToLocal $file_dir thumail.data"
function safe_run(){
	cmd=$1  

	eval $cmd
	if [ $? -ne 0 ] 
	then	
	   exit -1 
	fi

	return 0
}

## ex:safe_duprun "${_hadoop_cmd} -copyToLocal $file_dir thumail.data" 5
function safe_duprun(){
	cmd=$1
	trynum=$2
	ret=-1

	for((i=0;i<trynum;i++))
	do
		eval $cmd
		if [ $? -eq 0 ]
		then
			ret=0
			break
		fi
	done

	if [ $ret -ne 0 ]
	then
		exit -1
	fi

	return 0
}


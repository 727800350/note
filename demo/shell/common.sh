#!/bin/bash
## commonly used functions

# check existence of regular file or directory
function exist_e(){
	file=$1
	if [ ! -e ${file} ]
	then
		echo "${file} does not exist." >&2
		return 1
	fi
	return 0
}

function exist_es(){
	files=$*
	for file in ${files}
	do
		exist_e ${file} || return 1
	done
	return 0
}

function exist_f(){
	file=$1
	if [ ! -f ${file} ]
	then
		echo "${file} does not exist." >&2
		return 1
	fi
	return 0
}

function exist_fs(){
	files=$*
	for file in ${files}
	do
		exist_f ${file} || return 1
	done
	return 0
}

function exist_d(){
	dir=$1
	if [ ! -d ${dir} ]
	then
		echo "${dir} does not exist." >&2
		return 1
	fi
	return 0
}

function exist_ds(){
	dirs=$*
	for dir in ${dirs}
	do
		exist_d ${dir} || return 1
	done
	return 0
}


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

## ex:safe_duprun "${_hadoop_cmd} -copyToLocal $file_dir thumail.data"
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
     ret=0

     for((i=0;i<trynum;i++))
     do
          eval $cmd
          if [ $? -eq 0 ]
          then
              ret=1
              break
          fi
     done

     if [ $ret -ne 1 ]
     then
         exit -1
     fi

     return 0
}


#!/bin/bash
set -x
set -o pipefail

if [ $# -eq 1 ]
then
	local=$1
else
	echo "arg error" >&2
	echo "usage: bash ./shell/mapper 1(local)|0(cluster)" >&2
	exit 1
fi

if [ $local -eq 1 ]
then
	conf="./conf/"
	bin="./bin/"
	shell="./shell/"
else
	conf="./"
	bin="./"
	shell="./"
fi

source $conf/common.conf || exit 1
source $conf/func.sh || exit 1

if [ $local -eq 1 ]
then
	hadoop_fs="$local_hadoop dfs"
	python="/usr/bin/python"
	id=0
else
	hadoop_fs="$cluster_hadoop_fs"
	python="./Python-2.7.5/python"
	id=${mapred_task_partition}
	chmod +x $bin/*
	mkdir log && chmod 777 log
fi

cat - > input

cat input | cmd >res
if [ $? -ne 0 ]
then
	$hadoop_fs -rm $mapred_output_dir/input.$id
	$hadoop_fs -put input $mapred_output_dir/input.$id
	exit 1
fi
$hadoop_fs -rm $mapred_output_dir/input.$id

if [ $local -eq 0 -a -s res ]
then
	$hadoop_fs -put res ${mapred_work_output_dir}/res.$id
	CHK_RET FATAL "put res error"
fi

if [ $local -eq 0 -a `expr $id % $reducer_capacity` -eq 0 ]
then
	$hadoop_fs -ls $mapred_output_dir | grep part | awk '{print $NF}' | head -n $reducer_capacity | while read line
	do
		$hadoop_fs -rm $line
	done
fi

exit 0


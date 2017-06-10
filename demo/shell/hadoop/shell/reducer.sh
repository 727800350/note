#!/bin/bash
set -x

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
fi

chmod +x $bin/*
mkdir log && chmod 777 log

## ${mapred_work_output_dir}/res.$id

exit 0


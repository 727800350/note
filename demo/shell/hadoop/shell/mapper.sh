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
	hadoop_fs="$local_hadoop dfs"
else
	conf="./"
	bin="./"
	shell="./"
	hadoop_fs="$cluster_hadoop_fs"
fi

source $conf/common.conf || exit 1
source $conf/func.sh || exit 1

exit 0


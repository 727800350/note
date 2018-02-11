#########################################################################
# File Name: shell/fix.sh
# Author: Eric Wang
# Email: wangchaogo1990 at gmail.com
# Created Time: 2017-11-25 17:59:10
#########################################################################
#!/bin/bash
set -x

cd /home/new_mount_point1/

rm -rf simid_bs_0/log/*
rm -rf simid_wbs_0/log/*
rm -rf simid_as_0/log/*

host=`hostname`
if [ "x$host" == "xyq01-sys-replace222.yq01.baidu.com" ]
then
	exit 0
fi

rm -rf simid_bs_1/log/*
rm -rf simid_wbs_1/log/*
rm -rf simid_as_1/log/*

exit 0


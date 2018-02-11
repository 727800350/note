#########################################################################
# File Name: shell/fix.sh
# Author: Eric Wang
# Email: wangchaogo1990 at gmail.com
# Created Time: 2017-11-25 17:59:10
#########################################################################
#!/bin/bash
set -x

cd /home/new_mount_point1/

pstree | grep simid_as_0 >&2
if [ $? -eq 0 ]
then
	cd simid_as_0/bin
	./as_control stop
	if [ $? -ne 0 ]
	then
		echo "stop as 0 error on `hostname`" >&2
		exit 1
	fi
	cd -
fi

pstree | grep simid_bs_0 >&2
if [ $? -eq 0 ]
then
	cd simid_bs_0/bin
	./bs_control stop
	if [ $? -ne 0 ]
	then
		echo "stop bs 0 error on `hostname`" >&2
		exit 1
	fi
	cd -
fi

pstree | grep simid_wbs_0 >&2
if [ $? -eq 0 ]
then
	cd simid_wbs_0/bin
	./wbs_control stop
	if [ $? -ne 0 ]
	then
		echo "stop wbs 0 error on `hostname`" >&2
		exit 1
	fi
	cd -
fi

host=`hostname`
if [ "x$host" == "xyq01-sys-replace222.yq01.baidu.com" ]
then
	exit 0
fi

pstree | grep simid_as_1 >&2
if [ $? -eq 0 ]
then
	cd simid_as_1/bin
	./as_control stop
	if [ $? -ne 0 ]
	then
		echo "stop as 1 error on `hostname`" >&2
		exit 1
	fi
	cd -
fi

pstree | grep simid_bs_1 >&2
if [ $? -eq 0 ]
then
	cd simid_bs_1/bin
	./bs_control stop
	if [ $? -ne 0 ]
	then
		echo "stop bs 1 error on `hostname`" >&2
		exit 1
	fi
	cd -
fi

pstree | grep simid_wbs_1 >&2
if [ $? -eq 0 ]
then
	cd simid_wbs_1/bin
	./wbs_control stop
	if [ $? -ne 0 ]
	then
		echo "stop wbs 1 error on `hostname`" >&2
		exit 1
	fi
	cd -
fi

exit 0


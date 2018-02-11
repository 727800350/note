#########################################################################
# File Name: shell/fix.sh
# Author: Eric Wang
# Email: wangchaogo1990 at gmail.com
# Created Time: 2017-11-25 17:59:10
#########################################################################
#!/bin/bash
set -x

cd /home/new_mount_point1/
cd simid_bs_0
bash -x shell/init.sh
if [ $? -ne 0 ]
then
	echo "init error on `hostname`" >&2
	exit 1
fi
cd -

cd simid_bs_1
bash -x shell/init.sh
if [ $? -ne 0 ]
then
	echo "init error on `hostname`" >&2
	exit 1
fi
cd -

cd simid_wbs_0
bash -x shell/init.sh
if [ $? -ne 0 ]
then
	echo "init error on `hostname`" >&2
	exit 1
fi
cd -

cd simid_wbs_1
bash -x shell/init.sh
if [ $? -ne 0 ]
then
	echo "init error on `hostname`" >&2
	exit 1
fi
cd -

exit 0


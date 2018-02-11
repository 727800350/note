#########################################################################
# File Name: shell/fix.sh
# Author: Eric Wang
# Email: wangchaogo1990 at gmail.com
# Created Time: 2017-11-25 17:59:10
#########################################################################
#!/bin/bash
set -x

cd /home/new_mount_point1/

>info
echo "`hostname`" >>info
pstree | grep simid_bs  | grep -v rt_simid_bs | sort >>info
pstree | grep simid_wbs | sort >>info
pstree | grep simid_as  | sort >>info

msg=""
while read line
do
	msg="$msg $line"
done < info

echo "$msg"
rm info

exit 0


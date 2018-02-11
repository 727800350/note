#########################################################################
# File Name: transfer.sh
# Author: Eric Wang
# Email: wangchaogo1990 at gmail.com
# Created Time: 2017-11-24 23:07:18
#########################################################################
#!/bin/bash
set -x

while read host
do
## 	scp -r -o StrictHostKeyChecking=no ./../bin root@$host:/root/ ## only first time
## 	rsync -av /etc/security/limits.conf root@$host:/etc/security/limits.conf
## 	rsync -av /home/work/hadoop-client /home/work/share work@$host:/home/work/
	rsync -av /home/new_mount_point1/simid_as_0  /home/new_mount_point1/simid_as_1  --exclude=log --exclude=status work@$host:/home/new_mount_point1/
## 	rsync -av /home/new_mount_point1/simid_bs_0  /home/new_mount_point1/simid_bs_1  --exclude=log --exclude=status --exclude=csinfodb work@$host:/home/new_mount_point1/
## 	rsync -av /home/new_mount_point1/simid_wbs_0 /home/new_mount_point1/simid_wbs_1 --exclude=log --exclude=status --exclude=csinfodb work@$host:/home/new_mount_point1/
	if [ $? -ne 0 ]
	then
		echo "sync to $host error" >&2
		exit 1
	fi
done

exit 0


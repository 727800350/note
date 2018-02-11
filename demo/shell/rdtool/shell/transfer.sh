#########################################################################
# File Name: transfer.sh
# Author: Eric Wang
# Email: wangchaogo1990 at gmail.com
# Created Time: 2017-11-24 23:07:18
#########################################################################
#!/bin/bash
set -x

user="img"

while read host
do
## 	scp -r -o StrictHostKeyChecking=no ./../bin $user@$host:/home/$user/ ## only first time
## 	rsync -av /home/$user/hadoop-client /home/$user/share $user@$host:/home/$user/
	rsync -av /home/new_mount_point1/simid_as_0  /home/new_mount_point1/simid_as_1  --exclude=log --exclude=status $user@$host:/home/new_mount_point1/
	if [ $? -ne 0 ]
	then
		echo "sync to $host error" >&2
		exit 1
	fi
done

exit 0


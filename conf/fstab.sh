#!/bin/bash
set -x

if [ -e /dev/sda ]
then
	num_a=`ls -l /dev/sda* | wc -l | awk '{print $1}'`
else
	num_a=0
fi

if [ -e /dev/sdb ]
then
	num_b=`ls -l /dev/sdb* | wc -l | awk '{print $1}'`
else
	num_b=0
fi
if [ ${num_a} -gt ${num_b} ]
then
	sep="a"
else
	sep="b"
fi

echo "uuid" >&2
ls -l /dev/disk/by-uuid/ | grep -v total | awk '{print $9, $NF}' | awk -F '../' '{print $NF, 0, $1}' | sort -t ${sep} -k 2 -n > uuid.tmp

echo "df" >&2
df | grep `whoami` | awk '{print $1, $NF}' | awk -F '/' '{print $3, 1, $NF}' | sort -t ${sep} -k 2 -n > df.tmp

cat uuid.tmp df.tmp | sort -t ${sep} -k 2 -n | awk '
BEGIN{
	df=""
## 	uuid or name
	un=""
}
{
## 	new record
	if(df == ""){
		df = $1
		un = $3
	}
## 	same as last record
	else{
		if($1 == df){
			if($2 == 0){
				print $3, un
			}
			if($2 == 1){
				print un, $3
			}
			df=""
		}
	}
}
' > tmp

cat /etc/fstab > fstab
cat tmp | awk '{print "UUID="$1, "/media/"$2, "ntfs defaults 0 0"}' >> fstab

cat tmp | while read line
do
	label=`echo $line | awk '{print $2}'`
	target="/media/${label}"
	if [ ! -d ${target} ]
	then
		sudo mkdir -p ${target}
	fi
done

rm uuid.tmp df.tmp tmp

cat fstab
echo "the next step is to replace fstab: Y/N"
read line
if [ ${line} = "Y" ]
then
	sudo mv /etc/fstab /etc/fstab.`date +%Y%m%d_%H_%M_%S`
	sudo mv fstab /etc/fstab
else
	echo "you choose N"
fi

exit 0


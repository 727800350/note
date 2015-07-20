#!/bin/bash
## generate ssh key
set -x

if [ `whoami` = "root" ]
then
	echo "do not use root" >&2
	exit 1
fi

cd ~
if [ ! -d .ssh ]
then
	mkdir .ssh
fi
cd .ssh
ls -l

if [ -f id_rsa -o -f id_rsa.pub ]
then
	echo "id_rsa exists"
	exit 1
fi

ssh-keygen -t rsa -C "wangchaogo1990@gmail.com" -N thinkpad -f id_rsa 1>/dev/null
ls -l
echo "make sure files in this dir has permission 600(rw-------)" >&2

cat id_rsa.pub
cd ~
echo "test github connection: ssh -T git@github.com" >&2

exit 0


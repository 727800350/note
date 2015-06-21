#!/bin/bash
## generate ssh key
set -x

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
cat id_rsa.pub
cd ~
echo "test github connection: ssh -T git@github.com"


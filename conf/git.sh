#!/bin/bash
## install software for new os
set -x

if [ `whoami` = "root" ]
then
	echo "do not use root"
	exit 1
fi

yum list installed | grep ^git.
if [ $? -ne 0 ]
then
	sudo yum install -y git || exit 1
fi

## mail必须和github上注册的帐号一样, name 可以不一样, 否则在github的个人主页上看不到自己的提交记录
git config --global user.name "Eric Wang"
git config --global user.email wangchaogo1990@gmail.com

cd ~ 
if [ ! -d git ]
then
	mkdir git
fi

cd git
if [ ! -d note ]
then
	git clone git@github.com:ericuni/note.git
	if [ $? -ne 0 ]
	then
		echo "git clone error" >&2
		exit 1
	fi
fi

ln -sf ~/git/note/conf/.vimrc ~/.vimrc
ln -sf ~/git/note/conf/.bashrc ~/.bashrc
ln -sf ~/git/note/conf/.tmux.conf ~/.tmux.conf


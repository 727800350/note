#!/bin/bash

set -x

source ./common.env

## 如果系统中自带是vim是vim-minimal, 而不是vim
## vi 默认是vi-minimal
## 直接安装vim-enhanced 会提示和vi-minimal 冲突
## 需要先卸载掉vi-minimal, 但是vi-minimal 又是cvs和sudo 的依赖项, 所以在安装完vim-enhanced, 再安装cvs, sudo的时候又会把vi-minimal 安装回来
if [ ${os} = "fedora" ]
then
	yum remove -y vim-minimal
	echo "please use visudo to give `whoami` sudo permission"
fi

installer sudo 
tail /etc/sudoers
cat /etc/sudoers | grep `whoami`
if [ $? -ne 0 ]
then
	echo "`whoami` ALL=(ALL) ALL" >> /etc/sudoers
fi
tail /etc/sudoers

installer vim-enhanced


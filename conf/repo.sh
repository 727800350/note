#!/bin/bash
## take care of repos
set -x

source ../common.env

installer yum-plugin-priorities

installer createrepo

local="/yum/server"
if [ ! -d ${local} ]
then
	sudo mkdir ${local}
fi

num=`ls -l ${local} | wc -l | awk '{print $1}'`
## 有些rpm 包的时候才将这个目录设置为 local repo
if [ ${num} -gt 10 ]
then
	sudo createrepo $local
fi

if [ ${os} = "centos" ]
then
	if [ ${version} -eq 7 ]
	then
		sudo yum-config-manager --add-repo=https://copr.fedoraproject.org/coprs/mosquito/myrepo/repo/epel-7/mosquito-myrepo-epel-7.repo
		## 默认的Centos 7不带能够播放MP3和视频文件的解码器支持, 默认repo和后来自己加的源都不带, 需要nux-desktop repo, 先把epel 的repo弄好
		sudo rpm -Uvh http://li.nux.ro/download/nux/dextop/el7/x86_64/nux-dextop-release-0-1.el7.nux.noarch.rpm
	elif [ ${version} -eq 6 ]	
	then
		sudo yum-config-manager --add-repo=https://copr.fedoraproject.org/coprs/mosquito/myrepo/repo/epel-$(rpm -E %?rhel)/mosquito-myrepo-epel-$(rpm -E %?rhel).repo 
		sudo yum install epel-release 
		sudo yum localinstall \
			   	http://li.nux.ro/download/nux/dextop/el$(rpm -E %rhel)/x86_64/nux-dextop-release-0-2.el$(rpm -E %rhel).nux.noarch.rpm \
				http://download1.rpmfusion.org/nonfree/el/updates/$(rpm -E %rhel)/x86_64/rpmfusion-nonfree-release-$(rpm -E %rhel)-1.noarch.rpm \
				http://download1.rpmfusion.org/free/el/updates/$(rpm -E %rhel)/x86_64/rpmfusion-free-release-$(rpm -E %rhel)-1.noarch.rpm 
	else
		echo "${os} ${version} not supported" >&2
	fi
elif [ ${os} = "fedora" ]
then
else
	echo "${os} not supported" >&2
fi


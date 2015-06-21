#!/bin/bash
## take care of repos
set -x

source ../common.env

## the repos is the lowest(99)
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
	## local repo
	sudo sed -i "/^\[base\]/a priority=2" /etc/yum.repos.d/CentOS-Base.repo 
	sudo sed -i "/^\[updates\]/a priority=2" /etc/yum.repos.d/CentOS-Base.repo 

	sudo yum-config-manager --add-repo=https://copr.fedoraproject.org/coprs/mosquito/myrepo/repo/epel-${version}/mosquito-myrepo-epel-${version}.repo

	## 默认的Centos 7不带能够播放MP3和视频文件的解码器支持, 默认repo和后来自己加的源都不带, 需要nux-desktop repo, 先把epel 的repo弄好
	sudo yum localinstall http://li.nux.ro/download/nux/dextop/el${version}/x86_64/nux-dextop-release-0-1.el${version}.nux.noarch.rpm

	sudo yum localinstall http://dl.fedoraproject.org/pub/epel/epel-release-latest-${version}.noarch.rpm
	sudo yum localinstall http://download1.rpmfusion.org/nonfree/el/updates/${version}/x86_64/rpmfusion-nonfree-release-${version}-1.noarch.rpm
	sudo yum localinstall http://download1.rpmfusion.org/free/el/updates/${version}/x86_64/rpmfusion-free-release-${version}-1.noarch.rpm

	sudo yum localinstall http://pkgs.repoforge.org/rpmforge-release/rpmforge-release-0.5.3-1.el${version}.rf.x86_64.rpm
elif [ ${os} = "fedora" ]
then
	## local repo
	sudo sed -i "/^\[fedora\]/a priority=2" /etc/yum.repos.d/fedora.repo 
	sudo sed -i "/^\[updates\]/a priority=2" /etc/yum.repos.d/fedora-updates.repo 

	sudo yum localinstall http://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm
else
	echo "${os} not supported" >&2
fi

## sudo sed -i "/^\[epel\]/a priority=4" /etc/yum.repos.d/

sudo sed -i "/^\[epel\]/a priority=3" /etc/yum.repos.d/CentOS-Base.repo 
## 3 mosquito-myrepo-epel-${version}.repo
## 4 nux-dextop-release-0-1.el${version}.nux.noarch.rpm
## 4 rpmfusion-nonfree-release-${version}-1.noarch.rpm
## 4 rpmfusion-free-release-${version}-1.noarch.rpm
sudo sed -i "/^\[rpmforge\]/a priority=4" /etc/yum.repos.d/rpmforge.repo


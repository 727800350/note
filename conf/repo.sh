#!/bin/bash
## take care of repos
set -x

if [ `whoami` != "root" ]
then
	echo "need root"
	exit 1
fi

source ./common.env

## the path should not contain backspace, even with back slash
if [ $# -eq 1 ]
then
	pkgs=$1
else
	echo "wrong parameter"
	echo "usage: $0 path"
	exit 1
fi

## the repos is the lowest(99)
yum list installed | grep ^yum-plugin-priorities. >&2
if [ $? -ne 0 ]
then
	yum install -y yum-plugin-priorities
fi

yum list installed | grep ^createrepo. >&2
if [ $? -ne 0 ]
then
	yum install -y installer createrepo
fi

local="/yum/"
if [ ! -d ${local} ]
then
	sudo mkdir -p ${local}
fi

target=${local}/`basename ${pkgs}`
ls -l ${target} | head >&2
ls -l ${target} | grep -v rpm | head >&2
if [ ! -e ${target}/TRANS.TBL ]
then
	num=`ls -l ${pkgs} | wc -l | awk '{print $1}'`
	if [ ${num} -gt 0 ]
	then
		cp -r ${pkgs} ${local} || exit 1
		createrepo $target
	else
		echo "no rpm packages in ${pkgs}" >&2
	fi
else
	echo "local repo already exists" >&2
fi

cd /etc/yum.repos.d

repo="localyum.repo"
if [ -e ${target}/TRANS.TBL -a ! -e ${repo} ]
then
	echo "[localyum]" > ${repo}
	echo "priority=1" >> ${repo}
	echo "Name=localyum" >> ${repo}
	echo "baseurl=file://${target}" >> ${repo}
	echo "enable=1" >> ${repo}
	echo "gpgcheck=0" >> ${repo}
fi

function update_repo(){
	repo=$1
	priority=$2
	name=$3

	if [ ! -e ${repo} ]
	then
		echo "${repo} does not exist" >&2
		return 1
	fi

	cat ${repo} | grep priority=${priority}
	if [ $? -ne 0 ]
	then
		sed -i "/^\[${name}\]/a priority=${priority}" ${repo}
	fi
}

suffix=`date +%Y%m%d_%H_%M_%S`
if [ ${os} = "centos" ]
then
	## local repo
	update_repo CentOS-Base.repo 2 base
	update_repo CentOS-Base.repo 2 updates

	if [ ! -e mosquito-myrepo-epel-${version}.repo ]
	then
		yum-config-manager --add-repo=https://copr.fedoraproject.org/coprs/mosquito/myrepo/repo/epel-${version}/mosquito-myrepo-epel-${version}.repo
	fi
	update_repo mosquito-myrepo-epel-${version}.repo 3 mosquito-myrepo

	## 默认的Centos 7不带能够播放MP3和视频文件的解码器支持, 默认repo和后来自己加的源都不带, 需要nux-dextop repo, 先把epel 的repo弄好
	if [ ! -e nux-dextop.repo ]
	then
		yum localinstall -y http://li.nux.ro/download/nux/dextop/el${version}/x86_64/nux-dextop-release-0-1.el${version}.nux.noarch.rpm
	fi
	update_repo nux-dextop.repo 4 nux-dextop

	if [ ! -e nux-dextop.repo ]
	then
		yum localinstall -y http://dl.fedoraproject.org/pub/epel/epel-release-latest-${version}.noarch.rpm
	fi
	update_repo epel.repo 3 epel

	if [ ! -e nux-dextop.repo ]
	then
		yum localinstall -y http://download1.rpmfusion.org/nonfree/el/updates/${version}/x86_64/rpmfusion-nonfree-release-${version}-1.noarch.rpm
	fi
	update_repo rpmfusion-nonfree.repo 4 rpmfusion-nonfree

	if [ ! -e nux-dextop.repo ]
	then
		yum localinstall -y http://download1.rpmfusion.org/free/el/updates/${version}/x86_64/rpmfusion-free-release-${version}-1.noarch.rpm
	fi
	update_repo rpmfusion-free.repo 4 rpmfusion-free

	if [ ! -e nux-dextop.repo ]
	then
		yum localinstall -y http://pkgs.repoforge.org/rpmforge-release/rpmforge-release-0.5.3-1.el${version}.rf.x86_64.rpm
	fi
	update_repo rpmforge.repo 4 rpmforge

	if [ ${version} -eq 7 ]
	then
		if [ ! -e nux-dextop.repo ]
		then
			yum -y localinstall http://www.elrepo.org/elrepo-release-7.0-2.el7.elrepo.noarch.rpm
		fi
		update_repo elrepo.repo 4 elrepo
	elif [ ${version} -eq 6 ]
	then
		if [ ! -e nux-dextop.repo ]
		then
			yum -y localinstall http://www.elrepo.org/elrepo-release-6-6.el6.elrepo.noarch.rpm
		fi
		update_repo elrepo.repo 4 elrepo
	else
		echo "${os} ${version} not supported"
	fi
elif [ ${os} = "fedora" ]
then
	## local repo
	update_repo fedora.repo 2 fedora
	update_repo fedora.repo 2 updates

	if [ ! -e nux-dextop.repo ]
	then
		yum -y localinstall http://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm
	fi
	update_repo epel.repo 3 epel
else
	echo "${os} not supported" >&2
fi

if [ -e google-chrome.repo ]
then
	sed -i "s/enabled=1/enabled=0/g" google-chrome.repo
fi

yum clean all
yum makecache
cd ~

exit 0


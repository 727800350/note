#!/bin/bash
set -x

source ${HOME}/git/note/conf/func.sh || exit 1

user=`whoami`
if [ ${user} != "root" ]
then
	echo "must be root" >&2
	exit 1
fi

# repo config
version="7"
## replace base repo
cd /etc/yum.repo.d/
wget http://mirrors.163.com/.help/CentOS7-Base-163.repo .
mv CentOS-Base.repo CentOS-Base.repo.bak

yum install -y yum-plugin-priorities
CHK_RET WARNING "install yum plugin priorities error"

yum localinstall -y http://pkgs.repoforge.org/rpmforge-release/rpmforge-release-0.5.3-1.el7.rf.x86_64.rpm
CHK_RET FATAL "install repo rpmforge error"

yum localinstall -y http://www.elrepo.org/elrepo-release-7.0-2.el7.elrepo.noarch.rpm
## http://www.elrepo.org/elrepo-release-6-6.el6.elrepo.noarch.rpm
CHK_RET FATAL "install repo elrepo error"

yum localinstall -y http://dl.fedoraproject.org/pub/epel/epel-release-latest-${version}.noarch.rpm
CHK_RET FATAL "install repo epel error"

yum-config-manager --add-repo=https://copr.fedoraproject.org/coprs/mosquito/myrepo/repo/epel-${version}/mosquito-myrepo-epel-${version}.repo
CHK_RET FATAL "install repo mosquite-myrepo-epel error"

yum localinstall -y http://li.nux.ro/download/nux/dextop/el${version}/x86_64/nux-dextop-release-0-1.el${version}.nux.noarch.rpm
CHK_RET FATAL "install repo nux-desktop-release error"

## http://mirrors.ustc.edu.cn/fedora/rpmfusion/nonfree/fedora/rpmfusion-nonfree-release-stable.noarch.rpm
yum localinstall -y http://download1.rpmfusion.org/nonfree/el/updates/${version}/x86_64/rpmfusion-nonfree-release-${version}-1.noarch.rpm
CHK_RET FATAL "install repo rpmfusion-nonfree error"

## http://mirrors.ustc.edu.cn/fedora/rpmfusion/free/fedora/rpmfusion-free-release-stable.noarch.rpm
yum localinstall -y http://download1.rpmfusion.org/free/el/updates/${version}/x86_64/rpmfusion-free-release-${version}-1.noarch.rpm
CHK_RET FATAL "install repo rpmfusion-free error"

yum repolist

yum clean all
yum makecache

# packages
yum install -y fuse-ntfs-3g
CHK_RET FATAL "fuse-ntfs-3g error"

yum install -y unrar
CHK_RET FATAL "unrar error"

## bash ./install_advp
## CHK_RET WARNING "advp error"

## bash ./install_chrome
## CHK_RET FATAL "chrome error"

yum localinstall -y http://kdl.cc.ksosoft.com/wps-community/download/a19/wps-office-9.1.0.4975-1.a19p1.x86_64.rpm
CHK_RET FATAL "wps error"

yum install -y unix2dos dos2unix
CHK_RET FATAL "unix2dos error"

yum install -y pdfgrep
CHK_RET FATAL "pdfgrep error"

yum install -y cmake clang golang valgrind
CHK_RET FATAL "dev error"

yum install -y mariadb-server mariadb-devel
CHK_RET FATAL "mysql error"

yum install -y php php-mysql php-gd php-imap php-ldap php-odbc php-pear php-xml php-xmlrpc
CHK_RET FATAL "php error"

yum install -y python-pip numpy scipy python-matplotlib
CHK_RET FATAL "python lib error"

yum install -y R
CHK_RET FATAL "R error"

yum install -y wiznote ## Stable version  
CHK_RET FATAL "wiznote error"

yum install -y alacarte ## set desktop menu
CHK_RET FATAL "alacarte error"

## yum install -y ffmpeg ## video and audio convertor
## yum install -y shutter ## screen capture

exit 0


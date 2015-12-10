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
yum install -y yum-plugin-priorities
CHK_RET WARNING "install yum plugin priorities error"

yum -y localinstall http://pkgs.repoforge.org/rpmforge-release/rpmforge-release-0.5.3-1.el${version}.rf.x86_64.rpm
CHK_RET FATAL "install repo rpmforge error"

yum -y localinstall http://www.elrepo.org/elrepo-release-7.0-2.el7.elrepo.noarch.rpm
## http://www.elrepo.org/elrepo-release-6-6.el6.elrepo.noarch.rpm
CHK_RET FATAL "install repo elrepo error"

yum -y localinstall http://dl.fedoraproject.org/pub/epel/epel-release-latest-${version}.noarch.rpm
CHK_RET FATAL "install repo epel error"

yum-config-manager --add-repo=https://copr.fedoraproject.org/coprs/mosquito/myrepo/repo/epel-${version}/mosquito-myrepo-epel-${version}.repo
CHK_RET FATAL "install repo mosquite-myrepo-epel error"

yum -y localinstall http://li.nux.ro/download/nux/dextop/el${version}/x86_64/nux-dextop-release-0-1.el${version}.nux.noarch.rpm
CHK_RET FATAL "install repo nux-desktop-release error"

yum -y localinstall http://download1.rpmfusion.org/nonfree/el/updates/${version}/x86_64/rpmfusion-nonfree-release-${version}-1.noarch.rpm
CHK_RET FATAL "install repo rpmfusion-nonfree error"

yum -y localinstall http://download1.rpmfusion.org/free/el/updates/${version}/x86_64/rpmfusion-free-release-${version}-1.noarch.rpm
CHK_RET FATAL "install repo rpmfusion-free error"

yum repolist

yum clean all
yum makecache

# packages
yum -y install fuse-ntfs-3g
CHK_RET FATAL "fuse-ntfs-3g error"

yum -y install unrar
CHK_RET FATAL "unrar error"

## bash ./install_advp
## CHK_RET WARNING "advp error"

## bash ./install_chrome
## CHK_RET FATAL "chrome error"

yum -y localinstall http://kdl.cc.ksosoft.com/wps-community/download/a19/wps-office-9.1.0.4975-1.a19p1.x86_64.rpm
CHK_RET FATAL "wps error"

yum -y install unix2dos dos2unix
CHK_RET FATAL "unix2dos error"

yum -y install pdfgrep
CHK_RET FATAL "pdfgrep error"

yum -y install cmake clang golang valgrind
CHK_RET FATAL "dev error"

yum -y install mariadb-server mariadb-devel
CHK_RET FATAL "mysql error"

yum -y install php php-mysql php-gd php-imap php-ldap php-odbc php-pear php-xml php-xmlrpc
CHK_RET FATAL "php error"

yum -y install python-pip numpy scipy python-matplotlib
CHK_RET FATAL "python lib error"

yum -y install R
CHK_RET FATAL "R error"

yum -y install wiznote ## Stable version  
CHK_RET FATAL "wiznote error"

yum -y install alacarte ## set desktop menu
CHK_RET FATAL "alacarte error"

## yum -y install ffmpeg ## video and audio convertor
## yum -y install shutter ## screen capture

exit 0


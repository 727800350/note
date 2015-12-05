#!/bin/bash
set -x

source ${HOME}/git/note/conf/func.sh || exit 1

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

yum -y install cmake clang installer golang valgrind
CHK_RET FATAL "dev error"

yum -y install mariadb-server mariadb-devel
CHK_RET FATAL "mysql error"

yum -y install php php-mysql php-gd php-imap php-ldap php-odbc php-pear php-xml php-xmlrpc
CHK_RET FATAL "php error"

yum -y install python-pip numpy scipy python-matplotlib
CHK_RET FATAL "python lib error"

yum -y install R

yum -y install wiznote ## Stable version  

yum -y install alacarte ## set desktop menu
yum -y install ffmpeg ## video and audio convertor
yum -y install shutter ## screen capture

exit 0


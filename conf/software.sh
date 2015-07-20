#!/bin/bash
## install software for new os
set -x

source ./common.env

installer tmux

installer samba
chkconfig --level 35 smb on

## screen capture
installer shutter

## video and audio convertor
installer ffmpeg

## text convertion between windows and linux
installer unix2dos dos2unix

## programming
installer gcc gcc-c++ gdb cmake
installer valgrind
installer strace
installer libasan libasan-static
installer flex byacc

## zmodem/ymodel/xmodem package
installer lrzsz

installer unrar
installer mlocate

installer mariadb-server mariadb mariadb-devel
installer glib2 glib2-devel
installer boost

## php
installer php
installer php-mysql php-gd php-imap php-ldap php-odbc php-pear php-xml php-xmlrpc

installer phpMyAdmin
## php data file in /usr/share/phpMyAdmin
## 访问权限的设置在/etc/httpd/conf.d/phpMyAdmin.conf
## Apache 2.4+ uses "Require all granted" instead of "Allow from all"
## 为了让局域网内的其他机器能够访问网站, 将防火墙关闭掉
if [ ${type} = "server" ]
then
	if [ ${os} = "fedora" ]
	then
		service firewalld stop
		setenforce 0
	elif [ ${os} = "centos" ]
	then
		service iptables stop
		setenforce 0 #enforce 的默认值在/etc/selinux/config, permissive = 0, enforcing = 1
	else
		echo "${os} not supported" >&2
	fi
fi
## 当在/var/www/html/ 中创建了一个指向 /home/eric 的symlink, 可能在浏览器中访问时, 提示权限问题
## Apache 需要这个目录以及上级目录的rx权限

## project
installer libpcap libpcap-devel

installer wireshark ## no gui
if [ ${type} = "desktop" ]
then
	installer wireshark-gnome
fi

## Berkeley Internet Name Domain (BIND) DNS (Domain Name System)
installer bind 

## Network exploration tool and security scanner
installer nmap 

## A whois client that accepts both traditional and finger-style queries
installer jwhois 

## ntopng prerequisite
installer libxml2-devel sqlite-devel

### zlog
if [ ! -e /usr/local/include/zlog.h ]
then
	wget -c https://codeload.github.com/HardySimpson/zlog/tar.gz/latest-stable
	if [ $? -eq 0 -a -e latest-stable ]
	then
		tar -xvf latest-stable
		cd zlog-latest-stable
		make
		make install
		cd ../
		rm -rf latest-stable zlog-latest-stable
	fi
fi

### local libs
if [ ! -e /etc/ld.so.conf.d/local_lib.conf -o ! -e /etc/ld.so.conf.d/local_lib64.conf ]
then
	cd /etc/ld.so.conf.d
	echo /usr/local/lib > local_lib.conf
	echo /usr/local/lib64 > local_lib64.conf
	ldconfig
else
	echo "/etc/ld.so.conf.d/local_lib.conf and /etc/ld.so.conf.d/local_lib64.conf already exist" >&2
fi

<<COM
## octave
## will install hdf5, hdf5-static and hdf5-devel
installer hdf5-static 
sudo rpm -ivh epstool-3.08-7.fc21.x86_64.rpm
installer octave
COM

## python
installer numpy scipy
installer python-pip 

function pip_install(){
	for soft in $@
	do
		## check
		pip list | grep ${soft}
		## the  exit  status  is 0 if selected lines are found and 1 otherwise.
		if [ $? -ne 0 ]
		then
			echo "${soft} has not been installed"
			## install
 			pip install ${soft}
		fi
	done
}

pip_install xlutils
## 	you-get和youtube-dl <https://github.com/rg3/youtube-dl>是两个用于从视频网站上下载视频文件的工具。其中，后者支持的网站更多，但前者对国内的视频网站支持更好。
if [ -e /usr/bin/python3 ]
then
	pip_install you-get ## needs python 3
fi
pip_install youtube-dl

installer mysql-connector-python

installer wiznote      # Stable version  

installer gstreamer gstreamer-plugins-base gstreamer-plugins-good gstreamer-plugins-bad-free

installer alacarte ## set desktop menu

## cpdf是一个跨平台的PDF处理工具，可以完成常见的PDF合并、切割、加密解密、书签、水印等功能。
## 下载已编译好的二进制包，解压，并将与自己的平台对应的二进制文件复制到${HOME}/bin目录下即可使用。
cd /home/
user=`ls | grep -v lost+found`
cd ${user}
if [ ! -d bin ]
then
	mkdir bin
fi
if [ ! -e bin/cpdf -o ! -e bin/smpdf ]
then
	wget -c https://github.com/coherentgraphics/cpdf-binaries/archive/master.zip
	unzip cpdf-binaries-master.zip

	cp cpdf-binaries-master/Linux-Intel-64bit/cpdf bin/
	cp cpdf-binaries-master/cpdfmanual.pdf bin/

	cp cpdf-binaries-master/LosslessPDFCompressor/Linux64/smpdf bin/
	cp cpdf-binaries-master/LosslessPDFCompressor/smpdfmanual.pdf bin/
	chmod +x bin/cpdf bin/smpdf
	rm -rf cpdf-binaries-master.zip cpdf-binaries-master
fi

<<COM
## mutt
sudo yum install -y mutt msmtp getmail procmail lynx elinks

## bt
sudo yum install -y transmission

## graphviz, dot
sudo yum install graphviz
sudo dot -c  ## configure plugins
sudo yum install graphviz-gd ## support for png, jpg, jpeg etc
COM

## http://seisman.info/fcitx-for-centos-7.html
## installer fcitx-googlepinyin fcitx-cloudpinyin

## wps office
yum list installed | grep ^wps-office\\.
if [ $? -ne 0 ]
then
	yum localinstall -y http://kdl.cc.ksosoft.com/wps-community/download/a18/wps-office-9.1.0.4961-1.a18p1.x86_64.rpm
fi

## R language
installer R
installer curl libcurl libcurl-devel

## 不含参数的library()函数会显示已安装的R包列表
## installed.packages()[,c("Package","Version")]
## nohup Rscript ./package.r 1>r.log 2>&1 &

exit 0


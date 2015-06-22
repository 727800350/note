#!/bin/bash
## install software for new os
set -x

source ./common.env

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

installer mysql mysql-server mysql-devel
installer glib2 glib2-devel
installer boost

## php
installer php
installer php-mysql php-gd php-imap php-ldap php-odbc php-pear php-xml php-xmlrpc

installer PhpMyAdmin
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

### local libs
if [ -d /etc/ld.so.conf.d ]
then
	cd /etc/ld.so.conf.d
	touch local_lib.conf
	touch local_lib64.conf
	echo /usr/local/lib > local_lib.conf
	echo /usr/local/lib64 > local_lib64.conf
	ldconfig
else
	echo "/etc/ld.conf.d does not exist, pleas check" >&2
fi

<<COM
## octave
## will install hdf5, hdf5-static and hdf5-devel
installer hdf5-static 
sudo rpm -ivh epstool-3.08-7.fc21.x86_64.rpm
installer octave
COM

## python
installer gnuplot numpy scipy
installer python-pip 

wget -c https://pypi.python.org/packages/source/p/pip/pip-1.5.6.tar.gz#md5=01026f87978932060cc86c1dc527903e
if [ $? -eq 0 -a -f pip-1.5.6.tar.gz ]
then
	tar -xvf pip-1.5.6.tar.gz
	cd pip-1.5.6
	python setup.py install
	cd ../
	rm -rf pip-1.5.6.tar.gz pip-1.5.6
fi

if [ -e /usr/bin/pip ]
then
## 	check
	pip install gnuplot-py
	pip install xlutils
## 	you-get和youtube-dl <https://github.com/rg3/youtube-dl>是两个用于从视频网站上下载视频文件的工具。其中，后者支持的网站更多，但前者对国内的视频网站支持更好。
	pip install you-get ## needs python 3
	pip install youtube-dl
fi

wget -c http://cdn.mysql.com/Downloads/Connector-Python/mysql-connector-python-2.0.2.tar.gz
tar -xvf mysql-connector-python-2.0.2.tar.gz
cd mysql-connector-python-2.0.2
python setup.py install
cd ../
rm -rf mysql-connector-2.0.2/*

## ## R language
installer R
installer curl libcurl libcurl-devel
nohup Rscript ./package.r 1>r.log 2>&1 &

## flash 
yum localinstall -y http://fpdownload.macromedia.com/get/flashplayer/pdc/11.2.202.425/flash-plugin-11.2.202.425-release.x86_64.rpm

installer wiznote      # Stable version  

installer gstreamer gstreamer-plugins-base gstreamer-plugins-good gstreamer-plugins-bad-free

installer alacarte ## set desktop menu

## wps office
yum localinstall -y http://kdl.cc.ksosoft.com/wps-community/download/a18/wps-office-9.1.0.4961-1.a18p1.x86_64.rpm

## cpdf是一个跨平台的PDF处理工具，可以完成常见的PDF合并、切割、加密解密、书签、水印等功能。
## 下载已编译好的二进制包，解压，并将与自己的平台对应的二进制文件复制到${HOME}/bin目录下即可使用。
wget -c https://github.com/coherentgraphics/cpdf-binaries/archive/master.zip

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


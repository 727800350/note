#!/bin/bash
## install software for new os
set -x

source ./common.env

## text convertion between windows and linux
installer unix2dos dos2unix

## git
installer git
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

cd note/conf
cp .vimrc .bashrc ~
cd ~

target="~/git/note/demo/shell/common.sh"
if [ ! -f ${target} ]
then
	echo "${target} does not exist" >&2
	exit 1
fi
source ${target}

## programming
installer clang gcc gcc-c++ gdb cmake
installer valgrind
installer strace
installer libasan libasan-static
installer flex byacc

## zmodem/ymodel/xmodem package
installer lrzsz

installer unrar
installer mlocate
sudo yum update -y firefox

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
		sudo service firewalld stop
		sudo setenforce 0
	elif [ ${os} = "centos" ]
	then
		sudo service iptables stop
		sudo setenforce 0 #enforce 的默认值在/etc/selinux/config, permissive = 0, enforcing = 1
	else
		echo "${os} not supported"
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
wget https://codeload.github.com/HardySimpson/zlog/tar.gz/latest-stable
if [ $? -eq 0 -a -e latest-stable ]
then
	tar -xvf latest-stable
	cd zlog-latest-stable
	make
	sudo make install
	cd ../
	sudo rm -rf latest-stable zlog-latest-stable
fi

### local libs
if [ -d /etc/ld.so.conf.d ]
then
	cd /etc/ld.so.conf.d
	sudo touch local_lib.conf
	sudo touch local_lib64.conf
	sudo chown eric:eric local_lib.conf local_lib64.conf
	echo /usr/local/lib > local_lib.conf
	echo /usr/local/lib64 > local_lib64.conf
	sudo chown root:root local_lib.conf local_lib64.conf
	sudo ldconfig
else
	echo "/etc/ld.conf.d does not exist, pleas check"
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

wget https://pypi.python.org/packages/source/p/pip/pip-1.5.6.tar.gz#md5=01026f87978932060cc86c1dc527903e
if [ $? -eq 0 -a -f pip-1.5.6.tar.gz ]
then
	tar -xvf pip-1.5.6.tar.gz
	cd pip-1.5.6
	sudo python setup.py install
	cd ../
	sudo rm -rf pip-1.5.6.tar.gz pip-1.5.6
fi

if [ -e /usr/bin/pip ]
then
## 	check
	sudo pip install gnuplot-py
	sudo pip install xlutils
fi

wget http://cdn.mysql.com/Downloads/Connector-Python/mysql-connector-python-2.0.2.tar.gz
tar -xvf mysql-connector-python-2.0.2.tar.gz
cd mysql-connector-python-2.0.2
sudo python setup.py install
cd ../
sudo rm -rf mysql-connector-2.0.2/*

## R language
installer R
installer curl libcurl libcurl-devel
if [ -f package.r ]
then
	sudo Rscript --slave package.r
fi

## flash 
wget http://fpdownload.macromedia.com/get/flashplayer/pdc/11.2.202.425/flash-plugin-11.2.202.425-release.x86_64.rpm
if [ $? -eq 0 -a -e flash-plugin-11.2.202.425-release.x86_64.rpm ]
then
	sudo rpm -ivh flash-plugin-11.2.202.425-release.x86_64.rpm
	rm flash-plugin-11.2.202.425-release.x86_64.rpm
fi

installer wiznote      # Stable version  

installer gstreamer gstreamer-plugins-base gstreamer-plugins-good gstreamer-plugins-bad-free

sudo yum install alacarte ## set desktop menu

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

## markdown
wget http://www.vim.org/scripts/download_script.php?src_id=15150 markdown
echo "plean open vim and execute :source markdown"


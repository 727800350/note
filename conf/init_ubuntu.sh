#!/bin/bash
set -x

source ${HOME}/git/note/conf/func.sh || exit 1

user=`whoami`
if [ ${user} != "root" ]
then
	echo "must be root" >&2
	exit 1
fi

apt-get update
CHK_RET FATAL "update error"

apt-get -y upgrade
CHK_RET FATAL "upgrade error"

## open terminals in arbitrary path
apt-get install -y nautilus-open-terminal \
	fcitx \
	vim \
	git \
	unrar \
	wiznote
CHK_RET FATAL "install soft error"

cd $HOME \
	&& mkdir git \
	&& cd git \
	&& git clone git@github.com:ericuni/note.git
CHK_RET FATAL "git clone note error"

ln -sf ${HOME}/git/note/conf/.bashrc ${HOME}/.bashrc \
	&& ln -sf ${HOME}/git/note/conf/.vimrc ${HOME}/.vimrc \
	&& ln -sf ${HOME}/git/note/conf/.gitconfig ${HOME}/.gitconfig \
	&& ln -sf ${HOME}/git/note/conf/.tmux.conf ${HOME}/.tmux.conf \
	&& ln -sf ${HOME}/git/note/conf/.guile ${HOME}/.guile \
	&& ln -sf ${HOME}/git/note/conf/.Rprofile ${HOME}/.Rprofile \
	&& ln -sf ${HOME}/git/note/conf/hosts /etc/hosts
CHK_RET FATAL "create soft link error"

wget http://kdl.cc.ksosoft.com/wps-community/download/a19/wps-office_9.1.0.4975~a19p1_amd64.deb -O wps.deb && dpkg -i wps.deb
CHK_RET WARNING "install wps error"

exit 0


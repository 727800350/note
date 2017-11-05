# .bashrc

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi
export PS1="[\u@\h:\W]\$ "

## bash use emacs by default, change it to vi
set -o vi

## source docker func
if [ -f $HOME/git/docker/.dockerfunc ]; then
	source $HOME/git/docker/.dockerfunc
fi

user=`whoami`
uname -a | grep Darwin 1>/dev/null 2>&1
if [ $? -eq 0 ]
then
	os="mac"
else
	os="linux"
fi

# Path
## my own pc
if [ "x$user" == "xeric" -o "x$user" == "xwangchao34" ]
then
	ulimit -c unlimited
else
	if [ -f /home/img/.enterprise ]
	then
		source /home/img/.enterprise
	fi
fi
export LIBRARY_PATH=$LD_LIBRARY_PATH:$HOME/git/note/demo/cxx/util

## texlive env
export TEXLIVE=${HOME}/opt/texlive/2015/
export PATH=$PATH:$TEXLIVE/bin/x86_64-linux/
export MANPATH=$MANPATH:$TEXLIVE/texmf/doc/man/
export INFOPATH=$INFOPATH:$TEXLIVE/texmf/doc/info/

export PATH=$PATH:/usr/local/sbin/
export PATH=$PATH:${HOME}/git/note/bin/
export PATH=$PATH:${HOME}/bin/

# aliases
version=`gcc -v 2>&1 | tail -n 1 | awk '{print $3}'`
if [[ $version > "4.7" ]]
then
	alias g++='g++ -std=c++11 -g -Wall'
else
	alias g++='g++ -g -Wall'
fi
alias gcc='gcc -g -Wall'

if [ "x$os" == "xmac" ]
then
	alias ls="ls -G"
	alias ll="ls -l"
else
	export LS_COLORS="no=00:fi=00:di=00;36:ln=00;36:pi=40;33:so=00;35:bd=40;33;01:cd=40;33;01:or=01;05;37;41:mi=01;05;37;41:ex=00;32:"
	export LS_COLORS="${LS_COLORS}*.cmd=00;32:*.exe=00;32:*.com=00;32:*.btm=00;32:*.bat=00;32:*.sh=00;32:*.csh=00;32:"
	export LS_COLORS="${LS_COLORS}*.tar=00;31:*.tgz=00;31:*.arj=00;31:*.taz=00;31:*.lzh=00;31:*.zip=00;31:*.z=00;31:*.Z=00;31:*.gz=00;31:*.bz2=00;31:*.bz=00;31:*.tz=00;31:*.rpm=00;31:*.cpio=00;31:"
	export LS_COLORS="${LS_COLORS}*.jpg=00;35:*.gif=00;35:*.bmp=00;35:*.xbm=00;35:*.xpm=00;35:*.png=00;35:*.tif=00;35:"
	alias ls="ls --color=auto"
	alias ll="ls -l"
fi


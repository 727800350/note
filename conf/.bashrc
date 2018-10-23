# .bashrc

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi
export PS1="[\u@\h:\W]\$ "
export LC_CTYPE="en_US.UTF-8"

## bash use emacs by default, change it to vi
set -o vi

user=`whoami`
uname -a | grep Darwin 1>/dev/null 2>&1
if [ $? -eq 0 ]
then
	os="mac"
else
	os="linux"
fi

export PATH=$PATH:/usr/local/sbin
export PATH=$PATH:${HOME}/git/note/bin
export PATH=$PATH:${HOME}/bin

ulimit -c unlimited

alias grep='grep --color=auto'
alias watch='watch -d -n 1'

if [ "x$os" == "xmac" ]
then
	alias ls="ls -G"
	alias ll="ls -l"
else
	LS_COLORS="no=00:fi=00:di=00;36:ln=00;36:pi=40;33:so=00;35:bd=40;33;01:cd=40;33;01:or=01;05;37;41:mi=01;05;37;41:ex=00;32:"
	LS_COLORS="${LS_COLORS}*.cmd=00;32:*.exe=00;32:*.com=00;32:*.btm=00;32:*.bat=00;32:*.sh=00;32:*.csh=00;32:"
	LS_COLORS="${LS_COLORS}*.tar=00;31:*.tgz=00;31:*.arj=00;31:*.taz=00;31:*.lzh=00;31:*.zip=00;31:*.z=00;31:*.Z=00;31:*.gz=00;31:*.bz2=00;31:*.bz=00;31:*.tz=00;31:*.rpm=00;31:*.cpio=00;31:"
	LS_COLORS="${LS_COLORS}*.jpg=00;35:*.gif=00;35:*.bmp=00;35:*.xbm=00;35:*.xpm=00;35:*.png=00;35:*.tif=00;35"
	export LS_COLORS
	alias ls="ls --color=auto"
	alias ll="ls -l"
fi

# blade
export BLADE_AUTO_UPGRADE="no"
alias bb="blade build --no-debug-info"
alias bt="blade test --no-debug-info"
alias br="blade run --no-debug-info"
test -s ~/bin/bladefunctions && . ~/bin/bladefunctions || true

# enterprise
export JAVA_HOME="~/opt/jdk/jdk1.7.0_45"
if [ -f ~/svn/trunk/thirdparty/perftools/bin/pprof ]
then
	alias pprof="~/svn/trunk/thirdparty/perftools/bin/pprof"
fi

# udf
## docker
if [ -f $HOME/git/docker/.dockerfunc ]
then
	source $HOME/git/docker/.dockerfunc
fi

## weather report
function weather(){
	if [ $# -eq 1 ]
	then
		city=$1
	else
		city="shenzhen"
	fi
	curl http://wttr.in/~$city
}


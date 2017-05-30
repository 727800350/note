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
if [ "x$user" = "xeric" -o "x$user" = "xwangchao34" ]
then
	ulimit -c unlimited
## enterprise
else
	export PYTHONIOENCODING=utf8 ## for eagle code style check
	export TMOUT= ## #turn off auto-logout (user session will not auto-logout due to session inactivity)
	export PATH=$PATH:${HOME}/.jumbo/bin/
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/samba/lib/
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${HOME}/hadoop-client/java6/jre/lib/amd64/server/:~/hadoop-client/hadoop/libhdfs/

	## hadoop client
	export HADOOP_HOME="${HOME}/hadoop-client/hadoop/"
	export JAVA_HOME="${HADOOP_HOME}/../java6/"
	export local_hadoop="${HADOOP_HOME}/bin/hadoop"
	alias hadoop='${HADOOP_HOME}/bin/hadoop'
	alias yq='${HOME}/hadoop-client/hadoop-yq/bin/hadoop'
	alias tiny='${HOME}/hadoop-client/hadoop-tiny/bin/hadoop'
	alias shitu='${HOME}/hadoop-client/hadoop-shitu/bin/hadoop'
	alias shixiao='${HOME}/hadoop-client/hadoop-shixiao/bin/hadoop'
	alias nl='${HOME}/hadoop-client/hadoop-tuku/bin/hadoop'
	alias spider='${HOME}/hadoop-client/hadoop-spider/bin/hadoop'

	## svn
	export PATH=$PATH:$HOME/opt/tools/tools/svn/1.6.5/64/bin
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${HOME}/opt/tools/tools/svn/1.6.5/64/lib
	export PATH=$PATH:$HOME/opt/tools/tools/scmtools/usr/bin
	export PATH=$PATH:$HOME/opt/tools/tools/../bin/64
	export MAC=64

	## php
	export PATH=$PATH:$HOME/opt/php/bin/

	## tool
	export PATH=$PATH:${HOME}/icode/baidu/wangchao34/tool/kv
	export PATH=$PATH:${HOME}/icode/baidu/wangchao34/tool/64sign

	if [ -f "/home/img/.jumbo/bin/vim" ]
	then
		alias vi="/home/img/.jumbo/bin/vim"
	fi

	ARTISTIC_STYLE_OPTIONS=~/.astylerc
	alias astyle="astyle --style=attach --convert-tabs --indent=spaces=4 --add-brackets \
	--pad-header --unpad-paren --pad-oper --max-code-length=100 --formatted --align-pointer=type \
	--align-reference=type --break-blocks -Y"
fi

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
	export LS_COLORS="no=00:fi=00:di=00;36:ln=00;36:pi=40;33:so=00;35:bd=40;33;01:cd=40;33;01:or=01;05;37;41:mi=01;05;37;41:ex=00;32:*.cmd=00;32:*.exe=00;32:*.com=00;32:*.btm=00;32:*.bat=00;32:*.sh=00;32:*.csh=00;32:*.tar=00;31:*.tgz=00;31:*.arj=00;31:*.taz=00;31:*.lzh=00;31:*.zip=00;31:*.z=00;31:*.Z=00;31:*.gz=00;31:*.bz2=00;31:*.bz=00;31:*.tz=00;31:*.rpm=00;31:*.cpio=00;31:*.jpg=00;35:*.gif=00;35:*.bmp=00;35:*.xbm=00;35:*.xpm=00;35:*.png=00;35:*.tif=00;35:"
	alias ls="ls --color=auto"
	alias ll="ls -l"
fi


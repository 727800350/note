# .bashrc

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

# Uncomment the following line if you don't like systemctl's auto-paging feature:
# export SYSTEMD_PAGER=

user=`whoami`

# Path
## my own pc
if [ ${user} = "eric" ]
then
	export TEXLIVE=/usr/local/texlive/2015/
	ulimit -c unlimited
## enterprise
else
	export TEXLIVE=${HOME}/program/texlive/2015/
	export PATH=$PATH:${HOME}/.jumbo/bin/
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/samba/lib/
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${HOME}/hadoop-client/java6/jre/lib/amd64/server/:~/hadoop-client/hadoop/libhdfs/

	## hadoop client
	export HADOOP_HOME="${HOME}/hadoop-client/hadoop/"
	alias hadoop='${HADOOP_HOME}/bin/hadoop'
	alias mulan='${HOME}/hadoop-client/hadoop-mulan/bin/hadoop'
	alias nanling='${HOME}/hadoop-client/hadoop-nanling/bin/hadoop'
	export local_hadoop="${HADOOP_HOME}/bin/hadoop"
fi

## texlive env
export PATH=$PATH:$TEXLIVE/bin/x86_64-linux/
export MANPATH=$MANPATH:$TEXLIVE/texmf/doc/man/
export INFOPATH=$INFOPATH:$TEXLIVE/texmf/doc/info/

## user defined bin
export PATH=$PATH:${HOME}/git/note/bin/
export PATH=$PATH:${HOME}/bin/

# aliases
## cp and mv
if [ -e /usr/local/bin/acp ]
then
	alias cp='acp -g'
fi
if [ -e /usr/local/bin/amv ]
then
	alias mv='amv -g'
fi

version=`gcc -v 2>&1 | tail -n 1 | awk '{print $3}'`
if [[ $version > "4.7" ]]
then
	alias g++='g++ -std=c++11 -g -Wall'
	alias clang++='clang++ -std=c++11 -g -Wall'
else
	alias g++='g++ -g -Wall'
	alias clang++='clang++ -g -Wall'
fi
alias gcc='gcc -g -Wall'

## colors
export LS_COLORS="no=00:fi=00:di=00;36:ln=00;36:pi=40;33:so=00;35:bd=40;33;01:cd=40;33;01:or=01;05;37;41:mi=01;05;37;41:ex=00;32:*.cmd=00;32:*.exe=00;32:*.com=00;32:*.btm=00;32:*.bat=00;32:*.sh=00;32:*.csh=00;32:*.tar=00;31:*.tgz=00;31:*.arj=00;31:*.taz=00;31:*.lzh=00;31:*.zip=00;31:*.z=00;31:*.Z=00;31:*.gz=00;31:*.bz2=00;31:*.bz=00;31:*.tz=00;31:*.rpm=00;31:*.cpio=00;31:*.jpg=00;35:*.gif=00;35:*.bmp=00;35:*.xbm=00;35:*.xpm=00;35:*.png=00;35:*.tif=00;35:"


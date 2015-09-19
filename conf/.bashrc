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
## enterprise
else
	export TEXLIVE=${HOME}/program/texlive/2015/
	export PATH=$PATH:${HOME}/.jumbo/bin/
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/samba/lib/
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/utils/gcc/lib/gcc/x86_64-unknown-linux-gnu/4.8.1/
	alias hadoop='${HOME}/hadoop-client/hadoop/bin/hadoop'
	alias mulan='${HOME}/hadoop-client/hadoop-mulan/bin/hadoop'
	alias nanling='${HOME}/hadoop-client/hadoop-nanling/bin/hadoop'
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

gcc -v 2>&1 | grep -i "version 4." > /dev/null
if [ $? -eq 0 ]
then
	alias g++='g++ -std=c++11'
	alias clang++='clang++ -std=c++11'
fi


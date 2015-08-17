# .bashrc

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

# Uncomment the following line if you don't like systemctl's auto-paging feature:
# export SYSTEMD_PAGER=

user=`whoami`

# Path
if [ ${user} = "eric" ]
then
	export TEXLIVE=/usr/local/texlive/2015/
else
	export TEXLIVE=${HOME}/program/texlive/2015/
	export PATH=$PATH:${HOME}/.jumbo/bin/
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/samba/lib/
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/utils/gcc/lib/gcc/x86_64-unknown-linux-gnu/4.8.1/
fi

## texlive env
export PATH=$PATH:$TEXLIVE/bin/x86_64-linux/
export MANPATH=$MANPATH:$TEXLIVE/texmf/doc/man/
export INFOPATH=$INFOPATH:$TEXLIVE/texmf/doc/info/

## maven
export MAVEN=${HOME}/program/apache-maven-3.2.5/
export PATH=$PATH:$MAVEN/bin/

## user defined bin
export PATH=$PATH:${HOME}/git/note/bin/

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

alias hadoop='${HOME}/hadoop-client/hadoop/bin/hadoop'
alias mulan='${HOME}/hadoop-client/hadoop-mulan/bin/hadoop'

# functions
## usage: search [extention] keyword [dir]
function search(){
    if [ $# -eq 3 ] 
    then    
        suffix=$1
        target=$2
        dir=$3  
        include="--include=*.${suffix}"
    elif [ $# -eq 2 ] 
    then    
        target=$1
        dir=$2  
        include=""
    elif [ $# -eq 1 ] 
    then    
        target=$1
        dir=.   
        include=""
    else    
        echo "wrong parameters"
        exit 1  
    fi
    
    grep -R -i ${include} ${target} ${dir}
}


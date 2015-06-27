# .bashrc

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

# Uncomment the following line if you don't like systemctl's auto-paging feature:
# export SYSTEMD_PAGER=

# texlive env
export TEXLIVE=/usr/local/texlive/2015
export PATH=$PATH:$TEXLIVE/bin/x86_64-linux
export MANPATH=$MANPATH:$TEXLIVE/texmf/doc/man
export INFOPATH=$INFOPATH:$TEXLIVE/texmf/doc/info

## maven
export MAVEN=/home/`whoami`/program/apache-maven-3.2.5
export PATH=$PATH:$MAVEN/bin

# aliases
target=/usr/local/bin/acp
if [ -e ${target} ]
then
	alias cp='acp -g'
fi
target=/usr/local/bin/amv
if [ -e ${target} ]
then
	alias mv='amv -g'
fi
unset target

alias tmux="TERM=screen-256color tmux"

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


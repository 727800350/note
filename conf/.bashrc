# .bashrc

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

# Uncomment the following line if you don't like systemctl's auto-paging feature:
# export SYSTEMD_PAGER=

# texlive env
export TEXLIVE=/usr/local/texlive/2012
export PATH=$PATH:$TEXLIVE/bin/x86_64-linux
export MANPATH=$MANPATH:$TEXLIVE/texmf/doc/man
export INFOPATH=$INFOPATH:$TEXLIVE/texmf/doc/info

# User specific aliases and functions
alias cp='acp -g'
alias mv='amv -g'

## maven
export MAVEN=/home/eric/program/apache-maven-3.2.5
export PATH=$PATH:$MAVEN/bin

## functions
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


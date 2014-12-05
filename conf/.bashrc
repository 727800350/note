# .bashrc

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

# Uncomment the following line if you don't like systemctl's auto-paging feature:
# export SYSTEMD_PAGER=

# texlive env
export TEXLIVE=/home/eric/program/texlive/2012
export PATH=$PATH:$TEXLIVE/bin/x86_64-linux
export MANPATH=$MANPATH:$TEXLIVE/texmf/doc/man
export INFOPATH=$INFOPATH:$TEXLIVE/texmf/doc/info

# User specific aliases and functions
alias cp='acp -g'
alias mv='amv -g'


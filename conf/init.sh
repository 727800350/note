#!/bin/bash
set -x

source ${HOME}/git/note/conf/func.sh || exit 1

version="7"

yum install -y yum-plugin-priorities
CHK_RET WARNING "install yum plugin priorities error"

yum -y localinstall http://pkgs.repoforge.org/rpmforge-release/rpmforge-release-0.5.3-1.el${version}.rf.x86_64.rpm
CHK_RET FATAL "install repo rpmforge error"

yum -y localinstall http://www.elrepo.org/elrepo-release-7.0-2.el7.elrepo.noarch.rpm
CHK_RET FATAL "install repo elrepo error"

yum -y localinstall http://dl.fedoraproject.org/pub/epel/epel-release-latest-${version}.noarch.rpm
CHK_RET FATAL "install repo epel error"

yum-config-manager --add-repo=https://copr.fedoraproject.org/coprs/mosquito/myrepo/repo/epel-${version}/mosquito-myrepo-epel-${version}.repo
CHK_RET FATAL "install repo mosquite-myrepo-epel error"

yum -y localinstall http://li.nux.ro/download/nux/dextop/el${version}/x86_64/nux-dextop-release-0-1.el${version}.nux.noarch.rpm
CHK_RET FATAL "install repo nux-desktop-release error"

yum -y localinstall http://download1.rpmfusion.org/nonfree/el/updates/${version}/x86_64/rpmfusion-nonfree-release-${version}-1.noarch.rpm
CHK_RET FATAL "install repo rpmfusion-nonfree error"

yum -y localinstall http://download1.rpmfusion.org/free/el/updates/${version}/x86_64/rpmfusion-free-release-${version}-1.noarch.rpm
CHK_RET FATAL "install repo rpmfusion-free error"

yum repolist

yum clean all
yum makecache

exit 0


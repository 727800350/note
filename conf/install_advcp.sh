#!/bin/bash
## do not use the root permission to execute this file

wget https://aur.archlinux.org/packages/ad/advcp/advcp.tar.gz
tar -zxvf advcp.tar.gz

source advcp/PKGBUILD

wget http://ftp.gnu.org/gnu/coreutils/coreutils-"${_pkgver}".tar.xz
tar -Jxvf coreutils-"${_pkgver}".tar.xz

cd coreutils-"${_pkgver}"

printf "Patching...\n"
patch -p1 -i ../advcp/advcpmv-"${_pkgver}"_"${pkgver}".patch || exit 1

printf "Start making...\n"
./configure || exit 1
make || exit 1

sudo install -Dm 755 "${PWD}"/src/cp /usr/local/bin/acp
sudo install -Dm 755 "${PWD}"/src/mv /usr/local/bin/amv

printf "Installation succeeded!\n"
cd ~/git/note/conf
rm -rf advcp/
rm advcp.tar.gz
rm -rf coreutils*
printf "Don't forget to add \"alias cp='acp -g'\" and \"alias mv='amv -g'\" in your bashrc!\n"

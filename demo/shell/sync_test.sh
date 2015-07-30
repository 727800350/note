#!/bin/bash
## sync with test machine

set -x

rsync -vzrtopg --progress -e ssh --delete img@cq01-image-rdtest1006.vm.baidu.com:/home/img/git/* /home/users/wangchao34/git/

<<COM
v: verbose
z: compress
r: recursive
topg: keep time, owner, permission group info
progress: show progress
delete: delete dst file if this file does not exist in src
COM

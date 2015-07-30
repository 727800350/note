#!/bin/bash
## sync with dev machine

set -x

rsync -vzrtopg --progress -e ssh --delete wangchao34@cp01-rdqa04-dev123.cp01.baidu.com:/home/users/wangchao34/git/* /home/img/git/

<<COM
v: verbose
z: compress
r: recursive
topg: keep time, owner, permission group info
progress: show progress
delete: delete dst file if this file does not exist in src
COM

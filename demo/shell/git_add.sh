#!/bin/bash

set -x

dir=`pwd`
cd ${dir}

find . | grep \\.sh$ | xargs git add 2>/dev/null
find . | grep \\.py$ | xargs git add 2>/dev/null
find . | grep \\.awk$ | xargs git add 2>/dev/null

find . | grep \\.cpp$ | xargs git add 2>/dev/null
find . | grep \\.c$ | xargs git add 2>/dev/null

find . | grep \\.sql$ | xargs git add 2>/dev/null

find . | grep \\.conf$ | xargs git add 2>/dev/null
find . | grep \\.env$ | xargs git add 2>/dev/null
find . | grep \\.inc$ | xargs git add 2>/dev/null
find . | grep \\.xml$ | xargs git add 2>/dev/null

git commit -m "add $@ `basename ${dir}`"
git push -v origin `where.sh`

exit 0


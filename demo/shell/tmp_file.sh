#!/bin/bash
## create and use a temp file

tmp=`mktemp tmpfile.XXXXX`

exec 3> $tmp

echo "this is the first line" >&3
echo "this is the second line" >&3
echo "this is the third line" >&3

exec 3>&-

echo "Done creaing tmp file. The contents are:"
cat $tmp
rm -f $tmp

echo
tmp=`mktemp -t tmpfile2.XXXXX`
echo "this is the first line" > $tmp
echo "this is the second line" >> $tmp
echo "this is the third line" >> $tmp
echo "The tmp file is located at : $tmp"
echo "It's contents are:"
cat $tmp
rm -f $tmp


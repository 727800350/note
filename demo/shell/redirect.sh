#!/bin/bash
## redirect the stdout, and then restore it

exec 3>&1
exec 1> log.out
echo "this should go to the output file"

exec 1>&3
echo "now things should be back to normal"


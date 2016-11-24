#!/bin/bash
set -x

bash -x shell/hadoop.sh
if [ $? -ne 0 ]
then
	echo "hadoop error" >&2
	exit 1
fi

exit 0


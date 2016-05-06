#!/bin/bash

set -x

cat input.demo | bash shell/mapper.sh 1 | sort | bash shell/reducer.sh 1

exit 0


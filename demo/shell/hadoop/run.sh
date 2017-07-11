#!/bin/bash
set -x

source ./conf/common.conf || exit 1
source ./conf/func.sh || exit 1

bash -x shell/hadoop.sh
CHK_RET FATAL "$job_name error"

exit 0


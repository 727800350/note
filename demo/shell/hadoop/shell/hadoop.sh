#!/bin/bash
set -x

source ./conf/common.conf
if [ $? -ne 0 ]
then
	echo source common.conf failed 1>&2
	exit 1
fi

source ./conf/func.sh
if [ $? -ne 0 ]
then
	echo source func.sh failed 1>&2
	exit 1
fi

${local_hadoop} dfs -test -d ${top_dir}
if [ $? -ne 0 ]
then
	${local_hadoop} dfs -mkdir ${top_dir}
fi

${local_hadoop} dfs -rmr ${output}

files=""
for file in `ls bin/* conf/* shell/*`
do
	files="$files -file $file"
done

${local_hadoop} streaming \
	-input ${input} \
	-output ${output} \
	-mapper "bash mapper.sh 0" \
	-reducer "bash reducer.sh 0" \
	-partitioner "org.apache.hadoop.mapred.lib.KeyFieldBasedPartitioner" \
	-jobconf mapred.map.tasks="$mapper_num" \
	-jobconf mapred.job.map.capacity="$mapper_capacity" \
	-jobconf mapred.map.over.capacity.allowed="false" \
	-jobconf mapred.map.tasks.speculative.execution="true" \
	-jobconf mapred.reduce.tasks="$reducer_num" \
	-jobconf mapred.job.reduce.capacity="$reducer_capacity" \
	-jobconf mapred.reduce.over.capacity.allowed="false" \
	-jobconf mapred.reduce.tasks.speculative.execution="true" \
	-jobconf stream.memory.limit="800" \
	-jobconf mapred.max.map.failures.percent="5" \
	-jobconf mapred.map.max.attempts="10" \
	-jobconf mapred.job.name="${owner}_${job_name}" \
	-jobconf mapred.job.priority="${priority}" \
	$files

CHK_RET FATAL "$jobname failed"

exit 0


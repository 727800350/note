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

${local_hadoop} dfs -rmr ${result}
${local_hadoop} dfs -mkdir ${result}

${local_hadoop} streaming \
	-input ${input} \
	-output ${output} \
	-mapper "bash mapper.sh 0" \
	-reducer "bash reducer.sh 0" \
	-partitioner org.apache.hadoop.mapred.lib.KeyFieldBasedPartitioner \
	-inputformat org.apache.hadoop.mapred.CombineTextInputFormat \
	-jobconf mapred.max.split.size="1073741824" \
	-jobconf mapred.compress.map.output="true" \
	-jobconf mapred.map.output.compression.codec="org.apache.hadoop.io.compress.QuickLzCodec" \
	-jobconf mapred.map.tasks="$mapper_num" \
	-jobconf mapred.job.map.capacity="$mapper_capacity" \
	-jobconf mapred.map.over.capacity.allowed="false" \
	-jobconf mapred.reduce.tasks="$reducer_num" \
	-jobconf mapred.job.reduce.capacity="$reducer_capacity" \
	-jobconf mapred.reduce.over.capacity.allowed="false" \
	-jobconf stream.memory.limit="800" \
	-jobconf mapred.max.map.failures.percent="5" \
	-jobconf mapred.map.tasks.speculative.execution="false" \
	-jobconf mapred.map.max.attempts="10" \
	-jobconf mapred.job.name="${jobname}" \
	-jobconf mapred.job.priority="${priority}" \
	-file ./shell/mapper.sh \
	-file ./shell/reducer.sh \
	-file ./conf/common.conf \
	-file ./conf/func.sh

CHK_RET FATAL "$jobname failed"

exit 0


#!/bin/bash
## commonly used functions

# check existence of regular file or directory
function exist_e(){
	file=$1
	if [ ! -e ${file} ]
	then
		echo "${file} does not exist."
		return 1
	fi
	return 0
}

function exist_es(){
	files=$*
	for file in ${files}
	do
		exist_e ${file} || return 1
	done
	return 0
}

function exist_f(){
	file=$1
	if [ ! -f ${file} ]
	then
		echo "${file} does not exist."
		return 1
	fi
	return 0
}

function exist_fs(){
	files=$*
	for file in ${files}
	do
		exist_f ${file} || return 1
	done
	return 0
}

function exist_d(){
	dir=$1
	if [ ! -d ${dir} ]
	then
		echo "${dir} does not exist."
		return 1
	fi
	return 0
}

function exist_ds(){
	dirs=$*
	for dir in ${dirs}
	do
		exist_d ${dir} || return 1
	done
	return 0
}


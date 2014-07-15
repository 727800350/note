#!/bin/bash
# GBK <-> UTF-8
#v1.0 2012/2/29
#v1.1 2012/10/17

#config
SUFFIX="*.h *.c *.hpp *.cpp"
declare -a dir_list

#begin
debug="false"
do_something=GBKtoUTF8
expr $0 : '.*\(toGBK\).*' 1>/dev/null &&  do_something=UTF8toGBK
expr $0 : '.*\(toUTF\).*' 1>/dev/null &&  do_something=GBKtoUTF8
to_UTF=1
if [ "${do_something}" == "UTF8toGBK" ]; then
    to_UTF=0
fi

function usage(){
    echo $0 [-h\|--help] [-d\|--debug] [-t\|--detect] [--suffix=str] dir1 [dir2] [dir3...]
}

function isUTF8(){   
    #try convert
    /usr/bin/iconv -f UTF-8 -t UTF-8 $1 > $1.icv 2>/dev/null
    #compare
    cmp -s $1 $1.icv
    local ret=$?
    rm -f $1.icv
    return ${ret}
}

function isGBK(){
    #first MS-ANSI ?
    /usr/bin/iconv -f ASCII -t ASCII $1 > $1.icv 2>/dev/null
    if [ $? -eq 0 ]; then
        rm -f $1.icv
        return 0;
    fi
    #UTF-8 ?
    /usr/bin/iconv -f UTF-8 -t UTF-8 $1 > $1.icv 2>/dev/null
    if [ $? -eq 0 ]; then
        rm -f $1.icv
        return 1;
    fi

    #try convert
    /usr/bin/iconv -f GB18030 -t GB18030 $1 > $1.icv 2>/dev/null
    #compare
    cmp -s $1 $1.icv
    local ret=$?
    rm -f $1.icv
    return ${ret}   
}

function UTF8toGBK(){
    isGBK $1
    if [ $? -ne 0 ]; then
        #is UTF-8
        iconv -f UTF-8 -t GB18030 $1 > $1.icv
        if [ $? -ne 0 ]; then
            echo "  $PWD/$1 from UTF-8 to GBK fail!"  1>&2
            echo fail $PWD/$1
            rm -f $1.icv
            return 1
        fi
        mv -f $1.icv $1
        echo yes $PWD/$1
    else
        echo nil $PWD/$1
    fi
}

function GBKtoUTF8(){
    isUTF8 $1
    if [ $? -ne 0 ]; then
        #not UTF-8
        iconv -f GB18030 -t UTF-8 $1 > $1.icv
        if [ $? -ne 0 ]; then
            echo "  $PWD/$1 from GBK to UTF-8 fail!"  1>&2
            echo fail $PWD/$1
            rm -f $1.icv
            return 1
        fi
        mv -f $1.icv $1
        echo yes $PWD/$1
    else
        echo nil $PWD/$1
    fi  
}

function chars_detect(){
    if [ ${to_UTF} -eq 1 ]; then
        isUTF8 $1 && echo UTF-8 $PWD/$1 || echo GBK $PWD/$1 
    else
        isGBK $1 && echo GBK $PWD/$1 || echo UTF-8 $PWD/$1
    fi
}

function scan_dir(){
    pushd $1 1>/dev/null
    local i
    for i in ${SUFFIX}; do
        [ -e $i ] && ${do_something} $i
    done
    
    for file in *; do
        test -d $file 2>/dev/null && scan_dir $file
    done
    popd 1>/dev/null
}

#param handler
i=0
until [ -z "$1" ]; do
    if [ "$1" == "--help" -o "$1" == "-h" ]; then
        usage
        exit 1      
    elif [ "$1" == "--detect" -o "$1" == "-t" ]; then
        do_something=chars_detect
    elif [ "$1" == "--debug" -o "$1" == "-d" ]; then
        debug="true"
    elif [ ${1:0:9} == "--suffix=" ]; then
        SUFFIX=${1:9}
    else
        dir_list[$i]=$1
        i=`expr $i + 1`
    fi

    shift
done

[ "${#dir_list[@]}" == "0" ] && dir_list[1]="."

if [ ${debug} == "true" ]; then
    echo "do_something=${do_something}"
    echo "suffix=${SUFFIX}"
    for dir in ${dir_list[@]}; do
        echo "do dir-> ${dir}"
    done
    exit 0
fi

for dir in ${dir_list[@]}; do
    [ -d ${dir} ] && scan_dir ${dir} || echo "${dir} isn't dir"
done


#!/bin/awk
## usage
## cat input | awk -F '\t' -v default="default_value" -f merger.awk

#input
#cs \t 0 \t value
#cs \t 1 \t fileid \t lineid

#output
#fileid \t lineid \t cs \t flag \t value
## if flag=1, then has value
## if flag=0, then value is set to default

BEGIN{
	lastkey = "";
	n = 0 ;
	value = default;
	has_value = 0;
}
{
	#contsign 0,0 线上不处理
	if($1 == "0,0")
	{
		if($2 == "1")
		{
			printf("%s\t%s\t%s\t%s\t%s\n", $3, $4, $1, 0, 0);
		}
	}
	else
	{
		#处理第一行时，n=0，也不输出
		if(lastkey != $1 || n > 20000){
			for(i=0; i<n; i++){
				printf("%s\t%s\t%s\t%s\t%s\n", fls[i], lines[i], lastkey, has_value, value);
			}
			n = 0;
			value = default;
			has_value = 0;
			lastkey="";
		}
		#考虑求交的5种情况
		if(lastkey == "" || lastkey == $1)
		{
			#通过flag来表示数据源
			if($2 == "1"){
			#只有contsign.0数据时，value=0
				fls[n]=$3;
				lines[n]=$4;
				n += 1;
				if ((n%10000)==0){
					printf("%s same %d\n", $1, n) > "/dev/stderr";
				}
			}else if($2 == "0"){
			#只有dumptnbl数据的情况,n=0
				if(has_value == 1){
				#TODO由于存在僵尸记录（有key无value的记录），会导致算法不稳定
					printf("same contsign: %s values: %s value2: %s\n", lastkey, value, $3) > "/dev/stderr";
				}
				value = $3;
				has_value = 1;
			}else{
				printf("format error: %s\n", $0) > "/dev/stderr";
				exit;
			}
		}

		lastkey=$1;
	}
}
END{
	#输出最后一组
	for(i=0; i<n; i++){
		printf("%s\t%s\t%s\t%s\t%s\n", fls[i], lines[i], lastkey, has_value, value);
	}
}


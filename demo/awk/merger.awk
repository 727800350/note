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
	#contsign 0,0 ���ϲ�����
	if($1 == "0,0")
	{
		if($2 == "1")
		{
			printf("%s\t%s\t%s\t%s\t%s\n", $3, $4, $1, 0, 0);
		}
	}
	else
	{
		#�����һ��ʱ��n=0��Ҳ�����
		if(lastkey != $1 || n > 20000){
			for(i=0; i<n; i++){
				printf("%s\t%s\t%s\t%s\t%s\n", fls[i], lines[i], lastkey, has_value, value);
			}
			n = 0;
			value = default;
			has_value = 0;
			lastkey="";
		}
		#�����󽻵�5�����
		if(lastkey == "" || lastkey == $1)
		{
			#ͨ��flag����ʾ����Դ
			if($2 == "1"){
			#ֻ��contsign.0����ʱ��value=0
				fls[n]=$3;
				lines[n]=$4;
				n += 1;
				if ((n%10000)==0)			
					system("echo ' " $1 "same : " n " ' 1>&2");
			}else if($2 == "0"){
			#ֻ��dumptnbl���ݵ����,n=0
				if(has_value == 1){
				#TODO���ڴ��ڽ�ʬ��¼����key��value�ļ�¼�����ᵼ���㷨���ȶ�
					system("echo '""same contsign: " lastkey  " value1: " value  " value2: " $3" ' 1>&2");
				}
				value = $3;
				has_value = 1;
			}else{
				system("echo ' " "format err : " $0 " ' 1>&2");
				exit;
			}
		}

		lastkey=$1;
	}
}
END{
	#������һ��
	for(i=0; i<n; i++){
		printf("%s\t%s\t%s\t%s\t%s\n", fls[i], lines[i], lastkey, has_value, value);
	}
}


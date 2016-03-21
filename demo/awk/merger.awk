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
	## do not process contsign 0,0
	if($1 == "0,0")
	{
		if($2 == "1")
		{
			printf("%s\t%s\t%s\t%s\t%s\n", $3, $4, $1, 0, 0);
		}
	}
	else
	{
		## the first line, n = 0, do not print out
		if(lastkey != $1 || n > 20000){
			for(i = 0; i < n; i ++){
				printf("%s\t%s\t%s\t%s\t%s\n", fls[i], lines[i], lastkey, has_value, value);
			}
			n = 0;
			value = default;
			has_value = 0;
			lastkey = "";
		}
		## 5 cases of merge
		if(lastkey == "" || lastkey == $1)
		{
			## use flat to indicate the data source
			if($2 == "1"){
			## value = 0 when there is only contsign.0
				fls[n] = $3;
				lines[n] = $4;
				n += 1;
				if ((n % 10000) == 0){
					printf("%s same %d\n", $1, n) > "/dev/stderr";
				}
			}else if($2 == "0"){
				## n = 0 when there is only dumptnbl
				if(has_value == 1){
				## TODO: when the record has a key and it has not the vlaue, it will destabilize the algo
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
	## print out the last record
	for(i=0; i<n; i++){
		printf("%s\t%s\t%s\t%s\t%s\n", fls[i], lines[i], lastkey, has_value, value);
	}
}


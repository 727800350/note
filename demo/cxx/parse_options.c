 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h> // getopt
 #include <ctype.h> // isprint

int usage(){
	char program[] = "parse_option_demo";
	fprintf(stderr, "usage:\n");
	fprintf(stderr, "%s -a -b -n num -c str\n", program);
	return 0;
}

int main(int argc, char *argv[]){
	int aflag = 0;
	int bflag = 0;
	int num = 0;
	char *str = NULL;

	int c;
	opterr = 0;
	while((c = getopt(argc, argv, "abn:c:")) != -1){
		switch(c){
		 case 'a':
			aflag = 1;
			break;
		 case 'b':
			bflag = 1;
			break;
		 case 'n':
			num = atoi(optarg);
			break;
		 case 'c':
			str = optarg;
			break;
		 case '?':
			if(isprint(optopt))
				fprintf(stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
			usage();
			return 1;
		default:
			abort();
		}
	}

	fprintf(stderr, "aflag = %d, bflag = %d, num = %d, str = %s\n", aflag, bflag, num, str);

	// args not prefixed by optstring
	for(index = optind; index < argc; index++){
		fprintf(stderr, "Non-option argument %s\n", argv[index]);
	}

	return 0;
}


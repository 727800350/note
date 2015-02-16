 #include <ctype.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>

 int main (int argc, char **argv){
	int aflag = 0;
	int bflag = 0;
	int num;
	char *str = NULL;
	int index;
	int c;

	opterr = 0;

	while ((c = getopt (argc, argv, "abn:c:")) != -1){
		switch (c){
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
			return 1;
		default:
			abort();
		} //end switch
	} //end while

	printf ("aflag = %d, bflag = %d, num = %d, str = %s\n", aflag, bflag, num, str);
	printf("optind = %d\n", optind);

// 	in the following for loop, the value of optind does not change
	for (index = optind; index < argc; index++){
		printf ("Non-option argument %s\n", argv[index]);
 	}
	return 0;
}//end main

/*
[eric@human c]$ ./a.out -a -b -n 2 -c foo arg1 arg2
aflag = 1, bflag = 1, num = 2, str = foo
optind = 7
Non-option argument arg1
Non-option argument arg2
**/

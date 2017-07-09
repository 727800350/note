 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h> 

void usage(const char *prog){
	fprintf(stderr, "%s -e -n num -h\n", prog);
}

int main(int argc, char *argv[]){
	bool enable = false;
	int num = 0;

	int opt = 0;
	while((opt = getopt(argc, argv, "en:h")) != -1){
		switch(opt){
			case 'e':
				enable = true;
				break;
			case 'n':
				num = atoi(optarg);
				break;
			case 'h':
				usage(argv[0]);
				return 0;
			default:
				usage(argv[0]);
				return -1;
		}
	}

	fprintf(stderr, "enable: %d, num: %d\n", enable, num);

	// args not prefixed by optstring
	for(int i = optind; i < argc; i++){
		fprintf(stderr, "Non-option argument %s\n", argv[i]);
	}

	return 0;
}


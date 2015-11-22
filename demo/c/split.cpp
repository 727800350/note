#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // getopt
#include <ctype.h> // isprint
#include <sys/types.h>

// The default stack max size is 1 MB
#define BUFFER_SIZE (1024 * 1024)

int usage(){
	char program[] = "split";
	fprintf(stderr, "%s -f file -l length -u unit(B|KB|MB|GB) -e\n", program);
	return 0;
}

int main(int argc, char *argv[]){
	bool encryption = false; // whether encryption
	char *file = NULL;
	u_int64_t len = 0; // length of each split part
	char *unit = NULL;
	char buffer[BUFFER_SIZE] = {0};

	int ret = -1;

	char c = 0;
	while ((c = getopt(argc, argv, "ef:l:u:")) != -1){
		switch(c){
		 case 'e':
			encryption = true;
			break;
		 case 'f':
			file = optarg;
			break;
		 case 'l':
			len = atoi(optarg);
			break;
		 case 'u':
			unit = optarg;
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

	if(file == NULL or len == 0 or unit == NULL){
		fprintf(stderr, "wrong args\n");
		usage();
		return 1;
	}

	fprintf(stderr, "file: %s, len: %lu, unit: %s, encryption: %d\n", file, len, unit, encryption);

	unit[0] = tolower(unit[0]);
	if(unit[0] == 'b'){
		;
	}
	else if(unit[0] == 'k'){
		len = len * 1024;
	}
	else if(unit[0] == 'm'){
		len = len * 1024 * 1024;
	}
	else if(unit[0] == 'g'){
		len = len * 1024 * 1024 * 1024;
	}
	else{
		fprintf(stderr, "unit %s not supported.\n", unit);
		return 1;
	}

	FILE *input = fopen(file, "rb");
	char outfile[100] = {0};
	FILE *output = NULL;
	int num = 0;
	u_int64_t sum = 0;

	memset(outfile, 0, 100);
	// 4 is for ".xx\0"
	snprintf(outfile, strlen(file) + 4, "%s.%02d", file, num);
	fprintf(stderr, "output file: %s\n", outfile);
	output = fopen(outfile, "wb");

	while(true){
		ret = fread(buffer, sizeof(char), sizeof(buffer), input);
		if(ret < 0){
			fprintf(stderr, "read error\n");
			return ret;
		}
		if(ret == 0){
			fprintf(stderr, "eof\n");
			break;
		}
		ret = fwrite(buffer, sizeof(char), ret, output);
		sum += ret;
		if(encryption or sum >= len){
			fclose(output);	
			sum = 0;

			num ++;
			memset(outfile, 0, 100);
			snprintf(outfile, strlen(file) + 4, "%s.%02d", file, num);
			fprintf(stderr, "output file: %s\n", outfile);
			output = fopen(outfile, "wb");

			// disable encryption for the rest of file
			encryption = false;
		}
	}

	if(input != NULL){
		fclose(input);
	}
	if(output != NULL){
		fclose(output);
	}

	return 0;
}


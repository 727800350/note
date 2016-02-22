#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // getopt
#include <ctype.h> // isprint

// The default stack max size is 1 MB
#define BUFFER_SIZE (1 * 1024 * 1024)

int usage(){
	char program[] = "kv_coding";
	fprintf(stderr, "%s -f file -o encode -k key > target\n", program);
	fprintf(stderr, "%s -f file -o decode > target\n", program);
	fprintf(stderr, "cat file | %s -o decode > target\n", program);
	return 0;
}

int main(int argc, char *argv[]){
	char *file = NULL;
	FILE *input = NULL;
	char *op = NULL;
	char *key = NULL;
	int kl = 0;
	char buffer[BUFFER_SIZE];
	u_int64_t vl = 0;

	int ret = -1;

	char c = 0;
	while ((c = getopt(argc, argv, "f:o:k:")) != -1){
		switch(c){
		 case 'f':
			file = optarg;
			break;
		 case 'o':
			op = optarg;
			break;
		 case 'k':
			key = optarg;
			break;
		 case '?':
			if(isprint(optopt))
				fprintf(stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
			usage();
			return -1;
		default:
			abort();
		}
	}

	// check args
	if(op == NULL){
		fprintf(stderr, "wrong args\n");
		usage();
		return -1;
	}

	if(strcmp(op, "encode") == 0){
		if(file == NULL){
			fprintf(stderr, "wrong args\n");
			usage();
			return -1;
		}

		input = fopen(file, "rb");
		if(input == NULL){
			fprintf(stderr, "%s open error.\n", file);
			usage();
			return -1;
		}

		if(key == NULL){
			key = file;
		}
	}else if(strcmp(op, "decode") == 0){
		if(file == NULL){
			input = stdin;
		}else{
			input = fopen(file, "rb");
			if(input == NULL){
				fprintf(stderr, "%s open error.\n", file);
				usage();
				return -1;
			}
		}
	}else{
		usage();
		return -1;
	}

	if(strcmp(op, "encode") == 0){
		// write out key length
		kl = strlen(key);
		ret = fwrite(&kl, sizeof(int), 1, stdout);
		if(ret < 1){
			fprintf(stderr, "write key length error\n");
			return -1;
		}

		// write out key
		ret = fwrite(key, sizeof(char), kl, stdout);
		if(ret < kl){
			fprintf(stderr, "write key error\n");
			return -1;
		}
		
		// get value length
		fseek(input, 0, SEEK_END);
		vl = ftell(input);
		fseek(input, 0, SEEK_SET);

		// write out value length
		ret = fwrite(&vl, sizeof(int), 1, stdout);
		if(ret < 1){
			fprintf(stderr, "write value length error\n");
			return -1;
		}
		fprintf(stderr, "op:%s, k:%s, kl:%d, vl:%lu\n", op, key, kl, vl);
	}


	if(strcmp(op, "decode") == 0){
		// read in key length
		ret = fread(&kl, sizeof(int), 1, input);
		if(ret < 1){
			fprintf(stderr, "read key length error\n");
			return -1;
		}

		// read in key
		key = (char *)malloc(sizeof(char) * kl);
		ret = fread(key, sizeof(char), kl, input);
		if(ret < kl){
			fprintf(stderr, "read key error\n");
			return -1;
		}

		// read in value length
		ret = fread(&vl, sizeof(int), 1, input);
		if(ret < 1){
			fprintf(stderr, "read value length error\n");
			return -1;
		}
		fprintf(stderr, "op:%s, k:%s, kl:%u, vl:%lu\n", op, key, kl, vl);

		free(key);
		key = NULL;
	}

	// process value
	u_int64_t sum_read = 0;
	u_int64_t sum_write = 0;
	while(true){
		ret = fread(buffer, sizeof(char), BUFFER_SIZE, input);
		if(ret < 0){
			fprintf(stderr, "read error\n");
			return ret;
		}
		if(ret == 0){
			fprintf(stderr, "reach EOF\n");
			break;
		}
		sum_read += ret;

		ret = fwrite(buffer, sizeof(char), ret, stdout);
		if(ret < 0){
			fprintf(stderr, "write error\n");
			return ret;
		}
		if(ret == 0){
			fprintf(stderr, "write None\n");
			break;
		}
		sum_write += ret;
	}
	fprintf(stderr, "value    read: %lu bytes\n", sum_read);
	fprintf(stderr, "value written: %lu bytes\n", sum_write);
	return 0;
}


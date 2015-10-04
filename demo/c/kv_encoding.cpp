#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // getopt
#include <ctype.h> // isprint

#define BUFFER_SIZE (1 * 1024 * 1024)

int usage(){
	char program[] = "kv_coding";
	fprintf(stderr, "usage: read from stdin and write to stdout\n");
	fprintf(stderr, "%s -o encode -k key -l content_length(in bytes)\n", program);
	fprintf(stderr, "%s -o decode\n", program);
	return 0;
}

int main(int argc, char *argv[]){
	char *op = NULL;
	char *key = {0};
	int kl = 0;
	char buffer[BUFFER_SIZE];
	int vl = 0;

	int ret = -1;

	char c;
	while ((c = getopt(argc, argv, "o:k:l:")) != -1){
		switch(c){
		 case 'o':
			op = optarg;
			break;
		 case 'k':
			key = optarg;
			break;
		 case 'l':
			vl = atoi(optarg);
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

	if(strcmp(op, "encode") == 0){
		if(key == NULL or vl == 0){
			usage();
			return -1;
		}
	}
	else if(strcmp(op, "decode") == 0){
		// do not have a pass statement, so ...
		key = NULL;
		vl = 0;
	}
	else{
		usage();
		return -1;
	}

	if(strcmp(op, "encode") == 0){
		// write out key length
		kl = strlen(key) + 1;
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

		// write out value length
		ret = fwrite(&vl, sizeof(int), 1, stdout);
		if(ret < 1){
			fprintf(stderr, "write value length error\n");
			return -1;
		}
		fprintf(stderr, "op:%s, k:%s, kl:%u, vl:%u\n", op, key, kl, vl);
	}


	if(strcmp(op, "decode") == 0){
		// read in key length
		ret = fread(&kl, sizeof(int), 1, stdin);
		if(ret < 1){
			fprintf(stderr, "read key length error\n");
			return -1;
		}

		// read in key
		key = (char *)malloc(sizeof(char) * kl);
		ret = fread(key, sizeof(char), kl, stdin);
		if(ret < kl){
			fprintf(stderr, "read key error\n");
			return -1;
		}

		// read in value length
		ret = fread(&vl, sizeof(int), 1, stdin);
		if(ret < 1){
			fprintf(stderr, "read value length error\n");
			return -1;
		}
		fprintf(stderr, "op:%s, k:%s, kl:%u, vl:%u\n", op, key, kl, vl);

		free(key);
		key = NULL;
	}

	// process value
	int sum_read = 0;
	int sum_write = 0;
	while(true){
		ret = fread(buffer, sizeof(char), BUFFER_SIZE, stdin);
		if(ret < 0){
			fprintf(stderr, "read error");
			return ret;
		}
		if(ret == 0){
			fprintf(stderr, "reach EOF\n");
			break;
		}
		vl = ret;
		sum_read += vl;

		ret = fwrite(buffer, sizeof(char), vl, stdout);
		sum_write += ret;
		if(ret < vl){
			fprintf(stderr, "write error");
			return -1;
		}
	}
	fprintf(stderr, "value    read: %d bytes\n", sum_read);
	fprintf(stderr, "value written: %d bytes\n", sum_write);
	return 0;
}


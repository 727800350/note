#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h> // getopt
#include <ctype.h> // isprint
#include <sys/types.h>
#include <pthread.h>

#define BUFFER_SIZE (1024 * 1024) // 1MB buffer
#define GAP 20 // 2 GB for a split part
// bool encryption = false; // whether encryption

// global variable
char *file = NULL;
u_int64_t gap = GAP;

int usage(){
	char program[] = "couper";
	fprintf(stderr, "%s -f file\n", program);

	return 0;
}

// extract the ith part of file
void *extract(void *i){
	int part = *(int *)i;
	fprintf(stderr, "part %d of %s\n", part, file);

	// open input file
	FILE *input = NULL;
	input = fopen(file, "rb");
	assert(input != NULL);
	fseek(input, gap * part, SEEK_SET);

	// pen output file
	char outfile[100] = {'\0'};
	// 4 is for ".xx\0"
	snprintf(outfile, strlen(file) + 4, "%s.%02d", file, part);
	fprintf(stderr, "output file: %s\n", outfile);
	FILE *output = NULL;
	output = fopen(outfile, "wb");
	assert(output != NULL);

	char buffer[BUFFER_SIZE] = {'\0'};
	u_int64_t sum = 0;
	int ret = 0;
	while(true){
		ret = fread(buffer, sizeof(char), sizeof(buffer), input);
		if(ret < 0){
			fprintf(stderr, "read error\n");
			return NULL;
		}
		if(ret == 0){
			fprintf(stderr, "reach eof\n");
			break;
		}
		ret = fwrite(buffer, sizeof(char), ret, output);
		sum += ret;
		if(sum >= gap){
			break;
		}
	}

	if(input != NULL){
		fclose(input);
	}
	if(output != NULL){
		fclose(output);
	}

	return NULL;
}

int main(int argc, char *argv[]){
	u_int64_t size = 0;

	char c = 0;
	while ((c = getopt(argc, argv, "ef:")) != -1){
		switch(c){
		 case 'e':
// 			encryption = true;
			break;
		 case 'f':
			file = optarg;
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

	if(file == NULL){
		fprintf(stderr, "wrong args\n");
		usage();
		return 1;
	}

	FILE *input = NULL;
	input = fopen(file, "r");
	assert(input != NULL);
	fseek(input, 0, SEEK_END);
	size = ftell(input);
	fprintf(stderr, "%s has %lu bytes\n", file, size);

	gap = gap * 1024 * 1024;
	fprintf(stderr, "gap = %lu\n", gap);

	int num = size / gap + 1;
	fprintf(stderr, "%s needs %d parts\n", file, num);

	pthread_t *thread = (pthread_t *)malloc(num * sizeof(pthread_t));
	for(int i = 0; i < num; i++){
		pthread_create(thread + i, NULL, &extract, (void *)&i);
		sleep(1);
	}

	for(int i = 0; i < num; i++){
		pthread_join(thread[i], NULL);
	}

	return 0;
}


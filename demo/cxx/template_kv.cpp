#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

const int max_kl = 1024;
const int max_vl = 10 * 1024 * 1024;

void usage(const char *prog){
	fprintf(stderr, "usage: cat - | %s\n", prog);
}

int process(char *k, int vl, char *v){
	(void)v;
	fprintf(stdout, "%s\t%d\n", k, vl);
	return 0;
}

int main(int argc, char* argv[]){
	int opt = 0;
	while((opt = getopt(argc, argv, "h")) != -1){
		switch(opt){
			case 'h':
				usage(argv[0]);
				return 0;
			default:
				usage(argv[0]);
				return -1;
		}
	}

	int kl = 0;
	char *k = new char[max_kl];
	int vl = 0;
	char *v = new char[max_vl];

	int num = 0;
	while(fread(&kl, sizeof(int), 1, stdin) == 1){
		fread(k, sizeof(char), kl, stdin);
		k[kl] = '\0';
		fread(&vl, sizeof(int), 1, stdin);
		fread(v, sizeof(char), vl, stdin);

		int ret = process(k, vl, v);
		if(ret != 0){
			fprintf(stderr, "process %s error %d\n", k, ret);
			return -1;
		}

		num++;
	}

	fprintf(stderr, "total num: %d\n", num);

	delete []k;
	delete []v;

	return 0;
}


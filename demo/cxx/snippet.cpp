#include <stdint.h> // int32_t etc

const int max_kl = 1024;
const int max_vl = 10 * 1024 * 1024;

// txt
	(void)max_kl;
	char *line = new char[max_vl];

	int num = 0;
	while(fgets(line, max_vl, stdin) != NULL){
		line[strlen(line) - 1] = '\0';
		int ret = process(line);
		if(ret != 0){
			LOG(ERROR) << "process " << line << " error";
			return -1;
		}

		num++;
	}

	LOG(INFO) << "total num: " << num;

	delete []line;

// kv
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
			LOG(ERROR) << "process " << k << " error";
			return -1;
		}

		num++;
	}

	LOG(INFO) << "total num: " << num;

	delete []k;
	delete []v;

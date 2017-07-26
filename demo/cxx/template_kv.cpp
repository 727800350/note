#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <glog/logging.h>

const int max_kl = 1024;
const int max_vl = 10 * 1024 * 1024;

void usage(const char *prog){
	LOG(INFO) << "usage: cat - | " << prog << " -lh";
}

int process(char *k, int vl, char *v){
	(void)v;
	LOG(INFO) << k << "\t" << vl;
	return 0;
}

int main(int argc, char* argv[]){
	google::InitGoogleLogging(argv[0]);
	FLAGS_logtostderr = true;
	FLAGS_alsologtostderr = true;
	FLAGS_colorlogtostderr = true;
	FLAGS_stderrthreshold = google::INFO;
	FLAGS_log_dir = "./";

	int opt = 0;
	while((opt = getopt(argc, argv, "lh")) != -1){
		switch(opt){
			case 'l':
				FLAGS_logtostderr = false;
				break;
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
			LOG(FATAL) << "process " << k << " error";
		}

		num++;
	}

	LOG(INFO) << "total num: " << num;

	delete []k;
	delete []v;

	google::ShutdownGoogleLogging();
	return 0;
}


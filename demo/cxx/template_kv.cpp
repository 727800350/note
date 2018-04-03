#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <gflags/gflags.h>
#include <glog/logging.h>

const int max_kl = 1024;
const int max_vl = 10 * 1024 * 1024;

int process(char *k, int vl, char *v){
	(void)v;
	LOG(INFO) << k << "\t" << vl;
	return 0;
}

int main(int argc, char* argv[]){
	google::InitGoogleLogging(argv[0]);

	google::SetVersionString("1.0.0.0");
	google::SetUsageMessage(std::string("usage: cat - | ") + argv[0]);
	google::ParseCommandLineFlags(&argc, &argv, true);

	FLAGS_alsologtostderr = true;
	FLAGS_colorlogtostderr = true;

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

	google::ShutdownGoogleLogging();
	return 0;
}


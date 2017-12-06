#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <glog/logging.h>
#include <gflags/gflags.h>

const int max_kl = 1024;
const int max_vl = 10 * 1024 * 1024;

int process(const char *line){
	fprintf(stdout, "%s\n", line);
	return 0;
}

int main(int argc, char* argv[]){
	google::InitGoogleLogging(argv[0]);

	google::SetVersionString("1.0.0.0");
	google::SetUsageMessage(std::string("usage: cat - | ") + argv[0]);
	google::ParseCommandLineFlags(&argc, &argv, true);

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

	google::ShutdownGoogleLogging();
	return 0;
}


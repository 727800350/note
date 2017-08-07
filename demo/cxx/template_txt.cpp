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

int process(const char *line){
	fprintf(stdout, "%s\n", line);
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


#include <stdio.h>

#include <algorithm>
#include <string>
#include <vector>

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <wcg_api.h>

int main(int argc, char* argv[]){
	google::InitGoogleLogging(argv[0]);
	google::ParseCommandLineFlags(&argc, &argv, true);
	FLAGS_alsologtostderr = true;
	FLAGS_colorlogtostderr = true;

	google::ShutdownGoogleLogging();
	return 0;
}


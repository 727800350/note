/*************************************************************************
	> File Name: demo.cpp
	> Author: Eric Wang
	> Email: wangchaogo1990 at gmail.com
	> Created Time: 2017-07-23 12:29:37
 ************************************************************************/

#include <stdio.h>
#include <glog/logging.h>

int main(int argc, char *argv[]){
	google::InitGoogleLogging(argv[0]);
	FLAGS_logtostderr = true;
	FLAGS_alsologtostderr = true;
    FLAGS_colorlogtostderr = true;
	FLAGS_stderrthreshold = google::INFO;
	FLAGS_log_dir = "./";

	LOG(INFO) << "google log info";
	LOG(WARNING) << "google log warning";
	LOG(ERROR) << "google log error";
	LOG(FATAL) << "google log fatal";

	/* debug mode is default on
	 * in debug mode, DLOG is just like LOG
	 * g++ -DNDEBUG
	 */
	DLOG(INFO) << "google log debug info";
	DLOG(WARNING) << "google log debug warning";
	DLOG(ERROR) << "google log debug error";
// 	DLOG(FATAL) << "google log debug fatal";

	google::ShutdownGoogleLogging();
	return 0;
}

/*
 * note:
 * init: even if in multi thread env, only init once
 *
 * logtostderr: only log to stderr
 *
 * LOG(FATAL): will cause abort(SIGABORT 134), and $? will be 134
*/


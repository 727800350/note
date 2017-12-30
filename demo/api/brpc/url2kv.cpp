#include <stdio.h>
#include <string>
#include <gflags/gflags.h>
#include <brpc/channel.h>

const int max_kl = 1024;
const int max_vl = 10 * 1024 * 1024;

DEFINE_string(protocol, "http", "http");
DEFINE_string(load_balancer, "", "The algorithm for load balancing");
DEFINE_int32(timeout_ms, 1000, "RPC timeout in milliseconds");
DEFINE_int32(max_retry, 3, "Max retries(not including the first RPC)");

int process(brpc::Channel &channel, const char *url){
	brpc::Controller cntl;
	cntl.http_request().uri() = url;
	cntl.http_request().set_method(brpc::HTTP_METHOD_GET);

	channel.CallMethod(NULL, &cntl, NULL, NULL, NULL);
	if(cntl.Failed()){
		LOG(ERROR) << cntl.ErrorText();
	}
	else{
		std::string res = cntl.response_attachment().to_string();
		int kl = strlen(url);
		fwrite(&kl, sizeof(int), 1, stdout);
		fwrite(url, sizeof(char), kl, stdout);
		int vl = res.length();
		fwrite(&vl, sizeof(int), 1, stdout);
		fwrite(res.c_str(), sizeof(char), vl, stdout);
		LOG(INFO) << url << "\t" << res.length();
	}

	return 0;
}

int main(int argc, char* argv[]){
	google::SetVersionString("1.0.0.0");
	google::SetUsageMessage(std::string("usage: cat - | ") + argv[0]);
	google::ParseCommandLineFlags(&argc, &argv, true);

	brpc::Channel channel;
	brpc::ChannelOptions options;
	options.protocol = FLAGS_protocol;
	options.timeout_ms = FLAGS_timeout_ms;
	options.max_retry = FLAGS_max_retry;
	if(channel.Init("imgsrc.baidu.com", FLAGS_load_balancer.c_str(), &options) != 0){
	 	LOG(ERROR) << "Fail to initialize channel";
	 	return -1;
	}

	char *line = new char[max_kl];

	int num = 0;
	while(fgets(line, max_kl, stdin) != NULL){
		line[strlen(line) - 1] = '\0';
		int ret = process(channel, line);
		if(ret != 0){
			LOG(ERROR) << line << " process error";
			return -1;
		}
		num++;
	}
 
	LOG(INFO) << "total num: " << num;

	delete []line;

	return 0;
}


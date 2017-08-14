#include <boost/thread/thread.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/atomic.hpp>

#include <glog/logging.h>

/*
 * boost::atomic_int consumer_count(0);
 */

boost::lockfree::queue<int> queue(128);

void usage(const char *prog){
	LOG(INFO) << "usage: " << prog << " -lh";
	LOG(INFO) << "compile: g++ -lboost_system -lglog";
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

	if(!queue.is_lock_free()){
		LOG(ERROR) << "queue is not lock free";
		return -1;
	}

	for(int i = 0; i < 10; i++){
		queue.push(i);
	}
	LOG(INFO) << queue.empty();

	int value = 0;
	while(not queue.empty()){
		queue.pop(value);
		LOG(INFO) << value;
	}

	google::ShutdownGoogleLogging();
	return 0;
}


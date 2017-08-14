#include <boost/thread/thread.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/atomic.hpp>

#include <atomic>
#include <glog/logging.h>

/*
 * set fixed_sized true to disable dynamic memory allocations during push in order to ensure lockfree behavior
 * there is not queue.size(), so we can use a atomic int to record it
 */
boost::lockfree::queue<int, boost::lockfree::fixed_sized<true> > queue(10);
std::atomic<int> size(0);

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

	for(int i = 0; i < 20; i++){
		bool succ = queue.push(i);
		if(not succ){
			LOG(ERROR) << "push " << i << " error, breaking...";
			break;
		}
		size += 1;
	}
	LOG(INFO) << "queue size: " << size;

	int value = 0;
	while(queue.pop(value)){
		LOG(INFO) << value;
	}

	google::ShutdownGoogleLogging();
	return 0;
}


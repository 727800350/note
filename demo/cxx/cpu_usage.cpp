#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <gflags/gflags.h>
#include <glog/logging.h>

/*
 * g++ cpu_usage.cpp -lglog -lgflags -lpthread
 */

DEFINE_int32(thread_num, 1, "thread num");
DEFINE_int32(percentage, 50, "usage percent");
DEFINE_int32(busy_time, 1000, "busy time(ms)");

typedef struct _thread_info_t{
	int thread_seq_num; // thread sequnence number
	pthread_t thread_id; // thread id
}thread_info_t;

void* worker(void* arg){
	(void)arg;
	long time_idle = 0;
	long time_used = 0;
	struct timeval t_start;
	struct timeval t_end;

	while(1){
		time_used = 0;
		gettimeofday(&t_start, NULL);
		while(time_used <= FLAGS_busy_time){
			gettimeofday(&t_end, NULL);
			time_used = 1000000 * (t_end.tv_sec - t_start.tv_sec) + (t_end.tv_usec - t_start.tv_usec);
			time_used /= 1000;
		}
		/*
		 * busy / (busy + idle) = FLAGS_percentage / 100
		 */
		time_idle = 100 * FLAGS_busy_time / FLAGS_percentage - FLAGS_busy_time;
		sleep(time_idle / 1000);
	}
}

int main(int argc, char* argv[]){
	google::InitGoogleLogging(argv[0]);
	google::ParseCommandLineFlags(&argc, &argv, true);
	FLAGS_alsologtostderr = true;
	FLAGS_colorlogtostderr = true;

	thread_info_t *p_tinfo = new thread_info_t[FLAGS_thread_num];
	if(p_tinfo == NULL){
		LOG(ERROR) << "new error";
		return -1;
	}

	for(int i = 0; i < FLAGS_thread_num; i++){
		p_tinfo[i].thread_seq_num = i + 1;
		pthread_create(&p_tinfo[i].thread_id, NULL, &worker, &p_tinfo[i]);
	}

	for(int i = 0; i < FLAGS_thread_num; i++){
		pthread_join(p_tinfo[i].thread_id, NULL);
	}
	delete []p_tinfo;
	p_tinfo = NULL;

	google::ShutdownGoogleLogging();
	return 0;
}


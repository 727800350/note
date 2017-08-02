#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <ev.h>
#include <glog/logging.h>

/*
 * g++ event.cpp -lev -lglog
 * callback all looks like: void cb_name(struct ev_loop *main_loop, ev_xxx *io_w, int event)
 */

void usage(const char *prog){
	LOG(INFO) << "usage: " << prog << " -lh";
	LOG(INFO) << "compile: g++ -lev -lglog";
}

void io_action(struct ev_loop *main_loop, ev_io *io_w, int e){
	char buf[1024] = {'\0'};
	LOG(INFO) << "in io cb";
	int nread = read(STDIN_FILENO, buf, sizeof(buf));
	if(nread == 0){
		LOG(WARNING) << "read over";
		ev_io_stop(main_loop, io_w);
	}
	fwrite(buf, sizeof(char), nread, stdout);
}

void timer_action(struct ev_loop *main_loop, ev_timer *timer_w, int e){
	LOG(INFO) << "in tiemr cb";
// 	ev_timer_stop(main_loop, timer_w);
}

void signal_action(struct ev_loop *main_loop, ev_signal *signal_w, int e){
	LOG(INFO) << "in signal cb";
	ev_signal_stop(main_loop, signal_w);
	ev_break(main_loop, EVBREAK_ALL);
}

int main(int argc, char *argv[]){
	google::InitGoogleLogging(argv[0]);
	FLAGS_logtostderr = true;
	FLAGS_alsologtostderr = true;
	FLAGS_colorlogtostderr = true;
	FLAGS_stderrthreshold = google::INFO;
	FLAGS_log_dir = "./";

	int opt = 0;
	while((opt = getopt(argc, argv, "lh")) != -1){
		switch (opt){
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

	struct ev_loop *main_loop = ev_default_loop(0);

	ev_io io_w;
	ev_io_init(&io_w, io_action, STDIN_FILENO, EV_READ);
	ev_io_start(main_loop, &io_w);

	ev_timer timer_w;
	ev_timer_init(&timer_w, timer_action, 2, 5); // timeout and period(if period == 0, then not periodic)
	ev_timer_start(main_loop, &timer_w);

	ev_signal signal_w;
	ev_signal_init(&signal_w, signal_action, SIGINT);
	ev_signal_start(main_loop, &signal_w);

	ev_run(main_loop, 0);

	google::ShutdownGoogleLogging();
	return 0;
}


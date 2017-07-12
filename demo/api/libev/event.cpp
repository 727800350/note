#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <ev.h>

/*
 * g++ event.cpp -lev
 * callback all looks like: void cb_name(struct ev_loop *main_loop, ev_xxx *io_w, int event)
 */

void io_action(struct ev_loop *main_loop, ev_io *io_w, int e){
	char buf[1024] = {'\0'};
	fprintf(stderr, "in io cb\n");
	int nread = read(STDIN_FILENO, buf, sizeof(buf));
	if(nread == 0){
		fprintf(stderr, "read over\n");
		ev_io_stop(main_loop, io_w);
	}
	fwrite(buf, sizeof(char), nread, stdout);
}

void timer_action(struct ev_loop *main_loop, ev_timer *timer_w, int e){
	fprintf(stderr, "in tiemr cb\n");
// 	ev_timer_stop(main_loop, timer_w);
}

void signal_action(struct ev_loop *main_loop, ev_signal *signal_w, int e){
	fprintf(stderr, "in signal cb\n");
	ev_signal_stop(main_loop, signal_w);
	ev_break(main_loop, EVBREAK_ALL);
}

int main(int argc, char *argv[]){
	struct ev_loop *main_loop = ev_default_loop(0);
	ev_io io_w;
	ev_timer timer_w;
	ev_signal signal_w;

	ev_io_init(&io_w, io_action, STDIN_FILENO, EV_READ);
	ev_timer_init(&timer_w, timer_action, 2, 5); // timeout and period(if period == 0, then not periodic)
	ev_signal_init(&signal_w, signal_action, SIGINT);

	ev_io_start(main_loop, &io_w);
	ev_timer_start(main_loop, &timer_w);
	ev_signal_start(main_loop, &signal_w);

	ev_run(main_loop, 0);

	return 0;
}


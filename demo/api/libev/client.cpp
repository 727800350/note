#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <ctype.h>
#include <ev.h>
#include <glog/logging.h>

const int max_vl = 1024;

/* epoll ref: http://www.cnblogs.com/Anker/archive/2013/08/17/3263780.html */
void usage(const char *prog) {
	LOG(INFO) << prog << " -i ip -p port";
	LOG(INFO) << "compile: g++ -lev -lglog";
}

void do_write(struct ev_loop *loop, ev_io *watcher, int e);
void do_read(struct ev_loop *loop, ev_io *watcher, int e);
void do_signal(struct ev_loop *loop, ev_signal *watcher, int e);

int make_socket_non_blocking(int fd);

int main(int argc,char *argv[]) {
	google::InitGoogleLogging(argv[0]);
	FLAGS_logtostderr = true;
	FLAGS_alsologtostderr = true;
	FLAGS_colorlogtostderr = true;
	FLAGS_stderrthreshold = google::INFO;
	FLAGS_log_dir = "./";

	char ip[NI_MAXHOST] = "localhost";
	int port = 8000;

	int opt = 0;
	while((opt = getopt(argc, argv, "i:p:lh")) != -1){
		switch(opt){
			case 'i':
				snprintf(ip, NI_MAXHOST, "%s", optarg);
				break;
			case 'p':
				port = atoi(optarg);
				break;
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

	int fd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	inet_pton(AF_INET, ip, &servaddr.sin_addr);
	int ret = connect(fd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	if(ret == -1){
		if(errno == EINPROGRESS){
			LOG(WARNING) << "connect success, but not completed, can be checked by epoll";
		}
		else{
			LOG(ERROR) << "connect to " << ip << ":" << port << " error";
			return -1;
		}
	}

	ret = make_socket_non_blocking(fd);
	if(ret != 0){
		LOG(ERROR) << "make fd " << fd << " non blocking error";
		return -1;
	}

	struct ev_loop *loop = ev_default_loop(0);

	ev_io io;
	ev_io_init(&io, do_write, fd, EV_WRITE);
	ev_io_start(loop, &io);

	ev_signal sig;
	ev_signal_init(&sig, do_signal, SIGINT);
	ev_signal_start(loop, &sig);

	LOG(INFO) << "loop started";
	ev_run(loop, 0);
	LOG(WARNING) << "loop stopped";

	close(fd);
	google::ShutdownGoogleLogging();
	return 0;
}

void do_write(struct ev_loop *loop, ev_io *watcher, int e){
	int fd = watcher->fd;
	char msg[] = "hello from client";
	int nwrite = write(fd, msg, sizeof(msg));
	if(nwrite == -1){
		if(errno == EAGAIN){
			LOG(WARNING) << "resource temp unavailable";
		}
		else{
			LOG(ERROR) << "write error";
			goto end;
		}
	}
	else{
		LOG(INFO) << "write " << nwrite << " bytes to " << fd;
		ev_io_stop(loop, watcher);
		ev_io *io = new ev_io;
		ev_io_init(io, do_read, fd, EV_READ);
		ev_io_start(loop, io);
	}
	return;

end:
	ev_io_stop(loop, watcher);
	ev_break(loop, EVBREAK_ALL);
	return;
}

void do_read(struct ev_loop *loop, ev_io *watcher, int e){
	int fd = watcher->fd;
	char buffer[1024] = {'\0'};
	int nread = read(fd, buffer, sizeof(buffer) - 1);
	if(nread == -1){
		if(errno == EAGAIN){
			LOG(WARNING) << "resource temp unavailable";
		}
		else{
			LOG(ERROR) << "write error";
			goto end;
		}
	}
	else if(nread == 0){
		LOG(WARNING) << "connection closed";
		ev_io_stop(loop, watcher);
	}
	else{
		LOG(INFO) << "get response " << buffer;
		ev_io_stop(loop, watcher);
		ev_io *io = new ev_io;
		ev_io_init(io, do_write, fd, EV_WRITE);
		ev_io_start(loop, io);
	}
	return;

end:
	ev_io_stop(loop, watcher);
	ev_break(loop, EVBREAK_ALL);
	return;
}

int make_socket_non_blocking(int fd){
	int flags = fcntl(fd, F_GETFL, 0);
	if(flags == -1){
		LOG(ERROR) << "fcntl get error";
		return -1;
	}

	flags |= O_NONBLOCK;
	int ret = fcntl(fd, F_SETFL, flags);
	if(ret == -1){
		LOG(ERROR) << "fcntl set error";
		return -1;
	}

	return 0;
}

void do_signal(struct ev_loop *loop, ev_signal *watcher, int e){
	LOG(WARNING) << "SIGINT signal recevied";
	ev_signal_stop(loop, watcher);
	ev_break(loop, EVBREAK_ALL);
	return;
}


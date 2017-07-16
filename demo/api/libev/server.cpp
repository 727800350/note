#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <ev.h>

const int max_vl = 1024;
const int LISTENQ = 5;

void usage(const char *prog) {
    fprintf(stderr, "%s -i ip -p port\n", prog);
}

/* just an example
 * in production, can not used same buffer for all clients
 */
char buffer[1024];
int socket_bind(const char* ip, int port);
int make_socket_non_blocking(int fd);

void do_write(struct ev_loop *main_loop, ev_io *watcher, int e){
	int nwrite = write(watcher->fd, buffer, strlen(buffer));
	if(nwrite == -1){
		if(errno == EAGAIN){
			fprintf(stderr, "resource temp unavailable");
		}
		else{
			fprintf(stderr, "write error\n");
			ev_io_stop(main_loop, watcher);
			close(watcher->fd);
			delete watcher;
		}
	}
	else if(nwrite == 0){
		fprintf(stderr, "connection closed\n");
		ev_io_stop(main_loop, watcher);
		close(watcher->fd);
		delete watcher;
	}
	else{
		fprintf(stdout , "write %d\n", nwrite);
		ev_io_stop(main_loop, watcher);
		delete watcher;
		buffer[0] = '\0';
	}
}

void do_read(struct ev_loop *main_loop, ev_io *watcher, int e){
	int nread = read(watcher->fd, buffer, sizeof(buffer) - 1);
	if(nread == -1){
		if(errno == EAGAIN){
			fprintf(stderr, "resource temp unavailable");
		}
		else{
			fprintf(stderr, "read error\n");
			ev_io_stop(main_loop, watcher);
			close(watcher->fd);
			delete watcher;
		}
	}
	else if(nread == 0){
		fprintf(stderr, "connection closed\n");
		ev_io_stop(main_loop, watcher);
		close(watcher->fd);
		delete watcher;
	}
	else{
		buffer[nread] = '\0';
		fprintf(stdout , "read %s\n", buffer);
		ev_io *w_io = new ev_io;
		ev_io_init(w_io, do_write, watcher->fd, EV_WRITE);
		ev_io_start(main_loop, w_io);
	}
}

void do_accept(struct ev_loop *main_loop, ev_io *watcher, int e){
	struct sockaddr_in addr;
	socklen_t len = sizeof(struct sockaddr_in);
	int fd = accept(watcher->fd, (struct sockaddr *)(&addr), &len);
	if(fd == -1){
		fprintf(stderr, "accept error\n");
		return;
	}

    char hbuf[NI_MAXHOST];
    char sbuf[NI_MAXSERV];
    int ret = getnameinfo((struct sockaddr *)&addr, len, hbuf, sizeof(hbuf), sbuf, sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV);
    if (ret == 0) {
        fprintf(stderr, "accepted connection on descriptor %d (%s:%s)\n", fd, hbuf, sbuf);
    }
    else {
        fprintf(stderr, "getnameinfo on %d error\n", fd);
    }

    ret = make_socket_non_blocking(fd);
    if (ret != 0) {
        fprintf(stderr, "make connection socket non blocking error\n");
        return;
    }

	ev_io *w_io = new ev_io;
	ev_io_init(w_io, do_read, fd, EV_READ);
	ev_io_start(main_loop, w_io);
}

int main(int argc,char *argv[]) {
    char ip[NI_MAXHOST] = "localhost";
    int port = 8000;

    int opt = 0;
    while ((opt = getopt(argc, argv, "i:p:h")) != -1) {
        switch (opt) {
            case 'i':
                snprintf(ip, NI_MAXHOST, "%s", optarg);
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case 'h':
                usage(argv[0]);
                return 0;
            default:
                usage(argv[0]);
                return -1;
        }
    }

    int listenfd = socket_bind(ip, port);
    int ret = make_socket_non_blocking(listenfd);
    if (ret != 0) {
        fprintf(stderr, "make listen socket non blocking error\n");
        return -1;
    }
    listen(listenfd, LISTENQ);

	struct ev_loop *main_loop = ev_default_loop(0);
	ev_io w_accept;
	ev_io_init(&w_accept, do_accept, listenfd, EV_READ);
	ev_io_start(main_loop, &w_accept);
	ev_run(main_loop, 0);

    return 0;
}

int socket_bind(const char* ip,int port) {
    struct sockaddr_in servaddr;
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1) {
        fprintf(stderr, "socket error\n");
        exit(1);
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &servaddr.sin_addr);
    servaddr.sin_port = htons(port);
    if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
        fprintf(stderr, "bind error\n");
        exit(1);
    }
    fprintf(stderr, "listening on %s:%d\n", ip, port);
    return listenfd;
}

int make_socket_non_blocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if(flags == -1){
        fprintf(stderr, "fcntl get error\n");
        return -1;
    }

    flags |= O_NONBLOCK;
    int ret = fcntl(fd, F_SETFL, flags);
    if(ret == -1){
        fprintf(stderr, "fcntl set error\n");
        return -1;
    }

    return 0;
}


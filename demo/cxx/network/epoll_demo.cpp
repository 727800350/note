#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/epoll.h>

const int max_event = 64;

void usage(const char *prog) {
	fprintf(stderr, "%s -p[listen port]\n", prog);
}

int make_socket_non_blocking(int fd);
int init_server(char *port, int *sfd);
int accept_con(int efd, int sfd);
int process_con(int efd, int fd);

int main(int argc, char *argv[]){
	char *serv_port = "8000";

	int c = 0;
	while ((c = getopt(argc, argv, "p:h")) != -1) {
		switch (c) {
			case 'p':
				serv_port = optarg;
				break;
			case 'h':
				usage(argv[0]);
				return 0;
			default:
				usage(argv[0]);
				return -1;
		}
	}

	int sfd = 0;
	int ret = init_server(serv_port, &sfd);
	if(ret != 0){
		fprintf(stderr, "init server error\n");
		return -1;
	}

	// same as epoll_create except that size parameter is ignored
	int efd = epoll_create1(0);
	if(efd == -1){
		perror("epoll_create");
		abort();
	}

	struct epoll_event event;
	event.data.fd = sfd;
	event.events = EPOLLIN;
	ret = epoll_ctl(efd, EPOLL_CTL_ADD, sfd, &event);
	if(ret == -1){
		fprintf(stderr, "epoll_ctl add error\n");
		return -1;
	}

	/* Buffer where events are returned */
	struct epoll_event *events = new struct epoll_event[max_event];

	/* The event loop */
	while(1){
		int n = epoll_wait(efd, events, max_event, -1);
		for(int i = 0; i < n; i++){
			if((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN))){
				/* An error has occured on this fd, or the socket is not ready for reading(why were we notified then?) */
				fprintf(stderr, "epoll error\n");
				close(events[i].data.fd);
			}
			else if(sfd == events[i].data.fd){
				/* We have a notification on the listening socket, which means one or more incoming connections. */
				int ret = accept_con(efd, sfd);
				if (ret != 0) {
					fprintf(stderr, "accept error\n");
				}
			}
			else{
				/* We have data on the fd waiting to be read. Read and display it.
	 			* We must read whatever data is available completely, 
	 			* as we are running in edge-triggered mode and won't get a notification again for the same data.
	 			*/

				int fd = events[i].data.fd;
				int ret = process_con(efd, fd);
				if (ret != 0) {
					fprintf(stderr, "process con %d error", fd);
				}
			}
		}
	}

	close(sfd);
	delete []events;

	return 0;
}

int accept_con(int efd, int sfd){
	struct sockaddr in_addr;
	socklen_t in_len = sizeof(in_addr);
	int infd = accept(sfd, &in_addr, &in_len);
	if(infd == -1){
		if((errno == EAGAIN) || (errno == EWOULDBLOCK)){
			/* We have processed all incoming connections. */
			return 0;
		}
		else{
			fprintf(stderr, "accept error");
			return -1;
		}
	}

	char hbuf[NI_MAXHOST];
	char sbuf[NI_MAXSERV];
	int ret = getnameinfo(&in_addr, in_len, hbuf, sizeof(hbuf), sbuf, sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV);
	if(ret == 0){
		fprintf(stderr, "Accepted connection on descriptor %d (%s:%s)\n", infd, hbuf, sbuf);
	}

	/* Make the incoming socket non-blocking and add it to the list of fds to monitor. */
	ret = make_socket_non_blocking(infd);
	if(ret == -1){
		fprintf(stderr, "make_socket_non_blocking error\n");
		return -1;
	}

	struct epoll_event event;
	event.data.fd = infd;
	event.events = EPOLLIN;
	ret = epoll_ctl(efd, EPOLL_CTL_ADD, infd, &event);
	if(ret == -1){
		fprintf(stderr, "epoll_ctl add error\n");
		return -1;
	}

	return 0;
}

int process_con(int efd, int fd){
	bool done = false;
	char buf[512];
	while(1){
		ssize_t count = read(fd, buf, sizeof(buf) - 1);
		if(count == -1){
			if(errno == EAGAIN){
				fprintf(stderr, "read temporarily unavailable\n");
			}
			else{
				fprintf(stderr, "read error\n");
				done = true;
			}
			break;
		}
		else if(count == 0){
			fprintf(stderr, "client closed\n");
			done = true;
			break;
		}

		buf[count] = '\0';
		fprintf(stdout, "%lu %s\n", count, buf);
	}

	if(done){
		fprintf(stderr, "Closed connection on descriptor %d\n", fd);
		/* Closing the descriptor will make epoll remove it from the set of descriptors which are monitored. */
		close(fd);
	}

	return 0;
}

int init_server(char *port, int *sfd){
	struct addrinfo hints;
	struct addrinfo *result = NULL;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;     /* Return IPv4 and IPv6 choices */
	hints.ai_socktype = SOCK_STREAM; /* We want a TCP socket */
	hints.ai_flags = AI_PASSIVE;     /* All interfaces */

	int ret = getaddrinfo(NULL, port, &hints, &result);
	if(ret != 0){
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
		return -1;
	}

	int fd;
	for(struct addrinfo *rp = result; rp != NULL; rp = rp->ai_next) {
		fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if(fd == -1){
			continue;
		}

		ret = bind(fd, rp->ai_addr, rp->ai_addrlen);
		if(ret == 0){
			if(rp == NULL){
				fprintf(stderr, "Could not bind\n");
				return -1;
			}
			/* We managed to bind successfully! */
			break;
		}

		close(fd);
	}
	freeaddrinfo(result);

	ret = make_socket_non_blocking(fd);
	if(ret == -1){
		fprintf(stderr, "make_socket_non_blocking error\n");
		return -1;
	}

	ret = listen(fd, SOMAXCONN); // SOMAXCONN is kernel parameter
	if(ret == -1){
		fprintf(stderr, "listen error\n");
		return -1;
	}
	*sfd = fd;

	return 0;
}

int make_socket_non_blocking(int fd){
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


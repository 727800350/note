#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>

const int max_vl = 1000;

void usage(const char *prog) {
	fprintf(stderr, "%s -h[host] -p[port]\n", prog);
}

int process(int socket);
int make_socket_non_blocking(int fd);

int main(int argc, char *argv[]) {
	char *host = "localhost";
	uint16_t port = 8000;

	int c = 0;
	while ((c = getopt(argc, argv, "h:p:")) != -1) {
		switch (c) {
			case 'h':
				host = optarg;
				break;
			case 'p':
				port = (uint16_t)atoi(optarg);
				break;
			default:
				usage(argv[0]);
				return -1;
		}
	}

	struct hostent *he = NULL;
	if((he = gethostbyname(host)) == NULL){
		fprintf(stderr, "gethostbyname() error\n");
		return -1;
	}

	int fd = 0;
	if((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		fprintf(stderr, "socket() error\n");
		return -1;
	}

	struct sockaddr_in server;
	memset(&server, '\0', sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr = *((struct in_addr *)he->h_addr);

	if((connect(fd, (struct sockaddr *)&server, sizeof(server))) == -1){
		fprintf(stderr, "connect() error\n");
		return -1;
	}

	int ret = make_socket_non_blocking(fd);
	if (ret != 0) {
		fprintf(stderr, "make_socket_non_blocking error\n");
		return -1;
	}

	ret = process(fd);
	if (ret != 0) {
		fprintf(stderr, "process socket %d error\n", fd);
	}
	close(fd);

	return 0;
}

int process(int fd){
	char *buf = new char[max_vl];

	fprintf(stderr, "please input message:");
	while(fgets(buf, max_vl, stdin) != NULL){
		int nwrite = write(fd, buf, strlen(buf));
		if (nwrite == -1) {
			fprintf(stderr, "write error\n");
		}

		int nread = read(fd, buf, max_vl);
		if (nread == -1) {
			if(errno == EAGAIN){
				fprintf(stderr, "read temporarily unavailable\n");
			}
			else{
				fprintf(stderr, "read error\n");
			}
		}
		else if (nread == 0) {
			fprintf(stderr, "read none\n");
		}
		else {
			buf[nread] = '\0';
			fprintf(stdout, "server message: %s", buf);
		}
		fprintf(stderr, "please input message:");
	}

	delete []buf;

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


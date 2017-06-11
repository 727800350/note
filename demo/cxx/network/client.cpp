/* client.c */
/*客户首先与服务器连接，然后接收用户输入的字符串，在将字符串发送给服务器，接受服务器发回的信息并显示
 * 之后，继续等待用户输入直至用户输入ctrl+D，当用户输入ctrl+D后，客户关闭连接并退出
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

const int max_vl = 1000;

void usage(const char *prog) {
	fprintf(stderr, "%s -h[host] -p[port]\n", prog);
}

int process(int socket);

int main(int argc, char *argv[]) {
	char *host = NULL;
	uint16_t port = 0;

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

	if (host == NULL or port == 0) {
		fprintf(stderr, "arg error\n");
		usage(argv[0]);
		return -1;
	}

	struct hostent *he;
	if((he = gethostbyname(host)) == NULL){
		fprintf(stderr, "gethostbyname() error\n");
		return -1;
	}

	int fd;
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

	int ret = process(fd);
	if (ret != 0) {
		fprintf(stderr, "process socket %d error\n", fd);
	}
	close(fd);

	return 0;
}

int process(int fd){
	char *sendline = new char[max_vl];
	char *recvline = new char[max_vl];
	int num = 0;

	fprintf(stderr, "please input message:\n");
	while(fgets(sendline, max_vl, stdin) != NULL){
		send(fd, sendline, strlen(sendline), 0);

		if((num = recv(fd, recvline, max_vl, 0)) == 0){
			fprintf(stderr, "server terminated\n");
			return 0;
		}
		recvline[num] = '\0';

		fprintf(stdout, "server message: %s\n", recvline);
		fprintf(stderr, "please input message:\n");
	}
	fprintf(stderr, "Exit.\n");

	delete []sendline;
	delete []recvline;

	return 0;
}


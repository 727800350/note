#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

const int max_vl = 1024;

void usage(const char *prog) {
	fprintf(stderr, "%s -p[listen port]\n", prog);
}

int main(int argc, char **argv) {
	uint16_t serv_port = 0;

	int c = 0;
	while ((c = getopt(argc, argv, "p:h")) != -1) {
		switch (c) {
			case 'p':
				serv_port = (uint16_t)atoi(optarg);
				break;
			case 'h':
				usage(argv[0]);
				return 0;
			default:
				usage(argv[0]);
				return -1;
		}
	}

	/* op1: create a socket
	 * AF_INET(AF is for address family) is ipv4
	 * SOCK_STREAM is a reliable stream-oriented service, which is used by TCP
	 * The value 0 may be used to select a default protocol from the selected domain and type
	 */
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);

	/* op2: set the server address to bind */
	struct sockaddr_in servaddr;
	memset(&servaddr, '\0', sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	/* special IP address INADDR_ANY, This allowed your program to work without knowing the IP address of the machine it was running on,
	 * or, in the case of a machine with multiple network interfaces, it allowed your server to receive packets destined to any of the interfaces.
	 * ref: http://www.cs.cmu.edu/~srini/15-441/F01.full/www/assignments/P2/htmlsim_split/node18.html
	 */
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(serv_port);

	/* op3: bind the socket the server address */
	bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	/* op4: start to listen */
	listen(listenfd, 20);
	fprintf(stderr, "accepting connections ...\n");

	char str[INET_ADDRSTRLEN] = {'\0'};
	char *buf = new char[max_vl];
	while (1) {
		/* op5: accept new connection */
		struct sockaddr_in cliaddr;
		socklen_t cliaddr_len = sizeof(cliaddr);
		int connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);

		const char *p = inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str));
		uint16_t port = ntohs(cliaddr.sin_port);
		fprintf(stderr, "received connection from %s:%d\n", p, port);

		/* op6: read and write */
		int n = 0;
		while ((n = read(connfd, buf, max_vl)) > 0) {
			buf[n] = '\0'; // \n is still in buf
			fprintf(stderr, "read from clinet: %s\n", buf);

			for (int i = 0; i < n; i++) {
				buf[i] = toupper(buf[i]);
			}
			write(connfd, buf, strlen(buf));
		}
		/* op7: close if needed */
		fprintf(stderr, "read error, closing connection\n");
		close(connfd);
	}

	delete []buf;
}


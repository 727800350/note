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
#include <ctype.h>
#include <ev.h>
#include <algorithm>

const int max_vl = 1024;
const int LISTENQ = 5;
const int max_fd = 10000;

void usage(const char *prog){
	fprintf(stderr, "%s -i ip -p port\n", prog);
}

typedef struct _client_t{
	int fd;
	char *ip;
	int port;
	char buf_req[max_vl]; // request buffer;
	int pos_req;
	char buf_res[max_vl]; // response buffer
	int pos_res;
}client_t;

typedef struct _server_t{
	char *ip;
	int port;
	int fd;
	struct ev_loop *loop;
	client_t *clients[max_fd + 1];
}server_t;
server_t server;

void do_accept(struct ev_loop *loop, ev_io *watcher, int e);
void do_read(struct ev_loop *loop, ev_io *watcher, int e);
void do_write(struct ev_loop *loop, ev_io *watcher, int e);

int process(client_t *client);
int free_client(client_t *client);

int socket_bind(const char* ip, int port);
int make_socket_non_blocking(int fd);

int main(int argc,char *argv[]){
	int opt = 0;
	while((opt = getopt(argc, argv, "i:p:h")) != -1){
		switch (opt){
			case 'i':
				server.ip = strndup(optarg, strlen(optarg));
				break;
			case 'p':
				server.port = atoi(optarg);
				break;
			case 'h':
				usage(argv[0]);
				return 0;
			default:
				usage(argv[0]);
				return -1;
		}
	}

	if(server.ip == NULL){
		server.ip = strndup("localhost", strlen("localhost"));
	}
	if(server.port == 0){
		server.port = 8000;
	}
	memset(server.clients, 0, sizeof(client_t *) * max_fd);

	server.fd = socket_bind(server.ip, server.port);
	int ret = make_socket_non_blocking(server.fd);
	if (ret != 0){
		fprintf(stderr, "make listen socket non blocking error\n");
		return -1;
	}
	listen(server.fd, LISTENQ);

	server.loop = ev_default_loop(0);
	ev_io io;
	ev_io_init(&io, do_accept, server.fd, EV_READ);
	ev_io_start(server.loop, &io);
	ev_run(server.loop, 0);

	fprintf(stderr, "loop stopped.\n");
	free(server.ip);

	return 0;
}

void do_accept(struct ev_loop *loop, ev_io *watcher, int e){
	struct sockaddr_in addr;
	socklen_t len = sizeof(struct sockaddr_in);
	int fd = accept(watcher->fd, (struct sockaddr *)(&addr), &len);
	if(fd == -1){
		fprintf(stderr, "accept error\n");
		return;
	}

	// check fd
	if(fd > max_fd){
		char msg[] = "fd exceeds";
		fprintf(stderr, "%s\n", msg);
		write(fd, msg, sizeof(msg));
		close(fd);
		return;
	}

	char hbuf[NI_MAXHOST];
	char sbuf[NI_MAXSERV];
	int ret = getnameinfo((struct sockaddr *)&addr, len, hbuf, sizeof(hbuf), sbuf, sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV);
	if (ret != 0){
		fprintf(stderr, "getnameinfo on %d error\n", fd);
		close(fd);
		return;
	}

	ret = make_socket_non_blocking(fd);
	if (ret != 0){
		fprintf(stderr, "make connection socket non blocking error\n");
		return;
	}

	/* create client */
	client_t *client = new client_t;
	client->ip = strndup(hbuf, strlen(hbuf));
	client->port = atoi(sbuf);
	client->fd = fd;
	client->pos_req = 0;
	client->pos_res = 0;
	server.clients[fd] = client;
	fprintf(stderr, "new client from %s:%d with fd %d\n", client->ip, client->port, client->fd);

	ev_io *io = new ev_io;
	ev_io_init(io, do_read, fd, EV_READ);
	ev_io_start(loop, io);
}

void do_read(struct ev_loop *loop, ev_io *watcher, int e){
	int fd = watcher->fd;
	client_t *client = server.clients[fd];
	int nread = read(fd, client->buf_req + client->pos_req, max_vl - client->pos_req);
	if(nread == -1){
		if(errno == EAGAIN){
			fprintf(stderr, "resource temp unavailable");
		}
		else{
			fprintf(stderr, "read error\n");
			goto end;
		}
	}
	else if(nread == 0){
		fprintf(stderr, "connection closed\n");
		goto end;
	}
	else{
		fwrite(client->buf_req + client->pos_req, sizeof(char), nread, stdout);
		client->pos_req += nread;
		int ret = process(client);
		if(ret != 0){
			fprintf(stderr, "process client %s:%d error\n", client->ip, client->port);
			goto end;
		}
	}

end:
	ev_io_stop(loop, watcher);
	delete watcher;
	free_client(client);
}

int process(client_t *client){
	/* echo request
	 * simply toupper request as response
	 */
	int n = std::min(client->pos_req, max_vl - client->pos_res);
	for(int i = 0; i < n; i++){
		client->buf_res[client->pos_res++] = toupper(client->buf_req[i]);
	}
	memcpy(client->buf_req, client->buf_req + n, client->pos_req - n);
	client->pos_req -= n;

	ev_io *io = new ev_io;
	ev_io_init(io, do_write, client->fd, EV_WRITE);
	ev_io_start(server.loop, io);
	return 0;
}

void do_write(struct ev_loop *loop, ev_io *watcher, int e){
	int fd = watcher->fd;
	client_t *client = server.clients[fd];

	if(client->buf_res <= 0){
		fprintf(stderr, "no response\n");
		ev_io_stop(loop, watcher);
		delete watcher;
		return;
	}

	int nwrite = write(fd, client->buf_res, client->pos_res);
	if(nwrite == -1){
		if(errno == EAGAIN){
			fprintf(stderr, "resource temp unavailable");
		}
		else{
			fprintf(stderr, "write error\n");
			goto end;
		}
	}
	else if(nwrite == 0){
		fprintf(stderr, "connection closed\n");
		goto end;
	}
	else{
		fprintf(stdout , "write %d\n", nwrite);
		client->pos_res -= nwrite;
		if(client->pos_res <= 0){
			ev_io_stop(loop, watcher);
			delete watcher;
		}
	}

end:
	ev_io_stop(loop, watcher);
	delete watcher;
	free_client(client);
}

int free_client(client_t *client){
	free(client->ip);
	/* whatif fd is non-blocking? */
	close(client->fd);
	server.clients[client->fd] = NULL;
	delete client;

	return 0;
}

int socket_bind(const char* ip, int port){
	struct sockaddr_in servaddr;
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd == -1){
		fprintf(stderr, "socket error\n");
		exit(1);
	}
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &servaddr.sin_addr);
	servaddr.sin_port = htons(port);
	if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
		fprintf(stderr, "bind error\n");
		exit(1);
	}
	fprintf(stderr, "listening on %s:%d\n", ip, port);
	return listenfd;
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


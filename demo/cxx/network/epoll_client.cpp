#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "epoll_util.h"

const int max_vl = 1024;
const int FDSIZE = 1024;
const int EPOLLEVENTS = 20;

/* epoll ref: http://www.cnblogs.com/Anker/archive/2013/08/17/3263780.html */
void usage(const char *prog) {
    fprintf(stderr, "%s -i ip -p port\n", prog);
}

int do_epoll(int sockfd);
int handle_events(int epollfd, struct epoll_event *events, int num, int sockfd, char *buf);
int do_read(int epollfd, int fd, int sockfd, char *buf);
int do_write(int epollfd, int fd, int sockfd, char *buf);

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

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &servaddr.sin_addr);
    int ret = connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    if (ret == -1) {
        fprintf(stderr, "connect to %s:%d error\n", ip, port);
        return -1;
    }

    ret = make_socket_non_blocking(sockfd);
    if (ret != 0) {
        fprintf(stderr, "make socketfd non blocking error\n");
        return -1;
    }

    do_epoll(sockfd);
    close(sockfd);
    return 0;
}


int do_epoll(int sockfd) {
    int epollfd = epoll_create(FDSIZE);
    int ret = add_event(epollfd, STDIN_FILENO, EPOLLIN);
    if (ret != 0) {
        fprintf(stderr, "add STDIN_FILENO to epoll error\n");
        return -1;
    }

    struct epoll_event events[EPOLLEVENTS];
    char *buf = new char[max_vl];
    int timeout = -1;
    while (true) {
        int num = epoll_wait(epollfd, events, EPOLLEVENTS, timeout);
        handle_events(epollfd, events, num, sockfd, buf);
    }
    close(epollfd);
    delete []buf;
    return 0;
}

int handle_events(int epollfd, struct epoll_event *events, int num, int sockfd, char *buf) {
    for (int i = 0; i < num; i++) {
        int fd = events[i].data.fd;
        if (events[i].events & EPOLLIN) {
            do_read(epollfd, fd, sockfd, buf);
        }
        else if (events[i].events & EPOLLOUT) {
            do_write(epollfd, fd, sockfd, buf);
        }
    }

    return 0;
}

int do_read(int epollfd, int fd, int sockfd, char *buf) {
    int nread = read(fd, buf, max_vl);
    if (nread == -1 and errno != EAGAIN) {
        fprintf(stderr, "read error\n");
        close(fd);
    }
    else if (nread == 0) {
        fprintf(stderr, "server close.\n");
        close(fd);
    }
    else {
        buf[nread] = '\0';
        if (fd == STDIN_FILENO) {
            /* after reading data from stdin, we need to write out to sockfd,
             * so we add sockfd EPOLLOUT to epoll
             */
            int ret = add_event(epollfd, sockfd, EPOLLOUT);
            if (ret != 0) {
                fprintf(stderr, "add sockfd EPOLLOUT to epoll error\n");
            }
        }
        else {
            /* after reading data from sockfd, we need to write out to stdout,
             * so we add STDOUT_FILENO EPOLLOUT to epoll
             * todo: but why delete SOCKFD EPOLLIN
             * in practice: if we do not delete sockfd epollin from epoll, add sockfd epollout to epoll will fail, then client can not send data to server
             */
            int ret = delete_event(epollfd, sockfd, EPOLLIN);
            if (ret != 0) {
                fprintf(stderr, "delete sockfd EPOLLIN to epoll error\n");
            }
            ret = add_event(epollfd, STDOUT_FILENO, EPOLLOUT);
            if (ret != 0) {
                fprintf(stderr, "add STDOUT_FILENO EPOLLOUT to epoll error\n");
            }
        }
    }

    return nread;
}

int do_write(int epollfd, int fd, int sockfd, char *buf) {
    int nwrite = write(fd, buf, strlen(buf));
    if (nwrite == -1) {
        fprintf(stderr, "write error\n");
        close(fd);
    }
    else {
        if (fd == STDOUT_FILENO) {
            /* after writing to stdout, it's done.
             * or we will be writing the same buf to stdout eternally, as stdout is always available.
             */
            int ret = delete_event(epollfd, fd, EPOLLOUT);
            if (ret != 0) {
                fprintf(stderr, "delete STDOUT_FILENO EPOLLOUT to epoll error\n");
            }
        }
        else {
            /* after writing to sockfd, we need to wait for server's response,
             * so we modify sockfd from EPOLLOUT to EPOLLIN
             */
            int ret = modify_event(epollfd, fd, EPOLLIN);
            if (ret != 0) {
                fprintf(stderr, "modify fd from EPOLLOUT to EPOLLIN error\n");
            }
        }
    }

    return nwrite;
}


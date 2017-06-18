#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "epoll_util.h"

const int max_vl = 1024;
const int LISTENQ = 5;
const int FDSIZE = 1000;
const int EPOLLEVENTS = 100;

/* epoll ref: http://www.cnblogs.com/Anker/archive/2013/08/17/3263780.html */
void usage(const char *prog) {
    fprintf(stderr, "%s -i ip -p port\n", prog);
}

int socket_bind(const char* ip, int port);
int do_epoll(int listenfd);
int handle_events(int epollfd, struct epoll_event *events, int num, int listenfd, char *buf);
int handle_accept(int epollfd, int listenfd);
int do_read(int epollfd, int fd, char *buf);
int do_write(int epollfd, int fd, char *buf);

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
    ret = do_epoll(listenfd);
    if (ret != 0) {
        fprintf(stderr, "do_epoll error\n");
        return -1;
    }
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

int do_epoll(int listenfd) {
    struct epoll_event events[EPOLLEVENTS];
    int epollfd = epoll_create(FDSIZE);
    int ret = add_event(epollfd, listenfd, EPOLLIN);
    if (ret != 0) {
        fprintf(stderr, "add listenfd %d to epoll error\n", listenfd);
        return -1;
    }

    int timeout = -1;
    char *buf = new char[max_vl];
    while(true) {
        int num = epoll_wait(epollfd, events, EPOLLEVENTS, timeout);
        handle_events(epollfd, events, num, listenfd, buf);
    }
    close(epollfd);
    delete []buf;

    return 0;
}

int handle_events(int epollfd, struct epoll_event *events, int num, int listenfd, char *buf) {
    for (int i = 0; i < num; i++) {
        int fd = events[i].data.fd;
        if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP)) {
            fprintf(stderr, "epoll error on %d, closing it...\n", fd);
            close(fd);
        }
        else if ((fd == listenfd) and (events[i].events & EPOLLIN)) {
            handle_accept(epollfd, fd);
        }
        else if (events[i].events & EPOLLIN) {
            do_read(epollfd, fd, buf);
        }
        else if (events[i].events & EPOLLOUT) {
            do_write(epollfd, fd, buf);
        }
    }

    return 0;
}

int handle_accept(int epollfd, int listenfd) {
    struct sockaddr_in cliaddr;
    socklen_t cliaddrlen = sizeof(struct sockaddr_in);
    int clifd = accept(listenfd, (struct sockaddr*)&cliaddr, &cliaddrlen);
    if (clifd == -1) {
        fprintf(stderr, "accpet error\n");
        return -1;
    }

    char hbuf[NI_MAXHOST];
    char sbuf[NI_MAXSERV];
    int ret = getnameinfo((struct sockaddr *)&cliaddr, cliaddrlen, hbuf, sizeof(hbuf), sbuf, sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV);
    if (ret == 0) {
        fprintf(stderr, "accepted connection on descriptor %d (%s:%s)\n", clifd, hbuf, sbuf);
    }
    else {
        fprintf(stderr, "getnameinfo on %d error\n", clifd);
    }

    ret = make_socket_non_blocking(clifd);
    if (ret != 0) {
        fprintf(stderr, "make connection socket non blocking error\n");
        return -1;
    }
    ret = add_event(epollfd, clifd, EPOLLIN);
    if (ret != 0) {
        fprintf(stderr, "add clifd %d to epoll error\n", clifd);
        return -1;
    }

    return 0;
}

int do_read(int epollfd, int fd, char *buf) {
    int nread = read(fd, buf, max_vl);
    if (nread == -1) {
        fprintf(stderr, "read %d error\n", fd);
        int ret = delete_event(epollfd, fd, EPOLLIN);
        if (ret != 0) {
            fprintf(stderr, "delete fd %d EPOLLIN from epoll error\n", fd);
            return -2;
        }
        close(fd);
        /* note:
         * close fd will automatically delete the related events from epoll
         * so delete_event here is not a necessary
         */
    }
    else if (nread == 0) {
        fprintf(stderr, "client %d close\n", fd);
        close(fd);
    }
    else {
        buf[nread] = '\0';
        fprintf(stdout, "read %d message: %s", fd, buf);
        int ret = modify_event(epollfd, fd, EPOLLOUT);
        if (ret != 0) {
            fprintf(stderr, "modify fd %d from EPOLLIN to EPOLLOUT error\n", fd);
            return -2;
        }
    }

    return nread;
}

int do_write(int epollfd, int fd, char *buf) {
    int nwrite = write(fd, buf, strlen(buf));
    if (nwrite == -1) {
        fprintf(stderr, "write %d error\n", fd);
        close(fd);
    }
    else {
        int ret = modify_event(epollfd, fd, EPOLLIN);
        if (ret != 0) {
            fprintf(stderr, "modify fd %d from EPOLLOUT to EPOLLIN error\n", fd);
            return -2;
        }
    }

    return nwrite;
}


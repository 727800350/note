#include <stdio.h>
#include "epoll_util.h"

int add_event(int epollfd, int fd, int state) {
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    int ret = epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
    if (ret != 0) {
        fprintf(stderr, "epoll add on %d error\n", fd);
        return -1;
    }

    return 0;
}

int delete_event(int epollfd, int fd, int state) {
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    int ret = epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
    if (ret != 0) {
        fprintf(stderr, "epoll del on %d error\n", fd);
        return -1;
    }

    return 0;
}

int modify_event(int epollfd, int fd, int state) {
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    int ret = epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
    if (ret != 0) {
        fprintf(stderr, "epoll mod on %d error\n", fd);
        return -1;
    }

    return 0;
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


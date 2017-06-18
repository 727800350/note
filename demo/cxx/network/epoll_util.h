#ifndef EPOLL_UTIL_H
#define EPOLL_UTIL_H

#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

int add_event(int epollfd, int fd, int state);
int modify_event(int epollfd, int fd, int state);
int delete_event(int epollfd, int fd, int state);
int make_socket_non_blocking(int fd);

#endif


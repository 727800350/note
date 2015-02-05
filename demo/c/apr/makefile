## CC=clang
CC=gcc
LDFLAGS=-pthread -L/usr/lib64/mysql -L/usr/local/apr/lib
CFLAGS=-pthread -g -Wall -I/usr/include/mysql -I/usr/local/apr/include/apr-1 -m64
CPPFLAGS=-I$(srcdir) 
LIBS=-lmysqlclient_r -lapr-1

all:
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LIBS) hash.c

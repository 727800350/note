prefix=/usr/local
exec_prefix=${prefix}
bindir=${exec_prefix}/bin
sbindir=${exec_prefix}/sbin
libexecdir=${exec_prefix}/libexec
datadir=${prefix}/share
mandir=${prefix}/share/man
sysconfdir=${prefix}/etc
srcdir=.
top_srcdir=.

## CC=clang
CC=gcc
LDFLAGS=-pthread -L/usr/lib64/mysql -L/usr/local/apr/lib
CFLAGS=-pthread -g -Wall -I/usr/include/mysql -I/usr/local/apr/include/apr-1 -m64
CPPFLAGS=-I$(srcdir) 
LIBS=-lmysqlclient_r -lapr-1

TARGETS=option
all: $(TARGETS)

TIME=time.o
time: $(TIME)
	$(CC) $(LDFLAGS) -rdynamic -o a.out $(TIME) $(LIBS)

IO=fread_fwrite.o
io: $(IO)
	$(CC) $(LDFLAGS) -rdynamic -o a.out $(IO) $(LIBS)

OPTION=parse_options.o
option: $(OPTION)
	$(CC) $(LDFLAGS) -rdynamic -o a.out $(OPTION) $(LIBS)

clean:
	rm a.out *.o

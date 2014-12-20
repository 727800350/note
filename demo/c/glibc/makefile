## [eric@human glib]$ pkg-config --cflags --libs glib-2.0
## -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include  -lglib-2.0
CFLAGS=-g -I/usr/include/mysql -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include -I/usr/local/include/glib-2.0 -I/usr/local/lib64/glib-2.0/include -I/usr/local/lib/glib-2.0/include
LIBS=-lpcap -L/usr/lib64/mysql -lmysqlclient -L/usr/local/lib -lglib-2.0 -lpthread
all: 
## 	clang $(CFLAGS) $(LIBS) gslist.c
## 	clang $(CFLAGS) $(LIBS) hashtable_int_str.c
	clang $(CFLAGS) $(LIBS) hashtable_uint_str.c
## 	clang $(CFLAGS) $(LIBS) hashtable_str_str.c

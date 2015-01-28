#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <zlog.h>

int main(){
	zlog_category_t *zc;
	if(zlog_init("./zlog.conf")){
		fprintf(stderr, "zlog init failed\n");
		exit(0);
	}
	zc = zlog_get_category("std");
	if (!zc) {
		fprintf(stderr, "get zlog category failed\n");
		zlog_fini();
		exit(0);
	}

	zlog_debug(zc, "debug");
	zlog_info(zc, "info");
	zlog_notice(zc, "notice");
	zlog_warn(zc, "warn");
	zlog_error(zc, "error");
	zlog_fatal(zc, "fatal");

	return 0;
}

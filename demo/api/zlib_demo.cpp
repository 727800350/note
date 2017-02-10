/*
 * clang++ zlib_demo.cpp -lz
 **/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <zlib.h>

int main(){
	char text[] = "zlib compress and uncompress test";
	unsigned long len_ori = strlen(text) + 1;  /* 需要把字符串的结束符'\0'也一并处理 */
	fprintf(stderr, "ori len: %lu\n", len_ori);

	unsigned long len_compressed = compressBound(len_ori); /* 压缩后的长度不会超过返回值 */
	char *buf = new char[len_compressed];
	if(buf == NULL){
		fprintf(stderr, "malloc error\n");
		return -1;
	}
	fprintf(stderr, "computed len: %lu\n", len_compressed);

	// the second para must be the capacity of buf
	if(compress((unsigned char *)buf, &len_compressed, (unsigned char *)text, len_ori) != Z_OK){
		fprintf(stderr, "compress error!\n");
		return -1;
	}
	fprintf(stderr, "compressed len: %lu\n", len_compressed);

	unsigned long len_new = len_ori;
	char *text_new = new char[len_new];
	if(text_new == NULL){
		fprintf(stderr, "malloc error\n");
		return -1;
	}
	// the second para must be the capacity of buffer(the first para)
	if(uncompress((unsigned char *)text_new, &len_new, (unsigned char *)buf, len_compressed) != Z_OK){
		fprintf(stderr, "uncompress error!\n");
		return -1;
	}

	fprintf(stderr, "%s\n", text);
	fprintf(stderr, "%s\n", text_new);

	delete []buf;
	delete []text_new;

	return 0;
}


#include <stdio.h>
#include <stdlib.h>
#define MAXLEN 1024

/**
 * copy binary file
 * ./a.out src dst
 * */

int main(int argc, char *argv[]){
	if( argc < 3 ){
		printf("usage: %s %s\n", argv[0], "infile outfile");
		exit(1);
	}
	
	FILE * outfile, *infile;
	outfile = fopen(argv[2], "wb" );
	infile = fopen(argv[1], "rb");
	char buf[MAXLEN];
	if(outfile == NULL || infile == NULL ){
		printf("%s, %s",argv[1],"not exit/n");
		exit(1);
	}
	
	int n_r;
	int n_w;
	int n = 0;
	int sum_read = 0;
	int sum_write = 0;
	while((n_r = fread(buf,sizeof(char), MAXLEN, infile)) != 0 ){
		n_w = fwrite(buf, sizeof(char), n_r, outfile);
		sum_read += n_r;
		sum_write += n_w;
		n ++;
	}
	printf("Maxsize each time: %lu(Bytes)\n", MAXLEN * sizeof(char));
	printf("loop: %d: read: %d(Bytes), write: %d(Bytes)\n", n, sum_read, sum_write);
	
	fclose(infile);
	fclose(outfile);
	return 0;
}

/**
#include<stdio.h>
FILE * fopen(const char *path,const char *mode);
加入b 字符用来告诉函数库打开的文件为二进制文件，而非纯文字文件

size_t fread(void *ptr,size_t size, size_t nmemb, FILE *stream);
The  function  fread()  reads  nmemb  elements of data, each size bytes long, from the stream pointed to by stream, storing them at  the  location given by ptr.
Fread()会返回实际读取到的nmemb数目，如果此值比参数nmemb 来得小，则代表可能读到了文件尾或有错误发生，这时必须用feof()或ferror()来决定发生什么情况。

size_t fwrite(const void * ptr,size_t size,size_t nmemb,FILE * stream);
**/

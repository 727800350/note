#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>
 
/*
 * compile: g++ demo.cpp -lcurl
 * function: download the content pointed by url and write it out as kv
 * ref: https://curl.haxx.se/libcurl/c/libcurl-tutorial.html
 */

const int max_kl = 1024;
const int max_vl = 10 * 1024 * 1024;

void usage(const char *prog) {
	fprintf(stderr, "usage: cat url.list | %s > data.kv\n", prog);
}

typedef struct _chunk{
	char *buffer;
	int size;
}chunk_t;

size_t callback(void *content, size_t size, size_t nmemb, void *userp){
	int realsize = size * nmemb;
	chunk_t *chunk = (chunk_t *)userp;
	int cpsize = 0;
	if (max_vl - chunk->size >= realsize) {
		cpsize = realsize;
	}
	else {
		cpsize = max_vl - chunk->size;
	}
	
	if (cpsize == 0) {
		fprintf(stderr, "buffer full\n");
		return 0;
	}
	memcpy(chunk->buffer + chunk->size, content, cpsize);
	chunk->size += cpsize;

	return realsize;
}

int process(char *line, CURL *curl, chunk_t *chunk){
	chunk->size = 0;
	curl_easy_setopt(curl, CURLOPT_URL, line);
	CURLcode res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		return -1;
	}

	int kl = strlen(line);
	fwrite(&kl, sizeof(int), 1, stdout);
	fwrite(line, sizeof(char), kl, stdout);
	fwrite(&chunk->size, sizeof(int), 1, stdout);
	fwrite(chunk->buffer, sizeof(char), chunk->size, stdout);

	return 0;
}

int main(int argc, char* argv[]) {
	int opt = 0;
	while ((opt = getopt(argc, argv, "h")) != -1) {
		switch (opt) {
			case 'h':
				usage(argv[0]);
				return 0;
			default:
				usage(argv[0]);
				return -1;
		}
	}

	chunk_t chunk;
	chunk.buffer = new char[max_vl];

	CURL *curl = NULL;
	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();
	if (curl == NULL) {
		fprintf(stderr, "init curl error\n");
		return -1;
	}
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

	char *line = new char[max_kl];
	while (fgets(line, max_kl, stdin) != NULL) {
		line[strlen(line) - 1] = '\0';
		int ret = process(line, curl, &chunk);
		if (ret != 0) {
			fprintf(stderr, "process %s error\n", line);
			return -1;
		}
	}
 
	curl_easy_cleanup(curl);
	curl_global_cleanup();
	delete []chunk.buffer;
 
	return 0;
}


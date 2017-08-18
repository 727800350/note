#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>
#include <glog/logging.h>
 
/*
 * compile: g++ demo.cpp -lcurl -lglog
 * function: download the content pointed by url and write it out as kv
 * ref: https://curl.haxx.se/libcurl/c/libcurl-tutorial.html
 */

const int max_kl = 1024;
const int max_vl = 10 * 1024 * 1024;

void usage(const char *prog){
	LOG(INFO) << "usage: cat url.list | " << prog << " > data.kv";
}

typedef struct _chunk{
	char *buffer;
	int size;
}chunk_t;

size_t callback(void *content, size_t size, size_t nmemb, void *userp){
	int realsize = size * nmemb;
	chunk_t *chunk = (chunk_t *)userp;
	int cpsize = std::min(max_vl - chunk->size, realsize);
	if(cpsize <= 0){
		LOG(ERROR) << "buffer full";
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
	if(res != CURLE_OK){
		LOG(ERROR) << "curl_easy_perform() failed: " << curl_easy_strerror(res);
		return -1;
	}

	int kl = strlen(line);
	fwrite(&kl, sizeof(int), 1, stdout);
	fwrite(line, sizeof(char), kl, stdout);
	fwrite(&chunk->size, sizeof(int), 1, stdout);
	fwrite(chunk->buffer, sizeof(char), chunk->size, stdout);

	return 0;
}

int main(int argc, char* argv[]){
	google::InitGoogleLogging(argv[0]);
	FLAGS_logtostderr = true;
	FLAGS_alsologtostderr = true;
	FLAGS_colorlogtostderr = true;
	FLAGS_stderrthreshold = google::INFO;
	FLAGS_log_dir = "./";

	int opt = 0;
	while((opt = getopt(argc, argv, "lh")) != -1){
		switch(opt){
			case 'l':
				FLAGS_logtostderr = false;
				break;
			case 'h':
				usage(argv[0]);
				return 0;
			default:
				usage(argv[0]);
				return -1;
		}
	}

	curl_global_init(CURL_GLOBAL_DEFAULT);

	CURL *curl = curl_easy_init();
	if(curl == NULL){
		LOG(ERROR) << "init curl error";
		return -1;
	}
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

	chunk_t chunk;
	chunk.buffer = new char[max_vl];
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

	char *line = new char[max_kl];
	while(fgets(line, max_kl, stdin) != NULL){
		line[strlen(line) - 1] = '\0';
		int ret = process(line, curl, &chunk);
		if(ret != 0){
			LOG(ERROR) << "process " << line << " error";
			return -1;
		}
	}
 
	curl_easy_cleanup(curl);
	delete []chunk.buffer;
	curl_global_cleanup();
 
	google::ShutdownGoogleLogging();
	return 0;
}


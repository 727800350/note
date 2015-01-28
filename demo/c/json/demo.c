#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "cJSON.h"

int main(){
	FILE *f;
	if((f =fopen("./demo.json", "rb")) == NULL){
		printf("Config file opening failed. error: %s.\n", strerror(errno));
		exit(0);
	}
	printf("Config file opened successfully.\n");
	fseek(f, 0, SEEK_END);
	long len = ftell(f);
	fseek(f, 0, SEEK_SET);
	char *data = (char *)malloc(len + 1);
	if(fread(data, 1, len, f) == 0){
		fprintf(stderr, "Reading config file errr.\n");
		exit(0);
	}
	fclose(f);
// 	get rid of the commens
	cJSON_Minify(data);

// 	parse the config file
	cJSON *root;
	if((root = cJSON_Parse(data)) == 0){
		fprintf(stderr,"Parse config file failed: [%s]\n", cJSON_GetErrorPtr());
	}

// 	int value
	int timeout = cJSON_GetObjectItem(root,"timeout")->valueint;
	printf("timeout: %d\n", timeout);

// 	string value
	cJSON *db = cJSON_GetObjectItem(root,"mysql");
	char *host = strdup(cJSON_GetObjectItem(db,"server")->valuestring);
	printf("host: %s\n", host);
	free(host);

// 	array
	cJSON *ips = cJSON_GetObjectItem(root,"ips");
	cJSON *ip = NULL;
	char *start, *end;
	int i;
	for(i = 0; i < cJSON_GetArraySize(ips); i++){
		ip = cJSON_GetArrayItem(ips, i);
		start = strdup(cJSON_GetObjectItem(ip,"start")->valuestring);
		end = strdup(cJSON_GetObjectItem(ip,"end")->valuestring);
		printf("ip start: %s, end: %s\n", start, end);
		free(start);
		free(end);
	}

	cJSON_Delete(root);
	free(data);

	return 0;
}

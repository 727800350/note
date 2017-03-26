#include <stdio.h>
#include <assert.h>
#include <string>
#include <string.h>
#include "hiredis.h"

// g++ *.cpp -I/usr/include/hiredis -lhiredis

redisContext *redis = NULL;
redisReply *reply = NULL;
const int MAX_LEN = 1024;
char *cmd = NULL;

/**
在不同的实现上, 采用的定义可能不一样
#define REDIS_REPLY_STRING 1
#define REDIS_REPLY_ARRAY 2
#define REDIS_REPLY_INTEGER 3
#define REDIS_REPLY_NIL 4
#define REDIS_REPLY_STATUS 5
#define REDIS_REPLY_ERROR 6
**/

int test_string(){
	// set
	snprintf(cmd, MAX_LEN, "set %s %d", "key", 123);
	reply = (redisReply *)redisCommand(redis, cmd);
	if(reply == NULL){
		fprintf(stderr, "redisCommand[%s] returned NULL\n", cmd);
		return -1;
	}
	if(redis->err != 0){
		fprintf(stderr, "after redisCommand[%s] err[%d] and errstr[%s]\n", cmd, redis->err, redis->errstr);
		return -1;
	}
	fprintf(stderr, "cmd[%s] reply->type: %d, reply->str: %s\n", cmd, reply->type, reply->str);
	freeReplyObject(reply);

	// get
	snprintf(cmd, MAX_LEN, "get %s", "key");
	reply = (redisReply *)redisCommand(redis, cmd);
	if(reply == NULL){
		fprintf(stderr, "redisCommand[%s] returned NULL\n", cmd);
		return -1;
	}
	if(redis->err != 0){
		fprintf(stderr, "after redisCommand[%s] err[%d] and errstr[%s]\n", cmd, redis->err, redis->errstr);
		return -1;
	}
	fprintf(stderr, "cmd[%s] reply->type: %d, reply->str: %s\n", cmd, reply->type, reply->str);
	freeReplyObject(reply);

	return 0;
}

int main(){
	std::string ip = "127.0.0.1";
	int port = 6379;

	redis = redisConnect(ip.c_str(), port);
	if(redis == NULL or redis->err != 0){
		fprintf(stderr, "connection error[%d]: %s\n", redis->err, redis->errstr);
		return -1;
	}

	cmd = new char[MAX_LEN];

	assert(test_string() == 0);

	delete []cmd;

	return 0;
}


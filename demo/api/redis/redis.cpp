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

int process_reply(){
	if(reply == NULL){
		fprintf(stderr, "redisCommand[%s] returned NULL\n", cmd);
		return -1;
	}
	if(redis->err != 0){
		fprintf(stderr, "after redisCommand[%s] err[%d] and errstr[%s]\n", cmd, redis->err, redis->errstr);
		return -1;
	}

	switch(reply->type){
		case REDIS_REPLY_STATUS:
		case REDIS_REPLY_STRING:
			fprintf(stderr, "cmd[%s] reply->type: %d, reply->str: %s\n", cmd, reply->type, reply->str);
			break;
		case REDIS_REPLY_INTEGER:
			fprintf(stderr, "cmd[%s] reply->type: %d, reply->integer: %lld\n", cmd, reply->type, reply->integer);
			break;
		case REDIS_REPLY_ARRAY:
			fprintf(stderr, "cmd[%s] reply->type: %d, reply->elements: %lu\n", cmd, reply->type, reply->elements);
			for(size_t i = 0; i < reply->elements; i++){
				redisReply *r = reply->element[i];
				fprintf(stderr, "r->type: %d, r->str: %s\n", r->type, r->str);
			}
			break;
		default:
			fprintf(stderr, "new type: %d\n", reply->type);
			return -1;
	}

	freeReplyObject(reply);

	return 0;
}

int test_string(){
	// set
	snprintf(cmd, MAX_LEN, "set %s %d", "key", 123);
	reply = (redisReply *)redisCommand(redis, cmd);
	assert(process_reply() == 0);

	// get
	snprintf(cmd, MAX_LEN, "get %s", "key");
	reply = (redisReply *)redisCommand(redis, cmd);
	assert(process_reply() == 0);

	// delete
	snprintf(cmd, MAX_LEN, "del %s", "key");
	reply = (redisReply *)redisCommand(redis, cmd);
	assert(process_reply() == 0);

	return 0;
}

int test_list(){
	// rpush
	snprintf(cmd, MAX_LEN, "rpush %s %d %d %d", "key", 1, 2, 3);
	reply = (redisReply *)redisCommand(redis, cmd);
	assert(process_reply() == 0);

	// lpop
	snprintf(cmd, MAX_LEN, "lpop %s", "key");
	reply = (redisReply *)redisCommand(redis, cmd);
	assert(process_reply() == 0);

	// lrange
	snprintf(cmd, MAX_LEN, "lrange %s %d %d", "key", 0, -1);
	reply = (redisReply *)redisCommand(redis, cmd);
	assert(process_reply() == 0);

	return 0;
}

int test_pipeline(){
	// rpush
	snprintf(cmd, MAX_LEN, "rpush %s %d %d %d", "key", 1, 2, 3);
	redisAppendCommand(redis, cmd);

	// rpush
	snprintf(cmd, MAX_LEN, "rpush %s %d %d", "key", 4, 5);
	redisAppendCommand(redis, cmd);

	// lpush
	snprintf(cmd, MAX_LEN, "lpush %s %d %d", "key", 6, 7);
	redisAppendCommand(redis, cmd);

	redisGetReply(redis, (void **)&reply);
	freeReplyObject(reply);
	redisGetReply(redis, (void **)&reply);
	freeReplyObject(reply);
	redisGetReply(redis, (void **)&reply);
	freeReplyObject(reply);

	// why while won't stop
// 	while(redisGetReply(redis, (void **)&reply) == REDIS_OK){
// 		process_reply();
// 	}

	// lrange
	snprintf(cmd, MAX_LEN, "lrange %s %d %d", "key", 0, -1);
	reply = (redisReply *)redisCommand(redis, cmd);
	assert(process_reply() == 0);

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

	redisCommand(redis, "FLUSHALL");
	assert(test_string() == 0);

	redisCommand(redis, "FLUSHALL");
	assert(test_list() == 0);

	redisCommand(redis, "FLUSHALL");
	assert(test_pipeline() == 0);

	delete []cmd;
	redisFree(redis);

	return 0;
}


# intro
redis 中数据对象都是 string 类型, 但是有时会把string 解释成其他类型, 例如 incr 会把 string 解释成int 类型

Redis keys are binary safe, this means that you can use any binary sequence as a key,
from a string like "foo" to the content of a JPEG file.
The empty string is also a valid key.

- FLUSHALL: 清空所有数据
- RANDOMKEY: 随机获取一个key
- DBSIZE: 返回当前数据库的 key 的数量
- INFO: Redis 服务器的各种信息和统计数值

- start: `redis-server redis.conf`
- stop: `redis-cli shutdown`

# data type
## Binary-safe string
- `set key value`: 如果key 之前有值, 会被替换掉
- `setnx key value`: set if not exist
- `get key`
- `incr key` => "11" ## incr 是原子操作, INCRBY, DECR and DECRBY
- `del key`
- `expire key time(in seconds)`, 只对之前紧挨着的set key value 有效
- `ttl key`: 返回key的仍然有效时间, The -2 means that the key does not exist (anymore), -1 means that it will never expire.
- `exists key`
- `type key`, which returns the kind of value stored at the specified key:

C API

- set 返回的`reply->type` 为 `REDIS_REPLY_STATUS`, `reply->str` 为 "OK"
- del 返回 `reply->type` 为 `REDIS_REPLY_INTEGER`
- get 返回的type 为 `REDIS_REPLY_STATUS`, 结果在 `reply->str` 中

## Lists
collections of string elements sorted according to the order of insertion. They are basically linked lists

- RPUSH(LPUSH): right push, left push
- LLEN: list length
- LRANGE: lrange list start end(包括两个边界), 0 表示第一个元素, -1 表示最后一个元素
- LPOP(RPOP): left(right) pop: removes the first(last) element from the list and returns it

## Sets
collections of unique, unsorted string elements

- SADD: set add
- SREM: set remove
- SISMEMBER: set is member
- SMEMBERS: set members: returns all members
- SUNION: set union

## Sorted sets
similar to Sets but where every string element is associated to a floating number value, called score.
The elements are always taken sorted(正序排列的) by their score, so unlike Sets it is possible to retrieve a range of
elements (for example you may ask: give me the top 10, or the bottom 10).

- `zadd set score value`
- `zrange set start end`;
- zrangebyscore: `zrangebyscore hackers -inf 1950`

## Hash
- `HSET key field1 value1`, 或者合在一起`HMSET key field1 value1 field2 value2 ... fieldn valuen`
- `HGETALL key`
- `HGET key fieldi`
	- C API 中 HGET 不存在的key, 返回的reply type 为 `REDIS_REPLY_NIL`
- `HDEL key field1 field2 ... fieldn`
- `HINCRBY key fieldi num`: num 设置为负, 就实现了减

## Bit arrays (or simply bitmaps)
it is possible, using special commands, to handle String values like an array of bits:
you can set and clear individual bits, count all the bits set to 1, find the first set or unset bit, and so forth

# API
```C++
typedef struct redisReply {
	int type; /* REDIS_REPLY_* */
	long long integer; /* The integer when type is REDIS_REPLY_INTEGER */
	int len; /* Length of string */
	char *str; /* Used for both REDIS_REPLY_ERROR and REDIS_REPLY_STRING */
	size_t elements; /* number of elements, for REDIS_REPLY_ARRAY */
	struct redisReply **element; /* elements vector for REDIS_REPLY_ARRAY */
} redisReply;
```

`redisReply *reply = (redisReply *)redisCommand(redis, cmd);` 每次都需要执行`freeReplyObject(reply);`
释放redisCommand 函数为reply 申请的内存

## pipeline
[Redis: Pipelining, Transactions and Lua Scripts](
	https://rafaeleyng.github.io/redis-pipelining-transactions-and-lua-scripts)

The server buffers all the answers in memory and sends all at once when the pipeline is done.
Using a few thousands of commands inside a pipeline is usually a good starting point.

Benefits of pipelining

1. batching several commands in a single message allows us to save multiple times the round trip time between the client
	and the Redis server
1. it avoids context switching, both in the client and in the server. When the server or the client need to read from or
	write to the network, a syscall is made and an expensive context switch happens between user space and kernel space.
	If we send 10 messages, each with a single command, 10 context switches will happen. If we send a single message with
	10 commands, it's likely that a single context switch will be needed.

注意点

1. pipelining is not atomic.
1. pipelining is non-blocking on the server. This means that even if a Client 1 has a huge and slow pipeline, other
	clients won't be blocked, because the commands from the other clients will be interleaved with the commands from
	Client 1 pipeline.
1. If some command fails, the pipeline continues and an error is returned at the end, as the response for that specific
	command (not for the whole pipeline).

## Transactions
A transaction works by issuing a MULTI command, then sending all the commands that compose the pipeline, and an EXEC or
a DISCARD at the end.

# lua
[深入分析 Redis Lua 脚本运行原理](https://juejin.im/post/6844903697034510343)

Redis 服务器会单线程原子性执行 lua 脚本,保证 lua 脚本在处理的过程中不会被任意其它请求打断.

比如在分布式锁小节,我们提到了`del_if_equals`伪指令,它可以将匹配 key 和删除 key 合并在一起原子性执行,Redis 原生没有提供这样功能的指令,它可
以使用 lua 脚本来完成.
```lua
if redis.call("get", KEYS[1]) == ARGV[1] then
  return redis.call("del", KEYS[1])
else
  return 0
end
```

那上面这个脚本如何执行呢?使用 EVAL 指令
```lua
EVAL SCRIPT KEY_NUM KEY1 KEY2 ... KEYN ARG1 ARG2 ....
```

```redis
127.0.0.1:6379> set foo bar
OK
127.0.0.1:6379> eval 'if redis.call("get",KEYS[1]) == ARGV[1] then return redis.call("del",KEYS[1]) else return 0 end'
1 foo bar
(integer) 1
127.0.0.1:6379> eval 'if redis.call("get",KEYS[1]) == ARGV[1] then return redis.call("del",KEYS[1]) else return 0 end'
1 foo bar
(integer) 0
```

## SCRIPT LOAD 和 EVALSHA 指令
在上面的例子中,脚本的内容很短.如果脚本的内容很长,而且客户端需要频繁执行,那么每次都需要传递冗长的脚本内容势必比较浪费网络流量.
所以 Redis 还提供了 SCRIPT LOAD 和 EVALSHA 指令来解决这个问题.

SCRIPT LOAD 指令用于将客户端提供的 lua 脚本传递到服务器而不执行,但是会得到脚本的唯一 ID,这个唯一 ID 是用来唯一标识服务器缓存的这段lua 脚
本,它是由 Redis 使用 sha1 算法揉捏脚本内容而得到的一个很长的字符串.有了这个唯一 ID,后面客户端就可以通过 EVALSHA 指令反复执行这个脚本了.

下面我们使用 SCRIPT LOAD 和 EVALSHA 指令来完成自乘运算.
```lua
local curVal = redis.call("get", KEYS[1])
if curVal == false then
  curVal = 0
else
  curVal = tonumber(curVal)
end
curVal = curVal * tonumber(ARGV[1])
redis.call("set", KEYS[1], curVal)
return curVal
```

加载脚本
```redis
127.0.0.1:6379> script load 'local curVal = redis.call("get", KEYS[1]); if curVal == false then curVal = 0 else
curVal = tonumber(curVal) end; curVal = curVal * tonumber(ARGV[1]); redis.call("set", KEYS[1], curVal); return curVal'
"be4f93d8a5379e5e5b768a74e77c8a4eb0434441"
```

使用脚本
```redis
127.0.0.1:6379> evalsha be4f93d8a5379e5e5b768a74e77c8a4eb0434441 1 notexistskey 5
(integer) 0
127.0.0.1:6379> evalsha be4f93d8a5379e5e5b768a74e77c8a4eb0434441 1 notexistskey 5
(integer) 0
127.0.0.1:6379> set foo 1
OK
127.0.0.1:6379> evalsha be4f93d8a5379e5e5b768a74e77c8a4eb0434441 1 foo 5
(integer) 5
127.0.0.1:6379> evalsha be4f93d8a5379e5e5b768a74e77c8a4eb0434441 1 foo 5
(integer) 25
```


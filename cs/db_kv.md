# redis
## intro
redis 中数据对象都是 string 类型, 但是有时会把string 解释成其他类型, 例如 incr 会把 string 解释成int 类型

Redis keys are binary safe, this means that you can use any binary sequence as a key, from a string like "foo" to the content of a JPEG file.
The empty string is also a valid key.

- FLUSHALL: 清空所有数据
- RANDOMKEY: 随机获取一个key
- DBSIZE: 返回当前数据库的 key 的数量
- INFO: Redis 服务器的各种信息和统计数值

- start: `redis-server redis.conf`
- stop: `redis-cli shutdown`

## data type
### Binary-safe string
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

### Lists
collections of string elements sorted according to the order of insertion. They are basically linked lists

- RPUSH(LPUSH): right push, left push
- LLEN: list length
- LRANGE: lrange list start end(包括两个边界), 0 表示第一个元素, -1 表示最后一个元素
- LPOP(RPOP): left(right) pop: removes the first(last) element from the list and returns it

### Sets
collections of unique, unsorted string elements

- SADD: set add
- SREM: set remove
- SISMEMBER: set is member
- SMEMBERS: set members: returns all members
- SUNION: set union

### Sorted sets
similar to Sets but where every string element is associated to a floating number value, called score.
The elements are always taken sorted(正序排列的) by their score, so unlike Sets it is possible to retrieve a range of elements
(for example you may ask: give me the top 10, or the bottom 10).

- `zadd set score value`
- `zrange set start end`;
- zrangebyscore: `zrangebyscore hackers -inf 1950`

### Hash
- `HSET key field1 value1`, 或者合在一起`HMSET key field1 value1 field2 value2 ... fieldn valuen`
- `HGETALL key`
- `HGET key fieldi`
	- C API 中 HGET 不存在的key, 返回的reply type 为 `REDIS_REPLY_NIL`
- `HDEL key field1 field2 ... fieldn`
- `HINCRBY key fieldi num`: num 设置为负, 就实现了减

### Bit arrays (or simply bitmaps)
it is possible, using special commands, to handle String values like an array of bits:
you can set and clear individual bits, count all the bits set to 1, find the first set or unset bit, and so forth

## API
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

`redisReply *reply = (redisReply *)redisCommand(redis, cmd);` 每次都需要执行`freeReplyObject(reply);`释放redisCommand 函数为reply 申请的内存

pipeline

1. redis的底层通信协议对管道(Pipelining)提供了支持,
2. 通过管道可以一次发送多条命令,并在执行完命令后将结果统一返回,
3. 当一组命令中每条命令都不依赖于之前命令的执行结果时,就可以将这组命令一起放入管道中发出,
4. 管道通过减少通信次数,降低往返时延累加造成的性能消耗,从而提升效率,

## 扩展写性能和内存容量
[Redis实战:如何构建类微博的亿级社交平台](http://www.tuicool.com/articles/eyAfeyq)

随着被缓存的数据越来越多,当数据没办法被存储到单台机器上面的时候,我们就需要想办法把数据分割存储到由多台机器组成的集群里面.
如果用尽了一切方法降低内存占用并且尽可能地提高性能之后,问题仍然未解决,那么说明我们已经遇到了只使用单台机器带来的瓶颈,是时候将数据分片到多台机器上面了.

本文介绍的数据分片方法要求用户使用固定数量的 Redis 服务器.
举个例子,如果写入量预计每 6 个月就会增加 4 倍,那么我们可以将数据预先分片(preshard)到 256 个分片里面,从而拥有一个在接下来的 2 年时间里面都能够满足预期写入量增长的分片方案
具体要规划多长远的方案要由你自己决定).
在为了应对未来可能出现的流量增长而对系统进行预先分片的时候,我们可能会陷入这样一种处境:目前拥有的数据实在太少,按照预先分片方法计算出的机器数量去存储这些数据只会得不偿失.
为了能够如常地对数据进行分割,我们可以在单台机器上面运行多个 Redis 服务器,并将每个服务器用作一个分片.
注意,在同一台机器上面运行多个 Redis 服务器的时候,请记得让每个服务器都监听不同的端口,并确保所有服务器写入的都是不同的快照文件或 AOF 文件.

# 实践
#### 用get/set方式使用Redis
作为一个key value存在, 很多开发者自然的使用set/get方式来使用Redis, 实际上这并不是最优化的使用方法. 尤其在未启用VM情况下, Redis全部数据需要放入内存, 节约内存尤其重要.
假如一个key-value单元需要最小占用512字节, 即使只存一个字节也占了512字节.
这时候就有一个设计模式, 可以把key复用, 几个key-value放入一个key中, value再作为一个set存入, 这样同样512字节就会存放10 - 100倍的容量.
这就是为了节约内存, 建议使用hashset而不是set/get的方式来使用Redis

# memcache
ref: [MemCache超详细解读](http://www.csdn.net/article/2016-03-16/2826609)

MemCache虽然被称为"分布式缓存",但是MemCache本身完全不具备分布式的功能,MemCache集群之间不会相互通信,所谓的"分布式",完全依赖于客户端程序的实现.

MemCache一次写缓存的流程:

1. 应用程序输入需要写缓存的数据
2. API将Key输入路由算法模块,路由算法根据Key和MemCache集群服务器列表得到一台服务器编号
3. 由服务器编号得到MemCache及其的ip地址和端口号
4. API调用通信模块和指定编号的服务器通信,将数据写入该服务器,完成一次分布式缓存的写操作

读缓存和写缓存一样,只要使用相同的路由算法和服务器列表,只要应用程序查询的是相同的Key

不能够遍历MemCache中所有的item,因为这个操作的速度相对缓慢且会阻塞其他的操作

MemCache的高性能源自于两阶段哈希结构:

1. 第一阶段在客户端,通过Hash算法根据Key值算出一个节点,
1. 第二阶段在服务端,通过一个内部的Hash算法,查找真正的item并返回给客户端.从实现的角度看,MemCache是一个非阻塞的,基于事件的服务器程序

## 一致性Hash算法

## Memcache内存管理机制
ref: [Memcache内存分配机制](https://my.oschina.net/hejiula/blog/151287)

![memcache 内存管理](http://img.ptcms.csdn.net/article/201603/16/56e8cb1b91a97_middle.jpg?_=30354)

- MemCache将内存空间分为一组slab
- 每个slab下又有若干个page,每个page默认是1M(所以每个value 的大小不能超过1M),如果一个slab占用100M内存的话,那么这个slab下应该有100个page
- 每个page里面包含一组chunk,chunk是真正存放数据的地方,同一个slab里面的chunk的大小是固定的
- 有相同大小chunk的slab被组织在一起,称为slab class
- MemCache内存分配的方式称为allocator,slab的数量是有限的,几个,十几个或者几十个,这个和启动参数的配置相关.

# redis vs memcache
ref: [ Redis和Memcache对比及选择](http://blog.csdn.net/sunmenggmail/article/details/36176029)

You should not care too much about performances.
**Redis is faster per core with small values, but memcached is able to use multiple cores** with a single executable and TCP port without help from the client.
Also memcached is faster with big values in the order of 100k. Redis recently improved a lot about big values (unstable branch) but still memcached is faster in this use case.
The point here is: nor one or the other will likely going to be your bottleneck for the query-per-second they can deliver.

You should care about memory usage.
For simple key-value pairs memcached is more memory efficient.
If you use Redis hashes, Redis is more memory efficient. Depends on the use case.

You should care about persistence and replication, two features only available in Redis.
Even if your goal is to build a cache it helps that after an upgrade or a reboot your data are still there.

You should care about the kind of operations you need.
In Redis there are a lot of complex operations, even just considering the caching use case, you often can do a lot more in a single operation,
without requiring data to be processed client side (a lot of I/O is sometimes needed).
This operations are often as fast as plain GET and SET. So if you don't need just GEt/SET but more complex things Redis can help a lot (think at timeline caching).


# intro
redis 中数据对象都是 string 类型, 但是有时会把string 解释成其他类型, 例如 incr 会把 string 解释成int 类型

Redis keys are binary safe, this means that you can use any binary sequence as a key, from a string like "foo" to the content of a JPEG file.
The empty string is also a valid key.

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
The elements are always taken sorted(正序排列的) by their score, so unlike Sets it is possible to retrieve a range of elements
(for example you may ask: give me the top 10, or the bottom 10).

- `zadd set score value`
- `zrange set start end`;
- zrangebyscore: `zrangebyscore hackers -inf 1950`

## Hash
- hset

		HSET user:1000 name "John Smith"
		HSET user:1000 email "john.smith@example.com"
		HSET user:1000 password "s3cret"

	或者和在一起`hmset user:1001 name "Mary Jones" password "hidden" email "mjones@example.com"`
- `hgetall user:1000`
- `hget user:1001 name`
- hdel
- hincrby

## Bit arrays (or simply bitmaps)
it is possible, using special commands, to handle String values like an array of bits:
you can set and clear individual bits, count all the bits set to 1, find the first set or unset bit, and so forth

# config
配置文件: `/etc/redis.conf`

- daemonize: 是否以后台daemon方式运行, 默认为 否
- port: 监听的端口号, 6379
- loglevel: log信息级别, notice
- logfile: log文件位置, `/var/log/redis/redis.log`
- rdbcompression: 是否使用压缩, yes
- dbfilename: 数据快照文件名(只是文件名,不包括目录), dump.rdb
- dir: 数据快照的保存目录(这个是目录), `/var/lib/redis/`
 
# install
https://console.tenxcloud.com/docker-registry/detail?imageName=tenxcloud/redis

1. 下载: `docker pull index.tenxcloud.com/tenxcloud/redis`
1. 启动server: `docker run -d -p 6379:6379 -e REDIS_PASS="None" --name server redis`, redis_dir: /data, 可以通过-v 进行挂载本地路径
1. client(另外一个具有redis-cli 的container): `redis-cli -h server -p 7379 -a "None"`, 这里的server 就是第二步骤中container 名字为server 的redis server container


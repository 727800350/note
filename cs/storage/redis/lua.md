[深入分析 Redis Lua 脚本运行原理](https://juejin.im/post/6844903697034510343)

Redis 服务器会单线程原子性执行 lua 脚本,保证 lua 脚本在处理的过程中不会被任意其它请求打断.

比如在分布式锁小节,我们提到了`del_if_equals`伪指令,它可以将匹配 key 和删除 key 合并在一起原子性执行,Redis 原生没有提供这
样功能的指令,它可以使用lua 脚本来完成.
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

# SCRIPT LOAD 和 EVALSHA 指令
在上面的例子中,脚本的内容很短.如果脚本的内容很长,而且客户端需要频繁执行,那么每次都需要传递冗长的脚本内容势必比较浪费网络
流量. 所以 Redis 还提供了 SCRIPT LOAD 和 EVALSHA 指令来解决这个问题.

SCRIPT LOAD 指令用于将客户端提供的 lua 脚本传递到服务器而不执行,但是会得到脚本的唯一ID,这个唯一ID 是用来唯一标识服务器缓
存的这段lua 脚本,它是由Redis 使用sha1 算法对脚本内容计算得到. 有了这个唯一ID,后面客户端就可以通过EVALSHA 指令反复执行这
个脚本了.

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


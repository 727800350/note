[MemCache超详细解读](http://www.csdn.net/article/2016-03-16/2826609)

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

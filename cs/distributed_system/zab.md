# zookeeper intro
[实例详解ZooKeeper ZAB协议,分布式锁与领导选举](https://dbaplus.cn/news-141-1875-1.html)

ZooKeeper是一个分布式协调服务,可用于服务发现,分布式锁,分布式领导选举,配置管理等.
 
这一切的基础,都是ZooKeeper提供了一个类似于Linux文件系统的树形结构(可认为是轻量级的内存文件系统,但只适合存少量信息,完全不适合存储大量文件或者大文件),同时提供了对于每个节点的监控与通知机制.
既然是一个文件系统,就不得不提ZooKeeper是如何保证数据的一致性的.本节将将介绍ZooKeeper如何保证数据一致性,如何进行领导选举,以及数据监控/通知机制的语义保证

ZooKeeper集群是一个基于主从复制的高可用集群,每个服务器承担如下三种角色中的一种:

- Leader 一个ZooKeeper集群同一时间只会有一个实际工作的Leader,它会发起并维护与各Follwer及Observer间的心跳.所有的写操作必须要通过Leader完成再由Leader将写操作广播给其它服务器.
- Follower 一个ZooKeeper集群可能同时存在多个Follower,它会响应Leader的心跳.Follower可直接处理并返回客户端的读请求,同时会将写请求转发给Leader处理,并且负责在Leader处理写请求时对请求进行投票.
- Observer 角色与Follower类似,但是无投票权.

# 原子广播zab
为了保证写操作的一致性与可用性,ZooKeeper专门设计了一种名为原子广播(ZAB)的支持崩溃恢复的一致性协议.基于该协议,ZooKeeper实现了一种主从模式的系统架构来保持集群中各个副本之间的数据一致性.

根据ZAB协议,所有的写操作都必须通过Leader完成,Leader写入本地日志后再复制到所有的Follower节点.

一旦Leader节点无法工作,ZAB协议能够自动从Follower节点中重新选出一个合适的替代者,即新的Leader,该过程即为领导选举.该领导选举过程,是ZAB协议中最为重要和复杂的过程.

## 读写服务
### 写Leader
通过Leader进行写操作流程如下图所示:
![write through leader](./pics/zab_write_through_leader.jpg)

由上图可见,通过Leader进行写操作,主要分为五步:

1. 客户端向Leader发起写请求
1. Leader将写请求以Proposal的形式发给所有Follower并等待ACK
1. Follower收到Leader的Proposal后返回ACK
1. Leader得到过半数的ACK(Leader对自己默认有一个ACK)后向所有的Follower和Observer发送Commmit
1. Leader将处理结果返回给客户端

### 写Follower/Observer
![write through follower](./pics/zab_write_through_follower.jpg)

从上图可见:

1. Follower/Observer均可接受写请求,但不能直接处理,而需要将写请求转发给Leader处理
1. 除了多了一步请求转发,其它流程与直接写Leader无任何区别

### 读操作
Leader/Follower/Observer都可直接处理读请求,从本地内存中读取数据并返回给客户端即可.

![read](./pics/zab_read.jpg)


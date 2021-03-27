ref

- [与程序员相关的CPU缓存知识](https://coolshell.cn/articles/20793.html)
- [理解CPU Cache](https://www.jianshu.com/p/900554f11881)

多核cpu 缓存结构

<img src="./pics/cpu_cache/cpu_cache_architecture.png" alt="cpu_cache_architecture" width="80%"/>

其中:

- L1 Cache分为D-Cache和I-Cache,D-Cache用来存储数据,I-Cache用来存放指令,一般L1 Cache的大小是32k,
- L2 Cache 更大一些,例如256K, 速度要慢一些, 一般情况下每个核上都有一个独立的L2 Cache,
- L3 Cache是三级缓存中最大的一级,例如12MB,同时也是最慢的一级,在同一个CPU插槽之间的核共享一个L3 Cache.

下面的列表表示了CPU到各缓存和内存之间的大概速度

| 从CPU到  | 大约需要的CPU周期 | 大约需要的时间(单位ns) |
| ---      | ---               | ---                    |
| 寄存器   | 1 cycle           |                        |
| L1 Cache | ~3-4 cycles       | ~0.5-1 ns              |
| L2 Cache | ~10-20 cycles     | ~3-7 ns                |
| L3 Cache | ~40-45 cycles     | ~15 ns                 |
| 跨槽传输 | ~20 ns            |                        |
| 内存     | ~120-240 cycles   | ~60-120ns              |

在Linux中可以通过如下命令查看CPU Cache 的大小
```bash
cat /sys/devices/system/cpu/cpu0/cache/index0/size
32K
cat /sys/devices/system/cpu/cpu0/cache/index1/size
32K
cat /sys/devices/system/cpu/cpu0/cache/index2/size
256K
cat /sys/devices/system/cpu/cpu0/cache/index3/size
20480K
cat /sys/devices/system/cpu/cpu0/cache/index0/type
Data
cat /sys/devices/system/cpu/cpu0/cache/index1/type
Instruction
```

这个世界永远是平衡的,一面变得有多光鲜,另一面也会变得有多黑暗.建立这么多级的缓存,一定就会引入其它的问题,这里有两个比较重
要的问题

- 一个是比较简单的缓存的命中率的问题.
- 另一个是比较复杂的缓存更新的一致性问题.

尤其是第二个问题,在多核技术下,这就很像分布式的系统了,要对多个地方进行更新.

# 缓存的命中
在说明这两个问题之前.我们需要要解一个术语 Cache Line.
缓存基本上来说就是把后面的数据加载到离自己近的地方,对于CPU来说,它是不会一个字节一个字节的加载的,因为这非常没有效率,一般
来说都是要一块一块的加载的,对于这样的一块一块的数据单位,术语叫"Cache Line".
一般来说,一个主流的CPU的Cache Line 是64 Bytes(也有的CPU用32Bytes和128Bytes), 64Bytes也就是16个32位的整型, 这就是CPU从内
存中捞数据上来的最小数据单位.

比如:Cache Line是最小单位(64Bytes),所以先把Cache分布多个Cache Line,比如:L1有32KB,那么,32KB/64B = 512 个 Cache Line.

通过如下命令查看缓存行的大小:
```bash
cat /sys/devices/system/cpu/cpu0/cache/index0/coherency_line_size
64
```

一方面,缓存需要把内存里的数据放到放进来,英文叫 CPU Associativity.
Cache的数据放置的策略决定了内存中的数据块会拷贝到CPU Cache中的哪个位置上,因为Cache的大小远远小于内存,所以,需要有一种地址
关联的算法,能够让内存中的数据可以被映射到Cache中来.这个有点像内存地址从逻辑地址向物理地址映射的方法,但不完全一样.

基本上来说,我们会有如下的一些方法.

- 一种方法是,任何一个内存地址的数据可以被缓存在任何一个Cache Line里. 这种方法是最灵活的,但是,如果我们要知道一个内存是否
  存在于Cache中,我们就需要进行O(n)复杂度的Cache遍历,这是很没有效率的.
- 另一种方法,为了降低缓存搜索算法,我们需要使用像Hash Table这样的数据结构. 最简单的hash table就是做"求模运算", 比如:我们
  的L1 Cache有512个Cache Line, 那么, 公式: `(内存地址 mod 512) * 64` 就可以直接找到所在的Cache地址的偏移了.但是,这样的方
  式需要我们的程序对内存地址的访问要非常地平均, 不然冲突就会非常严重. 这成了一种非常理想的情况了.
- 为了避免上述的两种方案的问题, 于是就要容忍一定的hash冲突, 也就出现了 N-Way 关联. 也就是把连续的N个Cache Line绑成一组,
  然后,先把找到相关的组, 然后再在这个组内找到相关的Cache Line. 这叫 Set Associativity.如下图所示.

<img src="./pics/cpu_cache/cache-associative-fill-both.png" alt="cache-associative-fill-both" width="70%"/>

此外,当有数据没有命中缓存的时候,CPU就会以最小为Cache Line的单元向内存更新数据.当然,CPU并不一定只是更新64Bytes,因为访问主
存实在是太慢了,所以,一般都会多更新一些.
好的CPU会有一些预测的技术,如果找到一种pattern的话,就会预先加载更多的内存,包括指令也可以预加载.这叫 Prefetching 技术.
比如,你在for-loop访问一个连续的数组,你的步长是一个固定的数,内存就可以做到prefetching.(注:指令也是以预加载的方式执行,参看
本站的<代码执行的效率>中的第三个示例)

了解这些细节,会有利于我们知道在什么情况下有可以导致缓存的失效.

# 缓存的一致性
对于主流的CPU来说,缓存的写操作基本上是两种策略(参看本站<缓存更新的套路>),

- 一种是Write Back,写操作只要在cache上,然后再flush到内存上.
- 一种是Write Through,写操作同时写到cache和内存上.

为了提高写的性能,一般来说,主流的CPU(如:Intel Core i7/i9)采用的是Write Back的策略,因为直接写内存实在是太慢了.

好了,现在问题来了,如果有一个数据 x 在 CPU 第0核的缓存上被更新了,那么其它CPU核上对于这个数据 x 的值也要被更新,这就是缓存
一致性的问题.(当然,对于我们上层的程序我们不用关心CPU多个核的缓存是怎么同步的,这对上层的代码来说都是透明的)

一般来说,在CPU硬件上,会有两种方法来解决这个问题.

- Directory 协议. 这种方法的典型实现是要设计一个集中式控制器,它是主存储器控制器的一部分.其中有一个目录存储在主存储器中,
  其中包含有关各种本地缓存内容的全局状态信息.当单个CPU Cache 发出读写请求时,这个集中式控制器会检查并发出必要的命令,以在
  主存和CPU Cache之间或在CPU Cache自身之间进行数据同步和传输.
- Snoopy 协议. 这种协议更像是一种数据通知的总线型的技术. CPU Cache通过这个协议可以识别其它Cache上的数据状态.如果有数据共
  享的话,可以通过广播机制将共享数据的状态通知给其它CPU Cache.这个协议要求每个CPU Cache 都可以"窥探"数据事件的通知并做出
  相应的反应.

因为Directory协议是一个中心式的,会有性能瓶颈,而且会增加整体设计的复杂度. 而Snoopy协议更像是微服务+消息通讯, 所以, 现在基
本都是使用Snoopy的总线的设计.

这里,我想多写一些细节,因为这种微观的东西,让人不自然地就会跟分布式系统关联起来,在分布式系统中我们一般用Paxos/Raft这样的分
布式一致性的算法.而在CPU的微观世界里,则不必使用这样的算法,原因是因为CPU的多个核的硬件不必考虑网络会断会延迟的问题.所以,
CPU的多核心缓存间的同步的核心就是要管理好数据的状态就好了.

这里介绍几个状态协议,先从最简单的开始,MESI协议,其主要表示缓存数据有四个状态: Modified(已修改), Exclusive(独占的),
Shared(共享的), Invalid(无效的).

下面是个示例:

| 当前操作           | CPU0    | CPU1   | Memory | 说明                                                       |
| ---                | ---     | ---    | ---    | ---                                                        |
| 1) CPU0 read(x)    | x=1 (E) |        | x=1    | 只有一个CPU有x, 所以状态是 Exclusive                       |
| 2) CPU1 read(x)    | x=1 (S) | x=1(S) | x=1    | 有两个CPU读取x, 所以状态变成 Shared                        |
| 3) CPU0 write(x,9) | x=9 (M) | x=1(I) | x=1    | 变量改变,在CPU0中状态变成Modified, 在CPU1中状态变成Invalid |
| 4) 变量 x 写回内存 | x=9 (M) | X=1(I) | x=9    | 目前的状态不变                                             |
| 5) CPU1  read(x)   | x=9 (S) | x=9(S) | x=9    | 变量同步到所有的Cache中,状态回到Shared                     |

MESI 这种协议在数据更新后,会标记其它共享的CPU缓存的数据拷贝为Invalid状态,然后当其它CPU再次read的时候,就会出现 cache miss
的问题,此时再从内存中更新数据.从内存中更新数据意味着20倍速度的降低.

我们能不能直接从我隔壁的CPU缓存中更新? 是的, 这就可以增加很多速度了, 但是状态控制也就变麻烦了.还需要多来一个状态:
Owner(宿主),用于标记,我是更新数据的源.于是,出现了 MOESI 协议. 性能是非常大的提升,但是控制逻辑也非常复杂.

顺便说一下,与 MOESI 协议类似的一个协议是 MESIF,其中的F 是Forward, 同样是把更新过的数据转发给别的CPU Cache 但是, MOESI 中
的 Owner 状态 和MESIF 中的 Forward 状态有一个非常大的不一样. Owner状态下的数据是dirty的,还没有写回内存,Forward状态下的数
据是clean的,可以丢弃而不用另行通知.

需要说明的是,AMD用 MOESI, Intel用MESIF. 所以, F 状态主要是针对CPU L3 Cache 设计的(前面我们说过,L3是所有CPU核心共享的).

# 程序性能
接下来,我们来看一下多核下的性能问题,参看如下的代码.两个线程在操作一个数组的两个不同的元素(无需加锁),线程循环1000万次,做
加法操作.
p2指针,要么是p[1],或是 p[30],理论上来说,无论访问哪两个数组元素,都应该是一样的执行时间.
```cpp
void fn (int* data) {
  for (int i = 0; i < 10*1024*1024; ++i) {
    *data += rand();
  }
}

int p[32];
int *p1 = &p[0];
int *p2 = &p[1]; // int *p2 = &p[30];

thread t1(fn, p1);
thread t2(fn, p2);
```
然而,并不是,在我的机器上执行下来的结果是:
```plain
对于 p[0] 和 p[1] :560ms
对于 p[0] 和 p[30]:104ms
```
这是因为p[0] 和 p[1] 在同一条 Cache Line 上, 而 p[0] 和 p[30] 则不可能在同一条Cache Line 上, CPU的缓存最小的更新单位是
Cache Line,所以,这导致虽然两个线程在写不同的数据,但是因为这两个数据在同一条Cache Line上,就会导致缓存需要不断在两个CPU的
L1/L2中进行同步,从而导致了5倍的时间差异.


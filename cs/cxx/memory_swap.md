ref: [调整linux内核尽量用内存,而不用swap](http://lxshopping.blog.51cto.com/4542643/1573947)

Linux uses kswapd for virtual memory management such that pages that have been recently accessed are kept in memory and less active pages are paged outto disk.

kswapd0进程的作用:它是虚拟内存管理中,负责换页的,操作系统每过一定时间就会唤醒kswapd ,看看内存是否紧张,如果不紧张,则睡眠.
在 kswapd 中,有2 个阀值,`pages_hige` 和 `pages_low`,当空闲内存页的数量低于 `pages_low` 的时候,
kswapd进程就会扫描内存并且每次释放出32 个free pages,直到 free page 的数量到达`pages_high`.

`vm.swappiness` 内核参数控制内存使用到多少开始使用swap.
`sysctl vm.swappiness` or `cat /proc/sys/vm/swappiness` 查看当前系统的设置

这个交换参数控制内核从物理内存移出进程,移到交换空间.该参数从0到100,默认值一般是60

- 当该参数=0,表示只要有可能就尽力避免交换进程移出物理内存, 并不代表禁用swap分区,只是告诉内核,能少用到swap分区就尽量少用到
- 该参数=100,这告诉内核疯狂的将数据移出物理内存移到swap缓存中.

## 关于linux内存分配机制
在linux的内存分配机制中,优先使用物理内存,当物理内存还有空闲时(还够用),不会释放其占用内存,
就算占用内存的程序已经被关闭了,该程序所占用的内存用来做缓存使用,对于开启过的程序,或是读取刚存取过得数据会比较快.

一个内存使用的例子:
```
[oracle@db1 ~]$ free -m
               total      used      free     shared  buffers   cached
Mem:           72433      67075     5357     0       558       62221
-/+ buffers/cache:        4295      68138
Swap:          72096      91        72004
```
一共有72433M 的内存,67075M被占用,但是buffers和cached部分作为缓存,可以使用命中率的方式提高使用效率,而且这部分缓存是根据指令随时可以释放的,
我们可以认为这部分内存没有实际被使用,也可以认为它是空闲的.
因此查看目前正在实际被使用的内存,是`used - (buffers + cache)`.
这里67075 - (558 + 62221) = 4296 得到的结果和-/+ buffers/cache 行used 下的4295 接近.
也可以认为如果swap没有大量使用,mem还是够用的,只有mem被当前进程实际占用完(没有了buffers和cache),才会使用到swap的.

### swap 配置对性能的影响
分配太多的Swap空间会浪费磁盘空间,而Swap空间太少,则系统会发生错误.
如果系统的物理内存用光了,系统就会跑得很慢,但仍能运行,
如果Swap空间用光了,那么系统就会发生错误. 例如,Web服务器能根据不同的请求数量衍生出多个服务进程(或线程),如果Swap空间用完,则服务进程无法启动,
通常会出现"application is out of memory"的错误,严重时会造成服务进程的死锁.
因此Swap空间的分配是很重要的.



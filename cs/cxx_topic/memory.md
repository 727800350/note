- [内存模型](#内存模型)
  - [`memory_order_seq_cst`/`memory_order_seq_cst`](#memory_order_seq_cstmemory_order_seq_cst)
  - [`memory_order_acquire/memory_order_release`](#memory_order_acquirememory_order_release)
  - [`memory_order_consume/memory_order_release`](#memory_order_consumememory_order_release)
  - [`memory_order_relaxed`/`memory_order_relaxed`](#memory_order_relaxedmemory_order_relaxed)
  - [spin lock using `std::atomic_flag`](#spin-lock-using-stdatomic_flag)
- [Lock-free vs spin-lock](#lock-free-vs-spin-lock)
- [ABA - A is not the same as A](#aba---a-is-not-the-same-as-a)
- [x86_64处理器的指针赋值是原子操作吗?](#x86_64处理器的指针赋值是原子操作吗)

# 内存模型
内存模型主要包含了下面三个部分:

- 原子操作:顾名思义,这类操作一旦执行就不会被打断,你无法看到它的中间状态,它要么是执行完成,要么没有执行.
- 操作的局部顺序:一系列的操作不能被乱序.
- 操作的可见性:定义了对于共享变量的操作如何对其他线程可见.

[理解 C++ 的 Memory Order](http://senlinzhan.github.io/2017/12/04/cpp-memory-order/)

为什么需要 Memory Order?

如果不使用任何同步机制(例如 mutex 或 atomic),在多线程中读写同一个变量,那么,程序的结果是难以预料的.
简单来说,编译器以及 CPU 的一些行为,会影响到程序的执行结果:

- 即使是简单的语句(CPU 层面可能是多个指令),C++ 也不保证是原子操作.
- CPU 可能会调整指令的执行顺序.
- 在 CPU cache 的影响下,一个 CPU 执行了某个指令,不会立即被其它 CPU 看见.

c++11提供了5种内存模型,分别是
```C++
memory_order_relaxed
memory_order_consume
memory_order_acquire
memory_order_release
memory_order_seq_cst
```

`memory_order_acq_rel`并不是一个内存模型, 只是`memory_order_acquire` 和`memory_order_release` 的合并,在一些读取并且修改
的操作中(比如cas操作)用到.

这5中内存模型中,有些用于写值(store),有些用于读值(load),有些需要配对使用,有些两者都能用.下面这张表是所有的用法

| load                   | store                  |
| ---                    | ---                    |
| `memory_order_seq_cst` | `memory_order_seq_cst` |
| `memory_order_acquire` | `memory_order_release` |
| `memory_order_consume` | `memory_order_release` |
| `memory_order_relaxed` | `memory_order_relaxed` |

永远不要混用这些模型,比如load用`memory_order_seq_cst`,store用`memory_order_release`.不作死就不会死.

大部分工程实践,更建议直接用默认参数(`memory_order_seq_cst`).
一方面,要正确理解并使用好release/acquire/consume的确要花点功夫,关键不好测试是否正确,出了问题不好定位;
另外一方面,除了核心底层代码,一般性能瓶颈不会在内存模型的选择上.

## `memory_order_seq_cst`/`memory_order_seq_cst`
`seq_cst`是sequentially consistent(顺序一致)的缩写,也是默认的内存模型.
这种内存模型会在所有CPU(或者核心)中,保持严格的代码线性执行顺序.

如下面代码(x为普通变量,y,z为原子变量)
```C++
-Thread 1-       -Thread 2-
x = 1            if (z.load() == 3) {
y.store(2);          assert(x == 1)
z.store(3);          assert(y.load() == 2)
                 }
```
如果线程2 中读到的z值为3,那么,在线程1中,由于x = 1 和 y.sotre 发生在z.store 之前, 所以在线程2中也保持了这种顺序.
于是线程2 中的两个assert是不可能失败的.
这种模型具有最强的数据一致性,因此性能也最差.

## `memory_order_acquire/memory_order_release`
这种模型比`memory_order_seq_cst`稍微宽松一些,它**移除了不相关的原子变量的顺序一致性**.
还是上面那段代码. 如果我们把内存模型改为acquire和release, 比如:
```C++
-Thread 1-
 x = 1
 y.store(2, memory_order_release);
 z.store(3, memory_order_release);

-Thread 2-
 if (z.load(memory_order_acquire) == 3) {
     assert(x == 1)
     assert(y.load(memory_order_acquire) == 2)
 }
```
原子变量使用的是acquire 和 release, y 和z 又是两个不相关的原子变量, 他们的执行顺序无法保证, 因此线程1中的y.store 和
z.store 两个操作的顺序是无法确定的, 因此线程2 中z = 3 时, y.store 可能还未执行, 所以y 的assert 可能会失败.

而由于x 不是原子变量, 因此x = 1 发生在z.store 之前是在线程1 中是可以保证的, 因此assert(x == 1) 一定成功.

但是, 如果把线程1 中 z.store替换为
```cpp
z.store(y.load(memory_order_acquire) + 1, memory_order_release)
```
在这种情况下,由于z 依赖y, 所以在线程1中, y.store发生在z.store 之前的顺序是可以保证的.

## `memory_order_consume/memory_order_release`
这种模型又比`memory_order_seq_cst`更加宽松, 它不仅移除了不相关的原子变量的顺序一致性, 还移除了不相关普通变量的一致性.
如果把上面代码中的`memory_order_acquire`改为`memory_order_consume`, 那么线程2中的两个assert都有可能失败.

同样的,如果某个原子变量依赖其它的变量(原子或者非原子)的话,那么这两个变量间的顺序一致性是可以保证的.

## `memory_order_relaxed`/`memory_order_relaxed`
这种模型最宽松, 他不对任何顺序做保证, 仅仅保证操作的原子性.
也就意味着,不提供任何跨线程的同步.

也就是一个线程store, 随后另外一线程load, 可能读到的是old value, 因为可能是从寄存器或者cpu cache 中读的, 即使是从内存中读
的, 调用store 的线程写可能读到的是old value, 因为可能是从寄存器或者cpu cache 中读的, 即使是从内存中读的, 调用store 的线
程写的结果也可能还在寄存器或者cpu cache 中而没有刷回内存.

如果某个操作只要求是原子操作,除此之外,不需要其它同步的保障,就可以使用 Relaxed ordering.
程序计数器是一种典型的应用场景.

[理解 C++ 的 Memory Order](http://senlinzhan.github.io/2017/12/04/cpp-memory-order/)
TODO: 计数器那个例子中, 写用的 memory_order_relaxed, 读没指定, 默认应该是 memory_order_seq_cst.
可是 memory_order_relaxed 只保证这个操作是原子的, 可能结果还在寄存器, 或者cpu cache 中, 没法保证写到了内存, 主线程去读,
我理解可能读到的是一个旧的数据, 所以结果可能不是10000.

## spin lock using `std::atomic_flag`
- `std::atomic<T>` guarantees that accesses to the variable will be atomic. It however does not says how the atomicity
  is achieved. It can be using lock-free variable, or using a lock. The actual implementation depends on your target
  architecture and the type T.
- `std::atomic_flag` on the other hand is guaranteed to be implemented using a lock-free technique.

当发生访问资源冲突的时候, 可以有两个选择: 一个是死等, 一个是挂起当前进程, 调度其他进程执行.
spin lock是一种死等的机制, 当前的执行thread会不断的重新尝试直到获取锁进入临界区.

```C++
// atomic_flag as a spinning lock
#include <iostream>       // std::cout
#include <atomic>         // std::atomic_flag
#include <thread>         // std::thread
#include <vector>         // std::vector
#include <sstream>        // std::stringstream

std::atomic_flag lock_stream = ATOMIC_FLAG_INIT;  // init to cleared state
std::stringstream stream;

void append_number(int x) {
  while (lock_stream.test_and_set(std::memory_order_acquire)) {}
  stream << "thread #" << x << '\n';
  lock_stream.clear(std::memory_order_release);
}

int main() {
  std::vector<std::thread> threads;

  for (auto i = 0; i < 10; ++i) {
    threads.push_back(std::thread(append_number, i));
  }

  for (auto&& th : threads) {
    th.join();
  }

  std::cout << stream.str();

  return 0;
}
```
`lock_stream` 被初始化为clear 的状态, 第一个运行到while 语句里面的 `lock_stream.test_and_set` 时候, 返回false, 同时会把
状态更改为set.
这个时候其他线程运行到这里的时候, `lock_stream.test_and_set` 就会返回true, 并一直循环重试, 直到第一个线程将`lock_stream`
的状态重新置为clear.
因此任何时候只有一个线程在调用stream, 所以得到的结果是各个线程分开的, 例如下面这种(顺序可能不一致).
```plain
thread #0
thread #4
...
thread #1
thread #3
```

如果把while 那一行代码注释掉, 多个线程就会同时调用sstream 来输出, 就有可能内容错乱在一起.
```plain
thread #3
threathread #1
thread #0
thread #5
thread #2
 #4
thread #6
thread #7
thread #8
thread #9
```

# [Lock-free vs spin-lock](https://lumian2015.github.io/lockFreeProgramming/lock-free-vs-spin-lock.html)
lock-free version
```cpp
#include <stdio.h>
#include <stdatomic.h>
#include <pthread.h>

int count = 0;

void* adding(void* input) {
  (void)input;
  int val;
  for (auto i = 0; i < 1000000; ++i) {
    do {
      val = count;
    } while(!atomic_compare_exchange_weak(&count, &val, val + 1));
  }
  pthread_exit(NULL);
}

int main() {
  pthread_t tid[10];
  for (auto i = 0; i < 10; ++i) {
    pthread_create(&tid[i], NULL, adding, NULL);
  }
  for (auto i = 0; i < 10; ++i) {
    pthread_join(tid[i], NULL);
  }
  printf("the value of count is %d\n", count);
}
```

spin lock version
```cpp
#include <stdio.h>
#include <stdatomic.h>
#include <pthread.h>

int count = 0;
int lock = 0;  // 0 表示lock 可用, 1 表示被占用

void* adding(void* input){
  (void)input;
  int expected = 0;
  for (int i = 0; i < 1000000; ++i) {
    /* if the lock is 0(unlock), then set it to 1(lock)
     * if the CAS fails, the expected will be set to 1, so we need to change it to 0 again.
     */
    while (!atomic_compare_exchange_weak(&lock, &expected, 1)) {
      expected = 0;
    }
    ++count;  // do work
    lock = 0;  // release the lock
  }
  pthread_exit(NULL);
}

int main() {
  pthread_t tid[10];
  for (int i = 0; i < 10; ++i) {
    pthread_create(&tid[i], NULL, adding, NULL);
  }
  for (int i = 0; i < 10; ++i) {
    pthread_join(tid[i], NULL);
  }
  printf("the value of count is %d\n", count);
}
```
In lock-free program,  while thread 1 is accessing the shared variable count, other threads could also get access to the
count(TODO: race condition 直接读不会有问题吗?) and make progress for the program.
(Although sometimes, there could be some conflicts(just like thread 1), and the thread should redo its work).
However, for the lock-based program, while thread 1 is accessing the shared variable count, it holds the lock and other
threads could not access the count but busy-waiting.

# [ABA - A is not the same as A](http://www.modernescpp.com/index.php/aba-a-is-not-the-same-as-a)
A common problem in concurrency is the so-called ABA problem. That means you read a value twice and each time it returns
the same value A.
Therefore you conclude that nothing changed in between. But you forgot the B.

Let me first use a simple scenario to introduce the problem.

An analogy
The scenario consist of you sitting in a car and waiting that the traffic light becomes green.
Green stands in our case for B, and red for A. What's happening?

You look at the traffic light and it is red (A).
Because you are bored, you begin to check the news on your smart phone and forget the time.
You look once more at the traffic light. Damn, is is still red (A).
Of course, it happened that the traffic light became green (B) between your two checks.
Therefore, what seems to be one red phase were actually two.

[ABA problem](https://lumian2015.github.io/lockFreeProgramming/aba-problem.html)

The problem arises from the C of CAS, where the comparison is value based. That is, as long as the value involved in the
comparison is the same, the swap can proceed.
However, there are still occasions that fool the CAS solution we presented.

[上篇 | 说说无锁(Lock-Free)编程那些事](https://mp.weixin.qq.com/s/T_z2_gsYfs6A-XjVTVV_uQ)

由于通过LL/SC对实现的CAS并不是一个原子性操作,于是,该CAS在执行过程中,可能会被中断.
例如:线程Ⅹ在执行L行后,OS决定将X调度出去,等OS重新调度恢复Ⅹ之后,SC将不再响应,这时CAS将返回false, CAS失败的原因不在数据本身
(数据没变化),而是其他外部事件(线程被中断了).
正是因为如此, C++11标准中添入两个compare exchange 原语-弱的和强的, `compare_exchange_weak`和 `compare_exchange_strong`.
即使当前的变量值等于预期值,这个弱的版本也可能失败. 可见任何weak CAS都能破坏CAS语乂,并返回 false,而它本应返回true.
而Strong CAS会严格遵循CAS语义.

那么, 何种情形下使用 Weak CAS, 何种情形下使用 Strong CAS呢? 通常执行以下原则.
若CAS在循环中(这是一种基本的CA应用模式),循环中不存在成千上万的运算(循环体是轻量级和简单的, 本例的无锁堆栈), 使用
`compare_ exchange_weak`. 否则, 采用强关型的`compare_exchange_strong`.

# x86_64处理器的指针赋值是原子操作吗?
[x86_64处理器的指针赋值是原子操作吗](https://blog.csdn.net/dog250/article/details/103948307)

下面这段摘自Intel的手册

> **guaranteed atomic operations**

> The Intel486 processor (and newer processors since) guarantees that the following basic memory operations will always
> be carried out atomically:

> - Reading or writing a byte
> - Reading or writing a word(2 bytes) aligned on a 16-bit boundary
> - Reading or writing a doulbe word aligned on a 32-bit boundary

> The Pentium processor (and newer processors since) guarantees that the following additional memory operations will
> always be carried out atomically:

> - Reading or writing a quadword aligned on a 64-bit boundary
> - 16-bit accesses to uncached memory locations that fit within a 32-bit data bus.

> The P6 family processors (and newer processors since) guarantee that the following additional memory operation will be
> carried out atomically:

> - Unaligned 16-, 32-, and 64-bit accesses to cached memory that fit within a cache line.

> Accesses to cacheable memory that are split across cache lines and page boundaries are not guaranteed to be atomically
> by the Intel Core 2 Duo, Intel Atom, Intel Core Duo, Pentium M, Pentium 4, Intel Xeon, P6 family, Pentium, and
> Intel486 processors.

也就是说在不跨域cache line 的前提下, 指针赋值是atomic 的.

## talk is cheap, show me the code
考虑一个指针p,两个或者多个线程分别对它进行赋值:
```cpp
long *p;

// thread 1
p = a1;

// thread 2
p = a2
```
结果可以预期吗?如果你笃信指针赋值是原子操作,那么最终结果,p不是a1,便是a2,这是确定的.

然而Intel手册里说,如果指针p跨越了cacheling的边界,便不能保证赋值操作是原子的.
为了复现Intel的说法,从而证明指针赋值并非原子的,只需要给出一个反例,即p既不是a1,也不是a2.

在编码之前,我们先查一下自己实验的机器上的cacheline的大小:
```bash
cat /sys/devices/system/cpu/cpu1/cache/index0/coherency_line_size
getconf LEVEL1_DCACHE_LINESIZE
```
两种方式得到的都是 64.
OK,64字节的cacheline,我们下面就制造一个跨越64字节边界的指针.
[is pointer assignment atomic](https://gist.github.com/ericuni/57301d2f63e6c6716d73d621b319c3e3)

结果能看到既不是a,也不是b 的结果(自己的机器上跑了一晚上却没有看到, 应该和CPU 有关).

这意味着什么?

这意味着不加锁的指针赋值,极其危险!

什么?难道编译器不帮忙?

编译器只能尽量帮忙,哦,对了,还有Linux内核的伙伴系统,slab系统,都只是尽量帮忙,其余的事,只能看造化.谁也不能保证你不会写出上
面类似pack的结构体,因此结构体的字段布局非常重要.

即便这样,你也不能保证结构体对象恰好被载入你希望的位置,只要超过一个cacheline大小的结构体,内部字段的赋值就一定小心再小心:

通常情况下, 内存是对齐的, 所以一般情况下指针赋值是原子的.
但最好还是加锁.


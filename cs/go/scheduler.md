- [scheduler](http://morsmachine.dk/go-scheduler)
- [调度器](https://draveness.me/golang/docs/part3-runtime/ch06-concurrency/golang-goroutine/)

# Why does the Go runtime need with a scheduler?
The POSIX thread API is very much a logical extension to the existing Unix process model and as such, threads get a lot
of the same controls as processes.
Threads have their own signal mask, can be assigned CPU affinity, can be put into cgroups and can be queried for which
resources they use.
**All these controls add overhead** for features that are simply not needed for how Go programs use goroutines and they
quickly add up when you have 100,000 threads in your program.

Another problem is that the **OS can't make informed scheduling decisions, based on the Go model**.
For example, the Go garbage collector requires that all threads are stopped when running a collection and that memory
must be in a consistent state.
This involves waiting for running threads to reach a point where we know that the memory is consistent. When you have
many threads scheduled out at random points, chances are that you're going to have to wait for a lot of them to reach a
consistent state.
The Go scheduler can make the decision of only scheduling at points where it knows that memory is consistent.
This means that when we stop for garbage collection, we only have to wait for the threads that are being actively run on
a CPU core.

Unlike the operating system's thread scheduler, the Go scheduler is not invoked periodically by a hardware timer, but
implicitly by cretain Go language constructs.
For example, when a goroutine calls time.Sleep or blocks in a channel or mutex operation, the scheduler puts it to sleep
and runs another goroutine until it is time to wake the first one up.
Because it does not need a switch to kernel context, **rescheduling a goroutine is much cheaper** that rescheduling a
thread.

# 设计原理
- 单线程调度器 0.x
  - 只包含 40 多行代码
  - 程序中只能存在一个活跃线程,由 G-M 模型组成
- 多线程调度器 1.0
  - 允许运行多线程的程序
  - 全局锁导致竞争严重
- 任务窃取调度器 1.1
  - 引入了处理器 P,构成了目前的 G-M-P 模型
  - 在处理器 P 的基础上实现了基于工作窃取的调度器
  - 在某些情况下,Goroutine 不会让出线程,进而造成饥饿问题
  - 时间过长的垃圾回收(Stop-the-world,STW)会导致程序长时间无法工作
- 抢占式调度器 1.2 ~ 至今
  - 基于协作的抢占式调度器 - 1.2 ~ 1.13
    - 通过编译器在函数调用时插入**抢占检查指令**,在函数调用时检查当前Goroutine是否发起了抢占请求,实现基于协作的抢占式调度
    - Goroutine 可能会因为垃圾回收和循环长时间占用资源导致程序暂停
  - 基于信号的抢占式调度器 - 1.14 ~ 至今
    - 实现**基于信号的真抢占式调度**
    - 垃圾回收在扫描栈时会触发抢占调度
    - 抢占的时间点不够多,还不能覆盖全部的边缘情况

# GMP model
GPM M:N scheduler

- G: The circle represents a goroutine. It includes the stack, the instruction pointer and other information important
  for scheduling goroutines, like any channel it might be blocked on.
- P: The rectangle represents a context for scheduling. You can look at it as a localized version of the scheduler which
  runs Go code on a single thread.
  It's the important part that lets us go from a N:1 scheduler to a M:N scheduler. In the runtime code, it's called P
  for processor.
- M: The triangle represents an OS thread

<img src="./pics/scheduler/gpm.png" alt="GPM model" width="70%"/>

[Go 语言标准库中 atomic.Value 的前世今生](https://blog.betacat.io/post/golang-atomic-value-exploration/)
runtime_procPin: runtime 中的一个函数,禁止调度器对当前goroutine 的抢占(preemption),使得它在执行当前逻辑的时候不被打断,
以便可以尽快地完成工作,因为别人一直在等待它; 同时在禁止抢占期间,GC 线程也无法被启用.

runtime_procUnpin: 对应的反向操作

## G
Goroutine 在Go 语言运行时使用私有结构体 runtime.g 表示.这个私有结构体非常复杂,总共包含40 多个用于表示各种状态的成员变量,
这里也不会介绍所有的字段,仅会挑选其中的一部分
```go
// src/runtime/runtime2.go
type g struct {
 	stack stack  // 当前 Goroutine 的栈内存范围 [stack.lo, stack.hi)

  // 抢占式调度相关
	stackguard0   uintptr
 	preempt       bool  // 抢占信号
	preemptStop   bool  // 抢占时将状态修改成`_Gpreempted`
	preemptShrink bool  // 在同步安全点收缩栈

  // 对应结构的链表
 	_panic *_panic  // 最内侧的panic 结构体
	_defer *_defer  // 最内侧的延迟函数结构体

 	m              *m      // 当前goroutine 占用的os thread, 可能为空
	sched          gobuf   // goroutine 调度相关的数据
	atomicstatus   uint32  // goroutine 的状态
	goid           int64   // goroutine 的ID, 
}
```
goid 对开发者不可见, Go 团队认为引入 ID 会让部分 Goroutine 变得更特殊,从而限制语言的并发能力.

我们需要展开介绍 sched 字段的 runtime.gobuf 结构
```go
type gobuf struct {
	sp   uintptr      // 栈指针
	pc   uintptr      // 程序计数器
	g    guintptr     // 持有 runtime.gobuf 的 goroutine
	ret  sys.Uintreg  // 系统调用的返回值
	...
}
```
这些内容会在调度器保存或者恢复上下文的时候用到,其中的栈指针和程序计数器会用来存储或者恢复寄存器中的值,改变程序即将执行的
代码.

结构体 runtime.g 的 atomicstatus 字段存储了当前 Goroutine 的状态.除了几个已经不被使用的以及与 GC 相关的状态之外,
Goroutine 可能处于以下 9 种状态.

| 状态          | 描述                                                                                      |
| ---           | ---                                                                                       |
| `_Gidle`      | 刚刚被分配并且还没有被初始化                                                              |
| `_Grunnable`  | 没有执行代码,没有栈的所有权,存储在运行队列中                                              |
| `_Grunning`   | 可以执行代码,拥有栈的所有权,被赋予了内核线程 M 和处理器 P                                 |
| `_Gsyscall`   | 正在执行系统调用,拥有栈的所有权,没有执行用户代码,被赋予了内核线程 M 但是不在运行队列上    |
| `_Gwaiting`   | 由于运行时而被阻塞,没有执行用户代码并且不在运行队列上,但是可能存在于 Channel 的等待队列上 |
| `_Gdead`      | 没有被使用,没有执行代码,可能有分配的栈                                                    |
| `_Gcopystack` | 栈正在被拷贝,没有执行代码,不在运行队列上                                                  |
| `_Gpreempted` | 由于抢占而被阻塞,没有执行用户代码并且不在运行队列上,等待唤醒                              |
| `_Gscan`      | GC 正在扫描栈空间,没有执行代码,可以与其他状态同时存在                                     |


上述状态中比较常见是`_Grunnable`,`_Grunning`,`_Gsyscall`,`_Gwaiting` 和 `_Gpreempted` 五个状态

## M
Go 语言并发模型中的 M 是操作系统线程.调度器最多可以创建10000 个线程,但是其中大多数的线程都不会执行用户代码(可能陷入系统
调用),最多只会有 GOMAXPROCS 个活跃线程能够正常运行.

在默认情况下,运行时会将GOMAXPROCS 设置成当前机器的核数,我们也可以在程序中使用 runtime.GOMAXPROCS 来改变最大的活跃线程数.

在大多数情况下,我们都会使用 Go 的默认设置,也就是线程数等于CPU 数,默认的设置不会频繁触发操作系统的线程调度和上下文切换,所
有的调度都会发生在用户态,由 Go 语言调度器触发,能够减少很多额外开销.

Go 语言会使用私有结构体runtime.m 表示操作系统线程
```go
type m struct {
  // 操作系统线程唯一关心的两个 Goroutine
	g0   *g  // 持有调度栈的 Goroutine
	curg *g  // curg 是在当前线程上运行的用户 Goroutine
	...
}
```

<img src="./pics/scheduler/g0_and_g.png" alt="g0_and_g" width="70%"/>

g0 是一个运行时中比较特殊的 Goroutine,它会深度参与运行时的调度过程,包括 Goroutine 的创建,大内存分配和 CGO 函数的执行.
在后面的小节中,我们会经常看到 g0 的身影.

## P
调度器中的处理器P 是线程和 Goroutine 的中间层,它能提供线程需要的上下文环境,也会负责调度线程上的等待队列,通过处理器P 的调
度,每一个内核线程都能够执行多个 Goroutine,它能在 Goroutine 进行一些 I/O 操作时及时让出计算资源,提高线程的利用率.

因为调度器在启动时就会创建GOMAXPROCS 个处理器,所以Go 语言程序的处理器数量一定会等于GOMAXPROCS,这些处理器会绑定到不同的内
核线程上.

runtime.p 是处理器的运行时表示,作为调度器的内部实现,它包含的字段也非常多,其中包括与性能追踪,垃圾回收和计时器相关的字段,
这些字段也非常重要,但是在这里就不展示了,我们主要关注处理器中的线程和运行队列:

```go
type p struct {
  status uint32  // one of pidle/prunning/...

	m      muintptr
  mcache *mcache

  G**	     runq
	runqhead uint32
	runqtail uint32
	runq     [256]guintptr
	runnext  guintptr
}
```
runtime.p 结构体中的状态 status 字段会是以下五种中的一种:

| 状态        | 描述                                                                             |
| ---         | ---                                                                              |
| `_Pidle`    | 处理器没有运行用户代码或者调度器,被空闲队列或者改变其状态的结构持有,运行队列为空 |
| `_Prunning` | 被线程 M 持有,并且正在执行用户代码或者调度器                                     |
| `_Psyscall` | 没有执行用户代码,当前线程陷入系统调用                                            |
| `_Pgcstop`  | 被线程 M 持有,当前处理器由于垃圾回收被停止                                       |
| `_Pdead`    | 当前处理器已经不被使用                                                           |

通过分析处理器 P 的状态,我们能够对处理器的工作过程有一些简单理解,例如处理器在执行用户代码时会处于`_Prunning` 状态,在当前
线程执行 I/O 操作时会陷入 `_Psyscall` 状态.

# 创建 Goroutine
runtime.gfget 通过两种不同的方式获取新的 runtime.g

1. 从 Goroutine 所在处理器的 `_p_.gFree` 列表或者调度器的 `sched.gFree` 列表中获取 runtime.g,
1. 调用 runtime.malg 生成一个新的 runtime.g 并将结构体**追加到全局的 Goroutine 列表 allgs 中**.

先从处理器的gFree 列表中查找空闲的Goroutine,如果不存在空闲的Goroutine,会通过 runtime.malg 创建一个栈大小足够的新结构体.

处理器本地的运行队列是一个使用数组构成的环形链表,它最多可以存储 256 个待执行任务.

<img src="./pics/scheduler/global_and_local_runnable_queue.png" alt="runq" width="70%"/>

# 调度循环
<img src="./pics/scheduler/scheduler_loop.png" alt="scheduler_loop" width="70%"/>

这里介绍的是 Goroutine 正常执行并退出的逻辑,实际情况会复杂得多,多数情况下 Goroutine 在执行的过程中都会经历协作式或者抢占
式调度,它会让出线程的使用权等待调度器的唤醒.

# 触发调度
这里简单介绍下所有触发调度的时间点,因为调度器的 runtime.schedule 会重新选择 Goroutine 在线程上执行,所以我们只要找到该函
数的调用方就能找到所有触发调度的时间点,经过分析和整理,我们能得到如下的树形结构:

<img src="./pics/scheduler/schedule-points.png" alt="scheduler points" width="70%"/>

除了上图中可能触发调度的时间点,运行时还会在线程启动 runtime.mstart 和 Goroutine 执行结束 runtime.goexit0 触发调度.
我们在这里会重点介绍运行时触发调度的几个路径:

- 主动挂起 — `runtime.gopark` -> `runtime.park_m`
- 系统调用 — `runtime.exitsyscall` -> `runtime.exitsyscall0`
- 协作式调度 — `runtime.Gosched` -> `runtime.gosched_m` -> `runtime.goschedImpl`
- 系统监控 — `runtime.sysmon` -> `runtime.retake` -> `runtime.preemptone`

我们在这里介绍的调度时间点不是将线程的运行权直接交给其他任务,而是通过调度器的 runtime.schedule 重新调度.

## 主动挂起
runtime.gopark 是触发调度最常见的方法,该函数会将当前 Goroutine 暂停,被暂停的任务不会放回运行队列.

`runtime.park_m` 会将当前 Goroutine 的状态从 `_Grunning` 切换至 `_Gwaiting`,调用 runtime.dropg 移除线程和 Goroutine 之间
的关联,在这之后就可以调用 runtime.schedule 触发新一轮的调度了.

当 Goroutine 等待的特定条件满足后,运行时会调用 runtime.goready 将因为调用 runtime.gopark 而陷入休眠的 Goroutine 唤醒.

runtime.ready 会将准备就绪的 Goroutine 的状态切换至 `_Grunnable` 并将其加入处理器的运行队列中,等待调度器的调度.

## 系统调用
**Who you gonna (sys)call?**

You might wonder now, why have contexts at all? Can't we just put the runqueues on the threads and get rid of contexts?
Not really.
The reason we have contexts is so that we can hand them off to other threads if the running thread needs to block for
some reason.

An example of when we need to block, is when we call into a syscall.
Since a thread cannot both be executing code and be blocked on a syscall, we need to hand off the context so it can keep
scheduling.

<img src="./pics/syscall.jpg" alt="block on a syscall" width="50%"/>

Here we see a thread giving up its context so that another thread can run it. The scheduler makes sure there are enough
threads to run all contexts.
M1 in the illustration above might be created just for the purpose of handling this syscall or it could come from a
thread cache.
The syscalling thread will hold on to the goroutine that made the syscall since it's technically still executing, albeit
blocked in the OS.

When the syscall returns, the thread must try and get a context in order to run the returning goroutine.
The normal mode of operation is to steal a context from one of the other threads. If it can't steal one, it will put the
goroutine on a global runqueue, put itself on the thread cache and go to sleep.

This handling of syscalls is why Go programs run with multiple threads, even when GOMAXPROCS is 1. The runtime uses
goroutines that call syscalls, leaving threads behind.

Goroutine 在 system call 和 channel call 时都可能发生阻塞,但这两种阻塞发生后,处理方式又不一样的.

- 当程序发生 system call,M 会发生阻塞,同时唤起(或创建)一个新的 M 继续执行其他的 G.
  If the Go code requires the M to block, for instance by invoking a system call, then another M will be woken up from
  the global queue of idle M's.
  This is done to ensure that goroutines, still capable of running, are not blocked from running by the lack of an
  available M.[11]
- 当程序发起一个 channel call,程序可能会阻塞,但不会阻塞 M,G 的状态会设置为 waiting,M 继续执行其他的 G.当 G 的调用完成,会
  有一个可用的 M 继续执行它.
  If a goroutine makes a channel call, it may need to block, but there is no reason that the M running that G should be
  forced to block as well.
  In a case such as this, the G's status is set to waiting and the M that was previously running it continues running
  other G's until the channel communication is complete.
  At that point the G's status is set back to runnable and will be run as soon as there is an M capable of running it.

Goroutines that are sleeping or blocked in a communication do not need a thread at all.
Goroutines that are blocked in IO or other system calles or are calling non-Go functions, do need an OS thread, but
GOMAXPROCS need not account for them.

## 协作式调度
我们在设计原理中介绍过了 Go 语言基于协作式和信号的两种抢占式调度,这里主要介绍其中的协作式调度.

runtime.Gosched 函数会主动让出处理器,允许其他 Goroutine 运行.该函数无法挂起 Goroutine,调度器可能会将当前 Goroutine 调度
到其他线程上.

# 线程管理
Go 语言的运行时会通过调度器改变线程的所有权,它也提供了 runtime.LockOSThread 和 runtime.UnlockOSThread 让我们有能力绑定
Goroutine 和线程完成一些比较特殊的操作.


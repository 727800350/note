[scheduler](http://morsmachine.dk/go-scheduler)

Why does the Go runtime need with a scheduler?

The POSIX thread API is very much a logical extension to the existing Unix process model and as such, threads get a lot
of the same controls as processes.
Threads have their own signal mask, can be assigned CPU affinity, can be put into cgroups and can be queried for which
resources they use.
All these controls add overhead for features that are simply not needed for how Go programs use goroutines and they
quickly add up when you have 100,000 threads in your program.

Another problem is that the OS can't make informed scheduling decisions, based on the Go model.
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
Because it does not need a switch to kernel context, rescheduling a goroutine is much cheaper that rescheduling a thread.

GPM M:N scheduler

- G: The circle represents a goroutine. It includes the stack, the instruction pointer and other information important
  for scheduling goroutines, like any channel it might be blocked on.
- P: The rectangle represents a context for scheduling. You can look at it as a localized version of the scheduler which
  runs Go code on a single thread.
  It's the important part that lets us go from a N:1 scheduler to a M:N scheduler. In the runtime code, it's called P
  for processor.
- M: The triangle represents an OS thread

<img src="./pics/GPM.jpg" alt="GPM model" width="40%"/>

Here we see 2 threads (M), each holding a context (P), each running a goroutine (G).
In order to run goroutines, a thread must hold a context.

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

The global runqueue is a runqueue that contexts pull from when they run out of their local runqueue.
Contexts also periodically check the global runqueue for goroutines. Otherwise the goroutines on global runqueue could
end up never running because of starvation.

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

[Go 语言标准库中 atomic.Value 的前世今生](https://blog.betacat.io/post/golang-atomic-value-exploration/)
runtime_procPin: runtime 中的一个函数,禁止调度器对当前goroutine 的抢占(preemption),使得它在执行当前逻辑的时候不被打断,
以便可以尽快地完成工作,因为别人一直在等待它; 同时在禁止抢占期间,GC 线程也无法被启用.

runtime_procUnpin: 对应的反向操作


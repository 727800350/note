- [go debug](#go-debug)
- [pprof](#pprof)
  - [go test](#go-test)
  - [intro](#intro)
  - [CPU profiling](#cpu-profiling)
  - [Memory profiling](#memory-profiling)
  - [Block profiling](#block-profiling)
    - [Note that not all blocking is bad](#note-that-not-all-blocking-is-bad)
    - [common suggestions that can help to reduce goroutine blocking](#common-suggestions-that-can-help-to-reduce-goroutine-blocking)
      - [using copy-on-write technique](#using-copy-on-write-technique)
      - [batch update](#batch-update)
      - [sync.Pool](#syncpool)
  - [Microbenchmarks](#microbenchmarks)
  - [Profiling whole programs](#profiling-whole-programs)
- [Framepointers](#framepointers)
- [Flame graph (cont.)](#flame-graph-cont)

# go debug
The Go runtime collects various statistics during the life of your program.
These stats are always collected, but normally supressed, you can enable their display by setting the GODEBUG environment variable.
As a garbage collected language, the performance of Go programs is often determined by their interaction with the garbage collector.

A simple way to obtain a general idea of how hard the garbage collector is working is to enable the output of GC logging.
```bash
env GODEBUG=gctrace=1 godoc -http=:8080

GODEBUG=gctrace=1 ./myserver
```

# pprof
## go test
go test tool has a build-in support for several kinds of profiling
- `go test -cpuprofile=cpu.out`
- `go test -blockprofile=block.out`
- `go test -memprofile=mem.out`

在生成profile 结果的同时, 还会生成一个${pkg}.test, 之后就可以用 `go tool pprof ${pkg}.test mem.out` 来看profile 结果.

## intro
- [youtube Golang UK Conference 2016 - Dave Cheney - Seven ways to Profile Go Applications](https://www.youtube.com/watch?v=zzAdEt3xZ1M)
- [Seven ways to profile a Go program](https://talks.godoc.org/github.com/davecheney/presentations/seven.slide)
- [Debugging performance issues in Go programs](https://software.intel.com/en-us/blogs/2014/05/10/debugging-performance-issues-in-go-programs)

A profiler runs your program and configures the operating system to interrupt it at regular intervals.
This is done by sending SIGPROF to the program being profiled, which suspends and transfers execution to the profiler.
The profiler then grabs the program counter for each executing thread and restarts the program.

pprof descends from the Google Performance Tools suite.
pprof profiling is built into the Go runtime.
It comes in two parts:

- runtime/pprof package built into every Go program
- go tool pprof for investigating profiles.

```bash
go tool pprof /path/to/your/binary /path/to/your/profile
```

This is a sample cpu profile:
```bash
% go tool pprof $BINARY /tmp/c.p
Entering interactive mode (type "help" for commands)
(pprof) top
Showing top 15 nodes out of 63 (cum >= 4.85s)
      flat  flat%   sum%        cum   cum%
    21.89s  9.84%  9.84%    128.32s 57.71%  net.(*netFD).Read
    17.58s  7.91% 17.75%     40.28s 18.11%  runtime.exitsyscall
    15.79s  7.10% 24.85%     15.79s  7.10%  runtime.newdefer
    12.96s  5.83% 30.68%    151.41s 68.09%  test_frame/connection.(*ServerConn).readBytes
    11.27s  5.07% 35.75%     23.35s 10.50%  runtime.reentersyscall
    10.45s  4.70% 40.45%     82.77s 37.22%  syscall.Syscall
     9.38s  4.22% 44.67%      9.38s  4.22%  runtime.deferproc_m
     9.17s  4.12% 48.79%     12.73s  5.72%  exitsyscallfast
     8.03s  3.61% 52.40%     11.86s  5.33%  runtime.casgstatus
     7.66s  3.44% 55.85%      7.66s  3.44%  runtime.cas
     7.59s  3.41% 59.26%      7.59s  3.41%  runtime.onM
     6.42s  2.89% 62.15%    134.74s 60.60%  net.(*conn).Read
     6.31s  2.84% 64.98%      6.31s  2.84%  runtime.writebarrierptr
     6.26s  2.82% 67.80%     32.09s 14.43%  runtime.entersyscall
```

## CPU profiling
CPU profiling is the most common type of profile.
When CPU profiling is enabled, the runtime will interrupt itself every 10ms and record the stack trace of the currently running goroutines.
Once the profile is saved to disk, we can analyse it to determine the hottest code paths.
The more times a function appears in the profile, the more time that code path is taking as a percentage of the total runtime.

## Memory profiling
Memory profiling records the stack trace **when a heap allocation is made**.
Memory profiling, like CPU profiling is sample based. By default memory profiling samples 1 in every 1000 allocations. This rate can be changed.

Stack allocations are assumed to be free and are not tracked in the memory profile.
Because of memory profiling is sample based and because it tracks allocations not use, using memory profiling to determine your application's overall memory usage is difficult.

You can change the sampling rate with go test --memprofilerate flag, or by setting runtime.MemProfileRate variable at program startup.

You can also visualize number of bytes allocated or number of objects allocated (`--inuse/alloc_space` and `--inuse/alloc_objects` flags, respectively).
The profiler tends to sample larger objects during profiling more.
But it's important to understand that large objects affect memory consumption and GC time, while large number of tiny allocations affects execution speed (and GC time to some degree as well).
So it may be useful to look at both.

## Block profiling
Block profiling is quite unique.
A block profile is similar to a CPU profile, but it records the amount of time a goroutine spent waiting for a shared resource.
This can be useful for determining concurrency bottlenecks in your application.

Block profiling can show you when a large number of goroutines could make progress, but were blocked. Blocking includes:

- Sending or receiving on a unbuffered channel.
- Sending to a full channel, receiving from an empty one.
- Trying to Lock a sync.Mutex that is locked by another goroutine.

Block profiling is a very specialised tool, it should not be used until you believe you have eliminated all your CPU and memory usage bottlenecks.

the blocking profiler is not enabled by default. 'go test --blockprofile' will enable it for you automatically.
However, if you use net/http/pprof or runtime/pprof, you need to enable it manually (otherwise the profile will be empty).
To enable the blocking profiler call runtime.SetBlockProfileRate. SetBlockProfileRate controls the fraction of goroutine blocking events that are reported in the blocking profile.
The profiler aims to sample an average of one blocking event per the specified amount of nanoseconds spent blocked. To include every blocking event in the profile, set the rate to 1.

If a function contains several blocking operations and it's not obvious which one leads to blocking, use --lines flag to pprof.

### Note that not all blocking is bad
When a goroutine blocks, the underlying worker thread simply switches to another goroutine.
So blocking in the cooperative Go environment is very different from blocking on a mutex in a non-cooperative systems
(e.g. typical C++ or Java threading libraries, where blocking leads to thread idling and expensive thread context switches). To give you some feeling, let's consider some examples.

- Blocking on a time.Ticker is usually OK. If a goroutine blocks on a Ticker for 10 seconds, you will see 10 seconds of blocking in the profile, which is perfectly fine.
- Blocking on sync.WaitGroup is frequently OK. For example, if a task takes 10 seconds, the goroutine waiting on a WaitGroup for completion will account for 10 seconds of blocking in the profile.
- Blocking on sync.Cond may or may not be OK, depending on the situation.
  - Consumer blocking on a channel suggests slow producers or lack of work.
  - Producer blocking on a channel suggests that consumers are slower, but this is frequently OK.
- Blocking on a channel-based semaphore shows how much goroutines are gated on the semaphore.
- Blocking on a sync.Mutex or sync.RWMutex is usually bad.

You can use --ignore flag to pprof to exclude known uninteresting blocking from a profile during visualization.

### common suggestions that can help to reduce goroutine blocking
#### using copy-on-write technique
In some cases it's possible to remove mutexes entirely by using copy-on-write technique.
If the protected data structure is modified infrequently and it's feasible to make copies of it, then it can be updated as follows:
```go
type Config struct {
  Routes   map[string]net.Addr
  Backends []net.Addr
}

var config unsafe.Pointer  // actual type is *Config

// Worker goroutines use this function to obtain the current config.
func CurrentConfig() *Config {
  return (*Config)(atomic.LoadPointer(&config))
}

// Background goroutine periodically creates a new Config object
// as sets it as current using this function.
func UpdateConfig(cfg *Config) {
  atomic.StorePointer(&config, unsafe.Pointer(cfg))
}
```
This pattern prevents the writer from blocking readers during update.

#### batch update
Local caching and batching of updates can help to reduce contention on un-partitionable data structures. Below you can see how to batch sends to a channel
```go
const CacheSize = 16

type Cache struct {
  buf [CacheSize]int
  pos int
}

func Send(c chan [CacheSize]int, cache *Cache, value int) {
  cache.buf[cache.pos] = value
  cache.pos++
  if cache.pos == CacheSize {
    c <- cache.buf
    cache.pos = 0
  }
}
```
This technique is not limited to channels. It can be used to batch updates to a map, batch allocations, etc.

#### sync.Pool
Use sync.Pool for freelists instead of chan-based or mutex-protected freelists. sync.Pool uses smart techniques internally to reduce blocking.

## Microbenchmarks
The easiest way to profile a function is with the testing package.
The testing package has built in support for generating CPU, memory, and block profiles.

- -cpuprofile=$FILE writes a CPU profile to $FILE.
- -memprofile=$FILE, writes a memory profile to $FILE, -memprofilerate=N adjusts the profile rate to 1/N.
- -blockprofile=$FILE writes a block profile to $FILE.

Using any of these flags also preserves the binary.
```bash
% go test -run=XXX -bench=IndexByte -cpuprofile=/tmp/c.p bytes
% go tool pprof bytes.test /tmp/c.p
```
Note: use -run=XXX to disable tests, you only want to profile benchmarks.

## Profiling whole programs
testing benchmarks is useful for microbenchmarks, but what if you want to profile a complete application?
To profile an application, you could use the runtime/pprof package, but that is fiddly and low level.
A few years ago I wrote a small package, github.com/pkg/profile, to make it easier to profile an application.
```go
import "github.com/pkg/profile"

func main() {
  defer profile.Start().Stop()
  ...
}
```

/debug/pprof
If your program runs a webserver you can enable debugging over http.
```go
import _ "net/http/pprof"
// init函数会启动一个异步协程采集该进程实例的资源占用情况,并以http服务接口方式提供给用户查询

func main() {
  log.Println(http.ListenAndServe("localhost:3999", nil))
}
```
Then use the pprof tool to look at

- a 30-second CPU profile: go tool pprof http://localhost:3999/debug/pprof/profile
- the heap profile: go tool pprof http://localhost:3999/debug/pprof/heap
- the goroutine blocking profile: go tool pprof http://localhost:3999/debug/pprof/block

或者直接通过网页看: http://localhost:3999/debug/pprof

# Framepointers
Go 1.7 has been released and along with a new compiler for amd64, the compiler now enables frame pointers by default.

The frame pointer is a register that always points to the top of the current stack frame.
Framepointers enable tools like gdb(1), and perf(1) to understand the Go call stack.

```bash
% go build -toolexec="perf record -g -o /tmp/p" cmd/compile/internal/gc
% perf report -i /tmp/p
```
我自己执行没有成功

# Flame graph (cont.)
Flame graphs can consume data from many sources, including pprof (and perf(1)).
Uber have open sourced a tool call go-torch which automates the process, assuming you have the /debug/pprof endpoint, or you can feed it an existing profile.
```bash
% go build -gcflags=-cpuprofile=/tmp/c.p .
% go-torch $(go tool compile -n) /tmp/c.p
INFO[16:00:09] Run pprof command: go tool pprof -raw -seconds 30 /Users/dfc/go/pkg/tool/darwin_amd64/compile /tmp/c.p
INFO[16:00:09] Writing svg to torch.svg
```


并发是指程序的逻辑结构, 并行是指程序的运行状态.

Concurrency is not parallelism, althouth it enables parallelism.

If you have only one processor, your program can still be concurrent but it cannot be parallel.

On the other hand, a well-written concurrent program might run efficiently in parallel on a multiprocessor.

Usually

- I/O Bound == Concurrency
- CPU Bound == Parallelism

# mutex
**Mutex locks are not re-entrant** - it's not possible to lock a mutex that's already locked.

自旋锁
```go
type SpinLock int32

func (p *SpinLock) Lock() {
  for !atomic.CompareAndSwapInt32((*int32)(p), 0, 1) {
  }
}

func ( *SpinLock) UnLock() {
  atomic.StoreInt32((*int32)(p), 0)
}
```
上面的Lock 函数实现在 go version >= 1.14 时是ok 的, 低版本就会踩坑.
因为 go 在 1.14 之前没有实现抢占式调度, 会发生死锁.

所以在 go 1.14 之前的版本可以这样实现
```go
func (p *SpinLock) Lock() {
  for !atomic.CompareAndSwapInt32((*int32)(p), 0, 1) {
    runtime.Gosched()  // 主动释放CPU
  }
}
```
但是这样也有一个弊端就是每次for 循环都额外引入了调度器的开销.

sync.Mutex 效率优先, 兼顾公平.
有两种模式, 效率模式和饥饿模式.

效率: 正常模式.

- 等待队列: 先进先出
- 新手优势: 先抢再排. 因为新手在还未进入队列时, 是已经拥有cpu 资源的, 更有可能加锁成功, 免去了后续的调度开销. 还可以充分
  利用缓存.
- 等待超 1ms -> 饥饿模式

公平: 饥饿模式

- 严格排队, 队首接盘
- 牺牲效率, 保p99
- 适时回归正常模式

# data race
When we cannot confidently say that one event happends before the other, then the events x and y are concurrent.

A data race occurs whenever two goroutines access the same variable concurrently and at least one of the accesses is a
write.

## data race detector
Just add the -race flag to your go build, go run or go test command.
This causes the compiler to build a modified version of your application or test with additional instrumentation that
effectively records all accesses to shared variables that occurred during execution, along with the identity of the
goroutine that read or write the variable.
In addition, the modified program records all synchronization events, such as go statements, channel operations and
calls to `(*sync.Mutex).Lock, (*sync.WaitGroup).Wait` and so on.

The race detector studies this stream of events, looking for cases in which one goroutine reads or writes a shared
variable that was most recently written by a different goroutine without an intervening synchronization operation.
This indicates a concurrent access to the shared variable, and thus a data race.

# Concurrent Non-Blocking Cache
```go
// Package memo provides a concurrency-safe memoization.
// Requests for different keys proceed in parallel. Concurrent requests for the same key block until the first completes.
// This implementation uses a Mutex.
package memo

import "sync"

// Func is the type of the function to memoize.
type Func func(string) (interface{}, error)

type result struct {
  value interface{}
  err   error
}

type entry struct {
  res   result
  ready chan struct{} // closed when res is ready
}

func New(f Func) *Memo {
  return &Memo{f: f, cache: make(map[string]*entry)}
}

type Memo struct {
  f     Func
  mu    sync.Mutex // guards cache
  cache map[string]*entry
}

func (memo *Memo) Get(key string) (value interface{}, err error) {
  memo.mu.Lock()
  e, ok := memo.cache[key]
  if !ok {
    // This is the first request for this key.
    // This goroutine becomes responsible for computing the value and broadcasting the ready condition.
    // put a placeholder first, so other requests could know they only need to wait the first request to complete
    e = &entry{ready: make(chan struct{})}
    memo.cache[key] = e
    memo.mu.Unlock()

    e.res.value, e.res.err = memo.f(key)
    close(e.ready) // broadcast ready condition
  } else {
    // This is a repeat request for this key.
    memo.mu.Unlock()
    <-e.ready // wait for ready condition
  }
  return e.res.value, e.res.err
}
```
The implementation of Memo above uses a mutex to guard a map variable that is shared by each goroutine that calls Get.

```go
// A request is a message requesting that the Func be applied to key.
type request struct {
  key      string
  response chan<- result // the client wants a single result
}

type Memo struct {
  requests chan request
}

// New returns a memoization of f.  Clients must subsequently call Close.
func New(f Func) *Memo {
  memo := &Memo{requests: make(chan request)}
  go memo.server(f)
  return memo
}

func (memo *Memo) Get(key string) (interface{}, error) {
  response := make(chan result)
  memo.requests <- request{key, response}
  res := <-response
  return res.value, res.err
}

func (memo *Memo) Close() {
  close(memo.requests)
}

func (memo *Memo) server(f Func) {
  cache := make(map[string]*entry)
  for req := range memo.requests {
    e, ok := cache[req.key]
    if !ok {
      // This is the first request for this key.
      e = &entry{ready: make(chan struct{})}
      cache[req.key] = e
      go e.call(f, req.key) // call f(key)
    }
    go e.deliver(req.response)
  }
}

func (e *entry) call(f Func, key string) {
  // Evaluate the function.
  e.res.value, e.res.err = f(key)
  // Broadcast the ready condition.
  close(e.ready)
}

func (e *entry) deliver(response chan<- result) {
  // Wait for the ready condition.
  <-e.ready
  // Send the result to the client.
  response <- e.res
}
```
The call and deliver methods must be called in their own goroutines to ensure that the monitor goroutine does not stop
processing new requests.

# sync.Pool
保存和复用临时对象,减少内存分配,降低 GC 压力.

sync/pool.go
```go
// Local per-P Pool appendix.
type poolLocalInternal struct {
  private interface{}   // Can be used only by the respective P.
  shared  []interface{} // Can be used by any P.
  Mutex                 // Protects shared.
}
```

Get:

1. 先从本P绑定的poolLocal获取对象:先从本poolLocal的private池获取对象,再从本poolLocal的shared池获取对象
1. 上一步没有成功获取对象,再从其他P的shared池获取对象
1. 上一步没有成功获取对象,则从Heap申请对象

Put

1. 如果poolLocalInternal的private为空,则将回收的对象放到private池中
1. 如果poolLocalInternal的private非空,则将回收的对象放到shared池中


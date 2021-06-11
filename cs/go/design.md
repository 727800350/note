# runtime
Go does have an extensive library, called the runtime, that is part of every Go program.
The runtime library implements garbage collection, concurrency, stack management, and other critical features of the Go
language.
Although it is more central to the language, Go's runtime is analogous to libc, the C library.

It is important to understand, however, that Go's runtime does not include a virtual machine, such as is provided by the
Java runtime.
Go programs are compiled ahead of time to native machine code.
Thus, although the term is often used to describe the virtual environment in which a program runs, in Go the word
"runtime" is just the name given to the library providing critical language services.

# goroutine
goroutine 是由 Go 运行时环境管理的轻量级线程.
the functionality is analogous to the & on the end of a shell command

- `runtime.GOMAXPROCS(num int)`: 控制并发度, -1 会用到所有的core
- `go run -race file.go`: dead lock 检测

What is a goroutine? It's an independently executing function, launched by a go statement.

It has its own call stack, which grows and shrinks as required.

It's very cheap. It's practical to have thousands, even hundreds of thousands of goroutines.

To make the stacks small, Go's run-time uses resizable, bounded stacks.
A newly minted goroutine is given a few kilobytes, which is almost always enough.
When it isn't, the run-time grows (and shrinks) the memory for storing the stack automatically, allowing many goroutines
to live in a modest amount of memory.

It's not a thread.

There might be only one thread in a program with thousands of goroutines.
Instread, goroutines are multiplexed dynamically onto threads as needed to keep all the goroutines running.
But if you think of it as a very cheap thread, you won't be far off.

# Garbage Collection
[Garbage Collection Semantics - GopherCon SG 2019](https://www.youtube.com/watch?v=q4HoWwdZUHs)

Responsibility

- tracking memory allocation in heap memory
- releasing allocations that are no longer needed
- keeping allocation that are still in-use

As of version 1.12, Go uses a **non-generational, concurrent, tri-color, mark and sweep collector**.

If the machine is a multiprocessor, the collector runs on a separate CPU core in parallel with the main program.

Three phases of the gargage collector

1. mark setup, STW(stop the world): your application is not getting done at all
  - Turn on the Write Barrier. In order to turn the Write Barrier on, every application goroutine runing must be stopped
    (all goroutines must find themself in a safe point, and right now, that safe point is only going to occur when we
    are inside of a function call, so now we are waiting function calls to happend).
1. marking, concurrent: we'll be able to work at the same time that marking is going on
  - Inspect the stacks to find root pointers to the heap
  - Traverse the heap graph form those root pointers
  - Mark values on the heap that are still in-use
1. mark termination, STW
  - Turn the Write Barrier off
  - Various clean up tasks are performed
  - Next collection goal is calculated

Sweeping - Freeing heap memory

- Occurs when Goroutines attemp to allocate new heap memory
- The latency os Sweeping is added to the cost of performing an allocation, not GC.

# compile
## dependencies in Go and compilation speed
```go
import "B"
```
when B is compiled, the generated object file includes type information for all dependencies of B that affect the public
interface of B.

This design has the important effect that when the compiler executes an import clause, it opens exactly one file, the
object file identified by the string in the import clause.

To make compilation even more efficient, the object file is arranged so the export data is the first thing in the file,
so the compiler can stop reading as soon as it reaches the end of that section.

## compiler implementation
There are three Go compiler implementations supported by the Go team.

- gc, the default compiler
- gccgo, which uses the GCC back end
- and a somewhat less mature gollvm, which uses the LLVM infrastructure.

Gc uses a different calling convention and linker from C and therefore cannot be called directly from C programs, or
vice versa.
The cgo program provides the mechanism for a "foreign function interface" to allow safe calling of C libraries from Go
code. SWIG extends this capability to C++ libraries.


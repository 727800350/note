# openMP
## intro
OpenMP API consists of:

- Compiler Directives
- Runtime subroutines/functions
- Environment variables

```c
#include <stdio.h>
#include "omp.h"

int main(){
	#pragma omp parallel
	{
		printf("Hello World\n");
	}

	return 0;
}

g++ -fopenmp hello.c
export OMP_NUM_THREADS=4 ## 默认的是两个, 也可以再
./a.out
```

C/C++ 的格式
```
#pragma omp parallel [clauses]
{
	...
}
```

OpenMP follows the fork/join model:

1. OpenMP programs start with a single thread; the master thread (Thread #0)
1. At start of parallel region master creates team of parallel "worker" threads (FORK) Statements in parallel block are executed in parallel by every thread
1. At end of parallel region, all threads synchronize, and join master thread (JOIN)

## API
### function
- omp_set_num_threads(n): set number of openMP threads
- omp_get_num_threads() Returns number of threads in parallel region, Returns 1 if called outside parallel region
- omp_get_thread_num() Returns id(value in [0, n - 1]) of thread, Master thread always has id 0
- double omp_get_wtime();  Time in Seconds since a fixed point in the past 
- omp_num_procs(): number of processors in the system, ex: `omp_set_num_threads(omp_num_procs())`

### env var
- `export OMP_NUM_THREADS=n`
- `export OMP_STACKSIZE=512M`

### [Shared and Private Variables](../demo/parallel/openmp/shared_private_var.cpp)
- SHARED(list): All variables in list will be considered shared.
- PRIVATE(list): Every openmp thread will have its own private copy of variables in list, ex: `#pragma omp parallel private(a, b, c)`

By default most variables are considered shared in OpenMP.
Exceptions include index variables (Fortran, C/C++) and variables declared inside parallel region (C/C++)

### synchronization
OpenMP programs use shared variables to communicate.
We need to make sure these variables are not accessed at the same time by different threads (will cause race conditions).
OpenMP provides a number of directives for synchronization.

- `#pragma omp do nowait { }`: For situations where we do not need or want to synchronize at the end OpenMP provides the NOWAIT clause
- !$OMP SINGLE: Only one thread(not neccesarily master) executes the block, other threads will wait(can remove the implicit barrier with a nowait clause),
	useful for thread-unsafe code and I/O operations.

	```c
	#pragma omp parallel
	{
		do_many_things();
		#pragma omp single
		{ exchange_boundaries(); }
		do_many_other_things();
	}
	```
- !$OMP MASTER: only the master threads excecutes instructions in the block, other threads do not need to wait master to finish, just skip it.

	```c
	#pragma omp parallel
	{
		do_many_things();
		#pragma omp master
		{ exchange_boundaries(); }
		#pragma omp barrier
		do_many_other_things();
	}
	```
- `#pragma opm critical`: 在任何时候只能有一个thread 运行, 这块代码

	```c
	#pragma omp critical
	consume(B, res); // Threads wait their turn – only one at a only one at a time calls time calls consume()}
	```
- `#pragma opm atomic`: only used for the update of a memory location. Sometimes !$OMP ATOMIC is also refered to as a mini critical section.

	```c
	#pragma omp atomic
	X += tmp; // Atomic only protects the read/update of X
	```
- !$OMP BARRIER: enforce every thread to wait at the barrier until all threads have reached the barrier

	```c
	#pragma omp parallel shared(A, B, C) private(id)
	{
		id = omp_get_thread_num();
		A[id] = big_calc1(id);
		#pragma omp barrier // explicit barrier
		#pragma omp for
		for(i = 0; i < N; i++){ C[i] = big_calc3(i, A); } // implicit barrier at the end of a for worksharing construct
		#pragma omp for nowait
		for(i = 0; i < N; i++){ B[i] = big_calc2(i, C); } // no implicit barrier due to nowait
		A[id] = big_calc4(id);

	}
	```

### IF
OpenMP provides another useful clause to decide at run time if a parallel region should actually be run in parallel (multiple threads) or just by the master thread:

- `$!OMP PARALLEL IF(n > 100000)` (fortran)
- `#pragma omp parallel if (n>100000)` (C/C++)

This will only run the parallel region when `n > 100000`

Math libraries have very specialized and optimized version of many functions and many of them have been parallelized using OpenMP.
On EOS we have the [Intel Math Kernel Library (MKL)](http://sc.tamu.edu/help/eos/mathlib.php)

## 使用场景
### [map 式的任务分割](../demo/parallel/openmp/map.cpp)
比如要对一个10000 个元素的数组进行map 操作
我们可以手动进行分割, 假设我们有4 个线程, 那么一个线程操作2500 数字
也可以使用omp 针对for的自动分割

loop index i 默认为private
```c
#pragma omp parallel
{
	#pragma omp for
	for(i = 0; i < 100; ++i){
		A(I) = A(I) + B
	}
}
```
or
```c
#pragma omp parallel for
for(i = 0; i < 100; ++i){
	A(I) = A(I) + B
}
```

Although the OpenMP standard does not specify how a loop should be partitioned.
most compilers split the loop in N/p (N #iterations, p #threads) chunks by default. This is called a static schedule (with chunk size N/p)

Dynamic Schedule

- $!OMP DO SCHEDULE(DYNAMIC, n) n is chunk size(对于iterations 的数量来说的),
	Loop iterations are divided into pieces of size chunk. When a thread finishes one chunk, it is dynamically assigned another.
- $!OMP DO SCHEDULE(GUIDED, n) Similar to DYNAMIC but chunk size is relative to number of iterations left.

### [reduction](../demo/parallel/openmp/reduce.cpp)
比如对一个数组求和
```c
for (int i=0; i<10; ++i){
	a = a op expr
}
```
OpenMP provides the `REDUCTION(op:list)` clause

This clause can be applied when the following restrictions are met:

1. a is a scalar variable in the list
1. expr is a scalar expression that does not reference a Only certain kind of op allowed; e.g. +,*,-
1. For fortran op can also be intrinsic; e.g. MAX,MIN,IOR
1. Vars in list have to be shared

# MPI
## intro
Message Passing Interface 
MPI是一种消息传递编程模型,并成为这种编程模型的代表.事实上,标准MPI虽然 很庞大,但是它的最终目的是服务于进程间通信这一目标的 
MPICH是影响最大,用户最多的MPI实现

```c
#include <mpi.h>
#include <stdio.h>

int main(int argc, char ** argv){
    int rank, size;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    printf("Hello World from thread %d of %d\n", rank, size);

    MPI_Finalize();
    return 0;
}

mpicc -o hello hello.c
mpiexec –n 16 ./hello (running with 16 processes)
```

## data type
MPI Datatype is very similar to a C or Fortran datatype

- int  MPI_INT
- double MPI_DOUBLE
- char MPI_CHAR

More complex datatypes are also possible:
E.g. you can create a structure datatype that comprises of other datatypes a char, an int and a double.
Or, a vector datatype for the columns of a matrix

## API
- MPI_Init 用来初始化MPI执行环境,建立多个MPI进程之间的联系,为后续通信做准备.
- MPI_Finalize 则是结束MPI执行环境.
- MPI_Comm_rank 函数就是用来标识各个MPI进程的,给出调用该函数的进程的进程号.
	- MPI_Comm类型的通信域,标识参与计算的MPI进程组.上面例子中使用的是MPI_COMM_WORLD,这个进程组是MPI实现预先定义好的进程组,指的是所有MPI进程所在的进程组.
	如果想要申请自己的特殊的进程组,则需要通过MPI_Comm定义并通过其它MPI函数生成.
	MPI还定义了另一个进程组MPI_COMM_SELF,只包含各个进程自己的进程组.
	- &rank 返回调用进程中的标识号.
- MPI_Comm_size 标识相应进程组中有多少个进程
	- &size返回相应进程组中的进程数.
- int MPI_Send(buf, counter, datatype, dest, tag, comm)
	- buf:发送缓冲区的起始地址,可以是数组或结构指针
	- count:非负整数,发送的数据个数
	- datatype:发送数据的数据类型
	- dest:整型,目的进程号
	- tag:整型,消息标志, 用以和本进程向同一目的进程发送的其它消息区别开来.
	- comm:MPI进程组所在的通信域

- int MPI_Recv(buf, count, datatype, source, tag, comm, status) comm域中source进程接收标签号为tag的数据,并保存到buf中.
	- buf:接收缓冲区的起始地址,可以是数组或结构指针
	- count:非负整数,最多可接收的数据个数
	- datatype:接收数据的数据类型
	- source:整型,接收数据的来源,即发送数据进程的进程号
	- tag:整型,消息标识,应与发送操作的消息标识相同
	- comm:消息接收进程所在的通信域
	- status:MPI_Status结构指针,返回状态信息, status主要显示接收函数的各种错误状态.
		通过访问status.MPI_SOURCE,status.MPI_TAG和status.MPI_ERROR就可以得到发送数据的进程号,使用的标签以及接收操作的错误代码.

- int MPI_Get_count(MPI_Status *status, MPI_Datatype datatype, int *count); 获得实际接收到的数据项数,存放到count中.

- MPI_Wtime() 返回一个双精度数,表示从过去某点的时刻到当前时刻所消耗的时间秒数.
- MPI_Wtick() 返回MPI_Wtime结果的精度

```c
#include <mpi.h>
#include <stdio.h>

int main(int argc, char ** argv)
{
    [...snip...]

    if (rank != 0)  /* worker process */
        MPI_Send(data, rand() % 100, MPI_INT, 0, group_id, MPI_COMM_WORLD);
    else {  /* master process */
        for (i = 0; i < size – 1; i++) {
            MPI_Recv(data, 100, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_INT, &count);
            printf("worker ID: %d; task ID: %d; count: %d\n", status.MPI_SOURCE, status.MPI_TAG, count);
        }
    }

    [...snip...]
}
```

# CUDA
CUDA(Compute Unified Device Architecture) 

- CPU: 兼顾程序执行和数据运算的并行性, 通用性以及它们的平衡性.CPU的微架构偏重于程序执行的效率,不会一味追求某种运算极致速度而牺牲程序执行的效率.
	如程序分支预测,推测执行,多重嵌套分支执行,并行执行时候的指令相关性和数据 相关性,多核协同处理时候的数据一致性等等复杂逻辑. 
- GPU: 由硬件实现的一组图形函数的集合,这些函数主要用于绘制各种图形所需要的运算. 这些和像素,光影处理,3D 坐标变换等相关的运算由GPU硬件加速来实现.
	图形运算的特点是大量同类型数据的密集运算-如图形数据的矩阵运算,GPU的微架构就是面向适合于矩阵类型的数值 计算而设计的,大量重复设计的计算单元,
	这类计算可以分成众多独立的数值计算-大量数值运 算的线程,而且数据之间没有像程序执行的那种逻辑关联性. 

# Issues
## False Sharing
[ref1](http://www.codeproject.com/Articles/51553/Concurrency-Hazards-False-Sharing)
[ref2](https://software.intel.com/en-us/articles/avoiding-and-identifying-false-sharing-among-threads/)

False sharing is also known as cache-line ping-ponging.
It is caused by one or more cores repeatedly invalidating the caches of the other cores, even while accessing isolated state.
This forces the other cores to read data from main memory instead of their local cache, which slows them down considerably.

### Cache lines
The data in a cache is grouped into blocks called cache-lines, which are typically 64 or 128 bytes wide.
These are the smallest units of memory that can be read from, or written to, main memory.
This works well in most programs as data that is close in memory is often needed close in time by a particular thread.
However, this is the root of the false sharing problem.

### Cache coherence
When a program writes a value to memory, it goes firstly to the cache of the core that ran the code.
If any other caches hold a copy of that cache line, their copy is marked as invalid and cannot be used.
The new value is written to main memory, and the other caches must re-read it from there if they need it.
Although this synchronization is implemented in hardware, it still takes time. And, of course, reading from main memory takes a few hundred clock cycles by itself.
Modern processors use the MESI protocol to implement cache coherence.
This basically means each cache line can be in one of four states:

- M odified
- E xclusive
- S hared
- I nvalid

When a core modifies any data in a cache line, it transitions to "Modified", and any other caches that hold a copy of the same cache line are forced to "Invalid".
The other cores must then reread the data from main memory next time they need it.

### [False sharing的发生](../demo/parallel/openmp/false_sharing.cpp)
Imagine two different variables are being used by two different threads on two different cores.
This appears to be embarrassingly parallel, as the different threads are using isolated data.
However, if the two variables are located in the same cache line and at least one is being written, then there will be contention for that cache line.
This is false sharing.
It is called false sharing because even though the different threads are not sharing data, they are, unintentionally, sharing a cache line.

![cache line](https://software.intel.com/sites/default/files/m/d/4/1/d/8/5-4-figure-1.gif)

Since compilers are aware of false sharing, they do a good job of eliminating instances where it could occur.
For example, when the above code is compiled with optimization options, the compiler eliminates false sharing using thread-private temporal variables.
Run-time false sharing from the above code will be only an issue if the code is compiled with optimization disabled.


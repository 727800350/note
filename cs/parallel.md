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
1. At start of parallel region master creates team of parallel ”worker” threads (FORK) Statements in parallel block are executed in parallel by every thread
1. At end of parallel region, all threads synchronize, and join master thread (JOIN)

## API
### function
- omp_set_num_threads(n): set number of openMP threads
- omp_get_num_threads() Returns number of threads in parallel region, Returns 1 if called outside parallel region
- omp_get_thread_num() Returns id(value in [0, n - 1]) of thread, Master thread always has id 0
- double omp_get_wtime();  Time in Seconds since a fixed point in the past 

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

- #pragma omp do nowait { }: For situations where we do not need or want to synchronize at the end OpenMP provides the NOWAIT clause
- !$OMP SINGLE: One thread (not neccesarily master) executes the block Other threads will wait, Useful for thread-unsafe code and I/O operations
- !$OMP MASTER: only the master threads excecutes instructions in the block. There is no implicit barrier so other threads will not wait for master to finish.
- #pragma opm critical: 在任何时候只能有一个thread 运行, 这块代码
	```c
	#pragma opm critical 
	consume(B, res);
	```
	Threads wait Threads wait their turn – only one at a only one at a time calls time calls consume()}
- #pragma opm atomic: only used for the update of a memory location. Sometimes !$OMP ATOMIC is also refered to as a mini critical section.
	```c
	#pragma opm atomic
	X += tmp;
	```
	Atomic only protects the read/update of X
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

- $!OMP PARALLEL IF(n > 100000) (fortran)
- #pragma omp parallel if (n>100000) (C/C++)

This will only run the parallel region when n> 100000

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


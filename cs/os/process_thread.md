- [并行与并发](#并行与并发)
- [进程 process](#进程-process)
	- [进程状态](#进程状态)
	- [进程间通信方式](#进程间通信方式)
		- [匿名管道pipe](#匿名管道pipe)
		- [有名管道FIFO](#有名管道fifo)
		- [消息队列](#消息队列)
		- [锁](#锁)
		- [信号量 semaphore](#信号量-semaphore)
		- [共享内存 shared memory](#共享内存-shared-memory)
		- [信号 signal](#信号-signal)
	- [fork](#fork)
- [线程](#线程)

# 并行与并发
并发和并行是即相似又有区别的两个概念,并行是指两个或者多个事件在同一时刻发生,而并发是指两个或多个事件在同一时间间隔内发生.

在多道程序环境下,并发性是指在一段时间内宏观上有多个程序在同时运行,但在单处理机系统中,每一时刻却仅能有一道程序执行,故微观
上这些程序只能是分时地交替执行.
倘若在计算机系统中有多个处理机,则这些可以并发执行的程序便可被分配到多个处理机上,实现并行执行,即利用每个处理机来处理一个
可并发执行的程序,这样,多个程序便可以同时执行.

# 进程 process
- 进程是指在系统中能独立运行并作为资源的分配的基本单位, 他是一组机器指令, 数据和堆栈等组成的, 是一个能独立运行的活动实体
- 多个进程之间可以并发执行和交换信息.
- 一个进程在运行时需要一定的资源, 如CPU, 存储空间及I/O设备等

操作系统中进程调度策略有哪几种? FCFS(First Come First Service),优先级,时间片轮转,多级反馈

## 进程状态
[Linux进程状态](https://www.cnblogs.com/diegodu/p/9167671.html)

- R(`TASK_RUNNING`): Running or runnable (on run queue)

- S(`TASK_INTERRUPTIBLE`): Interruptible sleep (waiting for an event to complete)
	处于这个状态的进程因为等待某某事件的发生(比如等待socket连接,等待信号量),而被挂起.这些进程的task_struct结构被放入对应事件的等待队列中.
	当这些事件发生时(由外部中断触发,或由其他进程触发),对应的等待队列中的一个或多个进程将被唤醒.
  通过ps命令我们会看到,一般情况下,进程列表中的绝大多数进程都处于`TASK_INTERRUPTIBLE`状态(除非机器的负载很高).
	毕竟CPU就这么一两个,进程动辄几十上百个,如果不是绝大多数进程都在睡眠,CPU又怎么响应得过来.

- D(`TASK_UNINTERRUPTIBLE`): Uninterruptible sleep (usually IO)
	与TASK_INTERRUPTIBLE状态类似,进程处于睡眠状态,但是此刻进程是不可中断的.不可中断,指的并不是CPU不响应外部硬件的中断,而是指进程不响应异步信号.
	绝大多数情况下,进程处在睡眠状态时,总是应该能够响应异步信号的.否则你将惊奇的发现,kill -9竟然杀不死一个正在睡眠的进程了!
	于是我们也很好理解,为什么ps命令看到的进程几乎不会出现TASK_UNINTERRUPTIBLE状态,而总是TASK_INTERRUPTIBLE状态.
  而`TASK_UNINTERRUPTIBLE`状态存在的意义就在于,内核的某些处理流程是不能被打断的.
	在进程对某些硬件进行操作时(比如进程调用read系统调用对某个设备文件进行读操作,而read系统调用最终执行到对应设备驱动的代码,并与对应的物理设备进行交互),
	可能需要使用TASK_UNINTERRUPTIBLE状态对进程进行保护,以避免进程与设备交互的过程被打断,造成设备陷入不可控的状态.这种情况下的TASK_UNINTERRUPTIBLE状态总是非常短暂的,通过ps命令基本上不可能捕捉到.

- T(`TASK_STOPPED or TASK_TRACED`): Stopped, either by a job control signal or because it is being traced.
	向进程发送一个SIGSTOP信号,它就会因响应该信号而进入TASK_STOPPED状态(除非该进程本身处于TASK_UNINTERRUPTIBLE状态而不响应信号).
	(SIGSTOP与SIGKILL信号一样,是非常强制的.不允许用户进程通过signal系列的系统调用重新设置对应的信号处理函数.)
	向进程发送一个SIGCONT信号,可以让其从TASK_STOPPED状态恢复到TASK_RUNNING状态.

	当进程正在被跟踪时,它处于TASK_TRACED这个特殊的状态."正在被跟踪"指的是进程暂停下来,等待跟踪它的进程对它进行操作.
	比如在gdb中对被跟踪的进程下一个断点,进程在断点处停下来的时候就处于TASK_TRACED状态.而在其他时候,被跟踪的进程还是处于前面提到的那些状态.

	对于进程本身来说,TASK_STOPPED和TASK_TRACED状态很类似,都是表示进程暂停下来.
	而TASK_TRACED状态相当于在TASK_STOPPED之上多了一层保护,处于TASK_TRACED状态的进程不能响应SIGCONT信号而被唤醒.
	只能等到调试进程通过ptrace系统调用执行PTRACE_CONT,PTRACE_DETACH等操作(通过ptrace系统调用的参数指定操作),或调试进程退出,被调试的进程才能恢复TASK_RUNNING状态.

- Z(`TASK_DEAD - EXIT_ZOMBIE`): Defunct ("zombie") process, 退出状态,进程成为僵尸进程
	进程在退出的过程中,处于TASK_DEAD状态.
	在这个退出过程中,进程占有的所有资源将被回收,除了task_struct结构(以及少数资源)以外.于是进程就只剩下task_struct这么个空壳,故称为僵尸.
	之所以保留task_struct,是因为task_struct里面保存了进程的退出码,以及一些统计信息.而其父进程很可能会关心这些信息.
	比如在shell中,$?变量就保存了最后一个退出的前台进程的退出码,而这个退出码往往被作为if语句的判断条件.

	父进程可以通过wait系列的系统调用(如wait4,waitid)来等待某个或某些子进程的退出,并获取它的退出信息.然后wait系列的系统调用会顺便将子进程的尸体(task_struct)也释放掉.
  子进程在退出的过程中,内核会给其父进程发送一个信号,通知父进程来"收尸".这个信号默认是SIGCHLD,但是在通过clone系统调用创建子进程时,可以设置这个信号.

- X(`TASK_DEAD - EXIT_DEAD`): dead (should never be seen)

其他附加状态

- <: 高优先级
- N: 低优先级
- L: 有些页被锁进内存
- s: 包含子进程
- +: 位于后台的进程组
- l: 多线程, 克隆线程 multi-threaded(using `CLONE_THREAD`, like NPTL pthreads do)

## 进程间通信方式
[进程间的五种通信方式介绍](https://www.cnblogs.com/zgq0/p/8780893.html)

主要分为: 匿名管道pipe, 有名管道FIFO, 消息队列, 锁, 信号量semaphore, 共享内存shared memory, 信号signal, 套接字socket

`ipcs`: report System V interprocess communication facilities status

### 匿名管道pipe
1. 管道是一种半双工的通信方式, 数据只能单项流动, 具有固定的读端和写端
1. 只能在具有父子关系的进程间流动
1. 它可以看成是一种特殊的文件,对于它的读写也可以使用普通的read,write 等函数.但是它不是普通的文件,并不属于其他任何文件系统,并且只存在于内存中

```C++
#include <unistd.h>
int pipe(int fd[2]);  // 返回值, 若成功返回0, 失败返回-1
```
- 当一个管道建立时,它会创建两个文件描述符:fd[0]为读而打开,fd[1]为写而打开
- 要关闭管道只需将这两个文件描述符关闭即可.
- 单个进程中的管道几乎没有任何用处.所以,通常调用 pipe 的进程接着调用 fork,这样就创建了父进程与子进程之间的 IPC 通道

<img src="./pics/process_thread/pipe_fork.png" alt="pipe in parent and child process" width="70%"/>

从图中可以看到, 调用pipe 之后创建了一个管道, 然后在调用fork 创建子进程, 由于子进程会从父进程拷贝文件描述符, 所以pipe 产生的两个fd 也会复制过去.
这样, 父子进程就能通过这两个fd 来通信, 父进程写fd[1], 子进程读fd[0]; 或者子进程写fd[1], 父进程读fd[0].
如果只需要单向通信, 可以把不需要的fd close 掉.

### 有名管道FIFO
FIFO,也称为命名管道,它是一种文件类型.

1. FIFO可以在无关的进程之间交换数据,与无名管道不同.
1. FIFO有路径名与之相关联,它以一种特殊设备文件形式存在于文件系统中.

```bash
# terminal 1
mkfifo pf
echo "hello world" > pf

# terminal 2
cat pf
```
当echo 之后, 但是还没有在terminal 2 执行cat 的读操作之前, echo 会阻塞住.
同样的, 如果先在terminal 2 执行cat 读, 但是terminal 1 此时还没写, 读操作也会被阻塞住.

[Named Pipe or FIFO with example C program](https://www.geeksforgeeks.org/named-pipe-fifo-example-c-program)

### 消息队列
消息队列是消息的链表,存放在内核中并由消息队列标识符标识.

1. 消息队列是面向记录的,其中的消息具有特定的格式以及特定的优先级.
1. 消息队列独立于发送与接收进程.进程终止时,消息队列及其内容并不会被删除.
1. 消息队列可以实现消息的随机查询,消息不一定要以先进先出的次序读取,也可以按消息的类型读取.

```C++
#include <sys/msg.h>
int msgget(key_t key, int flag);  // 创建或打开消息队列:成功返回队列ID,失败返回-1
int msgsnd(int msqid, const void *ptr, size_t size, int flag);  // 添加消息:成功返回0,失败返回-1
int msgrcv(int msqid, void *ptr, size_t size, long type,int flag);  // 读取消息:成功返回消息数据的长度,失败返回-1
```
当消息队列为空, 调用msgrcv 会阻塞住

msgget, msgsnd, msgrcv 这一套是system V 的实现.
posix 还有一套自己的实现, `mq_open`, 返回一个`mqd_t` 类型的message queue descriptor.
posix 的优势在于返回的描述符是一个文件描述符, 因为可以作为epoll 等多路复用的监听对象, 而system V 的那套实现都是通过msgid 来弄的, 无法给epoll 使用.

### 锁
进程间的文件锁
```C++
#include <sys/file.h>
int flock(int fd, int operation);  // 通过operation 来表明是加互斥锁/共享锁, 还是解锁
```
- `LOCK_SH`: Place a shared lock
- `LOCK_EX`: Place an exclusive lock
- `LOCK_UN`: Remove an existing lock held by this process

Locks created by flock are associated with an open file description.
This means that duplicate file descriptors (created by, for example, fork(2) or dup(2)) refer to the same lock, and this lock may be modified or released using any of these file descriptors.
Furthermore, the lock is released either by an explicit `LOCK_UN` operation on any of these duplicate file descriptors, or when all such file descriptors have been closed.

如果一个进程使用flock 独占锁锁住了一个文件, 另外一个文件不去调用flock 进行检查, 直接打开文件进行操作, 是否可以进行写操作取决于内核的实现是强制锁还是建议锁.

强制锁的概念是传统UNIX为了强制应用程序遵守锁规则而引入的一个概念,与之对应的概念就是建议锁(Advisory locking).
我们日常使用的基本都是建议锁,它并不强制生效.这里的不强制生效的意思是,如果某一个进程对一个文件持有一把锁之后,其他进程仍然可以直接对文件进行各种操作的,比如open,read,write.
只有当多个进程在操作文件前都去检查和对相关锁进行锁操作的时候,文件锁的规则才会生效.这就是一般建议锁的行为.
而强制性锁试图实现一套内核级的锁操作.当有进程对某个文件上锁之后,其他进程即使不在操作文件之前检查锁,也会在open,read或write等文件操作时发生错误.
内核将对有锁的文件在任何情况下的锁规则都生效,这就是强制锁的行为.由此可以理解,如果内核想要支持强制锁,将需要在内核实现open,read,write等系统调用内部进行支持.

C语言的标准IO库中还提供了一套文件锁,它们的原型如下:
```C++
#include <stdio.h>
void flockfile(FILE *filehandle);
int ftrylockfile(FILE *filehandle);
void funlockfile(FILE *filehandle);
```
从实现角度来说,stdio库中实现的文件锁与flock或lockf有本质区别.作为一种标准库,其实现的锁必然要考虑跨平台的特性,所以其结构都是在用户态的FILE结构体中实现的,而非内核中的数据结构来实现.
这直接导致的结果就是,标准IO的锁在多进程环境中使用是有问题的.进程在fork的时候会复制一整套父进程的地址空间,这将导致子进程中的FILE结构与父进程完全一致.
就是说,父进程如果加锁了,子进程也将持有这把锁,父进程没加锁,子进程由于地址空间跟父进程是独立的,所以也无法通过FILE结构体检查别的进程的用户态空间是否加了标准IO库提供的文件锁.
这种限制导致这套文件锁只能处理一个进程中的多个线程之间共享的FILE 的进行文件操作, 而且还必须操作同一个用fopen打开的FILE 变量.

### 信号量 semaphore
信号量是一个计数器, 用于实现进程间的互斥与同步,而不是用于存储进程间通信数据.

1. 信号量用于进程间同步,若要在进程间传递数据需要结合共享内存.
1. 信号量基于操作系统的 PV 操作,程序对信号量的操作都是原子操作.
1. 每次对信号量的 PV 操作不仅限于对信号量值加 1 或减 1,而且可以加减任意正整数.
1. 支持信号量组.

```C++
#include <sys/sem.h>
int semget(key_t key, int num_sems, int sem_flags);  // 创建或获取一个信号量组:若成功返回信号量集ID,失败返回-1
int semop(int semid, struct sembuf semoparray[], size_t numops);  // 对信号量组进行操作,改变信号量的值:成功返回0,失败返回-1
int semctl(int semid, int sem_num, int cmd, ...);  // 控制信号量的相关信息
```

### 共享内存 shared memory
共享内存就是映射一段能被其它进程访问的内存,这段共享内存由一个进程创建,但是多个进程可以访问.

1. 共享内存是最快的一种 IPC,因为进程是直接对内存进行存取.
1. 因为多个进程可以同时操作,所以需要进行同步.
1. 信号量+共享内存通常结合在一起使用,信号量用来同步对共享内存的访问.

```C++
#include <sys/shm.h>
int shmget(key_t key, size_t size, int flag);  // 创建或获取一个共享内存:成功返回共享内存ID,失败返回-1
void* shmat(int shm_id, const void* addr, int flag);  // 连接共享内存到当前进程的地址空间:成功返回指向共享内存的指针,失败返回-1
int shmdt(void* addr);  // 断开与共享内存的连接:成功返回0,失败返回-1
int shmctl(int shm_id, int cmd, struct shmid_ds* buf);  // 控制共享内存的相关信息:成功返回0,失败返回-1
```
- 当用shmget函数创建一段共享内存时,必须指定其 size,而如果引用一个已存在的共享内存,则将 size 指定为0 .
- 当一段共享内存被创建以后,它并不能被任何进程访问.必须使用shmat函数连接该共享内存到当前进程的地址空间,连接成功后把共享内存区对象映射到调用进程的地址空间,随后可像本地空间一样访问.
- shmdt函数是用来断开shmat建立的连接的.注意,这并不是从系统中删除该共享内存,只是当前进程不能再访问该共享内存而已.
- shmctl函数可以对共享内存执行多种操作,根据参数 cmd 执行相应的操作.常用的是`IPC_RMID`(从系统中删除该共享内存).

或者用 mmap, 这个比较适合父子进程, 相当于匿名的共享内存.

### 信号 signal
信号是一种比较复杂的通信方式,用于通知接收进程某个事件已经发生, 信号注册函数

## fork
在fork 函数调用时, 父进程和子进程会被kernel 分配到不同的虚拟内存空间中,所以在两个进程看来它们访问的是不同的内存.

cow:
在真正访问虚拟内存空间时,kernel 会将虚拟内存映射到物理内存上,所以父子进程共享了物理上的内存空间, 当父进程或者子进程对共
享的内存进行修改时,共享的内存才会以页为单位进行拷贝,父进程会保留原有的物理空间,而子进程会使用拷贝后的新物理空间.

# 线程
- 线程是进程的实体, 是CPU调度和分派的基本单位,它是比进程更小的能独立运行的基本单位.
- 一个进程可以包含若干个线程, 多个线程也可以并发执行
- 将进程作为分配资源的基本单位, 而把线程作为独立运行和调度的基本单位.

`fork` 是昂贵的, 子进程中复制所有描述符等等.
子进程获取父进程数据空间,堆和栈的副本, 包括缓冲区.

同一进程内的所有线程共享

1. 相同的全局内存(也就是全局变量)
1. 打开的文件(即描述符)
1. 信号处理函数和信号处置
1. 进程指令
1. 大多数数据
1. 当前工作目录
1. 用户ID和组ID
1. All threads share a common heap. so access to the allocator/deallocator must be synchronized.

不过每个线程拥有各自的

1. 线程ID
1. 寄存器集合, 包括程序计数器和栈指针
1. 栈(用于存放局部变量和返回地址)
1. errno: pthread 库的函数都是通过返回值返回错误号,虽然每个线程也都有一个errno,但这是为了兼容其它函数接口而提供的,
  pthread库本身并不使用它,所以errno 还是看成同一个进程的所有线程共享一个全局的errno.
1. 信号掩码
1. 优先级

线程同步的方式有哪些?

- 互斥量: 采用互斥对象机制,只有拥有互斥对象的线程才有访问公共资源的权限.
- 信号量: 它允许同一时刻多个线程访问同一资源, 但是需要控制同一时刻访问此资源的最大线程数量.
- 事件(信号): 通过通知操作的方式来保持多线程同步, 还可以方便的实现多线程优先级的比较操作,
  `pthread_cond_signal, pthread_cond_wait`.


# 并行与并发
并发和并行是即相似又有区别的两个概念,并行是指两个或者多个事件在同一时刻发生,而并发是指两个或多个事件在同一时间间隔内发生.

在多道程序环境下,并发性是指在一段时间内宏观上有多个程序在同时运行,但在单处理机系统中,每一时刻却仅能有一道程序执行,故微观上这些程序只能是分时地交替执行.
倘若在计算机系统中有多个处理机,则这些可以并发执行的程序便可被分配到多个处理机上,实现并行执行,即利用每个处理机来处理一个可并发执行的程序,这样,多个程序便可以同时执行.

# 进程 Process
- 进程是指在系统中能独立运行并作为资源的分配的基本单位, 他是一组机器指令, 数据和堆栈等组成的, 是一个能独立运行的活动实体
- 多个进程之间可以并发执行和交换信息.
- 一个进程在运行时需要一定的资源, 如CPU, 存储空间及I/O设备等

进程同步有哪几种机制: 原子操作,信号量机制,自旋锁管程,会合,分布式系统

操作系统中进程调度策略有哪几种? FCFS(First Come First Service),优先级,时间片轮转,多级反馈

进程有哪几种状态?

- 就绪状态:进程已获得除处理机以外的所需资源,等待分配处理机资源
- 运行状态:占用处理机资源运行,处于此状态的进程数小于等于CPU数
- 阻塞状态: 进程等待某种条件,在条件满足之前无法执行

## 进程的通信方式
[进程间的五种通信方式介绍](https://www.cnblogs.com/zgq0/p/8780893.html)

主要分为: 匿名管道pipe, 有名管道FIFO, 消息队列, 锁, 信号量semaphore, 共享内存shared memory, 信号signal, 套接字socket

`ipcs`: report System V interprocess communication facilities status

### 匿名管道pipe
特性

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

特点

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

特点

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

## 锁
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

### 信号量 semaphore
信号量是一个计数器, 用于实现进程间的互斥与同步,而不是用于存储进程间通信数据.

特点

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

特点

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

### 信号 signal
信号是一种比较复杂的通信方式,用于通知接收进程某个事件已经发生, 信号注册函数

共享内存区:能够很容易控制容量,速度快,但要保持同步,比如一个进程在写的时候,另一个进程要注意读写的问题,相当于线程中的线程安全,当然,共享内存区同样可以用作线程间通讯,不过没这个必要,线程间本来就已经共享了同一进程内的一块内存

# 线程
- 线程是进程的实体, 是CPU调度和分派的基本单位,它是比进程更小的能独立运行的基本单位.
- 一个进程可以包含若干个线程, 多个线程也可以并发执行
- 将进程作为分配资源的基本单位, 而把线程作为独立运行和调度的基本单位.

`fork` 是昂贵的. fork要把父进程的内存镜像复制到子进程, 并在子进程中复制所有描述符等等.
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
1. errno. [ref](http://learn.akae.cn/media/ch35s02.html).
	pthread库的函数都是通过返回值返回错误号,虽然每个线程也都有一个errno,但这是为了兼容其它函数接口而提供的,pthread库本身并不使用它.
	所以errno 还是看成同一个进程的所有线程共享一个全局的errno.
1. 信号掩码
1. 优先级

线程同步的方式有哪些?

- 互斥量:采用互斥对象机制,只有拥有互斥对象的线程才有访问公共资源的权限.因为互斥对象只有一个,所以可以保证公共资源不会被多个线程同时访问.
- 信号量:它允许同一时刻多个线程访问同一资源,但是需要控制同一时刻访问此资源的最大线程数量.
- 事件(信号):通过通知操作的方式来保持多线程同步,还可以方便的实现多线程优先级的比较操作, `pthread_cond_signal, pthread_cond_wait`.


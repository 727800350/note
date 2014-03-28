Unix network Programming note

Note: This markdown file also contains other resources, but it is mostly from UNP.

# TCP and UDP
Note that TCP does not guarantee that the data will be received by the other endpoint, as this is impossible. It delivers data to the other endpoint if possible, and notifies the user (by giving up on retransmissions and breaking the connection) if it is not possible. Therefore, TCP cannot be described as a 100% reliable protocol; it provides **reliable delivery of data or reliable notification of failure.**

We also say that UDP provides a connectionless service, as there need not be any long-term relationship between a UDP client and server. For example, a UDP client can create a socket and send a datagram to a given server and then immediately send another datagram on the same socket to a different server. Similarly, a UDP server can receive several datagrams on a single UDP socket, each from a different client.

## API
	inet_pton: presentation to network

# Signal
就是告知某个进程发生了某个事件的通知, 有时也称为软件中断.
信号通常是异步发生的, 也就是说进程预先不知道信号的准确发生时刻
 
在进程表的表项中有一个软中断信号域,该域中每一位对应一个信号,当有信号发送给进程时,对应位置位.由此可以看出,进程对不同的信号可以同时保留,但对于同一个信号,进程并不知道在处理之前来过多少个, 也就是信号不排队(但是现在有排队的信号实现)

在一个信号处理函数运行期间, 正被递交的信号是阻塞的. 而且, 安装处理函数时在传递给sigaction函数的sa_mask 信号集中指定的任何额外信号也被阻塞.

一般有3 种方式进行操作
(1) eg: signal(SIGINT ,SIG_IGN );
//SIG_IGN 代表忽略信号,SIGINT信号代表由InterruptKey产生,通常是CTRL +C 或者是DELETE .发送给所有ForeGround Group的进程.
(2) eg: signal(SIGINT ,SIG_DFL );
//SIGINT信号代表由InterruptKey产生,通常是CTRL +C或者是DELETE.发送给所有ForeGroundGroup的进程. SIG_DFL代表执行系统默认操作,其实对于大多数信号的系统默认动作时终止该进程.这与不写此处理函数是一样的.
(3) 自定义的信号或是信号处理函数

## API
函数signal 的正常函数原型:
`void (*signal (int signo, void (* func)(int)))(int);`  
为了简化, 我们定义  
```
typedef void Sigfunc(int);
Sigfunc *signal(int signo, Sigfunc *func);
```  
func 是指向信号处理函数的指针
**返回值**也是指向信号处理函数的指针

僵死进程

    #include <sys/wait.h>
    pid_t wait(int *status);
    pid_t waitpid(pid_t pid, int *status, int options);

Used to wait for **state changes** in a child of the calling process, and obtain information about the child whose state has changed.  
A state change is considered to be: the child terminated; the child was stopped by a signal; or the child was **resumed by a signal**.  
In the case of a terminated child, performing a wait allows the system to release the resources associated with the child; if a wait is not performed, then the terminated child remains in a "zombie" state
If a child has already changed state, then these calls return immediately.  Otherwise they block until either a child changes state or a signal handler interrupts the call

if pid == -1: meaning wait for any child process.

The value of options is an OR of zero or more of the following constants:
WNOHANG最常用的选项    return immediately if no child has exited.
WUNTRACED  also return if a child has stopped 
WCONTINUED also return if a stopped child has been resumed by delivery of SIGCONT.

# I/O 模型
阻塞操作是指在执行设备操作时若不能获得资源则挂起进程,直到满足可操作的条件后再进行操作.

在Unix 下的可用的5 中I/O 模型  
阻塞式I/O;
非阻塞式I/O;
I/O 复用(select/pull);
信号驱动式I/O(SIGIO);
异步IO.

- 阻塞式IO模型
默认情况下, 所有套接字都是阻塞的.
以数据报套接字为例  
[阻塞式IO模型](http://www.kankanews.com/ICkengine/wp-content/plugins/wp-o-matic/cache/5415ca1f52_063846-CPcp-255033.jpg)

- 非阻塞式IO  
[非阻塞式IO](http://images.cnblogs.com/cnblogs_com/yjf512/201205/201205310957235923.jpg)


- IO复用模型  
[IO复用模型](http://blog.chinaunix.net/attachment/201206/20/10275706_1340176181jjx0.jpg)

- 信号驱动式IO  
[信号驱动式IO](http://www.kankanews.com/ICkengine/wp-content/plugins/wp-o-matic/cache/5415ca1f52_064039-xaMw-255033.jpg)

- 异步IO模型  
[异步IO模型](http://images.cnblogs.com/cnblogs_com/yjf512/201205/201205310957257186.jpg)

- 5 中模型的比较  
[比较](http://hi.csdn.net/attachment/201012/24/0_129318173593Bo.gif) 

## IO复用模型
### Select
允许进程指示内核等待多个(**我们感兴趣的**)事件中的任何一个发生, 并只在有一个或多个事件发生或经历一段指定的时间后才唤醒它.  
这样就不必为每个client fork 一个子进程来处理.
#### 描述符就绪条件
- 满足下面4 个条件中的任何一个时, 一个套接字准备好读
	1. todo:该套接字接受缓冲区中的数据字节数大于等于套接字接受缓冲区**低水位标记**的当前大小. 对这样的套接字执行读操作不会阻塞并将**返回一个大于0的值**
	2. 该连接的读半部关闭(也就是接受了FIN的TCP连接). **读不阻塞并返回0**(也就是EOF)(读已经关闭了, 读不了内容, 所以不用阻塞)
	3. 该套接字是一个监听套接字, 且已完成的连接数不为0. 对这样的套接字的accept通常不会阻塞
	4. 有一个套接字错误待处理. 不会阻塞并返回-1, 同时把errno 置为确切的错误条件
- 满足下面4 个条件中的任何一个时, 一个套接字准备好写
	1. todo该套接字发送缓冲区中的可用空间
	2. 该连接的写半部关闭. 对这样的套接字的写将返回SIGPIPE信号
	3. 使用非阻塞式connect 的套接字已建立连接, 或者connect 以失败告终
	4. 其上有一个套接字错误待处理. 不会阻塞并返回-1, 同时把errno 置为确切的错误条件

Example:  

1. 如果对端TCP发送一个RST(对端主机崩溃并重新启动), 那么该套接字变位可读, 并且`read`返回-1, 而`errno`中含有确切的错误代码.
2. 如果对端TCP发送数据, 那么该套接字变位可读, 并且read 返回一个大雨0 的值(即读入数据的字节数).

**使用select库的步骤**:

1. 创建所关注的事件的描述符集合(`fd_set`),对于一个描述符,可以关注其上面的读(read),写(write),异常(exception)事件,所以通常,要创建三个fd_set, 当如如果不关注, 可以设为`null`, 一个用来收集关注读事件的描述符,一个用来收集关注写事件的描述符,另外一个用来收集关注异常事件的描述符集合
2. 然后调用`FD_SET` 对集合进程初始化, 然后用`FD_SET` 打开需要关注的事件
2. 调用select(),等待事件发生.这里需要注意的一点是,select的阻塞与是否设置非阻塞I/O是没有关系的.  
其中,最后一个参数timeout,可以设置select等待的时间.i)如果该值设置为0,那么,select()在检查描述符后就立即返回.ii)如果该值为`null`;那么select 会永远等待下去,一直到有一个描述符准备好IO; iii)其他, 那么,select()会等待指定的时间,然后再返回.select()的返回值指定了发生事件的fd个数
1. 用`FD_ISSET`测试fd_set中的每一个fd ,检查是否有相应的事件发生,如果有,就进行处理.


#### API

    #include <sys/select.h>
    int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
	//the nfds 等于关注的fds中的最大值加1  
    void FD_CLR(int fd, fd_set *set);
    int  FD_ISSET(int fd, fd_set *set);  //test
    void FD_SET(int fd, fd_set *set);
    void FD_ZERO(fd_set *set); //set the set empty
	
	example: tcpcliser/tcpservselect01.c; elect/strcliselect02.c

当一个服务器在处理多个客户时, 他绝对不能阻塞只与单个客户相关的某个函数调用. 否则可能导致服务器被挂起, 拒绝为所有其他客户提供服务. 这就是所谓的拒绝服务(denial of service)攻击todo: 这个攻击的意思不是耗尽服务器资源吗? 难道是以这种方式耗尽的.
### poll
poll库是在linux2.1.23中引入的,windows平台不支持poll.  
poll与select的基本方式相同,都是先创建一个关注事件的描述符的集合,然后再去等待这些事件发生,然后再轮询描述符集合,检查有没有事件发生,如果有,就进行处理.因此,poll有着与select相似的处理流程:  
(1)创建描述符集合,设置关注的事件  
(2)调用poll(),等待事件发生.下面是poll的原型:  
`int poll(struct pollfd *fds, nfds_t nfds, int timeout);`
类似select,poll也可以设置等待时间,效果与select一样.  
(3)轮询描述符集合,检查事件,处理事件.

在这里要说明的是,poll与select的主要区别在与,select需要为读,写,异常事件分配创建一个描述符集合,最后轮询的时候,需要分别轮询这三个集合.而poll只需要一个集合,在每个描述符对应的结构上分别设置读,写,异常事件,最后轮询的时候,可以同时检查三种事件.

#### API

    #include<poll.h>
    int poll(struct pollfd *fdarray, unsigend long nfds, int timeout);
    返回: 若有就绪描述符, 则为其数目, 若超时则为0, 若出错则为-1
	
	struct pollfd{
		int fd; //descriptor to check
		short events; //events of interset on fd
		short revents; //events that occuring on fd, returend events
	};

    timeout值, 说明
    INFTIM, 永远等待
    0, 立即返回
    >0, 等待指定时间

	example: tcpcliser/tcpservpoll01.c

如果我们不再关心某个特定描述符, 那么可以把与它对应的`pollfd`结构的`fd` 成员设置成一个负值. `poll` 函数将忽略这样的`pollfd` 结构的`events` 成员, 返回时, 将他的`revents`成员的值置为0.


### fcntl(file control) 函数

    #include <unistd.h>
    #include <fcntl.h>

    int fcntl(int fd, int cmd, ... /* arg */ );

    F_SETFL, O_NONBLOCK 设置套接字为非阻塞式IO
    F_SETFL, O_ASYNC 设置套接字为信号驱动式IO
    
    F_SETOWN 设置套接字属主
    F_GETOWN 获取套接字属主
    
    使用fcntl开启非阻塞式IO的典型代码:
    int flags;
    if((flags = fcntl(fd, F_GETFL,0)) < 0)
    	err_sys("F_GETFL error");
    flags |= O_NONBLOCK;
    if(fcntl(fd, F_SETFL, flags) < 0)
    	err_sys("F_SETFL error");
	设置某个文件状态标志的唯一正确的方法是: 先取得当前标志, 与新标志逻辑或后在设置标志. 如果不取得之前的标志而直接设置, 则之前的标志就会丢失


### 可重入函数
若一个程序或子程序可以"安全的被并行执行(Parallel computing)",则称其为可重入(reentrant或re-entrant)的.

若一个函数是可重入的,则该函数:

1. 不能含有静态(全局)非常量数据.
1. 不能返回静态(全局)非常量数据的地址.
1. 只能处理由调用者提供的数据.
1. 不能依赖于单实例模式资源的锁.
1. 不能调用(call)不可重入的函数(有呼叫(call)到的函数需满足前述条件).

多"用户/对象/进程优先级"以及多进程,一般会使得对可重入代码的控制变得复杂.同时, **IO代码通常不是可重入的**,因为他们依赖于像磁盘这样共享的,单独的(类似编程中的静态(Static),全域(Global))资源.

#### errno 变量
errno 变量存在不可重入的问题, 这个整型变量历来每个进程各有一个副本. 但是同一个进程的各个线程共享一个errno 变量.

	if(close(fd) < 0){
		fprintf(stderr,"close error, errno=%d\n", errno);
		exit(1);
	}
	从close 系统调用返回时把错误代码存入errno到稍后由程序显示errno 的值之间存在一个小的时间窗口. 期间同一个进程内的另一个执行线程(例如一个信号处理函数的某次调用) 可能改变了errno 的值.
	就信号处理函数考虑这个问题, 可通过把信号处理函数编写成预先保存并事后恢复errno 的值加以避免, 代码如下:

    void sig_alarm(int signo){
    	int errno_save;
    	errno_save = errno; //save its value en entry
    	if(write(...) != nbytes){
    		fprintf(stderr, "write error, errno=%d\n,errno);
    	errno = errno_save;
    }
	这段代码中我们, 在信号处理函数中调用了fpritnf 这个标准IO 函数, 它引入了另外一个重入问题.

# 守护进程和inetd 超级服务器


    #include"unp.h"
    #include<syslog.h>
    
    #define MAXFD 64
    extern int daemon_proc; /* defined in error.c */
    
    int daemon_init(const char *pname, int facility){
    	int i;
    	pid_t pid;
    
		// 如果本程序是从前台作为一个shell 命令启动的, 当父进程终止时, shell 就认为该命令已执行完毕. 这样子进程就自动在后台运行. 
		// 另外, 子进程继承了父进程的进程组ID, 这就保证了子进程不是一个进程组的头进程, 这是接下去调用setsid的必要条件
    	if ( (pid = Fork()) < 0)
    			return (-1); //error
    	else if (pid)
    			_exit(0);   /* parent terminates */
    
    	/* child 1 continues... */
    
		// 当前进程变为新会话的会话头进程以及新进程组的进程组头进程, 从而不再有控制终端
    	if (setsid() < 0)   /* become session leader */
    			return (-1);

		// 这里必须忽略SIGHUP信号, 因为当会话头进程终止时(即首次fork 产生的子进程) 终止时, 其会话中的所有进程(即在此fork产生的子进程) 都收到SIGHUP信号    
    	Signal(SIGHUP, SIG_IGN);

		// 再次调用fork的目的是确保本守护进程即使将来打开了一个终端设备, 也不会自动获得控制终端.
		// 因为当没有控制终端的会话头进程打开一个终端设备时, 该终端自动成为这个会话头进程的控制终端. 然后再次调用fork 之后, 我们确保新的子进程不再是一个会话头进程, 从而不能自动获得一个控制终端
    	if ( (pid = Fork()) < 0)
    			return (-1);
    	else if (pid)
    			_exit(0);   /* child 1 terminates */
    
    	/* child 2 continues... */
    
    	daemon_proc = 1;/* for err_XXX() functions */
    
		// 使用fork创建的子进程继承了父进程的当前工作目录, 切换到根目录, 可以避免将来的文件系统卸载问题
    	chdir("/"); /* change working directory */
    
    	/* close off file descriptors */
    	for (i = 0; i < MAXFD; i++)
    			close(i);
    
    	/* redirect stdin, stdout, and stderr to /dev/null */
		// 保证这些常用描述符是打开的, 针对他们的系统调用read 返回0(EOF)
    	open("/dev/null", O_RDONLY);
    	open("/dev/null", O_RDWR);
    	open("/dev/null", O_RDWR);
    
    	openlog(pname, LOG_PID, facility);
    
    	return (0); /* success */
    }
	
	example: inetd/daytimetcpserv2.c

进程组:是一个或多个进程的集合.进程组有进程组ID来唯一标识.除了进程号(PID)之外,进程组ID也是一个进程的必备属性.每个进程组都有一个组长进程,其组长进程的进程号等于进程组ID.且该**进程组ID不会因组长进程的退出而受到影响**.  
会话周期:会话期是一个或多个进程组的集合.通常,一个会话开始于用户登录,终止于用户退出,在此期间该用户运行的所有进程都属于这个会话期.

setsid函数用于创建一个新的会话,并担任该会话组的组长.调用setsid有下面的3个作用:

1. 让进程摆脱原会话的控制
1. 让进程摆脱原进程组的控制
1. 让进程摆脱原控制终端的控制

那么,在创建守护进程时为什么要调用setsid函数呢?由于创建守护进程的第一步调用了fork函数来创建子进程,再将父进程退出.由于在调用了fork函数时,子进程全盘拷贝了父进程的会话期,进程组,控制终端等,虽然父进程退出了,但会话期,进程组,控制终端等并没有改变,因此,这还不是真正意义上的独立开来,而setsid函数能够使进程完全独立出来,从而摆脱其他进程的控制.

既然守护进程在没有控制终端的环境下运行, 他绝不会收到来自内核的`SIGHUP` 信号. 许多守护进程因此把这个吸纳后作为来自管理员的一个通知, 表示其配置文件爱你已经发生更改, 需要重新读取配置文件.  
守护进程同样绝不会收到来自内核的`SIGINT` 和`SIGWINCH` 信号, 因此这些信号也可以安全地用作系统管理员的通知手段, 指示守护进程应作出某种反应.

当用户需要外部停止守护进程运行时,往往会使用 kill命令停止该守护进程.所以,守护进程中需要编码来实现kill发出的signal信号处理,达到进程的正常退出

## inetd 守护进程
![inetd 的工作流程](http://blog.chinaunix.net/attachment/201103/15/20196318_1300195218oqhT.jpg)

我们知道通常情况下所有描述符跨`exec` 保持打开, 因此exec加载的实际服务器程序使用描述符0,1,2与客户通信.

	example: 
	lib/daemon_inetd.c: daemon_inetd 函数, 用于由`inetd` 启动的服务器程序中
	inetd/daytimetcpserv3.c: 使用daemon_inted 函数的例子

# 非阻塞式IO
套接字的默认状态是阻塞的. 这就意味着当发出一个不能立即完成的套接字调用时, 其进程将被投入睡眠, 等待相应操作完成.  
可能阻塞的套接字调用可分为以下四类

1. 输入操作: 包括`read, readv, recv, recvfrom, recvmsg` 共5 个函数.  
	- 如果某个进程对一个阻塞的TCP套接字(默认设置) 调用这些输入函数, 而且**该套接字的接收缓冲区中没有数据可读**, 该进程将被投入睡眠, 直到有一些数据到达. 既然TCP 是字节流协议, 该进程的唤醒就是只要有一些数据到达, 可以使单个字节, 也可以是一个完整的TCP分节. 如果想等到某个固定数目的数据可读为止, 那么可以调用我们的`readn` 函数或者指定`MSG_WAITALL` 标志.  
	- 既然UDP是数据报协议, 如果一个阻塞的UDP套接字的接收缓冲区为空, 对它调用输入函数的进程将被投入睡眠, 直到有UDP数据报到达.  
	- 对于非阻塞的套接字, 如果输入操作不能被满足, 相应调用将立即返回一个`EWOULDBLOCK` 错误.
1. 输出操作: 包括`write, writev, send, sendto, sendmsg` 共5 个函数.  
	- 对于TCP, 内核将从应用进程的缓冲区到该套接字的发送缓冲区复制数据. 对于阻塞的套接字, 如果其发送发送缓冲区中没有空间, 进程将被投入睡眠, 直到有空间为止.
	- 对于一个非阻塞的TCP套接字, 如果其发送缓冲区中根本没有空间, 输出操作调用将立即返回一个`EWOULDBLOCK` 错误.
	- UDP套接字不存在真正的发送缓冲区. 内核只是复制应用进程数据并把它沿协议栈向下传送, 渐次冠以UDP首部和IP首部. 因此对一个阻塞的UDP套接字(默认设置), 输出函数调用将不会因与TCP套接字一样的原因二阻塞, 不过有可能会因为其他的原因而阻塞.
1. 接受外来连接, 即`accept`函数
	- 对一个阻塞的套接字调用accept 函数, 并且尚无新的连接到达, 调用进程将被投入睡眠
	- 对一个非阻塞的套接字调用accept函数, 并且尚无新的连接到达, 调用将立即返回一个`EWOULDBLOCK` 错误.
1. 发起外出连接, 即用于TCP的`connect` 函数
	- TCP的建立涉及一个三路握手过程, 而且`connect` 函数一直要等到客户收到对于自己的`SYN`的`ACK` 为止才返回. 这意味着TCP的每个connect 总会阻塞其调用进程至少一个到服务器的RTT时间.
	- 如果对一个非阻塞的TCP套接字调用connect, 并且连接不能立即建立, 那么连接的建立能照样发起(譬如发送出TCP三路握手的第一个分组), 不过会返回一个`EINPROGRESS`错误. 另请注意, 有些连接可以立即建立, 通常是由于服务器和客户同在一个主机, 因此对于一个非阻塞的connect, 我们也得预备connect 成功返回的情况发生.

# 信号驱动式IO
进程预先告知内核, 使得当某个描述符上发生某事时, 内核使用信号通知相关进程.


# 线程
`fork` 是昂贵的. fork要把父进程的内存镜像复制到子进程, 并在子进程中复制所有描述符, 如此, 等等.

统一进程内的所有线程共享

1. 相同的全局内存(也就是全局变量)
1. 打开的文件(即描述符)
1. 信号处理函数和信号处置
1. 进程指令
1. 大多数数据
1. 当前工作目录
1. 用户ID和组ID

不过每个线程拥有各自的

1. 线程ID
1. 寄存器集合, 包括程序计数器和栈指针
1. 栈(用于存放局部变量和返回地址)
1. errno:todo. [ref](http://learn.akae.cn/media/ch35s02.html)
1. 信号掩码
1. 优先级

**运行流程**  
在一个线程中调用`pthread_create()`创建新的线程后,当前线程从`pthread_create()`返回继续往下执行,而新的线程所执行的代码由我们传给`pthread_create`的函数指针`start_routine`决定.  
`start_routine`函数接收一个参数,是通过`pthread_create`的`arg`参数传递给它的.  
`start_routine`返回时,这个线程就退出了,其它线程可以调用`pthread_join`得到`start_routine`的返回值,类似于父进程调用`wait(2)`得到子进程的退出状态,稍后详细介绍`pthread_join`.

如果需要只终止某个线程而不终止整个进程,可以有三种方法:

1. 从线程函数`return`. 既然该函数必须声明成返回一个`void`指针, 它的返回值就是相应线程的终止状态.  
这种方法对主线程不适用,从`main`函数`return`相当于调用`exit`
1. 一个线程可以调用`pthread_cancel`终止同一进程中的另一个线程
1. 线程可以调用`pthread_exit`终止自己
2. 如果进程的`main` 函数返回或者任何线程调用了`exit`, 整个进程就终止, 其中包括它的任何线程

一般情况下,线程终止后,其终止状态一直保留到其它线程调用`pthread_join`获取它的状态为止.  
但是线程也可以被置为`detach`状态,这样的线程一旦终止就**立刻回收它占用的所有资源**,而不保留终止状态.  
不能对一个已经处于`detach`状态的线程调用`pthread_join`,这样的调用将返回`EINVAL`

## 线程特定数据


## 互斥锁 Mutual Exclusive Lock
互斥锁变量(`pthread_mutex_t`类型)是静态分配的(全局变量或static变量), 我们就必须把他初始化为常值`PTHREAD_MUTEX_INITIALIZER`  
如果我们在共享内存区中分配一个互斥锁, 那么必须通过调用`pthread_mutex_init` 函数在运行时把它初始化.

**"挂起等待"和"唤醒等待线程"的操作如何实现**?  
每个Mutex有一个等待队列,一个线程要在Mutex上挂起等待,首先在把自己加入等待队列中,然后置线程状态为睡眠,然后调用调度器函数切换到别的线程.一个线程要唤醒等待队列中的其它线程,只需从等待队列中取出一项,把它的状态从睡眠改为就绪,加入就绪队列,那么下次调度器函数执行时就有可能切换到被唤醒的线程.

## 条件变量 condition variable
互斥锁提供互斥机制, 条件变量提供信号机制

线程间的同步还有这样一种情况:线程A需要等某个条件成立才能继续往下执行,现在这个条件不成立,线程A就阻塞等待,而线程B在执行过程中使这个条件成立了,就唤醒线程A继续执行.

一个Condition Variable总是和一个Mutex搭配使用的.

一个线程可以调用`pthread_cond_wait`在一个Condition Variable上阻塞等待,这个函数做以下三步操作:

1. 释放Mutex
1. 阻塞等待
1. 当被唤醒时,重新获得Mutex并返回

## Semaphore
Mutex变量是非0即1的,可看作一种资源的可用数量,初始化时Mutex是1,表示有一个可用资源,加锁时获得该资源,将Mutex减到0,表示不再有可用资源,解锁时释放该资源,将Mutex重新加到1,表示又有了一个可用资源.

信号量(Semaphore)和Mutex类似,表示可用资源的数量,和Mutex不同的是这个数量可以大于1.

本节介绍的是POSIX semaphore库函数,详见sem_overview(7),这种信号量不仅可用于同一进程的线程间同步,也可用于不同进程间的同步.

## 其它线程间同步机制
如果共享数据是只读的,那么各线程读到的数据应该总是一致的,不会出现访问冲突.只要有一个线程可以改写数据,就必须考虑线程间同步的问题.由此引出了读者写者锁(Reader-Writer Lock)的概念,Reader之间并不互斥,可以同时读共享数据,而Writer是独占的(exclusive),在Writer修改数据时其它Reader或Writer不能访问数据,可见Reader-Writer Lock比Mutex具有更好的并发性.

用挂起等待的方式解决访问冲突不见得是最好的办法,因为这样毕竟会影响系统的并发性,在某些情况下解决访问冲突的问题可以尽量避免挂起某个线程,例如Linux内核的Seqlock,RCU(read-copy-update)等机制.

## pthread API
    #include <pthread.h>
    int pthread_create(pthread_t *restrict thread, const pthread_attr_t *restrict attr, void *(*start_routine)(void*), void *restrict arg);
    返回值:成功返回0,失败返回正的错误号, Exxx
    类似于fork
    
    pthread_t pthread_self(void)
    获得当前线程的id
    类似于getpid
    
    void pthread_exit(void *value_ptr);
    pthread_exit或者return返回的指针所指向的内存单元必须是全局的或者是用malloc分配的,不能在线程函数的栈上分配,因为当其它线程得到这个返回指针时线程函数已经退出了.
    
    int pthread_join(pthread_t thread, void **value_ptr);
    返回值:成功返回0,失败返回错误号
    类似于waitpid
    
    int pthread_detach(pthread_t tid);
    返回值:成功返回0,失败返回错误号

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    int pthread_mutex_destroy(pthread_mutex_t *mutex);
    int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);
	返回值:成功返回0,失败返回错误号

    int pthread_mutex_lock(pthread_mutex_t *mutex);
    int pthread_mutex_unlock(pthread_mutex_t *mutex);
    int pthread_mutex_trylock(pthread_mutex_t *mutex);
    如果一个线程既想获得锁,又不想挂起等待,可以调用pthread_mutex_trylock,如果Mutex已经被另一个线程获得,这个函数会失败返回EBUSY,而不会使线程挂起等待.还可以缓解死锁

    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

    int pthread_cond_destroy(pthread_cond_t *cond);
    int pthread_cond_init(pthread_cond_t *restrict cond, const pthread_condattr_t *restrict attr);
    返回值:成功返回0,失败返回错误号.
    和Mutex的初始化和销毁类似,pthread_cond_init函数初始化一个Condition Variable,attr参数为NULL则表示缺省属性,pthread_cond_destroy函数销毁一个Condition Variable.如果Condition Variable是静态分配的,也可以用宏定义PTHEAD_COND_INITIALIZER初始化,相当于用pthread_cond_init函数初始化并且attr参数为NULL
    
    int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex);
    int pthread_cond_signal(pthread_cond_t *cond);

    int pthread_cond_timedwait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex, const struct timespec *restrict abstime);
    int pthread_cond_broadcast(pthread_cond_t *cond);


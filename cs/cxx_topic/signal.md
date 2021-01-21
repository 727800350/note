# Signal
就是告知某个进程发生了某个事件的通知, 有时也称为软件中断.
信号通常是异步发生的, 也就是说进程预先不知道信号的准确发生时刻

在进程表的表项中有一个软中断信号域,该域中每一位对应一个信号,当有信号发送给进程时,对应位置位被设置.  
由此可以看出,进程对不同的信号可以同时保留,但对于同一个信号,进程并不知道在处理之前来过多少个, 也就是信号不排队(但是现在有排队的信号实现).

在一个信号处理函数运行期间, 正被递交的信号是阻塞的. 而且, 安装处理函数时在传递给sigaction函数的sa_mask 信号集中指定的任何额外信号也被阻塞.

A process can selectively block the receipt of certain signals.
When a signal is blocked, it can be delivered, but the resulting pending signal will not be received until the process unblocks the signal.

一般有3 种方式进行操作

1. `signal(SIGINT ,SIG_IGN)`: `SIG_IGN` 代表忽略信号. SIGINT信号代表由`InterruptKey`产生,通常是`CTRL+C` 或者是`DELETE` .发送给所有ForeGround Group的进程.
2. `signal(SIGINT ,SIG_DFL)`: `SIG_DFL`代表执行系统默认操作,其实对于大多数信号的系统默认动作时终止该进程.这与不写此处理函数是一样的.
3. 自定义的信号或是信号处理函数

网络编程中经常出现, 程序没有任何预兆的就退出了, 一般情况都是没有设置忽略PIPE信号.
在我们的环境中当网络触发broken pipe (一般情况是write的时候, 没有write完毕, 接受端异常断开了), 系统默认的行为是直接退出.
在我们的程序中一般都要在启动的时候加上`signal(SIGPIPE, SIG_IGN);` 来强制忽略这种错误.

## 内核如何实现信号的捕捉
如果信号的处理动作是用户自定义函数, 在信号递达时就调用这个函数, 这称为捕捉信号. 由于信号处理函数的代码是在用户空间的, 处理过程比较复杂:

1. 用户程序注册了SIGQUIT信号的处理函数sighandler.
1. 当前正在执行main函数,这时发生中断或异常切换到内核态.
1. 在中断处理完毕后要返回用户态的`main`函数之前检查到有信号SIGQUIT递达.
1. 内核返回用户态后不是恢复main函数的上下文继续执行, 而是执行sighandler函数. sighandler和main函数使用不同的堆栈空间, 它们之间不存在调用和被调用的关系,是两个独立的控制流程.
1. sighandler函数返回后自动执行特殊的系统调用sigreturn再次进入内核态.
1. 如果没有新的信号要递达,这次再返回用户态就是恢复main函数的上下文继续执行了.

当捕捉到信号时, 不论进程的主控制流程当前执行到哪儿, 都会先跳到信号处理函数中执行, 从信号处理函数返回后再继续执行主控制流程.
信号处理函数是一个单独的控制流程, 因为它和主控制流程是异步的, 二者不存在调用和被调用的关系,并且使用不同的堆栈空间.
引入了信号处理函数使得一个进程具有多个控制流程, 如果这些控制流程访问相同的全局资源(全局变量, 硬件资源等), 就有可能出现冲突.

## Pause
    #include <unistd.h>
    int pause(void);

pause函数使调用进程挂起直到有信号递达.

- 如果信号的处理动作是终止进程,则进程终止,pause函数没有机会返回
- 如果信号的处理动作是忽略,则进程继续处于挂起状态,pause不返回
- 如果信号的处理动作是捕捉,则调用了信号处理函数之后pause返回-1,errno设置为EINTR.

所以pause只有出错的返回值(想想以前还学过什么函数只有出错返回值?).错误码EINTR表示"被信号中断".

    #include <unistd.h>
    #include <signal.h>
    #include <stdio.h>
    
    void sig_alrm(int signo) {
    	/* nothing to do */
    }
    
    unsigned int mysleep(unsigned int nsecs) {
    	struct sigaction newact, oldact;
    	unsigned int unslept;
    
    	newact.sa_handler = sig_alrm;
    	sigemptyset(&newact.sa_mask);
    	newact.sa_flags = 0;
    	sigaction(SIGALRM, &newact, &oldact);
    
    	alarm(nsecs);
    	pause();
    
    	unslept = alarm(0);
    	sigaction(SIGALRM, &oldact, NULL);
    
    	return unslept;
    }
    
    int main(void) {
    	while(1){
    		mysleep(2);
    		printf("Two seconds passed\n");
    	}
    	return 0;
    }

上述代码的运行流程:

1. `main`函数调用`mysleep`函数,后者调用`sigaction`注册了`SIGALRM`信号的处理函数`sig_alrm`.
1. 调用`alarm(nsecs)`设定闹钟.
1. 调用`pause`等待,内核切换到别的进程运行.
1. `nsecs`秒之后,闹钟超时,内核发`SIGALRM`给这个进程.
1. 从内核态返回这个进程的用户态之前处理未决信号,发现有`SIGALRM`信号,其处理函数是`sig_alrm`.
1. 切换到用户态执行`sig_alrm`函数,进入`sig_alrm`函数时`SIGALRM`信号被自动屏蔽,
	从`sig_alrm`函数返回时`SIGALRM`信号自动解除屏蔽.然后自动执行系统调用`sigreturn`再次进入内核,再返回用户态继续执行进程的主控制流程(`main`函数调用的`mysleep`函数).
1. `pause`函数返回-1,然后调用`alarm(0)`取消闹钟,调用`sigaction`恢复`SIGALRM`信号以前的处理动作.

现在重新审视上面"mysleep",设想这样的时序:

1. 注册SIGALRM信号的处理函数.
1. 调用`alarm(nsecs)`设定闹钟.
1. 内核调度优先级更高的进程取代当前进程执行,并且优先级更高的进程有很多个,每个都要执行很长时间
1. nsecs秒钟之后闹钟超时了,内核发送`SIGALRM`信号给这个进程,处于未决状态.
1. 优先级更高的进程执行完了,内核要调度回这个进程执行.`SIGALRM`信号递达,执行处理函数`sig_alrm`之后再次进入内核.
1. 返回这个进程的主控制流程,`alarm(nsecs)`返回,调用`pause()`挂起等待.
**可是SIGALRM信号已经处理完了,还等待什么呢**?

出现这个问题的根本原因是**系统运行的时序(Timing)**并不像我们写程序时所设想的那样.  
虽然alarm(nsecs)紧接着的下一行就是`pause()`,但是无法保证`pause()`一定会在调用`alarm(nsecs)`之后的nsecs秒之内被调用.  
由于异步事件在任何时候都有可能发生(这里的异步事件指出现更高优先级的进程),如果我们写程序时考虑不周密,就可能由于时序问题而导致错误,这叫做**竞态条件(Race Condition)**

使用`sigsuspend` 代替可以解决`pause` 的问题.

## API
函数signal 的正常函数原型: `void (*signal (int signo, void (* func)(int)))(int);`  
为了简化, 我们定义  
```C
typedef void Sigfunc(int);
Sigfunc *signal(int signo, Sigfunc *func); 
func 是指向信号处理函数的指针, 返回值也是指向信号处理函数的指针
```

僵死进程
```C
#include <sys/wait.h>
pid_t wait(int *status);
pid_t waitpid(pid_t pid, int *status, int options); if pid == -1: meaning wait for any child process.
```

Used to wait for **state changes** in a child of the calling process, and obtain information about the child whose state has changed.  
A state change is considered to be: the child terminated; the child was stopped by a signal; or the child was **resumed by a signal**.  
In the case of a terminated child, performing a wait allows the system to release the resources associated with the child;
if a wait is not performed, then the terminated child remains in a "zombie" state
If a child has already changed state, then these calls return immediately.  Otherwise they block until either a child changes state or a signal handler interrupts the call

The value of options is an OR of zero or more of the following constants:

- WNOHANG: return immediately if no child has exited, 最常用的选项
- WUNTRACED: also return if a child has stopped
- WCONTINUED: also return if a stopped child has been resumed by delivery of SIGCONT


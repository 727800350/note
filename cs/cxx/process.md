## fork vs vfork
[fork与vfork的区别](https://www.jianshu.com/p/6a83ac98e77a)

[fork和vfork](https://www.cnblogs.com/1932238825qq/p/7373443.html)

[Linux中fork,vfork和clone详解(区别与联系)](https://blog.csdn.net/gatieme/article/details/51417488)

[fork() gets slower as parent process uses more memory](https://blog.famzah.net/2009/11/20/fork-gets-slower-as-parent-process-use-more-memory/)

There are two main fork() patterns when a parent process wants to execute a command:

1. The parent does not need to communicate with the child process in any way – the child process executes, and the parent gets its exit code back.
  No input/output with the child is done at all, only the inital command line arguments are passed.
1. The parent needs to communicate with the child process – either it needs to supply something at the standard input or some other file descriptor of the child,
  or it wants to get some information from the standard output or some other file descriptor of the child, or both.

For the case when there is no communication involved, Unix guys developed the vfork() call. It is a very light-weight version of fork(), very close to threading.

- fork
  - fork子进程拷贝父进程的数据段, 代码段
  - fork()父子进程的执行次序不确定
- vfork
  - 由vfork创造出来的子进程还会导致父进程挂起,除非子进程exit或者execve才会唤起父进程. 如果在调用这两个函数之前子进程依赖于父进程的进一步动作,则会导致死锁.
  - 由vfok创建出来的子进程共享了父进程的所有内存,包括栈地址,直至子进程使用execve启动新的应用程序为止
  - 由vfork创建出来得子进程不应该使用return返回调用者,或者使用exit()退出,但是它可以使用`_exit()`函数来退出, 如果我们使用return来退出,你会发现程序陷入一种逻辑混乱的重复vfork状态.
- clone
  - clone函数功能强大,带了众多参数,因此由他创建的进程要比前面2种方法要复杂.
  - clone可以让你有选择性的继承父进程的资源,你可以选择想vfork一样和父进程共享一个虚存空间,从而使创造的是线程,你也可以不和父进程共享,你甚至可以选择创造出来的进程和父进程不再是父子关系,而是兄弟关系.

为什么会有vfork,因为以前的fork 很傻, 它创建一个子进程时,将会创建一个新的地址空间,并且拷贝父进程的资源,而往往在子进程中会执行exec 调用,这样,前面的拷贝工作就是白费力气了,
这种情况下,聪明的人就想出了vfork,它产生的子进程刚开始暂时与父进程共享地址空间(其实就是线程的概念了),因为这时候子进程在父进程的地址空间中运行,所以子进程不能进行写操作,
并且在儿子 霸占"着老子的房子时候,要委屈老子一下了,让他在外面歇着(阻塞),一旦儿子执行了exec 或者exit 后,相于儿子买了自己的房子了,这时候就相当于分家了.

很明显,fork太重,而vfork又太危险,所以,就有人开始优化fork这个系统调用.优化的技术用到了著名的写时拷贝(COW).
也就是说,对于fork后并不是马上拷贝内存,而是只有你在需要改变的时候,才会从父进程中拷贝到子进程中,这样fork后立马执行exec的成本就非常小了.
所以,Linux的Man Page中并不鼓励使用vfork().

Under Linux, fork(2) is implemented using copy-on-write pages, so the only penalty incurred by fork(2) is
the time and memory required to duplicate the parent's page tables, and to create a unique task structure for the child.

[A much faster popen() and system() implementation for Linux](https://blog.famzah.net/2009/11/20/a-much-faster-popen-and-system-implementation-for-linux/)

The parent process calling the popen() function(will call fork internally) communicates with the child process by reading its standard output.
Therefore, we cannot use vfork() to speed things up, because it doesn’t allow the child process to close its standard output and duplicate the passed file descriptors from the parent to
its standard output before exec()'uting the command.
A child process created by vfork() can only call exec() right away, nothing more.
If we used threads to re-implement popen(), because the creation of a thread is very light-weight, we couldn't then use exec(),
because invoking exec() from a thread terminates the execution of all other threads, including the parent one.

### 同步进程和异步进程
Tests show that fork() calls on Linux get slower as the parent process uses more memory. So do popen() and system().

- fork, fork-exec, `posix_spawn` 等函数创建的进程都是异步进程, 父进程在成功创建子进程后并不挂起, 两个进程将会各自独立的运行.
- system 创建的进程是同步进程,它会创建一个shell来执行命令或可执行文件.父进程将会被挂起,直到子进程结束,且system调用返回.
- wait, waitpid 函数: 异步进程可以通过执行它将自身挂起,等到子进程结束.当子进程结束后,等待中的父进程收集子进程的退出状态,防止出现僵死进程.

### exit() and `_eixt()`
exit()函数定义在stdlib.h中,而`_exit()`定义在unistd.h中.
exit()和`_exit()`都用于正常终止一个函数.
但`_exit()`直接是一个`sys_exit`系统调用,
而exit()则通常是普通函数库中的一个函数.它会先执行一些清除操作,例如调用执行各终止处理函数,关闭所有标准IO等,然后调用`sys_exit`.


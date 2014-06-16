# gdb

**Shortcuts**  
for `break, delete, run, continue, step, next and print`  
**you need only use the first letter, `b,d,r,c,s,n,and p`**

## GUI
ddd: Display Data Debugger

ddd is just a graphical environment on top of gdb

启动ddd 的三种方式:

	ddd
	ddd <program> core
	ddd <program> <process_id>

ddd runs a number of debuggers under the scenes:

- GNU Debugger: gdb
- Perl Debugger: perl -d
- Bash Debugger: bashdb
- Python Debugger: pydb
- GNU Make Debugger: remake
- dbx: dbx
- Ladebug: ladebug

## Running the program

- run: Reset the program, run (or rerun) from the beginning. You can supply command-line arguments to 'run' the same way you can supply command-line arguments to your executable from the shell.  
**程序的参数是通过run给的**, eg: `(gdb) run -i wlan0 -m 10`

- step: Run next line of source and return to debugger. If a subroutine call is encountered, **follow into that subroutine**

- step count: Run count lines of source

- next: Similar to step, but **doesn't step into subroutines**

- finish: Run until the current function/method returns

- return:	Make selected stack frame return to its caller.

- jump address:	Continue program at specified **line or address**

- quit:	退出调试

## Breakpoints

- info break: Prints a list of all breakpoints with numbers and status

- break function: Place a breakpoint at start of the specified function 

- break linenumber: Place a breakpoint at line, relative to current source file

- break filename:linenumber: Place a breakpoint at the specified line within the specified source file

- break fn if expression: Stop at the breakpoint, only if expression  evaluates to true.  Expression is any valid C expression, evaluated within current stack frame when hitting the breakpoint. 

- disable breaknum: Disable/enable breakpoint identified by breaknum.. 

- enable breaknum: Disable/enable breakpoint identified by breaknum.. 

- delete breaknum: Delete the breakpoint identified by breaknum

- commands breaknum: Specify commands to be executed when breaknum is reached.  The commands can be any list of C statements or gdb commands.  This can be useful to fix code on-the-fly in the debugger without re-compiling

- cont: Continue a program that has been stopped

## Examining Stack

The runtime stack is like a "trail of breadcrumbs" in a program; each time a function call is made, a "crumb is dropped" (an RT stack frame is pushed).  
When a return from a function occurs, the corresponding RT stack frame is popped and discarded.

**gdb assigns numbers to stack frames counting from zero** for the innermost (currently executing) frame
 
- backtrace: Show stack frames, useful to find the calling sequence that produced a crash.

- frame framenumber: Start examining the frame with framenumber.  
This does not change the execution context, but allows to examine variables for a different frame.  每个函数调用在堆栈中称为一帧

- down: Select and print stack frame called by this one. 

- up: Select and print stack frame that called this one

- info args: Show the argument variables of current stack frame. 

- info locals: Show the local variables of current stack frame.

- where: 输出堆栈中函数的调用记录,最近调用的在最前面输出

## Examining source files
- list linenum: Print ten lines centered around linenum in current source file. 

- list function: Print ten lines centered around beginning of function (or method). 

- list: Print ten more lines.

## Examining data
- print expression: Print value of expression. Expression is any valid C expression, can include function calls and arithmetic expressions, all evaluated within current stack frame

- set variable = expression: Assign value of variable to expression.  You can set any variable in the current scope.  Variables which begin with $ can be used as convenience variables in gdb.

- display expression: Print value of expression each time the program stops.  This can be useful to watch the change in a variable as you step through code. 

- undisplay: Cancels previous display requests. 

- what i: 观察变量i 的变化,一旦指定变量发生变化就停止程序

you can specify **how many elements you want to print from a the n-th element** in the array by using this extension to the print command:  
`print array[starting_point]@num_elements_to_print`  
This works on the display command also.  

For example, if you want to print ten elements of the array x starting from the 0th element, you would issue this:  
`print x[0]@10`

### examine
你可以使用examine命令(简写是x)来查看内存地址中的值.x命令的语法如下所示:

	x/ 
	n,f,u是可选的参数.

n 是一个正整数,表示显示内存的长度,也就是说从当前地址向后显示几个地址的内容.  
f 表示显示的格式,参见上面.如果地址所指的是字符串,那么格式可以是s,如果地十是指令地址,那么格式可以是i.  
u表示从当前地址往后请求的字节数,如果不指定的话,GDB默认是4个bytes.u参数可以用下面的字符来代替,b表示单字节,h表示双字节,w表示四字节,g表示八字节.当我们指定了字节长度后,GDB会从指内存定的内存地址开始,读写指定字节,并把其当作一个值取出来.

表示一个内存地址.  
n/f/u三个参数可以一起使用.例如:  
`x/3uh 0x54320` 表示,从内存地址0x54320读取内容,h表示以双字节为一个单位,3表示三个单位,u表示按十六进制显示.

### 输出格式
一般来说,GDB会根据变量的类型输出变量的值.但你也可以自定义GDB的输出的格式.例如,你想输出一个整数的十六进制,或是二进制来查看这个整型变量的中的位的情况.要做到这样,你可以使用GDB的数据显示格式:

- x 按十六进制格式显示变量.
- d 按十进制格式显示变量.
- u 按十六进制格式显示无符号整型.
- o 按八进制格式显示变量.
- t 按二进制格式显示变量.
- a 按十六进制格式显示变量.
- c 按字符格式显示变量.
- f 按浮点数格式显示变量.

	(gdb) p i
	$21 = 101 
	
	(gdb) p/a i
	$22 = 0x65
	
	(gdb) p/c i
	$23 = 101 'e'
	
	(gdb) p/f i
	$24 = 1.41531145e-43
	
	(gdb) p/x i
	$25 = 0x65
	
	(gdb) p/t i
	$26 = 1100101$$$$$$

## [Debugging programs with multiple processes](http://developer.apple.com/library/mac/#documentation/DeveloperTools/gdb/gdb/gdb_5.html)

On most systems, GDB has no special support for debugging programs which create additional processes using the fork function. When a program forks, GDB will continue to debug the parent process and the child process will run unimpeded. If you have set a breakpoint in any code which the child then executes, the child will get a SIGTRAP signal which (unless it catches the signal) will cause it to terminate.

However, if you want to debug the child process there is a workaround which isn't too painful. Put a call to sleep in the code which the child process executes after the fork.

On some systems, GDB provides support for debugging programs that create additional processes using the fork or vfork functions. Currently, the only platforms with this feature are HP-UX (11.x and later only?) and GNU/Linux (kernel version 2.5.60 and later).

By default, when a program forks, GDB will continue to debug the parent process and the child process will run unimpeded.

If you want to follow the child process instead of the parent process, use the command `set follow-fork-mode`.

`set follow-fork-mode mode`  
Set the debugger response to a program call of fork or vfork. A call to fork or vfork creates a new process. The mode argument can be:

- parent  
The original process is debugged after a fork. The child process runs unimpeded. This is the default.

- child  
The new process is debugged after a fork. The parent process runs unimpeded.

`show follow-fork-mode`  
Display the current debugger response to a `fork` or `vfork` call.

On Linux, if you want to debug both the parent and child processes, use the command [set detach-on-fork](http://sourceware.org/gdb/onlinedocs/gdb/Forks.html).

`set detach-on-fork mode`  
Tells gdb whether to detach one of the processes after a fork, or retain debugger control over them both.

- on  
The child process (or parent process, depending on the value of follow-fork-mode) will be detached and allowed to run independently. This is the default. 

- off  
Both processes will be held under the control of gdb. One process (child or parent, depending on the value of follow-fork-mode) is debugged as usual, while the other is held suspended.

`show detach-on-fork`  
Show whether detach-on-fork mode is on/off.

对于**守护进程**, 我们只需要关注child, 可以进行如下设置: `set follow-fork-mode child, set detach-on-fork on` 

If you choose to set 'detach-on-fork' mode off, then gdb will retain control of all forked processes (including nested forks).  
You can list the forked processes under the control of gdb by using the `info inferiors` command, and **switch from one fork to another** by using the `inferior` command (see [Debugging Multiple Inferiors and Programs](https://sourceware.org/gdb/onlinedocs/gdb/Inferiors-and-Programs.html#Inferiors-and-Programs)).

## Thread
[linux gdb-多线程调试](http://blog.csdn.net/lhl_blog/article/details/8888010)

gdb provides these facilities for debugging multi-thread programs:

automatic notification of new threads  
- `thread threadno`, a command to switch among threads
- `info threads`, a command to inquire about existing threads
- `thread apply [threadno] [all] args`, a command to apply a command to a list of threads

thread-specific breakpoints  
- `set print thread-events`, which controls printing of messages on thread start and exit.
- `set libthread-db-search-path path`, which lets the user specify which libthread_db to use if the default choice isn't compatible with the program.

## core dump
[使用gdb和core dump迅速定位段错误](http://my.oschina.net/michaelyuanyuan/blog/68618)

`$ulimit -c unlimited`: 这里就是设置生成的core文件无大小限制  
`$gdb 程序名 core.xxx`  
运行命令: `where`, 即可看到出现段错误的行数了

## Other
### Program's environment
Environment variables conventionally record such things as your user name, your home directory, your terminal type, and your search path for programs to run.

`path directory`  
Add directory to the front of the PATH environment variable (the search path for executables) that will be passed to your program. The value of PATH used by GDB does not change. You may specify several directory names, separated by whitespace or by a system-dependent separator character (`:' on Unix, `;' on MS-DOS and MS-Windows). If directory is already in the path, it is moved to the front, so it is searched sooner. You can use the string `$cwd' to refer to whatever is the current working directory at the time GDB searches the path. If you use `.' instead, it refers to the directory where you executed the path command. GDB replaces `.' in the directory argument (with the current path) before adding directory to the search path.

`show paths`  
Display the list of search paths for executables (the PATH environment variable).

`show environment [varname]`  
Print the value of environment variable varname to be given to your program when it starts. If you do not supply varname, print the names and values of all environment variables to be given to your program. You can abbreviate environment as env.

`set environment varname [=value]`  
Set environment variable varname to value. The value changes for your program only, not for GDB itself. value may be any string; the values of environment variables are just strings, and any interpretation is supplied by your program itself. The value parameter is optional; if it is eliminated, the variable is set to a null value. For example, this command:  
`set env USER = foo`  
tells the debugged program, when subsequently run, that its user is named `foo'. (The spaces around `=' are used for clarity here; they are not actually required.)

`unset environment varname`  
Remove variable varname from the environment to be passed to your program. This is different from `set env varname ='; unset environment removes the variable from the environment, rather than assigning it an empty value.

### input and output
`info terminal`  
Displays information recorded by GDB about the terminal modes your program is using.

You can redirect your program's input and/or output using shell redirection with the run command. For example,  
`run > outfile`  
starts your program, diverting its output to the file `outfile`

### Debugging an already-running process
`attach process-id`  
This command attaches to a running process,one that was started outside GDB.

`detach`  
When you have finished debugging the attached process, you can use the detach command to release it from GDB control.   
Detaching the process continues its execution. After the detach command, that process and GDB become completely independent once more, and you are ready to attach another process or start one with run. detach does not repeat if you press RET again after executing the command.  
If you exit GDB or use the run command while you have an attached process, you kill that process. By default, GDB asks for confirmation if you try to do either of these things;

### Killing the child process
`kill`  
Kill the child process in which your program is running under GDB.
This command is useful if you wish to debug a core dump instead of a running process. GDB ignores any core dump file while your program is running.

On some operating systems, a program cannot be executed outside GDB while you have breakpoints set on it inside GDB. You can use the kill command in this situation to permit running your program outside the debugger.

The kill command is also useful if you wish to recompile and relink your program, since on many systems it is impossible to modify an executable file while it is running in a process. In this case, when you next type run, GDB notices that the file has changed, and reads the symbol table again (while trying to preserve your current breakpoint settings).

# trace
## strace
Strace monitors the system calls and signals of a specific program. It is helpful when you do not have the source code and would like to debug the execution of a program. strace provides you the execution sequence of a binary from start to end.

Each line in the trace contains the system call name, followed by its arguments in parentheses and its return value.

- Trace the Execution of an Executable
You can use strace command to trace the execution of any executable. The following example shows the output of strace for the Linux ls command.

		$  strace ls
		execve("/bin/ls", ["ls"], [/* 21 vars */]) = 0
		brk(0)                                  = 0x8c31000
		access("/etc/ld.so.nohwcap", F_OK)      = -1 ENOENT (No such file or directory)
		mmap2(NULL, 8192, PROT_READ, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0xb78c7000
		access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
		open("/etc/ld.so.cache", O_RDONLY)      = 3
		fstat64(3, {st_mode=S_IFREG|0644, st_size=65354, ...}) = 0
		...
		...

- Be default, strace displays all system calls for the given executable. To display **only a specific system call**, use the strace -e option as shown below.  
`$ strace -e open ls`  
If you want to trace multiple system calls use the `-e trace=` option. The following example displays both open and read system calls.  
`$ strace -e trace=open,read ls /home`

- Save the Trace Execution to a File Using Option `-o`, `-o` best follow the `strace`  
`$ strace -o output.txt ls`  
也可以使用> 进行重定向, 但是必须使用`2>`, 而不是默认的`> == 1>`

- Execute Strace on a Running Linux Process Using Option `-p`    
`$  strace -p 1725 -o output.txt`

- Print Timestamp for Each Trace Output Line Using Option `-t`  
`$ strace -t -e open ls /home`  
Print Relative Time for System Calls Using Option `-r`    
但是需要注意的是在多任务背景下，CPU随时可能会被切换出去做别的事情，所以相对时间不一定准确，此时最好使用 `-T` 
	 $ strace -r ls 
     
- **Generate Statistics Report** of System Calls Using Option `-c`
Using option -c, strace provides useful statistical report for the execution trace. The “calls” column in the following output indicated how many times that particular system call was executed.
	
		$ strace -c ls /home
		bala
		% time     seconds  usecs/call     calls    errors syscall
		------ ----------- ----------- --------- --------- ----------------
		  -nan    0.000000           0         9           read
		  -nan    0.000000           0         1           write
		  -nan    0.000000           0        11           open
		  -nan    0.000000           0        13           close
		  -nan    0.000000           0         1           execve
		  ...

- `strace -f` to trace child process

- `-ff`: If the `-o filename` option is in effect, each processes trace is written to `filename.pid` where pid is the numeric process id of each process. This is incompatible with -c, since no per-process counts are kept.

如果一个系统调用在执行时被另外一个进程调用,strace将保持当前调用的顺序,标记为unfinished,当调用返回时再标记为resumed

	[pid 28772] select(4, [3], NULL, NULL, NULL <unfinished ...>
	[pid 28779] clock_gettime(CLOCK_REALTIME, {1130322148, 939977000}) = 0
	[pid 28772] <... select resumed> )      = 1 (in [3])

## backtrace

	#include <execinfo.h>
	int backtrace(void **buffer, int size);
	char **backtrace_symbols(void *const *buffer, int size);
	void backtrace_symbols_fd(void *const *buffer, int size, int fd);

backtrace() returns a backtrace for the calling program, in the array
pointed to by buffer.  A backtrace is the series of currently active
function calls for the program.  Each item in the array pointed to by
buffer is of type void *, and is the return address from the
corresponding stack frame.  
The size argument specifies the maximum number of addresses that can be stored in buffer.

Given the set of addresses returned by backtrace() in buffer,
backtrace_symbols() translates the addresses into an array of strings
that describe the addresses symbolically.  The size argument
specifies the number of addresses in buffer.

Ex:
	
	// gcc -rdynamic backtrace.c
	// -rdynamic 必须用在-o 的时候, -c的时候可用可不用
	#include <execinfo.h>
	#include <stdio.h>
	#include <stdlib.h>
	
	/* 打印调用栈的最大深度 */
	#define DUMP_STACK_DEPTH_MAX 16
	
	/* 打印调用栈函数 */
	void dump_trace() {
	    void *stack_trace[DUMP_STACK_DEPTH_MAX] = {0};
	    char **stack_strings = NULL;
	    int stack_depth = 0;
	    int i = 0;
	
	    /* 获取栈中各层调用函数地址 */
	    stack_depth = backtrace(stack_trace, DUMP_STACK_DEPTH_MAX);
	    
	    /* 查找符号表将函数调用地址转换为函数名称 */
	    stack_strings = (char **)backtrace_symbols(stack_trace, stack_depth);
	    if (NULL == stack_strings) {
	        printf(" Memory is not enough while dump Stack Trace! \r\n");
	        return;
	    }
	
	    /* 打印调用栈 */
	    printf(" Stack Trace: \r\n");
	    for (i = 0; i < stack_depth; ++i) {
	        printf(" [%d] %s \r\n", i, stack_strings[i]);
	    }
	
	    /* 获取函数名称时申请的内存需要自行释放 */
	    free(stack_strings);
	    stack_strings = NULL;
	    return;
	}
	
	/* 测试函数 2 */
	void test_meloner() {
	    dump_trace();
	    return;
	}
	
	/* 测试函数 1 */
	void test_hutaow() {
	    test_meloner();
	    return;
	}
	
	/* 主函数 */
	int main(int argc, char *argv[]) {
	    test_hutaow();
	    return 0;
	}

Output:

	Stack Trace: 
	[0] ./a.out(dump_trace+0x50) [0x4008b4] 
	[1] ./a.out(test_meloner+0xe) [0x400955] 
	[2] ./a.out(test_hutaow+0xe) [0x400966] 
	[3] ./a.out(main+0x19) [0x400982] 
	[4] /lib64/libc.so.6(__libc_start_main+0xfd) [0x3af001ecdd] 
	[5] ./a.out() [0x4007a9] 

"static" means don't export the symbol...

**-rdynamic**  
Pass the flag -export-dynamic to the ELF linker, on targets that support it. 
This instructs the linker to add all symbols, not only used ones, to the dynamic symbol table. 
This option is needed for some uses of dlopen or to allow obtaining backtraces from within a program.

`-g`是一个编译选项, 即在源代码编译的过程中起作用,让gcc把更多调试信息(也就包括符号信息)收集起来并将存放到最终的可执行文件内  

- 根据调试工具的不同,还能直接选择更有针对性的说明,比如 `-ggdb`  
- 可以被strip掉  
- `.symtab`表在程序加载时会被加载器丢弃(not needed by the running process), 因为gdb等调试工具可以直接访问到磁盘上的二进制程序文件

`-rdynamic` 却是一个 连接选项, 它将指示连接器把所有符号(而不仅仅只是程序已使用到的外部符号)都添加到动态符号表(即.dynsym表)里,以便那些通过 dlopen() 或 backtrace() (这一系列函数使用.dynsym表内符号)这样的函数使用  

- 不能被strip掉, 即强制strip将导致程序无法执行
- backtrace()系列函数作为程序执行的逻辑功能,无法去读取磁盘上的二进制程序文件,因此只能使用`.dynsym`表
- 其它几个工具可以动态指定查看,比如`nm, objdump`

For more info, please reference [Inside ELF Symbol Tables](https://blogs.oracle.com/ali/entry/inside_elf_symbol_tables)

# [tcpdump](http://www.danielmiessler.com/study/tcpdump/)
	sudo tcpdump -i eth1 port 53 -l > dnscap.txt
TcpDump可以将网络中传送的数据包的"头"完全截获下来提供分析.它支持针对网络层,协议,主机,网络或端口的过滤,并提供and,or,not等逻辑语句来帮助你去掉无用的信息

tcpdump存在于基本的 FreeBSD系统中,由于它需要将网络接口设置为混杂模式,普通用户不能正常执行,但具备root权限的用户可以直接执行它来获取网络上的信息.

## A short list of the options mostly used

- -i any : Listen on all interfaces just to see if you're seeing any traffic.
- -n : Don't resolve hostnames to avoid DNS lookups
- -nn : Don't resolve hostnames or port names
- -X : Show the packet's contents in both hex and ASCII
- -XX : Same as -X, but also shows the ethernet header
- -v, -vv, -vvv : Increase the amount of packet information you get back, 比普通的多了个TTL和服务类型等
- -c : Only get x number of packets and then stop
- -S : Print absolute sequence numbers
- -s : Define the snaplength (size) of the capture in bytes.  
Use `-s0` to get everything, unless you are intentionally capturing less.
- -e : Get the ethernet header as well
- -b 在网络层上选择协议,包括ip,arp,rarp,ipx都是这一层的.  
例如:tcpdump -b arp 将只显示网络中的arp即地址转换协议信息
- -l 使标准输出变为缓冲行形式, 可以将数据重定向  
如`tcpdump -l >tcpcap.txt`将得到的数据存入tcpcap.txt文件中
- -w 直接将包写入文件中,并不分析和打印出来, 将来可以用`-r`或其他软件如Wireshark, Snort, etc读取
- -r 从指定的文件中读取包(这些包一般通过-w选项产生)
- -F 从指定的文件中读取**表达式**,忽略其它的表达式

## 表达式 Berkeley Packet Filter (BPF) syntax
表达式是一个正则表达式,tcpdump利用它作为过滤报文的条件,如果一个报文满足表
达式的条件,则这个报文将会被捕获.如果没有给出任何条件,则网络上所有的信息包将会
被截获.

在表达式中一般如下几种类型的关键字,

- 第一种是**关于类型**的关键字,主要包括*host, net, port, portrange*
例如 host 210.27.48.2,指明 210.27.48.2是一台主机,
net 202.0.0.0 指明202.0.0.0是一个网络地址,
port 23 指明端口号是23.如果没有指定类型,缺省的类型是host.  
portrange: `tcpdump portrange 21-23`

- 第二种是**确定传输方向**的关键字,主要包括*src, dst, src or dst, dst and src* ,
这些关键字指明了传输的方向.举例说明,src 210.27.48.2 ,指明ip包中源地址是210.27.
48.2 , dst net 202.0.0.0 指明目的网络地址是202.0.0.0 .如果没有指明方向关键字,则
缺省是src or dst关键字.

- 第三种是**协议**的关键字,主要包括*fddi, ip ,arp, rarp, tcp, udp*等类型.Fddi指明是在
FDDI(分布式光纤数据接口网络)上的特定的网络协议,实际上它是"ether"的别名,fddi和e
ther具有类似的源地址和目的地址,所以可以将fddi协议包当作ether的包进行处理和分析.
其他的几个关键字就是指明了监听的包的协议内容.如果没有指定任何协议,则tcpdump将会
监听所有协议的信息包.

- 除了这三种类型的关键字之外,其他重要的关键字如下:*gateway, broadcast,less,
greater*, 
还有三种逻辑运算,取非运算是 'not ' '! ', 与运算是'and','&&';或运算 是'or' ,'||',

**Packet Size Filter** 
	
	tcpdump less 32 
	tcpdump greater 128
	
	// filtering for size using symbols 
	tcpdump > 32 
	tcpdump <= 128

### Example
(1)想要截获所有210.27.48.1 的主机收到的和发出的所有的数据包:
	
	#tcpdump host 210.27.48.1
(2) 想要截获主机210.27.48.1 和主机210.27.48.2 或210.27.48.3的通信,使用命令
:(在命令行中适用　括号时,一定要
	
	#tcpdump host 210.27.48.1 and \ (210.27.48.2 or 210.27.48.3 \)
(3) 如果想要获取主机210.27.48.1除了和主机210.27.48.2之外所有主机通信的ip包
,使用命令:
	
	#tcpdump ip host 210.27.48.1 and ! 210.27.48.2
	
## 输出信息
基本上tcpdump的总的输出格式为:系统时间 来源主机.端口 > 目标主机.端口 数据包参数

从上面tcpdump的输出可以看出,tcpdump对截获的数据并没有进行彻底解码,数据包内的大部分内容是使用十六进制的形式直接打印输出的.显然这不利于分析网络故障,通常的解决办法是先使用带-w参数的tcpdump 截获数据并保存到文件中,然后再使用其他程序进行解码分析.

- TCP包的输出信息
用TCPDUMP捕获的TCP包的一般输出信息是:
		
		src > dst: flags data-seqno ack window urgent options
src > dst:表明从源地址到目的地址,    
flags是TCP包中的标志信息,S 是SYN标志, F (FIN), P (PUSH) , R (RST) "." (没有标记);   data-seqno是数据包中的数据的顺序号,   
ack是下次期望的顺序号,   
window是接收缓存的窗口大小,   
urgent表明数据包中是否有紧急指针.  
Options是选项.

- UDP包的输出信息
用TCPDUMP捕获的UDP包的一般输出信息是:
		
		route.port1 > ice.port2: udp lenth
UDP十分简单,上面的输出行表明从主机ROUTE的port1端口发出的一个UDP数据包到主机
ICE的port2端口,类型是UDP, 包的长度是lenth

## Advanced
filter based on specific portions of a packet

Show me all URGENT (URG) packets...

	# tcpdump 'tcp[13] & 32!=0'

Show me all ACKNOWLEDGE (ACK) packets...

	# tcpdump 'tcp[13] & 16!=0'

Show me all PUSH (PSH) packets...

	# tcpdump 'tcp[13] & 8!=0'

Show me all RESET (RST) packets...

	# tcpdump 'tcp[13] & 4!=0'

Show me all SYNCHRONIZE (SYN) packets...

	# tcpdump 'tcp[13] & 2!=0'

Show me all FINISH (FIN) packets...

	# tcpdump 'tcp[13] & 1!=0'

Show me all SYNCHRONIZE/ACKNOWLEDGE (SYNACK) packets...

	# tcpdump 'tcp[13]=18'

Note: Only the PSH, RST, SYN, and FIN flags are displayed in tcpdump's flag field output. URGs and ACKs are displayed, but they are shown elsewhere in the output rather than in the flags field

tcp[13] looks at offset 13 in the TCP header, the number represents the location within the byte, and the !=0 means that the flag in question is set to 1, i.e. it's on

Capture TCP Flags Using the tcpflags Option...

	# tcpdump 'tcp[tcpflags] & & tcp-syn != 0'

### Specialized Traffic

Finally, there are a few quick recipes you'll want to remember for catching specific and specialized traffic, such as IPv6 and malformed/likely-malicious packets.

IPv6 traffic

	# tcpdump ip6

Packets with both the RST and SYN flags set (why?)

	# tcpdump 'tcp[13] = 6'

Traffic with the 'Evil Bit' Set

	# tcpdump 'ip[6] & 128 != 0'


# [lsof](http://www.danielmiessler.com/study/lsof/)
lsof: list open files

lsof has a truly staggering number of options. You can use it to get information about devices on your system, what a given user is touching at any given point, or even what files or network connectivity a process is using.

- -U: UNIX socket

- -u: user
	- `-u ^user` //Show what all users are doing except a certain user using
	- # kill -9 \`lsof -t -u daniel\` //Kill everything a given user is doing

- -a: AND, For example, specifying `-a -U -ufoo` produces a listing of only UNIX socket files that belong to processes owned by user foo.  
**Caution: the -a option causes all list selection options to be ANDed;**

- -i: internet address
    - -i6 // IPv6 only
    - -iTCP // Show only TCP connections (works the same for UDP)
    - -i:22 // related to a given port using -i:port
    - -i@172.16.12.5 // Show connections to a specific host using @host 
    - -i@172.16.12.5:22
    - #lsof -i -sTCP:LISTEN or # lsof -i | grep -i LISTEN // Find listening ports
    - # lsof -i -sTCP:ESTABLISHED // Find established connections

- -p: process
	- # lsof -p 10075  // See what a given process ID has open using -p

- -t : get process IDs only

- Files and Directories
	- # lsof /var/log/messages/  // Show everything interacting with a given directory
	- # lsof /home/daniel/firewall_whitelist.txt  // Show everything interacting with a given file


- Similar to tcpdump, the power really shows itself when you start combining queries.
	- # lsof -u daniel -i @1.1.1.1 // Show me everything daniel is doing connected to 1.1.1.1
	- # lsof -i @fw.google.com:2150=2180  // Show open connections with a port range

Items of the **same selection set** - command names, file descriptors, network addresses, process identifiers, user identifiers - are joined in a single ORed set and applied before the result participates in ANDing.  
Thus, for example, specifying `-i@aaa.bbb, -i@ccc.ddd, -a -ufff,ggg` will select the listing of files that belong to either *fff* OR *ggg* AND have network connections to either host *aaa.bbb* OR *ccc.ddd*.  
`-i@aaa.bbb` 与 `-i@ccc.ddd` 属于同一个selection set; 同样, `-ufff` 与`-uggg` 也属于同一个selection set


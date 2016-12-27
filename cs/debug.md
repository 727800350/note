# gcc
## [mudflap](https://gcc.gnu.org/wiki/Mudflap_Pointer_Debugging)
The mudflap run time checker was removed in GCC 4.9 and it is superseded by Address Sanitizer.

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

- next: Similar to step, but **does not step into subroutines**

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
- print expression: Print value of expression. 
Expression is any valid C expression, can include function calls and arithmetic expressions, all evaluated within current stack frame  
`set print elements number-of-elements`, Set a limit on how many elements of an array GDB will print, 如果设置为0, 则全部打印

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

However, if you want to debug the child process there is a workaround which is not too painful. Put a call to sleep in the code which the child process executes after the fork.

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
- `set libthread-db-search-path path`, which lets the user specify which libthread_db to use if the default choice is not compatible with the program.

## core dump
[使用gdb和core dump迅速定位段错误](http://my.oschina.net/michaelyuanyuan/blog/68618)

查看资源限制情况: `ulimit -a`

`$ulimit -c unlimited`: 这里就是设置生成的core文件无大小限制  
`$gdb 程序名 core.xxx`  
运行命令: `where`, 即可看到出现段错误的行数了

如果程序的运行需要root 权限, 那么ulimit的设置也应该在root权限下进行.

ulimit 全局配置文件: `/etc/security/limits.conf`

```
# /etc/security/limits.conf
# Each line describes a limit for a user in the form:
# <domain> <type> <item> <value>
*  soft  core  unlimited
```

还有就是里面某个线程停住,也没死,这种情况一般就是死锁或者涉及消息接受的超时问题(听人说的,没有遇到过).遇到这种情况,可以使用: 
```
gcore pid (调试进程的pid号) 
```
手动生成core文件,在使用pstack(linux下好像不好使)查看堆栈的情况.如果都看不出来,就仔细查看代码,看看是不是在 if,return,break,continue这种语句操作是忘记解锁,还有嵌套锁的问题,都需要分析清楚了

## Other
### Program environment
Environment variables conventionally record such things as your user name, your home directory, your terminal type, and your search path for programs to run.

`path directory`  
Add directory to the front of the PATH environment variable (the search path for executables) that will be passed to your program. 
The value of PATH used by GDB does not change. 
You may specify several directory names, separated by whitespace or by a system-dependent separator character (`:' on Unix, `;' on MS-DOS and MS-Windows). 
If directory is already in the path, it is moved to the front, so it is searched sooner. 
You can use the string `$cwd` to refer to whatever is the current working directory at the time GDB searches the path. 
If you use `.` instead, it refers to the directory where you executed the path command. 
GDB replaces `.` in the directory argument (with the current path) before adding directory to the search path.

`show paths`  
Display the list of search paths for executables (the PATH environment variable).

`show environment [varname]`  
Print the value of environment variable varname to be given to your program when it starts. If you do not supply varname, print the names and values of all environment variables to be given to your program. You can abbreviate environment as env.

`set environment varname [=value]`  
Set environment variable varname to value. The value changes for your program only, not for GDB itself. value may be any string; the values of environment variables are just strings, and any interpretation is supplied by your program itself. The value parameter is optional; if it is eliminated, the variable is set to a null value. For example, this command:  
`set env USER = foo`  
tells the debugged program, when subsequently run, that its user is named `foo`. (The spaces around `=` are used for clarity here; they are not actually required.)

`unset environment varname`  
Remove variable varname from the environment to be passed to your program. This is different from `set env varname =`; unset environment removes the variable from the environment, rather than assigning it an empty value.

### input and output
`info terminal`  
Displays information recorded by GDB about the terminal modes your program is using.

You can redirect your program input and/or output using shell redirection with the run command. For example,  
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

# valgrind
Valgrind is a programming tool for memory debugging, memory leak detection, and profiling.

## Memcheck
The problems Memcheck can detect and warn about include the following:

- Use of uninitialized memory
- Reading/writing memory after it has been freed
- Reading/writing off the end of malloced blocks
- Memory leaks

[memcheck demo](https://www.ibm.com/developerworks/community/blogs/6e6f6d1b-95c3-46df-8a26-b7efd8ee4b57/entry/detect_memory_leaks_with_memcheck_tool_provided_by_valgrind_part_i8?lang=zh_cn)

### report error examples
valgrind --leak-check=yes cmd
```
==25832== Invalid read of size 4
==25832==
at 0x8048724: BandMatrix::ReSize(int, int, int) (bogon.cpp:45)
==25832==
by 0x80487AF: main (bogon.cpp:66)
==25832== Address 0xBFFFF74C is not stack'd, malloc'd or free'd
```
This message says that the program did an illegal 4-byte read of address 0xBFFFF74C, which, as far as Memcheck
can tell, is not a valid stack address, nor corresponds to any current heap blocks or recently freed heap blocks. The
read is happening at line 45 of bogon.cpp, called from line 66 of the same file, etc.

If you want to know how many times each error occurred, run with the -v option. When execution finishes, all the
reports are printed out, along with, and sorted by, their occurrence counts. This makes it easy to see which errors have
occurred most frequently.

```
==26866==    by 0x4E399A9: ??? (in /usr/lib64/libpcap.so.1.5.3)
==26866==    by 0x4E3DE1D: ??? (in /usr/lib64/libpcap.so.1.5.3)
```
The name ??? is used if the file name and/or function name could not be determined from debugging information. 
If most of the entries have the form ???:??? the program probably was not compiled with -g.
包括库的函数

## Scheduling and Multi-Thread Performance 
Threaded programs are fully supported.  
Valgrind **serialises execution** so that only one (kernel) thread is running at a time. 
but it does mean that threaded apps never use more than one CPU simultaneously, even if you have a multiprocessor or multicore machine.
Valgrind does not schedule the threads itself. It merely ensures that only one thread runs at once, using a simple locking scheme. 
The actual thread scheduling remains under control of the OS kernel. 
This is both because Valgrind is serialising the threads, and because the code runs so much slower than normal.  
if you have some kind of concurrency, critical race, locking, or similar, bugs. 
In that case you might consider using the tools Helgrind and/or DRD to track them down.

## Handling of Signals
Valgrind has a fairly complete signal implementation. It should be able to cope with any POSIX-compliant use of signals.
If you are using signals in clever ways (for example, catching SIGSEGV, modifying page state and restarting the instruction), you are probably relying on precise exceptions.
In this case, you will need to use
```
--vex-iropt-register-updates=allregs-at-mem-access
```
or
```
--vex-iropt-register-updates=allregs-at-each-insn.
```
If your program dies as a result of a fatal core-dumping signal, Valgrind will generate its own core file
(vgcore.NNNNN) containing your program state.  
You may use this core file for post-mortem debugging with GDB or similar.  
In the unlikely event that Valgrind itself crashes, the operating system will create a core dump in the usual way.

## other tools
In addition to Memcheck, Valgrind has several other tools:

- None, runs the code in the virtual machine without performing any analysis. Since valgrind itself provides a trace back from a segmentation fault, the none tool provides this traceback at minimal overhead.
- Massif, a heap profiler. The separate GUI massif-visualizer visualizes output from Massif.
- Helgrind and DRD, detect **race conditions** in multithreaded code
- Cachegrind, a cache profiler. The separate GUI KCacheGrind visualizes output from Cachegrind.
- Callgrind, a callgraph analyzer. KCacheGrind can visualize output from Callgrind as well as Cachegrind.
- exp-sgcheck (named exp-ptrcheck prior to version 3.7), an experimental tool to find stack and global array overrun errors which Memcheck cannot find. Some code results in false positives from this tool.
- exp-dhat, dynamic heap analysis tool which analyzes how much memory is allocated and for how long as well as patterns of memory usage.
- exp-bbv, a performance simulator that extrapolates performance from a small sample set.

There are also several externally developed tools available. One such tool is ThreadSanitizer, another detector of race conditions.


# trace
## strace
Strace monitors the system calls and signals of a specific program. It is helpful when you do not have the source code and would like to debug the execution of a program. strace provides you the execution sequence of a binary from start to end.

Each line in the trace contains the system call name, followed by its arguments in parentheses and its return value.

- Trace the Execution of an Executable
You can use strace command to trace the execution of any executable. The following example shows the output of strace for the Linux ls command.

		$  strace ls
		execve("/bin/ls", ["ls"], [/* 21 vars \*/]) = 0
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
但是需要注意的是在多任务背景下,CPU随时可能会被切换出去做别的事情,所以相对时间不一定准确,此时最好使用 `-T` 
	 $ strace -r ls 
     
- **Generate Statistics Report** of System Calls Using Option `-c`
Using option -c, strace provides useful statistical report for the execution trace. The "calls" column in the following output indicated how many times that particular system call was executed.
	
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
buffer is of type void \*, and is the return address from the
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

"static" means do not export the symbol...

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

### [Name mangling in C++](http://en.wikipedia.org/wiki/Name_mangling#Name_mangling_in_C.2B.2B)
**Simple example**  

	int  f (void) { return 1; }
	int  f (int)  { return 0; }
	void g (void) { int i = f(), j = f(0); }
These are distinct functions, with no relation to each other apart from the name. 
If they were natively translated into C with no changes, the result would be an error — C does not permit two functions with the same name. 
The C++ compiler therefore will **encode the type information** in the symbol name, the result being something resembling:

	int  __f_v (void) { return 1; }
	int  __f_i (int)  { return 0; }
	void __g_v (void) { int i = __f_v(), j = __f_i(0); }
Notice that g() is mangled even though there is no conflict; name mangling applies to all symbols.

**example class**

	namespace wikipedia{
	   class article{
	   public:
	      std::string format (void); 
	         /* = _ZN9wikipedia7article6formatEv */
	 
	      bool print_to (std::ostream&); 
	         /* = _ZN9wikipedia7article8print_toERSo */
	 
	      class wikilink{
	      public:
	         wikilink (std::string const& name);
	            /* = _ZN9wikipedia7article8wikilinkC1ERKSs */
	      };
	   };
	}

All mangled symbols begin with `_Z` (note that an underscore followed by a capital is a reserved identifier in C, so conflict with user identifiers is avoided); 
for nested names (including both namespaces and classes), this is followed by 
**N, then a series of <length, id> pairs** (the length being the length of the next identifier), and finally E. 
For example, `wikipedia::article::format` becomes

	_ZN9wikipedia7article6formatE
For functions, this is then followed by the type information; as format() is a void function, this is simply v; hence:

	_ZN9wikipedia7article6formatEv
For print_to, a standard type `std::ostream` (or more properly `std::basic_ostream<char, char_traits<char>>`) is used, 
which has the special alias So; a reference to this type is therefore RSo, with the complete name for the function being:

	_ZN9wikipedia7article8print_toERSo

### [Backtrace for c++](http://charette.no-ip.com:81/programming/2010-01-25_Backtrace/)
[example code](http://charette.no-ip.com:81/programming/2010-01-25_Backtrace/2010-01-25_Backtrace.c)

compiled with gcc:

	$ gcc -rdynamic 2010-01-25_Backtrace.c && ./a.out 
	0: ./a.out(displayBacktrace+0x72) [0x4009d6]
	1: ./a.out(bar+0x9) [0x400a5c]
	....

Here is that same example compile with g++:

	$ g++ -rdynamic 2010-01-25_Backtrace.c && ./a.out 
	0: ./a.out(_Z16displayBacktracev+0x26) [0x4009da]
	1: ./a.out(_Z3barv+0x9) [0x400a4a]
	...

With g++, you have at this point is the mangled names.  
Luckily for us, there is a function that will take a g++ mangled name, and return to us the full prototype including namespace or class if applicable. 
When combined with the backtrace, this can be a useful feature.

	#include <cxxabi.h>
	...
	int status = -1;
	char *demangledName = abi::__cxa_demangle( "_Z16displayBacktracev", NULL, NULL, &status );
	if ( status == 0 ){
		std::cout << demangledName << std::endl;
	}
	free( demangledName );

It is important to note that demangledName is a malloc()'d buffer, so remember to free it.  
Also note the text strings returned from `backtrace_symbols()` cannot be used directly as input to `abi::__cxa_demangle()`. 
You'll need to parse each backtrace line to extract just the mangled name.

# [tcpdump](http://www.danielmiessler.com/study/tcpdump/)
	sudo tcpdump -i eth1 port 53 -l > dnscap.txt
TcpDump可以将网络中传送的数据包的"头"完全截获下来提供分析.它支持针对网络层,协议,主机,网络或端口的过滤,并提供and,or,not等逻辑语句来帮助你去掉无用的信息

tcpdump存在于基本的 FreeBSD系统中,由于它需要将网络接口设置为混杂模式,普通用户不能正常执行,但具备root权限的用户可以直接执行它来获取网络上的信息.

## A short list of the options mostly used
- `-i <device>|any`: Listen on all interfaces just to see if you're seeing any traffic.** 注意: 当是混杂模式的时候, any 无效**
- -l 使标准输出变为缓冲行形式, 可以将数据重定向, 如`tcpdump -l >tcpcap.txt`将得到的数据存入tcpcap.txt文件中
- `-w out_file` 直接将包写入文件中,并不分析和打印出来, 将来可以用`-r`或其他软件如Wireshark, Snort, etc读取. 用`-C num(MB)` 可以限制单个文件的大小

- -n : Don't resolve hostnames to avoid DNS lookups
- -nn : Don't resolve hostnames or port names
- -X : Show the packet's contents in both hex and ASCII
- -XX : Same as -X, but also shows the ethernet header
- -v, -vv, -vvv : Increase the amount of packet information you get back, 比普通的多了个TTL和服务类型等
- -c : Only get x number of packets and then stop
- -S : Print absolute sequence numbers
- -s : Define the snaplength (size) of the capture in **bytes**. Use `-s0` to get everything, unless you are intentionally capturing less.
- -e : Get the ethernet header as well
- -b 在网络层上选择协议,包括ip,arp,rarp,ipx都是这一层的. 例如:tcpdump -b arp 将只显示网络中的arp即地址转换协议信息
- -r 从指定的文件中读取包(这些包一般通过-w选项产生)
- -F 从指定的文件中读取**表达式**,忽略其它的表达式

可以利用tcpdump 来对pcap 进行分割, 以减少单个文件的大小
```
tcpdump -r input.pcap -C 1000 -w ouput
```
但是当tcpdump 从文件读, 而不是从网卡时, -s 选项无效.

## 表达式 Berkeley Packet Filter (BPF) syntax
表达式是一个正则表达式,tcpdump利用它作为过滤报文的条件,如果一个报文满足表
达式的条件,则这个报文将会被捕获.如果没有给出任何条件,则网络上所有的信息包将会
被截获.

在表达式中一般如下几种类型的关键字,

- 第一种是**关于类型**的关键字,主要包括*host, net, port, portrange*
	例如 `host 210.27.48.2`, 指明 210.27.48.2是一台主机; net 202.0.0.0 指明202.0.0.0是一个网络地址
	port 23 指明端口号是23.如果没有指定类型,缺省的类型是host. portrange: `tcpdump portrange 21-23`

- 第二种是**确定传输方向**的关键字,主要包括*src, dst, src or dst, dst and src* , 这些关键字指明了传输的方向.
	举例说明,`src 210.27.48.2`,指明ip包中源地址是210.27.48.2, `dst net 202.0.0.0` 指明目的网络地址是202.0.0.0.
	如果没有指明方向关键字,则缺省是src or dst关键字.

- 第三种是**协议**的关键字,主要包括*fddi, ip ,arp, rarp, tcp, udp*等类型.
	Fddi指明是在FDDI(分布式光纤数据接口网络)上的特定的网络协议,实际上它是"ether"的别名,fddi和ether具有类似的源地址和目的地址,所以可以将fddi协议包当作ether的包进行处理和分析.
	其他的几个关键字就是指明了监听的包的协议内容.如果没有指定任何协议,则tcpdump将会监听所有协议的信息包.

- 除了这三种类型的关键字之外,其他重要的关键字如下:*gateway, broadcast,less, greater*, 还有三种逻辑运算,取非运算是 'not ' '! ', 与运算是'and','&&';或运算 是'or' ,'||'

Example

- 想要截获所有210.27.48.1 的主机收到的和发出的所有的数据包: #tcpdump host 210.27.48.1
- 想要截获主机210.27.48.1 和主机210.27.48.2 或210.27.48.3的通信, #tcpdump host 210.27.48.1 and (210.27.48.2 or 210.27.48.3)
- 如果想要获取主机210.27.48.1除了和主机210.27.48.2之外所有主机通信的ip包 #tcpdump ip host 210.27.48.1 and ! 210.27.48.2
	
## 输出信息
基本上tcpdump的总的输出格式为:`系统时间 来源主机.端口 > 目标主机.端口 数据包参数`

从上面tcpdump的输出可以看出,tcpdump对截获的数据并没有进行彻底解码,数据包内的大部分内容是使用十六进制的形式直接打印输出的.显然这不利于分析网络故障,
通常的解决办法是先使用带-w参数的tcpdump 截获数据并保存到文件中,然后再使用其他程序进行解码分析.

- TCP包的输出信息
用TCPDUMP捕获的TCP包的一般输出信息是:
```
src > dst: flags data-seqno ack window urgent options
src > dst:表明从源地址到目的地址,    
```
- flags是TCP包中的标志信息, S是SYN标志, F(FIN), P(PUSH), R(RST), "." (没有标记)
- data-seqno是数据包中的数据的顺序号
- ack是下次期望的顺序号
- window是接收缓存的窗口大小
- urgent表明数据包中是否有紧急指针
- Options是选项

- UDP包的输出信息
用TCPDUMP捕获的UDP包的一般输出信息是: `route.port1 > ice.port2: udp lenth`
UDP十分简单,上面的输出行表明从主机ROUTE的port1端口发出的一个UDP数据包到主机
ICE的port2端口,类型是UDP, 包的长度是lenth

## Advanced
filter based on specific portions of a packet

- URGENT (URG) packets: `# tcpdump 'tcp[13] & 32!=0'`
- ACKNOWLEDGE (ACK) packets: `# tcpdump 'tcp[13] & 16!=0'`
- PUSH (PSH) packets: `# tcpdump 'tcp[13] & 8!=0'`
- RESET (RST) packets `# tcpdump 'tcp[13] & 4!=0'`
- SYNCHRONIZE (SYN) packets `# tcpdump 'tcp[13] & 2!=0'`
- FINISH (FIN) packets `# tcpdump 'tcp[13] & 1!=0'`
- SYNCHRONIZE/ACKNOWLEDGE (SYNACK) packets `# tcpdump 'tcp[13]=18'`
- Capture TCP Flags Using the tcpflags Option. `# tcpdump 'tcp[tcpflags] && tcp-syn != 0'`
- IPv6 traffic # tcpdump ip6
- Packets with both the RST and SYN flags set # tcpdump 'tcp[13] = 6'
- Traffic with the 'Evil Bit' Set # tcpdump 'ip[6] & 128 != 0'

Note: Only the PSH, RST, SYN, and FIN flags are displayed in tcpdump s flag field output.
URGs and ACKs are displayed, but they are shown elsewhere in the output rather than in the flags field

tcp[13] looks at offset 13 in the TCP header, the number represents the location within the byte, and the !=0 means that the flag in question is set to 1, i.e. it is on
# wireshark
## wireshark filter
- `ip.addr == 10.43.54.65` is equivalent to `ip.src == 10.43.54.65 or ip.dst == 10.43.54.65`
- Show only SMTP (port 25) and ICMP traffic: `tcp.port eq 25 or icmp`

## process pcap file
[Linux下如何过滤,分割以及合并 pcap 文件](http://linux.cn/article-4762-1.html)

[Editcap Guide: 11 Examples To Handle Network Packet Dumps Effectively](http://www.thegeekstuff.com/2009/02/editcap-guide-11-examples-to-handle-network-packet-dumps-effectively/)

capinfos - Prints information about capture files.  
If no options are specified, Capinfos will report all statistics available in "long" format.
```
$ capinfos lotsapackets.cap
File name:           lotsapackets.cap
File type:           Wireshark/tcpdump/... - libpcap
File encapsulation:  Ethernet
Number of packets:   260778
File size:           267802612 bytes
Data size:           263630140 bytes
Capture duration:    204 seconds
Start time:          Mon Apr  4 22:31:52 2011
End time:            Mon Apr  4 22:35:16 2011
Data byte rate:      1290166.92 bytes/sec
Data bit rate:       10321335.37 bits/sec
Average packet size: 1010.94 bytes
Average packet rate: 1276.21 packets/sec
```

### 文件过滤
individual packet numbers separated by whitespace and/or ranges of packet numbers can be specified as start-end,
By default the selected packets with those numbers will **not** be written to the capture file.

editcap can automatically detects the compressed capture file formats. Currently it supports for the gzip format.

The output_dump file will contain all packets except the first 10 packets.
```
editcap -v input_dump output_dump 1-10
```

If the -r flag is specified, the whole packet selection is reversed; in that case only the selected packets will be written to the capture file.
```
但是editcap 也许要将完整的pcap都读过一遍才生成最终的前100个packets
editcap -r input.pcap output.pcap 1-100

The output_dump file will contain first 10 packets and packets from 100 and 200.
editcap -r  -v input_dump output_dump 1-10  100-200
editcap -r -v input_dump.gz output_dump 1-10 100-200
```

Extract packets between a specific timeperiod using option -A and -B
```
editcap -v -A "2009-02-11 11:26:30" -B "2009-02-11 11:27:00"  input_dump output_dump
```

Remove duplicate packets from the output_dump file using option -d
```
editcap -v -d input_dump output_dump
```

使用 "-D < dup-window >" (dup-window可以看成是对比的窗口大小,仅与此范围内的包进行对比)选项可以提取出重复包.
每个包都依次与它之前的 < dup-window > -1 个包对比长度与MD5值,如果有匹配的则丢弃.
```
editcap -D 10 input.pcap output.pcap
```
也可以将 < dup-window > 定义成时间间隔.使用"-w < dup-time-window >"选项,对比< dup-time-window > 时间内到达的包.
```
editcap -w 0.5 input.pcap output.pcap 
```

Remove certain bytes from the bottom of all packets using option -C

This example removes 10 bytes from every packets and writes into the output file. 
```
# editcap -C 10 input_dump output
```

Truncate the packets to the specific length using option -s  
Produces the ouptut_dump file with packets length limited to 100. 
This can be very helpful under lot of situations. 
For example, you can use this method if you want to get only the IP layer of all the packets and does not require other layer.
```
editcap -s 100 -v -A "2009-02-11 11:26:30" -B "2009-02-11 11:27:00"  input_dump.gz output
```

Extract all DNS traffic from our large capture file
```
tshark -r lotsapackets.cap -R dns -w dns.cap
```
Filters can be mixed and matched just like in Wireshark. The example below matches all DNS traffic and all traffic sent to or from TCP port 80.
```
tshark -r lotsapackets.cap -R "dns or tcp.port==80" -w web.cap
```

[查看DNS query 和 response 包](http://stackoverflow.com/questions/20892222/how-to-extract-dns-records-from-cap-files)
```
tshark -nr dns.pcap -T fields -e dns.qry.name -e dns.resp.addr | sort | uniq -c
tshark -nr dns.pcap -R dns -V
```

### 文件分割
Divide the single dump into multiple file and each contains specified number of packets.
```
# editcap -v -c 1000 input_dump output
```

以时间间隔分割 pcap 文件
```
editcap -i <seconds-per-file> <input-pcap-file> <output-prefix> 
```

### 文件合并 mergecap
当合并多个文件时,mergecap 默认将内部的数据包以时间先后来排序.
```
mergecap -w output.pcap input.pcap input2.pcap [input3.pcap . . .]
```

如果要忽略时间戳,仅仅想以命令行中的顺序来合并文件,那么使用 -a 选项即可.  
例如,下列命令会将 input.pcap 文件的内容写入到 output.pcap, 并且将 input2.pcap 的内容追加在后面.
```
mergecap -a -w output.pcap input.pcap input2.pcap 
```

### 其它
Change packet's timestamp (reduce or advance) using option -t
To advances the timestamp of packets to one hour.
```
# editcap -t 3600 input_dump output_dump
```

To reduces the timestamp of packets to 30 minutes,
```
# editcap -t -1800 input_dump output_dump
```

By default the encapsulation type of the dump file is ether. The example below, translates the capture file into ieee-802-11-bsd format
```
editcap -v -T  ieee-802-11-radiotap input_dump output_dump
```


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


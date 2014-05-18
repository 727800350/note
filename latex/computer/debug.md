# gdb

**Shortcuts**  
break, delete, run, continue, step, next and print  
**you need only use the first letter**

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

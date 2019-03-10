- `readelf -s` 查看so a 文件的symbol
- `lsof -p pid` 可以看到这个进程打开的所有文件, 如果文件被其他进程删除, 会有一个删除的标记

# gdb
- `bt`: Show stack frames, useful to find the calling sequence that produced a crash.
- `thread apply all bt`: bt 只能看到一个线程的结果, 有时候core 是其他线程引起的, 这个命令相当于在所有的thread 中执行bt
- `info threads`: 显示所有的线程
- `thread id`: 切换线程
- `up`: Select and print stack frame that called this one, 后面跟一个数字, 可以多级跳
- `down`: Select and print stack frame called by this one

## Running the program
- run(r): run from the beginning.程序的参数是通过run给的, eg: `(gdb) run -n num -f file > outfile`
- step(s): Run next line of source and return to debugger. If a subroutine call is encountered, **follow into that subroutine**
- step count: Run count lines of source
- next(n): Similar to step, but **does not step into subroutines**
- finish: Run until the current function/method returns
- continue(c): execute until a break
- jump address:	Continue program at specified **line or address**
- quit:	退出调试

## Breakpoints
- info break: Prints a list of all breakpoints with numbers and status
- break function: Place a breakpoint at start of the specified function 
- break linenumber: Place a breakpoint at line, relative to current source file
- break filename:linenumber: Place a breakpoint at the specified line within the specified source file
- break fn if expression: Stop at the breakpoint, only if expression  evaluates to true.
- disable breaknum: Disable/enable breakpoint identified by breaknum.. 
- enable breaknum: Disable/enable breakpoint identified by breaknum.. 
- delete breaknum: Delete the breakpoint identified by breaknum
- commands breaknum: Specify commands to be executed when breaknum is reached.

## Examining Stack
The runtime stack is like a "trail of breadcrumbs" in a program; each time a function call is made, a "crumb is dropped" (an RT stack frame is pushed).  
When a return from a function occurs, the corresponding RT stack frame is popped and discarded.

gdb assigns numbers to stack frames counting from zero for the innermost (currently executing) frame
 
- frame framenumber: Start examining the frame with framenumber.  
	This does not change the execution context, but allows to examine variables for a different frame.  每个函数调用在堆栈中称为一帧
- info args: Show the argument variables of current stack frame. 
- info locals: Show the local variables of current stack frame.
- where: 输出堆栈中函数的调用记录,最近调用的在最前面输出

## Examining source files
- list linenum: Print ten lines centered around linenum in current source file. 
- list function: Print ten lines centered around beginning of function (or method). 
- list: Print ten more lines.

## Examining data
- print expression: Print value of expression. 
	- `set print elements number-of-elements`, Set a limit on how many elements of an array GDB will print, 如果设置为0, 则全部打印
	- `print array[index]@num`: print num elements from index
- set variable = expression: Assign value of variable to expression.
- display expression: Print value of expression each time the program stops.
- undisplay: Cancels previous display requests. 
- what i: 观察变量i 的变化,一旦指定变量发生变化就停止程序

### examine
你可以使用examine命令(简写是x)来查看内存地址中的值: `x/nfu`

- n(number) 是一个正整数,表示显示内存单元的个数, 也就是说从当前地址向后显示几个地址的内容
- f(format) 表示显示的格式,参见上面.如果地址所指的是字符串,那么格式可以是s,如果地址是指令地址,那么格式可以是i.  
- u(unit) 一个内存单元的大小. 默认是4个bytes. b表示单字节, h表示双字节, w表示四字节, g表示八字节

eg: `x/3uh 0x54320`: 表示从内存地址0x54320读取内容, h表示以双字节为一个单位, 3 表示三个单位, u表示按无符号十进制显示

### 输出格式
一般来说,GDB会根据变量的类型输出变量的值.但你也可以自定义GDB的输出的格式.

- x 按十六进制格式显示变量.
- d 按十进制格式显示变量.
- u 按十六进制格式显示无符号整型.
- o 按八进制格式显示变量.
- t 按二进制格式显示变量.
- a 按十六进制格式显示变量.
- c 按字符格式显示变量.
- f 按浮点数格式显示变量.

```
(gdb) p i   ##  $21 = 101
(gdb) p/a i ##  $22 = 0x65
(gdb) p/c i ##  $23 = 101 'e'
(gdb) p/f i ##  $24 = 1.41531145e-43
(gdb) p/x i ##  $25 = 0x65
(gdb) p/t i ##  $26 = 1100101$$$$$$
```

## [Debugging programs with multiple processes](http://developer.apple.com/library/mac/#documentation/DeveloperTools/gdb/gdb/gdb_5.html)
By default, when a program forks, GDB will continue to debug the parent process and the child process will run unimpeded.

If you want to follow the child process instead of the parent process, use the command `set follow-fork-mode`.

- `set follow-fork-mode mode`
	- parent: The original process is debugged after a fork. The child process runs unimpeded. This is the default.
	- child: The new process is debugged after a fork. The parent process runs unimpeded.
- `show follow-fork-mode`: Display the current debugger response to a `fork` or `vfork` call.

On Linux, if you want to debug both the parent and child processes, use the command [set detach-on-fork](http://sourceware.org/gdb/onlinedocs/gdb/Forks.html).

- `set detach-on-fork mode`: Tells gdb whether to detach one of the processes after a fork, or retain debugger control over them both.
	- on: The child process (or parent process, depending on the value of follow-fork-mode) will be detached and allowed to run independently. This is the default.
	- off: Both processes will be held under the control of gdb. One process (child or parent, depending on the value of follow-fork-mode) is debugged as usual, while the other is held suspended.
- `show detach-on-fork`: Show whether detach-on-fork mode is on/off.

对于**守护进程**, 我们只需要关注child, 可以进行如下设置: `set follow-fork-mode child, set detach-on-fork on` 

If you choose to set 'detach-on-fork' mode off, then gdb will retain control of all forked processes (including nested forks).  
You can list the forked processes under the control of gdb by using the `info inferiors` command, and **switch from one fork to another** by using the `inferior` command
(see [Debugging Multiple Inferiors and Programs](https://sourceware.org/gdb/onlinedocs/gdb/Inferiors-and-Programs.html#Inferiors-and-Programs)).


## core dump
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

还有就是里面某个线程停住,也没死,这种情况一般就是死锁或者涉及消息接受的超时问题(听人说的,没有遇到过).
遇到这种情况,可以使用: `gcore pid (调试进程的pid号)`. 手动生成core文件, 再使用pstack(linux下好像不好使)查看堆栈的情况.
如果都看不出来,就仔细查看代码,看看是不是在 if,return,break,continue这种语句操作是忘记解锁,还有嵌套锁的问题,都需要分析清楚了

## Debugging an already-running process
- `attach process-id`: This command attaches to a running process, one that was started outside GDB.
- `detach`: used after you have finished debugging the attached process. After the detach command, that process and GDB become completely independent once more

# valgrind
Valgrind is a programming tool for memory debugging, memory leak detection, and profiling.

[ex](http://www.cnblogs.com/skynet/archive/2011/02/20/1959162.html)

```Bash
valgrind --leak-check=full --show-leak-kinds=all ./a.out
```

## Memcheck
The problems Memcheck can detect and warn about include the following:

- Use of uninitialized memory
- Reading/writing memory after it has been freed
- Reading/writing off the end of malloced blocks
- Memory leaks

[memcheck demo](https://www.ibm.com/developerworks/community/blogs/6e6f6d1b-95c3-46df-8a26-b7efd8ee4b57/entry/detect_memory_leaks_with_memcheck_tool_provided_by_valgrind_part_i8?lang=zh_cn)

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

# [tcpdump](http://www.danielmiessler.com/study/tcpdump/)
	sudo tcpdump -i eth1 port 53 -l > dnscap.txt
TcpDump可以将网络中传送的数据包的"头"完全截获下来提供分析.它支持针对网络层,协议,主机,网络或端口的过滤,并提供and,or,not等逻辑语句来帮助你去掉无用的信息

注意: 如果server 和 client 都在一个机器, 数据包是不经过网卡的, 所以tcpdump 捕获不到.

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
表达式是一个正则表达式,tcpdump利用它作为过滤报文的条件,如果一个报文满足表达式的条件,则这个报文将会被捕获.如果没有给出任何条件,则网络上所有的信息包将会被截获.

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

- 想要截获所有210.27.48.1 的主机收到的和发出的所有的数据包: `#tcpdump host 210.27.48.1`
- 想要截获主机210.27.48.1 和主机210.27.48.2 或210.27.48.3的通信, `#tcpdump host 210.27.48.1 and \(210.27.48.2 or 210.27.48.3\)`
- 如果想要获取主机210.27.48.1除了和主机210.27.48.2之外所有主机通信的ip包 `#tcpdump ip host 210.27.48.1 and ! 210.27.48.2`
- 10.207.214.52 与7000 端口的交互: `tcpdump -i xgbe0 \(src host 10.207.214.52 and dst port 7000\) or \(src port 7000 and dst host 10.207.214.52\) -n -w pack.bin`
	
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
- `tcp.port == 25 or icmp`: show only SMTP (port 25) and ICMP traffic
- `tcp.flags.reset == 1`: 筛选出tcp 包中有reset 标记的


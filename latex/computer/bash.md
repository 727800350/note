# 文字编辑
**strings**  
returns each string of printable characters in files. Its main uses are to determine the contents of and to extract text from binary files (i.e., non-text files).


**xargs**  
将参数列表转换成小块分段传递给其他命令,以避免参数列表过长的问题  
Commands like `grep` and `awk` can accept the standard input as a parameter, or argument by using a pipe. 
However, others like `cp` and `echo` disregard the standard input stream and rely solely on the arguments found after the command. 
Additionally, under the Linux kernel before version 2.6.23, arbitrarily long lists of parameters could not be passed to a command, 
so `xargs` breaks the list of arguments into sublists small enough to be acceptable.

下面的命令:  
	
	rm `find /path -type f`
如果path目录下文件过多就会因为"参数列表过长"而报错无法执行.但改用xargs以后,问题即获解决.

	find /path -type f -print0 | xargs -0 rm
本例中xargs将find产生的长串文件列表拆散成多个子串,然后对每个子串调用rm.这样要比如下使用find命令效率高的多

# 程序
**pgrep**  
`pgrep` looks through the currently running processes and **lists the process IDs** which matches the selection criteria to stdout. All the criteria have to match.  
`-n`: Select only the newest (most recently started) of the matching processes.
`shell> strace -c -p $(pgrep -n php-cgi)`

# 网络
**putty 中文乱码**  
Window->Appearance->Font: 选择Courier New  
Window->Translation->Remote character set: UTF-8  
之后, 在Session->Saved Sessions 中起一个名字, 然后Save, 下次使用的时候, 直接load, 就可以了

# File System
`du`: 查看目录大小  
查看某个目录的大小 `/home/master/documents`  
查看目录下所有目录的大小并按大小降序排列:`sudo du -sm /etc/* | sort -nr | less`

`df`: 查看磁盘使用情况  
于du不同的是,du是面向文件的命令,只计算被文件占用的空间.不计算文件系统metadata 占用的空间.df则是基于文件系统总体来计算,通过文件系统中未分配空间来确定系统中已经分配空间的大小

# 杂项
`man number item`: find item in section number, eg: `man 3 fopen`


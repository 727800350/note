## apache 服务配置

如果要访问其他目录,比如`/home/eric/git`, 可以在`/var/www/html` 里面建立软链接.  
但是会提示"Forbidden 没有权限"  
原因: Apache wants access to not just the directory I am serving, `/home/eric/git/`, 
but also every directory above that, namely `/home/eric/`, `/home`, and `/`.
所以需要把它的上级目录也给与x权限

# 系统调用
## 内存管理
`brk, sbrk`: 内存不够用时通过它来申请新内存

`mlock, munlock, mlockall, munlockall` - lock and unlock memory

`mmap, munmap` - map or unmap files or devices into memory  
mmap creates a new mapping in the virtual address space of the calling process.  
munmap system call deletes the mappings for the specified address range, and causes further references to addresses within the range to generate invalid memory references.

`futex` - fast user-space locking  
provides a method for a program to wait for a value at a given address to change, and a method to wake up anyone waiting on a particular address 

`mprotect` - set protection on a region of memory

## 文件系统操作
`access` checks whether the calling process can access the file pathname.

`getdents` - get directory entries

`stat, lstat, fstat` 取文件状态信息

## 文件读写操作
`fcntl` 文件控制

`open, creat, close` 打开, 创建, 关闭文件

`read, write` 读, 写文件

## Socket 套接字

## 系统控制
`getrlimit, setrlimit` 获取, 设置系统资源上限

`void exit_group(int status);` equivalent to exit(2) except that it terminates not only the calling thread, but all threads in the calling process thread group

## 用户管理
`getuid, setuid` 获取, 设置用户标志号

## 信号


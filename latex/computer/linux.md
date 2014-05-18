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

## 用户管理
`getuid, setuid` 获取, 设置用户标志号

## 信号

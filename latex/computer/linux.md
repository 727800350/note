`man number item`: find item in section number, eg: `man 3 fopen`

# File System
`du`: 查看目录大小  
查看某个目录的大小 `/home/master/documents`  
查看目录下所有目录的大小并按大小降序排列：`sudo du -sm /etc/* | sort -nr | less`

`df`: 查看磁盘使用情况  
于du不同的是，du是面向文件的命令，只计算被文件占用的空间。不计算文件系统metadata 占用的空间。df则是基于文件系统总体来计算，通过文件系统中未分配空间来确定系统中已经分配空间的大小

# options
- PIC stands for Position Independent Code

# Error
- Floating Point Exception: 高版本的gcc glibc 编译的程序在低版本的glibc上运行, 可能导致Floating Point Exception运行时错误.
	这是由于高版本gcc在link的时候默认会采用选项`--hash-style=gnu`,而使用这种Hash表的方式可以大大提升动态链结时的效率.
	而老版本的glibc本并未支持,我们只要在在程序连接选项中加上-Wl,--hash-style=sysv就可以解决这个问题了


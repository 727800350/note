[编写Linux C++程序如何影响VIRT(虚存)和RES(实存/常驻内存)](http://blog.csdn.net/huyiyang2010/article/details/7815491)

在Linux命令行中执行top命令,可以查询到所有进程使用的VIRT虚拟内存,RES常驻内存和共享内存SHR.

那么,什么是VIRT虚拟内存,RES常驻内存和共享内存SHR?我们编写的Linux C++程序如何影响它们呢?

查阅资料后,归纳一下.

VIRT

1. 进程"需要的"虚拟内存大小,包括进程使用的库,代码,数据,以及malloc,new分配的堆空间和分配的栈空间等,
2. 假如进程新申请10MB的内存,但实际只使用了1MB,那么它会增长10MB,而不是实际的1MB使用量.
3. VIRT = SWAP + RES

RES

1. 进程当前使用的内存大小,包括**使用中**的malloc,new分配的堆空间和分配的栈空间,但不包括swap out量,
2. 包含其他进程的共享,
3. 如果申请10MB的内存,实际使用1MB,它只增长1MB,与VIRT相反,
4. 关于库占用内存的情况,它只统计加载的库文件所占内存大小.
5. RES = CODE + DATA

SHR:

1. 除了自身进程的共享内存,也包括其他进程的共享内存,
2. 虽然进程只使用了几个共享库的函数,但它包含了整个共享库的大小,
3. 计算某个进程所占的物理内存大小公式:RES - SHR,
4. swap out后,它将会降下来.


```C++
#include <stdio.h>

int main(){
	char *p = new char [1024 * 1024 * 512];
	getchar();
	return 0;
}
```
VIRT包含了new出来的512MB空间, 但是RES不包含该空间.
即刚new出来的空间, 如果没有使用, 会放入SWAP中, 并不在内容中真实的分配物理内存.
+++++++++++++++++++++++++++++++++++++


```C++
#include <stdio.h>
#include <string.h>

int main(){
	char *p = new char [1024 * 1024 * 512];
	memset(p, 0, 1024 * 1024 * 128);
	getchar();
	return 0;
}
```
VIRT包含new出来的512MB空间,RES包含目前使用的memset的128M空间.即new出来的空间,如果只使用部分,则只分配部分物理内存.
+++++++++++++++++++++++++++++++++++++


```C++
#include <stdio.h>

int main(){
	char p[1024 * 1024 * 10];
	getchar();
	return 0;
}
```
没有使用的栈空间,VIRT会包含(没有使用的栈空间会在SWAP中).
+++++++++++++++++++++++++++++++++++++


```C++
#include <stdio.h>
#include <string.h>

int main(){
	char p[1024 * 1024 * 10];
	memset(p, 0, 1024 * 1024 * 10);
	getchar();
	return 0;
}
```
已经使用的栈空间,VIRT和RES都会包含.
+++++++++++++++++++++++++++++++++++++

```C++
#include <stdio.h>
#include <string.h>

int main(){
	char ** pp = new char * [1024];
	for(int i=0;i<1024;i++){
		pp[i] = new char [1024 * 1024 * 512];
		memset(pp[i], 0, 1024 * 1024 * 512);
		printf("p%d\n", i);
		getchar();
	}
	return 0;
}
```
在我的服务器上,当执行到第四个循环时,并且有其他程序占用较大内存的情况下,top结果如下:
出现了swap out的情况.


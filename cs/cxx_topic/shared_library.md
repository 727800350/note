# Linux 下的动态与静态库
查询:

1. `ldd program`: 查询program 使用了哪些动态链接库
1. 使用locate命令找到该库的被默认安装的路径,使用方法是先用updatedb更新一下locate命令的索引库,然后"locate libnet.so"查找就可以了

Linux 中的应用程序以以下两种方式之一链接到外部函数:

- 要么在构建时与静态库`lib*.a`静态地链接,并且将库代码包含在该应用程序的可执行文件里
- 要么在运行时与共享库`lib*.so`(Shared Object)动态地链接. 通过动态链接装入器, 将动态库映射进应用程序的可执行内存中.
	在启动应用程序之前,动态链接装入器将所需的共享目标库映射到应用程序的内存,或者使用系统共享的目标并为应用程序解析所需的外部引用.
	然后应用程序就可以运行了

- `LIBRARY_PATH` is used by gcc before compilation to search for directories containing libraries that need to be linked to your program
- `LD_LIBRARY_PATH`: 运行时动态链接库从这个环境变量指定的路径搜索

当静态库和动态库同名时, gcc 命令将优先使用动态库

需要打印函数调用堆栈时, 都可以使用 `-rdynamic` 来进行, 经过测试都是可以的

## 动态链接库
[添加搜索路径方法步骤](http://blog.sciencenet.cn/blog-402211-745740.html)

1. 修改`/etc/ld.so.conf`
1. `ldconfig`, 强制更新记录so文件的缓存文件(`/etc/ld.so.cache`)

ldconfig几个需要注意的地方

1. `/lib`和`/usr/lib` 是包含在/etc/ld.so.conf 里面的; `/usr/local/lib` 和 `/usr/local/lib64` 不在标准路径之列
1. 如果想在这两个目录以外放lib,但是又不想在/etc/ld.so.conf中加东西(或者是没有权限加东西). 可以export一个全局变量`LD_LIBRARY_PATH`
1. 总之,就是不管做了什么关于library的变动后,最好都ldconfig一下,不然会出现一些意想不到的结果.不会花太多的时间,但是会省很多的事.

### [Demo](http://www.cppblog.com/deane/archive/2014/05/23/165216.html)
hello.h
```C
#ifndef HELLO_H_
#define HELLO_H_

void hello(const char* name);

#endif
```

hello.c
```C
#include <stdio.h>

void hello(const char* name){
	printf("Hello %s!\n", name);
}
```

main.c

```C
#include "hello.h"

int main(){
	hello("everyone");
	return 0;
}
```

创建动态链接库
```Bash
gcc -c -fPIC hello.c  // 这个里面的 -fPIC 也是必须的, 否则进行下面的一个步骤会出错
gcc -shared -fPIC -o libmyhello.so hello.o
```
"PIC"命令行标记告诉GCC产生的代码不要包含对函数和变量具体内存位置的引用,这是因为现在还无法知道使用该消息代码的应用程序会将它连接到哪一段内存地址空间.  
表示编译为位置独立的代码,不用此选项的话编译后的代码是位置相关的所以动态载入时是通过代码拷贝的方式来满足不同进程的需要,而不能达到真正代码段共享的目的

使用
```Bash
gcc -c main.c
gcc -o hello main.o -L. -lmyhello
./hello
```

但是出错`./hello: error while loading shared libraries: libmyhello.so: cannot open shared object file: No such file or directory`
原因: libmyhello.so 不再系统的动态链接库搜索目录中.  
需要把他拷贝到/usr/lib或者其他搜索目录中, 之后运行 `ldconfig`.  
对于这个测试, 可以将这个环境变量设置成当前目录:
```Bash
export LD_LIBRARY_PATH=$(pwd)
./hello
```
我们可以查看hello 依赖了哪些动态链接库:
```
[eric@iis computer]$ ldd hello
linux-vdso.so.1 =>  (0x00007fffdad30000)
libmyhello.so => /home/eric/git/note/latex/computer/libmyhello.so (0x00007f497a5e6000)
libc.so.6 => /lib64/libc.so.6 (0x000000382ec00000)
/lib64/ld-linux-x86-64.so.2 (0x000000382e400000)
```

## 静态链接库
静态函数库实际上就是简单的一个普通的目标文件的集合,一般来说习惯用".a"作为文件的后缀.
可以用ar这个程序来产生静态函数库文件.Ar 是archiver的缩写.  
静态函数库现在已经不在像以前用得那么多了,主要是共享函数库与之相比较有很多的优势的原因.慢慢地,大家都喜欢使用共享函数库了.
不过,在一些场所静态函数库仍然在使用,一来是保持一些与以前某些程序的兼容,二来它描述起来也比较简单.

### Demo
假设有下面几个文件:String.h, Strlen.c, Strlnen.c

String.h
```C
#ifndef _STRING_H_
#define _STRING_H_

int Strlen(char *pStr);
int Strnlen(char *pStr, unsigned long ulMaxLen);

#endif
```

Strlen.c
```
#include<stdio.h>
#include<assert.h>

int Strlen(char *pStr){
    unsigned long ulLength;
    assert(NULL != pStr);
    ulLength = 0;
    while(*pStr++){
        ulLength++;
    }
    return ulLength;
}
```

Strlnen.c
```
#include<stdio.h>
#include<assert.h>
// 函数StrNlen的实现,获取给定字符串的长度,如果输入字符串的长度大于指定的最大长度,则返回最大长度,否者返回字符串的实际长度
int Strnlen(char *pStr, unsigned long ulMaxLen){
    unsigned long ulLength;
    assert(NULL != pStr);
    if(ulMaxLen <= 0){
        printf("Wrong Max Length!\n");
        return -1;
    }
    ulLength = 0;
    while(*pStr++ && ulLength < ulMaxLen){
        ulLength++;
    }
    return ulLength;
}
```

生成静态链接库

1. 利用GCC生成对应目标文件: `#>gcc –c Strlen.c Strnlen.c`
1. 用ar创建一个名字为libstr.a的库文件,并把Strlen.o 和Strnlen.o的内容插入到对应的库文件中. `#>ar –rc libstr.a Strlen.o Strnlen.o`

命令执行成功以后,对应的静态库libstr.a已经成功生成.

rc option of ar

- r Insert into archive
- c Create the archive

静态库的使用
假设有下面的文件要使用对应的的静态库:
1. 编译生成对应的目标文件: `#>gcc -c -I/home/hcj/dir_of_String.h main.c`
	其中`-I/home/hcj/xxxxxxxx`指定对应的头文件的路径.
1. 生成可执行文件: `#>gcc -o main main.o libstr.a`

上面的命令要求libstr.a 在当前目录中.

如果libstr不再当前目录, gcc会在静态库名前加上前缀lib,然后追加扩展名.a得到的静态库文件名来查找静态库文件,
因此,我们在写需要连接的库时,只写名字就可以,如libstr.a的库, 只写:-lstr `#>gcc -o main -Ldir_of_libstr.a main.o -lstr`

main.c
```C
// Stirng static library test
#include "String.h"   //静态库对应函数的头文件, 当然声明也可以写在main.c文件中, 就不需要引用String.h

int main(int argc, char* argv[]){
    char str[] = {"hello world"};
    unsigned long ulLength = 0;
    printf("The string is : %s\n", str);
    ulLength = Strlen(str);
    printf("The string length is : %d(use Strlen)\n", ulLength);
    ulLength = StrNlen(str, 10);
    printf("The string length is : %d(use StrNlen)\n", ulLength);

   	return 0;
}
```


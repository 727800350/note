[运算符](http://www.math.pku.edu.cn/teachers/qiuzy/c/operator.htm)

| 运算符                           | 解释                                 | 结合方式 |
| -------------------------------- | ---------------------------------- | ---- |
| () [] -> .                       | 括号(函数等),数组,两种结构成员访问                | 由左向右 |
| ! ~ ++ -- + - * & (类型) sizeof  | 否定,按位否定,增量,减量,正负号, 间接,取地址,类型转换,求大小 | 由右向左 |
| * / %                            | 乘,除,取模                             | 由左向右 |
| + -                              | 加,减                                | 由左向右 |
| << >>                            | 左移,右移                              | 由左向右 |
| < <= >= >                        | 小于,小于等于,大于等于,大于                    | 由左向右 |
| == !=                            | 等于,不等于                             | 由左向右 |
| &                                | 按位与                                | 由左向右 |
| ^                                | 按位异或                               | 由左向右 |
| |                                | 按位或                                | 由左向右 |
| &&                               | 逻辑与                                | 由左向右 |
| ||                               | 逻辑或                                | 由左向右 |
| ? :                              | 条件                                 | 由右向左 |
| = += -= *= /= &= ^= |= <<= >>=   | 各种赋值                               | 由右向左 |
| ,                                | 逗号(顺序)                             | 由左向右 |
|                                  |                                    |      |

# String
### 字符数组与字符指针
	char *str = "This is a pointer to a string.";
字符指针指向的是一个字符串常量的首地址,即指向字符串的首地址.
	
	char string[ ]="This is an array of char."
string是字符数组,它存放了一个字符串, 编译会自动在string 末尾添加`\0`.

str是一个变量,可以改变str, 使它指向不同的字符串,但不能改变str所指的字符串常量.  
string是一个数组,可以改变数组中保存的内容(但是数组的名字string本身, 它是一个常量, 也就是说string is not assignable), 但是必须一个一个字符的操作, 而不能进行整体操作.  
错误的赋值操作: `string = "This is a NEW string.";`

**"abc"是常量吗?答案是有时是,有时不是.**  

不是常量的情况:"abc"作为字符数组初始值的时候就不是,如
	
	char str[] = "abc";
因为定义的是一个字符数组,所以就相当于定义了一些空间来存放"abc",而又因为字符数组就是把字符一个一个地存放的,
所以编译器把这个语句解析为 `char str[3] = {'a','b','c'};`, 然后补零, 所以最终结果是 `char str[4] = {'a','b','c','/0'};`  
做一下扩展,如果`char str[] = "abc";`, 是在函数内部写的话,那么这里的`"abc/0"`, 因为不是常量,所以应该被放在栈上.
 
是常量的情况: 把"abc"赋给一个字符指针变量时,如
 	
 	char* ptr = "abc";
因为定义的是一个普通指针,并**没有定义空间来存放"abc"**,所以编译器得帮我们找地方来放"abc",
显然,把这里的"abc"当成常量并把它放到程序的常量区是编译器最合适的选择.
所以尽管ptr的类型不是`const char *`,并且`ptr[0] = 'x'`;也能编译通过,但是执行`ptr[0] = 'x';`就会发生运行时异常,因为这个语句试图去修改程序常量区中的东西.  
但是建议的写法应该是`const char* ptr = "abc";`, 这样如果后面写`ptr[0] = 'x'`, 的话编译器就不会让它编译通过,也就避免了上面说的运行时异常.  
又扩展一下,如果`char* ptr = "abc";`, 写在函数体内,那么虽然这里的`"abc/0"`被
放在常量区中,但是ptr本身只是一个普通的指针变量,所以ptr是被放在栈上的, 只不过是它所指向的东西被放在常量区罢了.  

1. 数组的类型是由该数组所存放的东西的类型以及数组本身的大小决定的.  
如`char s1[3]`和`char s2[4]`,s1的类型就是`char[3]`,s2的类型就是`char[4]`  
也就是说尽管s1和s2都是字符数组,但两者的类型却是不同的
1. 字符串常量的类型可以理解为相应字符常量数组的类型,如"abcdef"的类型就可以看成是`const char[7]`
1. 对于函数参数列表中的以数组类型书写的形式参数,编译器把其解释为普通的指针类型,如对于
 	
 		void func(char sa[100],int ia[20],char *p)
则sa的类型为`char*`,ia的类型为`int*`,p的类型为`char*`, 所以我们看到的`string.h` 中的函数的参数都是 `char *` 或者`const char *`类型的, 而不是 `char []`.

`strcpy(ptr2, ptr1)` is equivalent to `while(*ptr2++ = *ptr1++)`
where as strdup is equivalent to

	ptr2 = malloc(strlen(ptr1)+1);
	strcpy(ptr2,ptr1);
**So if you want the string which you have copied to be used in another function (as it is created in heap section) you can use `strdup`, else strcpy is enough.**

The functions `strcpy` and `strncpy` are part of the C standard library and **operate on existing memory**.   
By constrast, `strdup` is a Posix function, and it performs **dynamic memory allocation** for you. It returns a pointer to newly allocated memory into which it has copied the string. But you are now responsible for this memory and must eventually free it.

	char *strdup(const char *s);
	char *strndup(const char *s, size_t n);

- `strchr` 查找字符串
- `strcasecmp, strncasecmp` compare two strings ignoring case
- `strsep, settok` extract token from string

# IO
- `fopen` opens the file whose name is the string pointed to by path and associates a stream with it.  
`FILE *fopen(const char *path, const char *mode);`
- `fdopen` function associates a stream with the existing file descriptor fd  
`FILE *fdopen(int fd, const char *mode);`
- `setbuf, setbuffer, setlinebuf, setvbuf` stream buffering operations in `<stdio.h>`

	#include <unistd.h>
    int dup(int oldfd);
	int dup2(int oldfd, int newfd);
dup() uses  the lowest-numbered unused descriptor for the new descriptor.  
dup2() makes newfd be the copy of oldfd, closing newfd first if necessary  
On success, these system calls return the new descriptor.  On error, -1 is returned, and errno is set appropriately.

# Time
- `time(NULL)` returns a timestamp in seconds (number of seconds since the epoch)  
`#include <time.h>`
-` times(struct tms *buf)` stores the current process times in the struct tms that buf points to  
`#include <sys/times.h>`

**struct timeval**  
The struct timeval structure represents an elapsed time. It is declared in sys/time.h and has the following members:

	struct timeval{
    	long int tv_sec; // the number of whole seconds of elapsed time.
    	long int tv_usec; // the rest of the elapsed time 
	}
	tv_usec(a fraction of a second), represented as the number of 
	microseconds. It is always less than one million.

`time_t` just stores seconds, so  
`time_t time = (time_t)ut_tv.tv_sec;`  
Should work, but since you're just looking for a difference, there's always the magic of subtraction.

## define  
Function macro definitions accept two special operators (`#` and `##`) in the replacement sequence:
If the operator # is used before a parameter is used in the replacement sequence, that parameter is replaced by a **string literal** (as if it were enclosed between double quotes)

    #define str(x) #x
    cout << str(test);
	This would be translated into:
	cout << "test";

The operator `##` concatenates two arguments **leaving no blank spaces** between them:

    #define glue(a,b) a ## b
    glue(c,out) << "test";
    This would also be translated into:
    cout << "test";

"\","#","#@"和"##"

在用`#define` 定义时  
斜杠("\")是用来续行的  
"#"用来把参数转换成字符串,是给参数加上双引号  
"##"则用来连接前后两个参数,把它们变成一个  
"#@"是给参数加上单引号.

下面的例子会使您很容易理解.
	
	#define Conn(x,y) x##y
	#define ToChar(a) #@a 
	#define ToString(x) #x
	 int n = Conn(123,456);   结果就是n=123456;
	 char* str = Conn("asdf", "adf")结果就是 str = "asdfadf";
	 char a = ToChar(1);结果就是a='1';
	 char* str = ToString(123132);就成了str="123132";

`realloc`: 重新分配内存, 但是原来的内容保留下来

# Linux
## [Example of Parsing Arguments with getopt](http://www.gnu.org/software/libc/manual/html_node/Getopt.html)
Normally, `getopt` is called in a loop. When `getopt` returns `-1`, indicating no more options are present, the loop terminates.  
A **switch statement** is used to dispatch on the return value from getopt. In typical use, each case just sets a variable that is used later in the program.  
A second loop is used to process the remaining non-option arguments.

optstring中后面的**冒号表示需要接变量**,并存于optarg中.如果接二个冒号,则表示变量可有可无.

扫描模式:

1. 如果第一个字符为加号: 停止于发现第一个非选项
2. 如果第一个字符为减号:全部扫描,此为默认模式.

碰到'--',则忽略扫描模式,必须停止扫描.

`getopt`返回值:  

1. 成功,返回option,  
1. 结束,返回-1,  
1. 发现不认识的option,返回?号  
1. 碰到option后面缺参数,则返回?号. 或返回:号(optstring中第一个字符(如果有加减号,在加减号后面)为冒号时返回,即为:或+:或-:).

`optarg` 存数据  
`optopt` 存储出错的option(如缺参数),或者不认识的option  
`optind`, 表示下一次运行getopt时将读取数组第optind个

For example:
     
     #include <ctype.h>
     #include <stdio.h>
     #include <stdlib.h>
     #include <unistd.h>
     
     int main (int argc, char **argv){
       int aflag = 0;
       int bflag = 0;
       char *cvalue = NULL;
       int index;
       int c;
     
       opterr = 0;
     
       while ((c = getopt (argc, argv, "abc:")) != -1){
         switch (c){
           case 'a':
             aflag = 1;
             break;
           case 'b':
             bflag = 1;
             break;
           case 'c':
             cvalue = optarg;
             break;
           case '?':
             if (optopt == 'c')
               fprintf (stderr, "Option -%c requires an argument.\n", optopt);
             else if (isprint (optopt))
               fprintf (stderr, "Unknown option `-%c'.\n", optopt);
             else
               fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
             return 1;
           default:
             abort ();
		 } //end switch
       }//end while
     
       printf ("aflag = %d, bflag = %d, cvalue = %s\n", aflag, bflag, cvalue);
     
       for (index = optind; index < argc; index++){
         printf ("Non-option argument %s\n", argv[index]);
       return 0;
     }
	}//end main
Here are some examples showing what this program prints with different combinations of arguments:

     % testopt
     aflag = 0, bflag = 0, cvalue = (null)
     
     % testopt -a -b
     aflag = 1, bflag = 1, cvalue = (null)
     
     % testopt -ab
     aflag = 1, bflag = 1, cvalue = (null)
     
     % testopt -c foo
     aflag = 0, bflag = 0, cvalue = foo
     
     % testopt -cfoo
     aflag = 0, bflag = 0, cvalue = foo
     
     % testopt arg1
     aflag = 0, bflag = 0, cvalue = (null)
     Non-option argument arg1
     
     % testopt -a arg1
     aflag = 1, bflag = 0, cvalue = (null)
     Non-option argument arg1
     
     % testopt -c foo arg1
     aflag = 0, bflag = 0, cvalue = foo
     Non-option argument arg1
     
     % testopt -a -- -b
     aflag = 1, bflag = 0, cvalue = (null)
     Non-option argument -b
     
     % testopt -a -
     aflag = 1, bflag = 0, cvalue = (null)
     Non-option argument -

**chroot**  
[Is chroot a security feature?](https://securityblog.redhat.com/2013/03/27/is-chroot-a-security-feature/)  
chroot: an operation that changes the apparent root directory for the current running process and its children.  
This call changes an ingredient in the pathname resolution process and does nothing else.  
This call does not close open file descriptors, and such file descriptors may allow access to files outside the chroot tree.  
A program that is run in such a modified environment cannot name (and therefore normally not access) files outside the designated directory tree.  
The modified environment is called a "chroot jail".  
This can be useful for: Testing and development and Privilege separation 

This call does not change the current working directory, so that after the call '.' can be outside the tree rooted at '/'.  In particular, the superuser can escape from a "chroot jail" by doing:  
`mkdir foo; chroot foo; cd ..`  
So chrooted programs should relinquish root privileges as soon as practical after chrooting

# Linux 下的动态与静态库
Linux 中的应用程序以以下两种方式之一链接到外部函数:

- 要么在构建时与静态库( lib*.a)静态地链接,并且将库代码包含在该应用程序的可执行文件里
- 要么在运行时与共享库( lib*.so(Shared Object))动态地链接.通过动态链接装入器,将动态库映射进应用程序的可执行内存中.
在启动应用程序之前,动态链接装入器将所需的共享目标库映射到应用程序的内存,或者使用系统共享的目标并为应用程序解析所需的外部引用.现在应用程序就可以运行了

`LIBRARY_PATH` is used by gcc before compilation to search for directories containing libraries that need to be linked to your program.

`LD_LIBRARY_PATH` is used by your program to search for directories containing the libraries after it has been successfully compiled and linked.

EDIT: As pointed below, your libraries can be static or shared. 
If it is static then the code is copied over into your program and you don't need to search for the library after your program is compiled and linked. 
If your library is shared then it needs to be dynamically linked to your program and that's when `LD_LIBRARY_PATH` comes into play.

需要打印函数调用堆栈时, 都可以使用 `-rdynamic` 来进行, 经过测试都是可以的

## 动态链接库
[添加搜索路径方法步骤](http://blog.sciencenet.cn/blog-402211-745740.html)

修改`/etc/ld.so.conf`,添加路径.  
在CentOS 6.3下我看到这个文件实际上是包含了`/etc/ld.so.conf.d/`这个目录下的所有`.conf`文件,因此我们可以在这个路径下面创建一个新的文件,其中写上诸如`/usr/local/lib` 等路径,保存退出.  
切记一定要主动执行命令:`ldconfig`,它会更新记录了系统中有哪些so文件的缓存文件(`/etc /ld.so.cache`)

ldconfig几个需要注意的地方
1. 往/lib和/usr/lib里面加东西,是不用修改/etc/ld.so.conf的,但是完了之后要调一下ldconfig,不然这个library会找不到
2. `/usr/local/lib` 和 `/usr/local/lib64` 居然不在标准路径之列
3. 如果想在这两个目录以外放lib,但是又不想在/etc/ld.so.conf中加东西(或者是没有权限加东西).那也可以,就是export一个全局变 量LD_LIBRARY_PATH,然后运行程序的时候就会去这个目录中找library.一般来讲这只是一种临时的解决方案,在没有权限或临时需要的时 候使用.
4. ldconfig做的这些东西都与运行程序时有关,跟编译时一点关系都没有.编译的时候还是该加-L就得加,不要混淆了.
5. 总之,就是不管做了什么关于library的变动后,最好都ldconfig一下,不然会出现一些意想不到的结果.不会花太多的时间,但是会省很多的事.

### [Demo](http://www.cppblog.com/deane/archive/2014/05/23/165216.html)
hello.h

	#ifndef HELLO_H_
	#define HELLO_H_
	void hello(const char* name);
	#endif

hello.c

	#include <stdio.h>
	void hello(const char* name){
		printf("Hello %s!\n", name);
	}

main.c

	#include "hello.h"
	int main(){
		hello("everyone");
		return 0;
	}

**创建动态链接库**  

	gcc -c -fPIC hello.c  // 这个里面的 -fPIC 也是必须的, 否则进行下面的一个步骤会出错
	gcc -shared -fPIC -o libmyhello.so hello.o
"PIC"命令行标记告诉GCC产生的代码不要包含对函数和变量具体内存位置的引用,这是因为现在还无法知道使用该消息代码的应用程序会将它连接到哪一段内存地址空间.  
表示编译为位置独立的代码,不用此选项的话编译后的代码是位置相关的所以动态载入时是通过代码拷贝的方式来满足不同进程的需要,而不能达到真正代码段共享的目的

**使用**  
	
	gcc -c main.c
	gcc -o hello main.o -L. -lmyhello
	./hello

但是出错

	./hello: error while loading shared libraries: libmyhello.so: cannot open shared object file: No such file or directory
原因: libmyhello.so 不再系统的动态链接库搜索目录中.  
需要把他拷贝到/usr/lib或者其他搜索目录中, 之后运行 `ldconfig`.  
对于这个测试, 可以将这个环境变量设置成当前目录:

	export LD_LIBRARY_PATH=$(pwd)
	./hello
我们可以查看hello 依赖了哪些动态链接库:

	[eric@iis computer]$ ldd hello
    linux-vdso.so.1 =>  (0x00007fffdad30000)
    libmyhello.so => /home/eric/git/note/latex/computer/libmyhello.so (0x00007f497a5e6000)
    libc.so.6 => /lib64/libc.so.6 (0x000000382ec00000)
    /lib64/ld-linux-x86-64.so.2 (0x000000382e400000)

## 静态链接库
静态函数库实际上就是简单的一个普通的目标文件的集合,一般来说习惯用".a"作为文件的后缀.
可以用ar这个程序来产生静态函数库文件.Ar 是archiver的缩写.  
静态函数库现在已经不在像以前用得那么多了,主要是共享函数库与之相比较有很多的优势的原因.慢慢地,大家都喜欢使用共享函数库了.
不过,在一些场所静态函数库仍然在使用,一来是保持一些与以前某些程序的兼容,二来它描述起来也比较简单.

### Demo
假设有下面几个文件:String.h, Strlen.c, Strlnen.c

String.h

	#ifndef _STRING_H_
	#define _STRING_H_
	int Strlen(char *pStr);
	int Strnlen(char *pStr, unsigned long ulMaxLen);
	#endif

Strlen.c

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

Strlnen.c

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

**生成静态链接库**:
利用GCC生成对应目标文件:

	#>gcc –c Strlen.c Strnlen.c
用ar创建一个名字为libstr.a的库文件,并把Strlen.o 和Strnlen.o的内容插入到对应的库文件中.

	#>ar –rc libstr.a Strlen.o Strnlen.o
命令执行成功以后,对应的静态库libstr.a已经成功生成.

rc option of ar

- r Insert the files member... into archive (with replacement). This operation differs from q in that any previously existing members are
deleted if their names match those being added
- c Create the archive


**静态库的使用**:
假设有下面的文件要使用对应的的静态库:
编译生成对应的目标文件:

	#>gcc -c -I/home/hcj/dir_of_String.h main.c 
其中`-I/home/hcj/xxxxxxxx`指定对应的头文件的路径.
生成可执行文件:

	#>gcc -o main main.o libstr.a 
	#>./main
上面的命令要求libstr.a 在当前目录中.

**如果libstr不再当前目录**  
gcc会在静态库名前加上前缀lib,然后追加扩展名.a得到的静态库文件名来查找静态库文件,
因此,我们在写需要连接的库时,只写名字就可以,如libstr.a的库,只写:-lstr

	#>gcc -o main -Ldir_of_libstr.a main.o -lstr
	#>./main

main.c

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

## 顺序
我们回过头看看,发现使用静态库和使用动态库编译成目标程序使用的gcc命令完全一样,
那当静态库和动态库同名时,gcc命令会使用哪个库文件呢?

	# ls
	hello.c hello.h hello.o libmyhello.a libmyhello.so main.c
通过上述最后一条ls命令,可以发现静态库文件libmyhello.a和动态库文件libmyhello.so都已经生成,并都在当前目录中
	
	# gcc -o hello main.c -L. -lmyhello
	# ./hello
	./hello: error while loading shared libraries: libmyhello.so: cannot open shared object file: No such file or directory
从程序hello运行的结果中很容易知道,当**静态库和动态库同名时, gcc命令将优先使用动态库**.

## User related
- `getpwnam()` function returns a pointer to a structure containing the broken-out fields of the record in the **password database** (e.g., the local password file /etc/passwd, NIS, and LDAP) that matches the username name.
- `getpwuid()` function returns a pointer to a structure containing the broken-out fields of the record in the password database that matches the user ID uid.
- `chroot` change root directory


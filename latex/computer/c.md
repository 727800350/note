# String
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

下面的例子会使您很容易理解。
	
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

## User related
- `getpwnam()` function returns a pointer to a structure containing the broken-out fields of the record in the **password database** (e.g., the local password file /etc/passwd, NIS, and LDAP) that matches the username name.
- `getpwuid()` function returns a pointer to a structure containing the broken-out fields of the record in the password database that matches the user ID uid.
- `chroot` change root directory

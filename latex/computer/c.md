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

# Unix network
**IP 的结构**
    
    struct ip{
    #if __BYTE_ORDER == __LITTLE_ENDIAN
    unsigned int ip_hl:4; /* header length */
    unsigned int ip_v:4; /* version */
    #endif
    #if __BYTE_ORDER == __BIG_ENDIAN
    unsigned int ip_v:4; /* version */
    unsigned int ip_hl:4; /* header length */
    #endif
    u_int8_t ip_tos; /* type of service */
    u_short ip_len; /* total length */
    u_short ip_id; /* identification */
    u_short ip_off; /* fragment offset field */
    #define IP_RF 0x8000 /* reserved fragment flag */
    #define IP_DF 0x4000 /* dont fragment flag */
    #define IP_MF 0x2000 /* more fragments flag */
    #define IP_OFFMASK 0x1fff /* mask for fragmenting bits */
    u_int8_t ip_ttl; /* time to live */
    u_int8_t ip_p; /* protocol */
    u_short ip_sum; /* checksum */
    struct in_addr ip_src, ip_dst; /* source and dest address */
    }; 
    

**TCP FLAG 标记**  
基于标记的TCP包匹配经常被用于过滤试图打开新连接的TCP数据包.  
TCP标记和他们的意义如下所列:

* F : FIN - 结束; 结束会话
* S : SYN - 同步; 表示开始会话请求
* R : RST - 复位;中断一个连接
* P : PUSH - 推送; 数据包立即发送
* A : ACK - 应答
* U : URG - 紧急
* E : ECE - 显式拥塞提醒回应
* W : CWR - 拥塞窗口减少

## 地址类API
`gethostbyname`和`gethostbyaddr`这两个函数仅仅支持IPv4,  
`getaddrinfo`函数能够处理名字到地址以及服务到端口这两种转换,返回的是一个`sockaddr`结构的链表而不是一个地址清单. 这些`sockaddr`结构随后可由套接口函数直接使用.如此一来,`getaddrinfo`函数把协议相关性安全隐藏在这个库函数内部.应用程序只要处理由getaddrinfo函数填写的套接口地址结构.该函数在 POSIX规范中定义了.

    #include<netdb.h>
    int getaddrinfo( const char *hostname, const char *service, 
					 const struct addrinfo *hints, struct addrinfo **result );
	返回值:0-成功,非0-出错
参数说明
hostname:一个主机名或者地址串(IPv4的点分十进制串或者IPv6的16进制串)  
service:服务名可以是十进制的端口号,也可以是已定义的服务名称,如ftp,http等  
hints:可以是一个空指针,也可以是一个指向某个addrinfo结构体的指针, 调用者在这个结构中填入关于期望返回的信息类型的暗示. 举例来说:指定的服务既可支持TCP也可支持UDP,所以调用者可以把	hints结构中的ai_socktype成员设置成SOCK_DGRAM使得返回的仅仅是适用于数据报套接口的信息.  
result:本函数通过result指针参数返回一个指向addrinfo结构体链表的指针.  

`getnameinfo` converts a socket address to a corresponding host and service, in a protocol-independent manner.  
It combines the functionality of `gethostbyaddr(3)` and `getservbyport(3)`, but unlike those functions, 
`getnameinfo()` is reentrant and allows programs to eliminate IPv4-versus-IPv6 dependencies.
the inverse is `getaddrinfo(3)`


# #define  
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

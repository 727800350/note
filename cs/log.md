# syslog
## Configuration
Every rule consists of two fields, a selector field and an action field.

### Selectors
The selector field itself again consists of two parts, **a facility and a priority**, separated by a period (''.''). 

The facility is one of the following keywords: `auth, authpriv, cron, daemon, kern, lpr, mail, mark, news, security (same as auth), syslog, user, uucp and local0 throughlocal7`.

The priority is one of the following keywords, in ascending order: `debug, info, notice, warning, warn (same as warning), err, error (same as err), crit, alert, emerg, panic(same as emerg)`.

### Actions
The action field of a rule describes the abstract term ''logfile''.

### Example
	# Store critical stuff in critical
	#
	*.=crit;kern.none            /var/adm/critical
	This will store all messages with the priority crit in the file /var/adm/critical, except for any kernel message.

# zlog
[zlog 使用手册](http://hardysimpson.github.io/zlog/UsersGuide-CN.html)

zlog是一个高可靠性、高性能、**线程安全**、灵活、概念清晰的纯C日志函数库。

事实上，在C的世界里面没有特别好的日志函数库（就像JAVA里面的的log4j，或者C++的log4cxx）。C程序员都喜欢用自己的轮子。printf就是个挺好的轮子，但没办法通过配置改变日志的格式或者输出文件。syslog是个系统级别的轮子，不过速度慢，而且功能比较单调。

zlog在效率、功能、安全性上大大超过了log4c，并且是用c写成的，具有比较好的通用性。

zlog有这些特性：

syslog分类模型，比log4j模型更加直接了当
日志格式定制，类似于log4j的pattern layout
多种输出，包括动态文件、静态文件、stdout、stderr、syslog、用户自定义输出函数
运行时手动、自动刷新配置文件（同时保证安全）
高性能，在我的笔记本上达到25万条日志每秒, 大概是syslog(3)配合rsyslogd的1000倍速度
用户自定义等级
多线程和多进程环境下保证安全转档
精确到微秒
简单调用包装dzlog（一个程序默认只用一个分类）
MDC，线程键-值对的表，可以扩展用户自定义的字段
自诊断，可以在运行时输出zlog自己的日志和配置状态
不依赖其他库，只要是个POSIX系统就成(当然还要一个C99兼容的vsnprintf)

## Configuration
%m - usermessage %n - newline

	[formats]
	simple = "%m%n"
	[rules]
	my_cat.DEBUG    >stdout; simple
	my_cat.*    "/var/log/aa.log", 1M; simple
	> 和 stdout 之间不能有空格等字符

syslog有一个通配符"*",匹配所有的设施(facility).zlog里面也一样,"*"匹配所有分类.这提供了一个很方便的办法来重定向你的系统中各个组件的错误.只要这么写:

	[rules]
	*.error    "/var/log/error.log"

zlog强大而独有的特性是**上下级分类匹配**.如果你的分类是这样的:

	c = zlog_get_category("my_cat");
然后配置文件是这样的

	[rules]
	my_cat.*      "/var/log/my_cat.log"
	my_.NOTICE    "/var/log/my.log"

这两条规则都匹配c分类"my_cat".通配符"_" 表示上级分类. **"my_"是"my_cat"和"my_dog"的上级分类**.还有一个通配符是"!", 表示否.

zlog有6个默认的级别:"DEBUG", "INFO", "NOTICE", "WARN", "ERROR"和"FATAL".就像其他的日志函数库那样, aa.DEBUG意味着任何大于等于DEBUG级别的日志会被输出.当然还有其他的表达式.配置文件中的级别是大小写不敏感的.

<TABLE BORDER=1 CELLSPACING=0 CELLPADDING=1>
<TR><TD ALIGN=center NOWRAP>表达式</TD><TD ALIGN=center NOWRAP>含义</TD></TR>
<TR><TD ALIGN=center NOWRAP>*</TD><TD ALIGN=center NOWRAP>所有等级</TD></TR>
<TR><TD ALIGN=center NOWRAP>aa.debug</TD><TD ALIGN=center NOWRAP>代码内等级&gt;=debug</TD></TR>
<TR><TD ALIGN=center NOWRAP>aa.=debug</TD><TD ALIGN=center NOWRAP>代码内等级==debug</TD></TR>
<TR><TD ALIGN=center NOWRAP>aa.!debug</TD><TD ALIGN=center NOWRAP>代码内等级!=debug</TD></TR>
</TABLE>

### 配置文件的例子

	# comments
	[global]
	strict init = true
	buffer min = 1024
	buffer max = 2MB
	rotate lock file = /tmp/zlog.lock
	default format = "%d.%us %-6V (%c:%F:%L) - %m%n"
	file perms = 600

	[levels]
	TRACE = 10
	CRIT = 130, LOG_CRIT
	
	[formats]
	simple = "%m%n"
	normal = "%d %m%n"
	
	[rules]
	default.*               >stdout; simple
	*.*                     "%12.2E(HOME)/log/%c.log", 1MB*12; simple
	my_.INFO                >stderr;
	my_cat.!ERROR           "/var/log/aa.log"
	my_dog.=DEBUG           >syslog, LOG_LOCAL0; simple
	my_mice.*               $user_define;

全局参数以[global]开头.[]代表一个节的开始,四个小节的顺序不能变,依次为global-levels-formats-rules.这一节可以忽略不写.语法为

`(key) = (value)`  
`strict init`  
如果"strict init"是true，zlog_init()将会严格检查所有的格式和规则，任何错误都会导致zlog_init() 失败并且返回-1。当"strict init"是false的时候，zlog_init()会忽略错误的格式和规则。 这个参数默认为true。

`reload conf period`  
这个选项让zlog能在一段时间间隔后自动重载配置文件。重载的间隔以每进程写日志的次数来定义。当写日志次数到了一定值后，内部将会调用zlog_reload()进行重载。每次zlog_reload()或者zlog_init()之后重新计数累加。因为zlog_reload()是原子性的，重载失败继续用当前的配置信息，所以自动重载是安全的。默认值是0，自动重载是关闭的。

`buffer min, buffer max`  
zlog在堆上为每个线程申请缓存。"buffer min"是单个缓存的最小值，zlog_init()的时候申请这个长度的内存。写日志的时候，如果单条日志长度大于缓存，缓存会自动扩充，直到到"buffer max"。 单条日志再长超过"buffer max"就会被截断。如果 "buffer max" 是 0，意味着不限制缓存，每次扩充为原先的2倍，直到这个进程用完所有内存为止。缓存大小可以加上 KB, MB 或 GB这些单位。默认来说"buffer min"是 1K ， "buffer max" 是2MB。

`rotate lock file`  
这个选项指定了一个锁文件，用来保证多进程情况下日志安全转档。zlog会在zlog_init()时候以读写权限打开这个文件。确认你执行程序的用户有权限创建和读写这个文件。转档日志的伪代码是：

default format
这个参数是缺省的日志格式,默认值为:` "%d %V [%p:%F:%L] %m%n"`
这种格式产生的输出类似这样:  
`2012-02-14 17:03:12 INFO [3758:test_hello.c:39] hello, zlog`

`file perms`  
这个指定了创建日志文件的缺省访问权限。必须注意的是最后的产生的日志文件的权限为"file perms"& ~umask。默认为600，只允许当前用户读写。
				
`fsync period`  
在每条规则写了一定次数的日志到文件后，zlog会调用fsync(3)来让操作系统马上把数据写到硬盘。次数是每条规则单独统计的，并且在zlog_reload()后会被清0。必须指出的是，在日志文件名是动态生成或者被转档的情况下，zlog不能保证把所有文件都搞定，zlog只fsync()那个时候刚刚write()的文件描述符。这提供了写日志速度和数据安全性之间的平衡。例子：
				
### 转换字符
| 字符       | 效果     | 例子        |
| -------- | ---------- | ----------- |
| %c       | 分类名     | aa_bb       |
| %d()     | 打日志的时间.需说明具体的日期格式.就像%d(%F)或者%d(%m-%d %T).如果不跟小括号,默认是%d(%F%T).括号内的格式和 strftime(2)的格式一致.详见5.4.3  | %d(%F) 2011-12-01%d(%m-%d %T) 12-01 17:17:42%d(%T) 17:17:42.035%d 2012-02-14 17:03:12%d()           |
| %E()     | 获取环境变量的值        | %E(USER) simpson                                   |
| %ms      | 毫秒,3位数字字符串取自gettimeofday(2)       | 013        |
| %us      | 微秒,6位数字字符串取自gettimeofday(2)    | 002323                                                                                              |
| %F       | 源代码文件名,来源于__FILE__宏.在某些编译器下 __FILE__是绝对路径|test_hello.c或者在某些编译器下/home/zlog/src/test/test_hello.c  |
| %f       | 源代码文件名，输出%F最后一个’/’后面的部分.当然这会有一定的性能损失  | test_hello.c          |
| %H       | 主机名,来源于 gethostname(2)    | zlog-dev                                  |
| %L       | 源代码行数,来源于__LINE__宏        | 135                                                                                                 |
| %m       | 用户日志,用户从zlog函数输入的日志.           | hello, zlog                                                                                         |
| %M       | MDC (mapped diagnostic context),每个线程一张键值对表,输出键相对应的值.例如 %M(clientNumber),clientNumbe是键| %M(clientNumber) 12345  |
| %n       | 换行符，目前还不支持windows换行符             | \n                                       |
| %p       | 进程ID，来源于getpid()                | 2134                                                                                                |
| %U       | 调用函数名，来自于__func__(C99)或者__FUNCTION__(gcc)，如果编译器支持的话。  | main                 |
| %V       | 日志级别，大写        | INFO                           |
| %v       | 日志级别，小写             | info                                                                                                |
| %t       | 16进制表示的线程ID，来源于pthread_self()"0x%x",(unsigned int) pthread_t    | 0xba01e700        |
| %T       | 相当于%t,不过是以长整型表示的"%lu", (unsigned long) pthread_t     | 140633234859776       |
| %%       | 一个百分号      | %           |
| %[其他字符]  | 解析为错误，zlog_init()将会失败    |            |
|          |        |                |

## zlog API
	/* zlog macros */
	zlog_fatal(cat, format, ...)
	zlog_error(cat, format, ...)
	zlog_warn(cat, format, ...)
	zlog_notice(cat, format, ...)
	zlog_info(cat, format, ...)
	zlog_debug(cat, format, ...)

	/* vzlog macros */
	vzlog_fatal(cat, format, args)
	vzlog_error(cat, format, args)
	vzlog_warn(cat, format, args)
	vzlog_notice(cat, format, args)
	vzlog_info(cat, format, args)
	vzlog_debug(cat, format, args)

	 /* hzlog macros */
	 hzlog_fatal(cat, buf, buf_len)
	 hzlog_error(cat, buf, buf_len)
	 hzlog_warn(cat, buf, buf_len)
	 hzlog_notice(cat, buf, buf_len)
	 hzlog_info(cat, buf, buf_len)
	 hzlog_debug(cat, buf, buf_len) 

返回值  
这些函数不返回.如果有错误发生,详细错误会被写在由环境变量ZLOG_PROFILE_ERROR指定的错误日志里面.

# log4c
## Install
	yum install log4c
	yum install log4c-devel

There are three fundamental types of object in Log4C: categories, appenders and layouts.

You can think of these objects as corresponding to the what, where and how of the logging system.

The convention of using dotted names for categories determines an obvious hierarchy which is used to determine parent and child relationships during logging. These are used because during logging (and this is configurable) a message logged by a category will also be logged to the appenders of any parent categories. 

There is always a root category which represents a conceptual root of the hierarchy of categories.

## Demo
see examples in the tar bar of log4c.

`gcc helloworld.c -o helloworld -llog4c`

如果通过源代码的方式安装的话，上面的gcc 命令可以通过，但是运行时会报错  
`./helloworld: error while loading shared libraries: liblog4c.so.3: cannot open shared object file: No such file or directory`

log4c的配置文件名为：`log4crc`，是一个XML文件，存放在工程目录(可执行文件所在的目录)中即可

## Data structures

	struct __log4c_category {
		char* cat_name;
		int   cat_priority;
		int   cat_additive;
		const log4c_category_t*   cat_parent;
		log4c_appender_t*     cat_appender;
	};
	typedef struct __log4c_category log4c_category_t;

	struct __log4c_appender	{
		char*             app_name;
		const log4c_layout_t*     app_layout;
		const log4c_appender_type_t*  app_type;
		int                   app_isopen;
		void*             app_udata;
	};
	typedef struct __log4c_appender log4c_appender_t;

	typedef struct log4c_appender_type {
	    const char*	  name;
	    int (*open)	  (log4c_appender_t*);
	    int (*append) (log4c_appender_t*, const log4c_logging_event_t*);
	    int (*close)  (log4c_appender_t*);
	} log4c_appender_type_t;

## log4c日志库的再封装
[log4c日志库的用法，再封装及其完整例子](http://blog.csdn.net/fdl19881/article/details/8192363)

the original logit function in softflowd, it use the syslog

    void logit(int level, const char *fmt,...){
        va_list args;
    
        va_start(args, fmt);
    
        if (logstderr) {
            vfprintf(stderr, fmt, args);
            fputs("\n", stderr);
        } else
            vsyslog(level, fmt, args);
    
        va_end(args);
    }

Now, I want to use *log4c*, instead *syslog*.

Example of using `log4c_category_log`

`log4c_category_log(mycat, LOG4C_PRIORITY_ERROR, "Hello World.");`

The function prototype:

    static void log4c_category_log(const log4c_category_t* a_category,
                          int a_priority,
                          const char* a_format,
                          ...)
    {
        if (log4c_category_is_priority_enabled(a_category, a_priority)) {
        va_list va; 
        va_start(va, a_format);
        log4c_category_vlog(a_category, a_priority, a_format, va);
        va_end(va);
        }   
    }

We can see that `log4c_category_log` calls `log4c_category_vlog`.

Please take attention to the `log4c_category_vlog`, its prototype is:

    static LOG4C_INLINE void log4c_category_vlog(const log4c_category_t* a_category, 
                           int a_priority,
                           const char* a_format, 
                           va_list a_args)

**封装**  

    #define LOG_NOTICE LOG4C_PRIORITY_NOTICE
    #define LOG_ERR LOG4C_PRIORITY_ERROR
    
    static log4c_category_t *mycat = NULL;
    
    void logit(log4c_priority_level_t level, const char *fmt,...) {    
        va_list args;
        va_start(args, fmt);
        log4c_category_vlog(mycat, level, fmt, args);
        va_end(args);
    }

In the definition of `logit`, `log4c_category_vlog` can not be changed to `log4c_category_log`, cause **the parameters of these two functions are different**.

Example of using `logit`:

`logit(LOG_ERR, "logit error test");`

## Multi threaded applications using Log4C
Log4C is not Multi thread safe (MT safe)

In the meantime, it is possible to use Log4C safely provided you follow some rules related to initialization and provided the appenders and layouts you use are themselves MT safe.

### Why the Log4C API is not fully MT safe

- `log4c_init()` This function must be called successfully just once by the application. If two threads call this at the same time the behavior is undefined. The problem is that both threads will start to initialize global objects and the behavior is this case is undefined.

- `log4c_fini()` The same is true of `log4c_fini()` — this function can safely be called by only one thread at a time. 

- Readonly type Log4C calls like `log4c_category_is_priority_enabled()` and so on are MT safe.

- Lookup and creation type calls like `log4c_category_get()`, `log4c_appender_get()` , `log4c_layout_get()` and so on are not MT safe, even between themselves.

- Calls to `log4c_category_log()` are not MT safe when called on the same categories. The problem is that `log4c_category_log()` eventually calls `log4c_appender_append()` to log the message. However, if the appender has not already been opened then it calls `log4c_appender_open()`, which is not MT safe. The result could be that the two threads will to open the appender at the same time which will give rise to undefined behavior.

### How to use Log4C in an MT safe way
It can be seen from the last section that if you can ensure the following two conditions then you should be able to use Log4C safely in an MT environment:

1. perform the initialization of Log4C and the creation and intialization of all the log4C logging objects before before starting up the multiple threads that call Log4C.

2. Ensure that there are no logging calls in flight when `log4c_fini()` is called.

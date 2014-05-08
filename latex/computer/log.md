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

# log4c
## Install
	yum install log4c
	yum install log4c-devel

There are three fundamental types of object in Log4C: categories, appenders and layouts.

You can think of these objects as corresponding to the what, where and how of the logging system.

The convention of using dotted names for categories determines an obvious hierarchy which is used to determine parent and child relationships during logging. These are used because during logging (and this is configurable) a message logged by a category will also be logged to the appenders of any parent categories. 

There is always a root category which represents a conceptual root of the hierarchy of categories.

## Demo

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

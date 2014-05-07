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

## Demo

`gcc helloworld.c -o helloworld -llog4c`

如果通过源代码的方式安装的话，上面的gcc 命令可以通过，但是运行时会报错  
`./helloworld: error while loading shared libraries: liblog4c.so.3: cannot open shared object file: No such file or directory`

log4c的配置文件名为：`log4crc`，是一个XML文件，存放在工程目录(可执行文件所在的目录)中即可

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

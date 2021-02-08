- [Oracle 基本术语](#oracle-基本术语)
- [SQL PLUS](#sql-plus)
- [数据库空间管理](#数据库空间管理)
	- [以v$ 开头的是动态视图, 系统在运行时内容会改变](#以v-开头的是动态视图-系统在运行时内容会改变)

# Oracle 基本术语
SGA 系统全局区
PGA 程序全局区

数据文件
一个数据库可拥有多个数据文件, 但一个数据文件只对应一个数据库
一个表空间(数据库存储的逻辑单位)可以由一个或多个数据文件组成
数据文件的大小可以用字节和数据块两种方式表示
当数据被修改或是插入新数据时, 先存储在oracle的内存存储区中, 由Oracle的后台进程DBWR来决定何时将其写入数据文件中, 提高IO效率

控制文件
二进制文件
维护着数据库的全局物理结构, 用以支持数据库成功地启动和运行
在数据库使用中, Oracle不断地更新该数据库对应的控制文件, 所以控制文件需是可写状态

日志文件
也称为重做日志文件 Redo Log File
分为归档模式(archivelog)与非归档模式(noarchivelog)

- V$LOG 可以确定系统在使用哪一个日志文件组
- V$LOGFILE 进一步确定是改组的哪个日志文件
- alter system switch logfile 强行进行日志切换
- v$database 查询数据库运行在何种模式下
- v$log_history 历史日志的信息

数据文件, 控制文件, 日志文件还有一些其他文件(如参数文件, 备份文件等)构成了Oracle数据库的物理存储结构
表空间tablespace
一个数据库中通常包含system, sysaux, temp 三个默认表空间, 一个或者多个临时表空间还有一个撤销表空间和几个应用程序专用表空间
表空间的状态: 读写和只读(对任何用户, 包括DBA, 保护数据), 脱机状态(暂时不能访问)

段segment
区extent: 物理上连续存放的块构成
数据块block: 最小的数据管理单位
	快头部
	表目录
	行目录
	空闲空间
	行空间
多个数据块组成区, 多个区组成段, 多个段组成表空间, 多个表空间组成数据库

Oracle 实例: SGA + 后台进程

https://localhost:1158/em
em 是enterprise manager

- redo log
- desc v$log;
- select group#,status from v$log;
- alter system switch logfile;
- oracle 实例: 后台进程加上内存
- LGWR 实例的后台进程

# SQL PLUS
- Show
- show release –显示数据库的版本
- show user –显示当前连接的用户
- show sqlcode –显示数据库操作之后的状态代码
- show parameter sga; 显示参数 oracle采用的是模糊匹配
- desc v$parameter; 查看所有参数
- select name, value from v$parameter;
- show parameter spfile;
- create pfile from spfile;

- show parameter session; 表示数据库可以连接多少个用户
- alter system set session = 250; 让他立即生效, 但是有可能被提示不能立即生效
- alter system set session = 250 scope = spfile; 对于不能立即生效的参数将其写到spfile中, 待数据库重启后,再生效
	scope 默认为both(spfile, memory), 将memory和spfile两处的参数都修改了, scope=memory 只在内存中修改, 而spfile 中的参数不修改


SQL PLUS no need to end a sql plus command with a semicolon.
- (hyphen) 换行符

Login
oracle默认有2个用户sys和system
sys和system都具有非常高的管理权限,但sys比system的权限大
qlplus sys/dwh as sysdba; 

sqlplus username/password[@connect_identifier] –直接连接到(connect_identifier定义的/默认的本地)数据库

sqlplus hr@//mymachine.mydomain:port/MYDB

SQL> connect username@connnect_identifier – 切换到另外一个数据库, 这个操作的执行顺序: 先退出当前数据库, 然后再登陆新的数据库

as sysoper/sysdba/system
enables privileged connection by users who have been granted SYSOPER, SYSDBA or SYSTEM system privileges. 

/nolog establishes no initial connection to oracle database.

When you configure a client to use the external password store
connect / @database_alias  --sqlplus / as sysdba

password  --change your password

login.sql – user specific settings
glogin.sql –global settings
数据库启动的时候, 先执行glogin.sql, 然后再执行login.sql
product user profile PUP

service_name: the global database name entered during the database creation
database_name.domain_name
Eg: sales.us.acme.com –sales is database_name, and us.acme.com is domain_name

# 数据库空间管理
```
create tablespace student_information
	datafile 'E:\oracle\oradata\student\stud01.dbf' size 100M,
			'E:\oracle\oradata\student\stud02.dbf' size 100M
	default storage( initial 10M   --表空间的初始大小
				next 10M     --当初始区满时, 分配第二个区间的大小为10M
			minextents 1  	-- 初始为该表控件分配一个区间
			maxextens 10     -- 最多为该表空间分配10个区间
			pctincrease 20)	--当在next的区间又满了, 按照20%的增长率分配区间大小
	online;
```

查看表空间的空间信息
`select * from dba_free_space;`

向表空间增加数据文件
alter tablespace student_information
add datafile 'E:\oracle\oradata\student\stud01.dbf' size 100M;

alter database orcl
datafile 'E:\oracle\oradata\system01.dbf'
resize 750M;

解决空间不足的方法
	增加数据文件大小 alter database
	创建新的表空间 create tablespace
	动态增加表空间 alter tablespace

- select table_name from user_tables;  //当前用户的表  mysql  show tables;
- select table_name from all_tables;  //所有用户的表
- select table_name from dba_tables;   //包括系统表
- select table_name from dba_tables where owner='用户名'
- select * from tab;
- select * from cat;

- 查看用户和默认表空间的关系: select username,default_tablespace from dba_users;
- 查询所有函数和储存过程:select * from user_source;
- Oracle查看当前用户连接:select * from v$Session;

查看所有用户:

- select * from dba_user;
- select * from all_users;
- select * from user_users;

查看用户系统权限:

- select * from dba_sys_privs;
- select * from all_sys_privs;
- select * from user_sys_privs;

查看用户对象权限:

- select * from dba_tab_privs;
- select * from all_tab_privs;
- select * from user_tab_privs;

查看所有角色: select * from dba_roles;

查看用户所拥有的角色:

- select * from dba_role_privs;
- select * from user_role_privs;
- select * from v$instance;

DBA_TABLES意为DBA拥有的或可以访问的所有的关系表.
ALL_TABLES意为某一用户拥有的或可以访问的所有的关系表.
USER_TABLES意为某一用户所拥有的所有的关系表.

需要注意的是在ORACLE数据库中大小写是敏感的,而此三表中数据默认都是大写的,所以在进行查询的时候注意小写的数据可能会造成数据无法查到.

常用的数据字典

- user_objects: 记录了用户的所有对象, 包含表, 索引, 过程, 视图等信息, 记忆创建时间, 状态是否有效等信息, 是非DBA用户的大本营
- user_source: 包含了系统中对象的源码, 如存储过程, function, procedure, package等信息
- cat/tab: 包含当前用户所有的用户和视图信息和同义词信息
- dict: 系统中所有的数据字典都存储在这里面

以user_, all_, dba_ 是静态视图
以v$ 开头的是动态视图, 系统在运行时内容会改变
--------------------------------------------------------------------------------------------

dba_开头

- dba_users     数据库用户信息 
- dba_segments  表段信息 
- dba_extents    数据区信息 
- dba_objects    数据库对象信息 
- dba_tablespaces   数据库表空间信息 
- dba_data_files    数据文件设置信息 
- dba_temp_files   临时数据文件信息 
- dba_rollback_segs   回滚段信息 
- dba_ts_quotas   用户表空间配额信息 
- dba_free_space  数据库空闲空间信息 
- dba_profiles   数据库用户资源限制信息 
- dba_sys_privs   用户的系统权限信息 
- dba_tab_privs  用户具有的对象权限信息 
- dba_col_privs  用户具有的列对象权限信息 
- dba_role_privs  用户具有的角色信息 
- dba_audit_trail  审计跟踪记录信息 
- dba_stmt_audit_opts  审计设置信息 
- dba_audit_object   对象审计结果信息 
- dba_audit_session  会话审计结果信息 
- dba_indexes  用户模式的索引信息
 
user_开头

- user_objects   用户对象信息 
- user_source   数据库用户的所有资源对象信息 
- user_segments   用户的表段信息 
- user_tables    用户的表对象信息 
- user_tab_columns   用户的表列信息

关于这个还涉及到两个常用的例子如下:

1. oracle中查询某个字段属于哪个表.
`select table_name,owner from dba_tab_columns t where t.COLUMN_NAME like upper('%username%');`

2. oracle中查询某个表的列数
select count( *) from user_tab_columns where table_name= upper('sys_operate');
注:这两个例子都用到了upper这个函数,是因为在这里表名得大写,否则查出的结果不是正确的

- user_constraints   用户的对象约束信息 
- user_sys_privs   当前用户的系统权限信息 
- user_tab_privs   当前用户的对象权限信息 
- user_col_privs   当前用户的表列权限信息 
- user_role_privs   当前用户的角色权限信息 
- user_indexes   用户的索引信息 
- user_ind_columns  用户的索引对应的表列信息 
- user_cons_columns   用户的约束对应的表列信息 
- user_clusters   用户的所有簇信息 
- user_clu_columns  用户的簇所包含的内容信息 
- user_cluster_hash_expressions   散列簇的信息

v$开头

- v$database    数据库信息 
- v$datafile   数据文件信息 
- v$controlfile  控制文件信息 
- v$logfile   重做日志信息 
- v$instance   数据库实例信息 
- v$log   日志组信息 
- v$loghist  日志历史信息 
- v$sga   数据库SGA信息 
- v$parameter  初始化参数信息 
- v$process   数据库服务器进程信息 
- v$bgprocess   数据库后台进程信息 
- v$controlfile_record_section   控制文件记载的各部分信息 
- v$thread   线程信息 
- v$datafile_header   数据文件头所记载的信息 
- v$archived_log  归档日志信息 
- v$archive_dest   归档日志的设置信息 
- v$logmnr_contents   归档日志分析的DML DDL结果信息 
- v$logmnr_dictionary   日志分析的字典文件信息 
- v$logmnr_logs   日志分析的日志列表信息 
- v$tablespace   表空间信息 
- v$tempfile   临时文件信息 
- v$filestat   数据文件的I/O统计信息 
- v$undostat   Undo数据信息 
- v$rollname   在线回滚段信息 
- v$session   会话信息 
- v$transaction  事务信息 
- v$rollstat    回滚段统计信息 
- v$pwfile_users   特权用户信息 
- v$sqlarea     当前查询过的sql语句访问过的资源及相关的信息 
- v$sql           与v$sqlarea基本相同的相关信息 
- v$sysstat    数据库系统状态信息

all_开头 

- all_users   数据库所有用户的信息 
- all_objects   数据库所有的对象的信息 
- all_def_audit_opts   所有默认的审计设置信息 
- all_tables   所有的表对象信息 
- all_indexes  所有的数据库对象索引的信息 

session_开头 

- session_roles   会话的角色信息 
- session_privs   会话的权限信息

index_开头 

- index_stats   索引的设置和存储信息

伪表 
- dual   系统伪列表信息

最重要的9个动态性能视图

- v$session + v$session_wait (在10g里功能被整合,凑合算1个吧.) 
- v$process 
- v$sql 
- v$sqltext 
- v$bh (更宁愿是x$bh) 
- v$lock 
- v$latch_children 
- v$sysstat 
- v$system_event 
 
按组分的几组重要的性能视图 

1. System 的 over view 
v$sysstat , v$system_event , v$parameter 

2. 某个session 的当前情况 
v$process , v$session , v$session_wait ,v$session_event , v$sesstat 

3. SQL 的情况 
v$sql , v$sqlarea , v$SQL_PLAN , V$SQL_PLAN_STATISTICS, v$sqltext_with_newlines 

3. Latch / lock /ENQUEUE 
v$latch , v$latch_children , v$latch_holder , v$lock ,V$ENQUEUE_STAT ,V$ENQUEUE_LOCK 

4. IO 方面的 
v$segstat , v$filestat , v$tempstat ,v$datafile , v$tempfile 

5. shared pool / Library cache 
v$Librarycache , v$rowcache , x$ksmsp 

6. 几个advice也不错 
v$db_cache_advice , v$PGA_TARGET_ADVICE, v$SHARED_POOL_ADVICE


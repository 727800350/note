# Python sequence
sequence在python不是一种特定的类型,而是泛指一系列的类型

list, tuple, 字符串都属于sequence

# IO
## 重定向

	import sys
	out = open("out.txt","w")
	sys.stdout = out  //之后的print语句就会输出到out 指向的文件中
	print .....
	out.close()

输出到文件

	out = open("out.txt","w")  //w is write, a+ 追加
	print>>out,"string",integer //输出到out中
	sys.stdout  //sys模块中

# random 随机数生成
- `random.random()`用于生成一个0到1的随机符点数: 0 <= n < 1.0
-  `random.uniform(a, b)`,指定范围内的随机符点数. 如果a > b,则生成的随机数n: a <= n <= b.如果 a <b, 则 b <= n <= a.
- `random.randint(a, b)`,指定范围内的整数, 生成的随机数n: a <= n <= b
- `random.randrange([start], stop[, step])`,从指定范围内,按指定基数递增的集合中 获取一个随机数  
如:random.randrange(10, 100, 2),结果相当于从[10, 12, 14, 16, ... 96, 98]序列中获取一个随机数.random.randrange(10, 100, 2)在结果上与 random.choice(range(10, 100, 2) 等效.
- `random.choice(sequence)`从序列中获取一个随机元素  
- `random.shuffle(x[, random])`,Shuffle the sequence x in place.   
The optional argument random is a 0-argument function returning a random float in [0.0, 1.0); by default, this is the function random().
- `random.sample(sequence, k)`,从指定序列中随机获取指定长度的片断.sample函数不会修改原有序列

# Numeric
`int(String)`: convert a string to integer

`divmod(x, y)` which returns a tuple `(x / y, x % y)`

## 小数位数

	>>> round(2.675,2)
	2.6800000000000002
	>>> round(2.675,2) == 2.68
	True
	>>> round(2.675,2) == 2.67
	False

保留小数位要用字符替换 

	>>> "%.0f" % 3.45
	'3'
	>>> "%.1f" % 3.45
	'3.5'
	>>> "%.2f" % 3.45
	'3.45'
	>>> "%.3f" % 3.45
	'3.450'

## math module
- `math.floor()`
- `math.ceil()`

# Database
## Mysql with MySQLdb module
[MySQL Python tutorial](http://zetcode.com/db/mysqlpython/)

### `_mysql` module
The _mysql module implements the MySQL C API directly. It is not compatible with the Python DB API interface. Generally, the programmers prefer the object oriented MySQLdb module.

	#!/usr/bin/python
	# -*- coding: utf-8 -*-
	import _mysql
	import sys
	
	try:
	    con = _mysql.connect('localhost', 'testuser', 'test623', 'testdb')
	        
	    con.query("SELECT VERSION()")
	    result = con.use_result()
	    
	    print "MySQL version: %s" % result.fetch_row()[0]
	    
	except _mysql.Error, e:
	    print "Error %d: %s" % (e.args[0], e.args[1])
	    sys.exit(1)
	
	finally:
	    if con:
	        con.close()

[MySQL C API function mapping](http://mysql-python.sourceforge.net/MySQLdb.html)

| C API                          | _mysql                             |
|--------------------------------|------------------------------------|
| mysql_affected_rows()          | conn.affected_rows()               |
| mysql_autocommit()             | conn.autocommit()                  |
| mysql_character_set_name()     | conn.character_set_name()          |
| mysql_close()                  | conn.close()                       |
| mysql_commit()                 | conn.commit()                      |
| mysql_connect()                | _mysql.connect()                   |
| mysql_data_seek()              | result.data_seek()                 |
| mysql_debug()                  | _mysql.debug()                     |
| mysql_dump_debug_info          | conn.dump_debug_info()             |
| mysql_escape_string()          | _mysql.escape_string()             |
| mysql_fetch_row()              | result.fetch_row()                 |
| mysql_get_character_set_info() | conn.get_character_set_info()      |
| mysql_get_client_info()        | _mysql.get_client_info()           |
| mysql_get_host_info()          | conn.get_host_info()               |
| mysql_get_proto_info()         | conn.get_proto_info()              |
| mysql_get_server_info()        | conn.get_server_info()             |
| mysql_info()                   | conn.info()                        |
| mysql_insert_id()              | conn.insert_id()                   |
| mysql_num_fields()             | result.num_fields()                |
| mysql_num_rows()               | result.num_rows()                  |
| mysql_options()                | various options to_mysql.connect() |
| mysql_ping()                   | conn.ping()                        |
| mysql_query()                  | conn.query()                       |
| mysql_real_connect()           | _mysql.connect()                   |
| mysql_real_query()             | conn.query()                       |
| mysql_real_escape_string()     | conn.escape_string()               |
| mysql_rollback()               | conn.rollback()                    |
| mysql_row_seek()               | result.row_seek()                  |
| mysql_row_tell()               | result.row_tell()                  |
| mysql_select_db()              | conn.select_db()                   |
| mysql_set_character_set()      | conn.set_character_set()           |
| mysql_ssl_set()                | ssl option to _mysql.connect()     |
| mysql_stat()                   | conn.stat()                        |
| mysql_store_result()           | conn.store_result()                |
| mysql_thread_id()              | conn.thread_id()                   |
| mysql_thread_safe_client()     | conn.thread_safe_client()          |
| mysql_use_result()             | conn.use_result()                  |
| mysql_warning_count()          | conn.warning_count()               |
| CLIENT_*                       | MySQLdb.constants.CLIENT.*         |
| CR_*                           | MySQLdb.constants.CR.*             |
| ER_*                           | MySQLdb.constants.ER.*             |
| FIELD_TYPE_*                   | MySQLdb.constants.FIELD_TYPE.*     |
| FLAG_*                         | MySQLdb.constants.FLAG.*           |

### MySQLdb module
MySQLdb is a thin Python wrapper around _mysql. It is compatible with the Python DB API, which makes the code more portable. Using this model is the preferred way of working with the MySQL.

	#!/usr/bin/python
	# -*- coding: utf-8 -*-
	import MySQLdb as mdb
	import sys
	
	try:
	    con = mdb.connect('localhost', 'testuser', 'test623', 'testdb');
	## 	mdb.connect(host,user,passwd,db):
	
	## 	cursor is used to traverse the records from the result set. 
	## 	call the execute() method of the cursor and execute the SQL statement.
	    cur = con.cursor()
	    cur.execute("SELECT VERSION()")
	
	## 	retrieve only one record
	    ver = cur.fetchone()
	    
	    print "Database version : %s " % ver
	    
	except mdb.Error, e:
	    print "Error %d: %s" % (e.args[0],e.args[1])
	    sys.exit(1)
	    
	finally:    
	    if con:    
	        con.close()

# General
应该尽量避免使用Python全局变量.  
在文件开头声明Python全局变量variable, 在具体函数中使用该变量时,需要事先声明 global variable,否则系统将该变量视为局部变量. CONSTANT = 0  (将全局变量大写便于识别)

# Mysql
登陆:
`mysql -u test -p db_name`

## Create
### Storage
`MyISAM` is the default storage engine. It is based on the older (and no longer available) ISAM storage engine but **has many useful extensions**.  
Each MyISAM table is stored on disk in **three files**. The files have names that begin with the table name and have an extension to indicate the file type.  
An `.frm` file stores the table format. The data file has an `.MYD (MYData)` extension. The index file has an `.MYI (MYIndex)` extension.  
To specify explicitly that you want a MyISAM table, indicate that with an ENGINE table option:

    CREATE TABLE t (i INT) ENGINE = MYISAM;
老版本的MySQL使用TYPE而不是ENGINE(例如，TYPE = MYISAM), but ENGINE is the preferred term and TYPE is deprecated.

## Alter
	alter table flows add column ip_prot tinyint(4) null default 0;

## Select
mysql不支持`select top n`的语法,应该用这个替换:  
`select * from tablename order by orderfield desc/asc limit position, counter;`
position 指示从哪里开始查询，如果是0则是从头开始，counter 表示查询的个数

取前15条记录：
`select * from tablename order by orderfield desc/asc limit 0,15`

//为了检索从某一个偏移量到记录集的结束所有的记录行，可以指定第二个参数为 -1：
`mysql> SELECT * FROM table LIMIT 95,-1; // 检索记录行 96-last.`
//如果只给定一个参数, 它表示返回最大的记录行数目:
`mysql> SELECT * FROM table LIMIT 5;` //检索前 5 个记录行,也就是说，LIMIT n 等价于 LIMIT 0,n。

## [用户管理及权限管理](http://www.libuchao.com/2013/04/06/mysql-user-and-privilege)

	CREATE USER test IDENTIFIED BY '123456';
	GRANT ALL PRIVILEGES ON test.* TO 'test'@'localhost' IDENTIFIED BY '123456';
	FLUSH PRIVILEGES;

MySQL 默认有个root用户,但是这个用户权限太大,一般只在管理数据库时候才用.如果在项目中要连接 MySQL 数据库,则建议新建一个权限较小的用户来连接.

在 MySQL 命令行模式下输入如下命令可以为 MySQL 创建一个新用户:  
`CREATE USER username IDENTIFIED BY 'password';`  
新用户创建完成,但是此刻如果以此用户登陆的话,会报错,因为我们还没有为这个用户分配相应权限,分配权限的命令如下:

`GRANT ALL PRIVILEGES ON *.* TO 'username'@'localhost' IDENTIFIED BY 'password';`  
授予username用户在所有数据库上的所有权限.

如果此时发现刚刚给的权限太大了,如果我们只是想授予它在某个数据库上的权限,那么需要切换到root 用户撤销刚才的权限,重新授权:  
`EVOKE ALL PRIVILEGES ON *.* FROM 'username'@'localhost';`  
`GRANT ALL PRIVILEGES ON wordpress.* TO 'username'@'localhost' IDENTIFIED BY 'password';`  

甚至还可以指定该用户只能执行 select 和 update 命令:  
`GRANT SELECT, UPDATE ON wordpress.* TO 'username'@'localhost' IDENTIFIED BY 'password';`  
这样一来,再次以username登陆 MySQL,只有wordpress数据库是对其可见的,并且如果你只授权它select权限,那么它就不能执行delete 语句.

另外每当调整权限后,通常需要执行以下语句刷新权限:  
`FLUSH PRIVILEGES;`

删除刚才创建的用户:  
`DROP USER username@localhost;`

仔细上面几个命令,可以发现不管是授权,还是撤销授权,都要指定响应的host(即 @ 符号后面的内容),因为以上及格命令实际上都是在操作mysql 数据库中的user表,可以用如下命令查看相应用户及对应的host:  
S`ELECT User, Host FROM user;`  
当然,这个表中还包含很多其它例如用户密码,权限设置等很多内容,操作时候尤其需要小心.

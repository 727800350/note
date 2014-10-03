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

## insert
```
INSERT INTO table_name VALUES (value1,value2,value3,...);

INSERT INTO table_name (column1,column2,column3,...) VALUES (value1,value2,value3,...);
```

- 当field为not null, 但是没有设置默认值, 插入的时候如果没有为这个field指定值, 那么这个field的位置为空, 什么都不显示
- 当field为not null, 设置有默认值, 插入的时候如果没有为这个field指定值, 那么这个field的值就为默认值
- 当field没有设置not null(也就是说可以为null), 也没有设置默认值, 那么插入的时候如果没有为这个field指定值, 那么这个field的值就为null
- 当field没有设置not null(也就是说可以为null), 设置有默认值, 那么插入的时候如果没有为这个field指定值, 那么这个field的值就为默认值

## update
```
UPDATE table_name SET column1=value1,column2=value2,...  WHERE some_column=some_value;
```
如果update 的一条记录在数据库不存在(就是说后面的where语句没有找到record), 不会对数据库产生影响, 同时语句的执行也不会报错.

## Alter
```
alter table flows add column ip_prot tinyint(4) null default 0;
```

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

**mySQL error: #1248 - Every derived table must have its own alias**  
如果按照下面的写法会报上面的错误:

	$sql3="SELECT * FROM (
	SELECT * FROM (
	SELECT * FROM Pre_Company ORDER BY ID DESC limit $n)
	ORDER BY ID  ASC limit $display)
	ORDER BY ID DESC";

需要为子查询指定别名, 如下所示:

	$sql3="SELECT * FROM (
	SELECT * FROM (
	SELECT * FROM Pre_Company ORDER BY ID DESC limit $n) as a
	ORDER BY ID  ASC limit $display) as b 
	ORDER BY ID DESC";
### group by, where, having
可以在包含 GROUP BY 子句的查询中使用 WHERE 子句.在完成任何分组之前,将消除不符合 WHERE 子句中的条件的行.例如:
```
SELECT ProductModelID, AVG(ListPrice) AS 'Average List Price'
FROM Production.Product
WHERE ListPrice > $1000
GROUP BY ProductModelID
ORDER BY ProductModelID ;
```

having子句与where有相似之处但也有区别,都是设定条件的语句.

1. having只能用在group by之后,对分组后的结果进行筛选(即使用having的前提条件是分组).
2. where肯定在group by 之前,即也在having之前
3. where后的条件表达式里不允许使用聚合函数,而having可以

在查询过程中优先级
```
where 子句 > 聚合语句(sum,min,max,avg,count) > having子句
```
eg:  

1. `select sum(num) as rmb from order where id>10` 只有对id大于10的记录才对进行聚合语句  
1. `select reportsto as manager, count(*) as reports from employees group by reportsto having count(*) > 4`  
首先将reportsto相同的group起来, 统计各个group里面成员的个数, 然后再筛选出各个大于4的groups.  
如果把上面的having换成where则会出错.

统计分组数据时用到聚合语句, 对分组数据再次判断时要用having. 如果不用这些关系就不存在使用having, 直接使用where就行了.
having就是来弥补where在分组数据判断时的不足. 因为where执行优先级别要快于聚合语句.

当一个查询语句同时出现了where,group by,having,order by的时候,执行顺序和编写顺序是:

1. 执行where xx对全表数据做筛选,返回第1个结果集
2. 针对第1个结果集使用group by分组,返回第2个结果集
3. 针对第2个结果集中的每1组数据执行select xx,有几组就执行几次,返回第3个结果集
4. 针对第3个结集执行having xx进行筛选,返回第4个结果集
5. 针对第4个结果集排序

eg: 完成一个复杂的查询语句,需求如下: 按由高到低的顺序显示个人平均分在70分以上的学生姓名和平均分,
为了尽可能地提高平均分,在计算平均分前不包括分数在60分以下的成绩  
分析

1. 要求显示学生姓名和平均分, 因此确定第1步`select s_name,avg(score) from student`
2. 计算平均分前不包括分数在60分以下的成绩,因此确定第2步 `where score>=60`
3. 显示个人平均分, 相同名字的学生(同一个学生)考了多门科目,因此按姓名分组.确定第3步 `group by s_name` 
4. 显示个人平均分在70分以上 因此确定第4步 `having avg(s_score)>=70` 
5. 按由高到低的顺序, 因此确定第5步 `order by avg(s_score) desc`

合起来就是:
```
select s_name,avg(s_score) from student 
where score>=60
group by s_name
having avg(s_score)>=70
order by avg(s_score) desc
```

### Backup
备份
```
$mysqldump -u 用户名 -p 数据库名 > 导出的文件名
```
恢复
```
mysql>source 导入的文件名
$mysql -u root -p voice<voice.sql
```

## index
在执行CREATE TABLE语句时可以创建索引,也可以单独用CREATE INDEX或ALTER TABLE来为表增加索引

### CREATE INDEX
CREATE INDEX可对表增加普通索引或UNIQUE索引

	 CREATE INDEX index_name ON table_name (column_list)
	 CREATE UNIQUE INDEX index_name ON table_name (column_list)

### ALTER TABLE
ALTER TABLE用来创建普通索引,UNIQUE索引或PRIMARY KEY索引

	ALTER TABLE table_name ADD INDEX index_name (column_list)
	ALTER TABLE table_name ADD UNIQUE (column_list)
	ALTER TABLE table_name ADD PRIMARY KEY (column_list)

`column_list` 指出对哪些列进行索引,多列时各列之间用逗号分隔.  
索引名`index_name`可选,缺省时,MySQL将根据第一个索引列赋一个名称

### 索引类型
在创建索引时,可以规定索引能否包含重复值,如果不包含,则索引应该创建为PRIMARY KEY或UNIQUE索引,对于单列惟一性索引,这保证单列不包含重复的值;
对于多列惟一性索引,保证多个值的组合不重复。

PRIMARY KEY索引和UNIQUE索引非常类似.
事实上,PRIMARY KEY索引仅是一个具有名称PRIMARY的UNIQUE索引.这表示一个表只能包含一个PRIMARY KEY,因为一个表中不可能具有两个同名的索引.

### 删除索引
可利用ALTER TABLE或DROP INDEX语句来删除索引.类似于CREATE INDEX语句,DROP INDEX可以在ALTER TABLE内部作为一条语句处理,语法如下:

	DROP INDEX index_name ON talbe_name
	ALTER TABLE table_name DROP INDEX index_name
	ALTER TABLE table_name DROP PRIMARY KEY

### 查看索引
	mysql> show index from tblname;
	mysql> show keys from tblname;

## [用户管理及权限管理](http://www.libuchao.com/2013/04/06/mysql-user-and-privilege)

	CREATE USER test IDENTIFIED BY '123456';
	GRANT ALL PRIVILEGES ON test.* TO 'test'@'localhost' IDENTIFIED BY '123456';
	FLUSH PRIVILEGES;

将权限给本地用户,且不需要用密码验证
```
GRANT ALL PRIVILEGES ON test.* TO ''@'localhost';
```

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
SELECT User, Host FROM user;`  
当然,这个表中还包含很多其它例如用户密码,权限设置等很多内容,操作时候尤其需要小心.

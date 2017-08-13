`mysql -u user -p$passwd -h host -P port -D db --default-character-set=utf8`

mysql的配置文件: `/etc/my.cnf`.  
mysql server 自己带有一些示例配置文件: `rpm -ql mysql-server | grep cnf`, 
得到的结果有`my-huge.cnf, my-innodb-heavy-4G.cnf, my-large.cnf, my-medium.cnf, my-small.cnf`,   
将这些文件复制为`/etc/my.cnf`, 就可以修改mysql的默认配置, 对于现在的硬件配置, 使用`my-huge.cnf`就可以.

当由于系统的空间不够时, mysql 会在`/var/lib/mysql` 目录下创建一个hostname.error 的文件对错误进行说明, 而不会在 `/var/log/mysqld.log` 中说明.  
如果经常出现空间不够的错误提示, 可以再`/etc/my.cnf` 中 修改数据的位置 `datadir=/var/lib/mysql`

## 查看
- `desc t1;`
- `show full columns from t1;`
- `show index from t1;`
- `show create table t1;`
- `show triggers;`
- `show triggers from db1;`

## Create
MySQL默认情况下指定字段为NULL修饰符

## Data Type
### date time
[时间差](http://blog.csdn.net/yzsind/article/details/8831429)

datetime 直接之间作差得到结果不是时间意义上的作差.
实际是mysql的时间相减是做了一个隐式转换操作, 直接把年月日时分秒拼起来, 如`2013-04-21 16:59:33` 直接转换为`20130421165933`, 由于时间不是十进制,所以最后得到的结果没有意义,这也是导致出现坑爹的结果.

要得到正确的时间相减秒值,有以下3种方法:

1. `time_to_sec(timediff(t2, t1))`: timediff 得到的结果是一个时间格式
2. `timestampdiff(second, t1, t2)`
3. `unix_timestamp(t2) - unix_timestamp(t1)`

时间位移: `ADDTIME('2014-05-26 18:26:21', '0:0:2')` 求后2秒的时间.

### str
- `length()`
- `char_length()`

### 复制数据到新表
- 复制表结构(包括index, key 等): 
	1. `CREATE TABLE 新表 LIKE 旧表`
-  复制数据
	1. `CREATE TABLE 新表 SELECT * FROM 旧表`
	1. `INSERT INTO 新表 SELECT * FROM 旧表`

## insert
- `INSERT INTO table_name VALUES(value1, value2, value3, ...);`
- `INSERT INTO table_name(column1, column2, column3, ...) VALUES(value1, value2, value3, ...);`

MySql避免重复插入记录(根据主键判重)

1. `insert ignore into xxx`
1. `replace into xxx`

## update
`UPDATE table_name SET column1 = value1, column2 = value2, ...  WHERE some_column = some_value;`

如果update 的一条记录在数据库不存在(就是说后面的where语句没有找到record), 不会对数据库产生影响, 同时语句的执行也不会报错.

两个表update

- `update a set age = (select age from b where b.name = a.name)`: 用到了子查询, 当数据量大的时候, 效率非常低
- `update a, b set a.age = b.age where a.name = b.name`: 比上面的子查询效率会高很多, 尤其是在建立有合适的索引时.

## Alter
```
ALTER TABLE table_name ADD column_name datatype
alter table flows add column ip_prot tinyint(4) null default 0;

ALTER TABLE table_name DROP COLUMN column_name

ALTER TABLE table_name MODIFY COLUMN column_name datatype

ALTER TABLE table_name CHANGE old_col_name new_col_name column_definition
ALTER TABLE table_name RENAME TO new_table_name;
```

## Select
mysql中提供了一个G标志,放到sql语句后,可以使一行的每个列打印到单独的行.
对于列数比较多, 屏幕不够宽时很好用.  
这样的显示效果与MYSQL命令的-E参数是一样的. 使用-E参数后,结果集默认以列的方式显示
```
mysql> select * from t_goods \G  
*************************** 1. row ***************************  
         id: 1  
 goods_name: MYSQL5  
   quantity: 50  
   add_date: 2012-12-12  
description: A book that has been read but is in good condition.
1 row in set (0.00 sec)
```

mysql不支持`select top n`的语法,应该用这个替换:  
`select * from tablename order by orderfield desc/asc limit position, counter;`
position 指示从哪里开始查询,如果是0则是从头开始,counter 表示查询的个数

取前15条记录:
`select * from tablename order by orderfield desc/asc limit 0,15`

//为了检索从某一个偏移量到记录集的结束所有的记录行,可以指定第二个参数为 -1:
`mysql> SELECT * FROM table LIMIT 95,-1; // 检索记录行 96-last.`
//如果只给定一个参数, 它表示返回最大的记录行数目:
`mysql> SELECT * FROM table LIMIT 5;` //检索前 5 个记录行,也就是说,LIMIT n 等价于 LIMIT 0,n.

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

1. **having只能用在group by之后,对分组后的结果进行筛选**(即使用having的前提条件是分组).
2. where肯定在group by 之前,即也在having之前
3. where后的条件表达式里不允许使用聚合函数,而having可以

在查询过程中优先级
```
where 子句 > 聚合语句(sum,min,max,avg,count) > having子句
```
eg:  

1. `select sum(num) as rmb from order where id>10` 只有对id大于10的记录才对进行聚合语句  
1. `select reportsto as manager, count(\*) as reports from employees group by reportsto having count(\*) > 4`  
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

### JOIN
[sql joins](http://www.techonthenet.com/sql/joins.php)

[A Visual Explanation of SQL Joins](http://blog.codinghorror.com/a-visual-explanation-of-sql-joins/)

There are 4 different types of SQL joins:

- SQL INNER JOIN (or sometimes called simple join)
- SQL LEFT OUTER JOIN (or sometimes called LEFT JOIN)
- SQL RIGHT OUTER JOIN (or sometimes called RIGHT JOIN)
- SQL FULL OUTER JOIN (or sometimes called FULL JOIN)

例子中用到的数据  
We have a table called suppliers with two fields (supplier_id and supplier_name). It contains the following data:
```
supplier_id	supplier_name
10000	IBM
10001	Hewlett Packard
10002	Microsoft
10003	NVIDIA
```
We have another table called orders with three fields (order_id, supplier_id, and order_date). It contains the following data:
```
order_id	supplier_id	order_date
500125	10000	2003/05/12
500126	10001	2003/05/13
500127	10004	2003/05/14
```

#### SQL INNER JOIN (SIMPLE JOIN)
It is the most common type of SQL join. 
SQL INNER JOINS return all rows from multiple tables where the join condition is met.

The syntax for the SQL INNER JOIN is:
```
SELECT columns
FROM table1 
INNER JOIN table2
ON table1.column = table2.column;
```

In this visual diagram, the SQL INNER JOIN returns the shaded area:  
![sql inner join](http://www.techonthenet.com/sql/images/inner_join.gif)  
The SQL INNER JOIN would return the records where table1 and table2 intersect.

ex:
```
SELECT s.supplier_id, s.supplier_name, od.order_date
FROM suppliers AS s
INNER JOIN order AS od
ON s.supplier_id = od.supplier_id;
```
result
```
supplier_id	name	order_date
10000	IBM	2003/05/12
10001	Hewlett Packard	2003/05/13
```

#### SQL LEFT OUTER JOIN
This type of join returns all rows from the LEFT-hand table specified in the ON condition and only those rows from the other table where the joined fields are equal (join condition is met).

```
SELECT columns
FROM table1
LEFT OUTER JOIN table2
ON table1.column = table2.column;
```
In some databases, the LEFT OUTER JOIN keywords are replaced with LEFT JOIN.

![left outer join](http://www.techonthenet.com/sql/images/left_outer_join.gif)

```
SELECT suppliers.supplier_id, suppliers.supplier_name, orders.order_date
FROM suppliers
LEFT OUTER JOIN orders
ON suppliers.supplier_id = orders.supplier_id;
```
Our result set would look like this:
```
supplier_id	supplier_name	order_date
10000	IBM	2003/05/12
10001	Hewlett Packard	2003/05/13
10002	Microsoft	<null>
10003	NVIDIA	<null>
```

##### 选取 A - B(也就是在A中不在B中)
![A - B](http://blog.codinghorror.com/content/images/uploads/2007/10/6a0120a85dcdae970b012877702754970c-pi.png)

```
SELECT * FROM TableA
LEFT OUTER JOIN TableB
ON TableA.name = TableB.name
WHERE TableB.id IS null
```

#### SQL RIGHT OUTER JOIN
![right outer join](http://www.techonthenet.com/sql/images/right_outer_join.gif)

Example
```
SELECT orders.order_id, orders.order_date, suppliers.supplier_name
FROM suppliers
RIGHT OUTER JOIN orders
ON suppliers.supplier_id = orders.supplier_id;
```
result
```
order_id	order_date	supplier_name
500125	2013/08/12	Apple
500126	2013/08/13	Google
500127	2013/08/14	<null>
```

#### SQL FULL OUTER JOIN
This type of join returns all rows from the LEFT-hand table and RIGHT-hand table **with nulls in place where the join condition is not met**.
```
SELECT columns
FROM table1
FULL OUTER JOIN table2
ON table1.column = table2.column;
```

![full outer join](http://www.techonthenet.com/sql/images/full_outer_join.gif)

Example
```
SELECT suppliers.supplier_id, suppliers.supplier_name, orders.order_date
FROM suppliers
FULL OUTER JOIN orders
ON suppliers.supplier_id = orders.supplier_id;
```
result
```
supplier_id	supplier_name	order_date
10000	IBM	2013/08/12
10001	Hewlett Packard	2013/08/13
10002	Microsoft	<null>
10003	NVIDIA	<null>
<null>	<null>	2013/08/14
```

##### 选取 (A - B) \CUP (B - A)
![set of records unique to Table A and Table B](http://blog.codinghorror.com/content/images/uploads/2007/10/6a0120a85dcdae970b012877702769970c-pi.png)
```
SELECT * FROM TableA
FULL OUTER JOIN TableB
ON TableA.name = TableB.name
WHERE TableA.id IS null
OR TableB.id IS null
```

mysql 不支持full outer join, 但是可以使用left join 和right join来模拟.
```
SELECT * FROM t1
LEFT JOIN t2 ON t1.id = t2.id
UNION
SELECT * FROM t1
RIGHT JOIN t2 ON t1.id = t2.id
```

### [Execution Plan](http://my.oschina.net/zimingforever/blog/60233)
mysql的查看执行计划的语句很简单,explain+你要执行的sql语句就OK了.

举一个例子
```
EXPLAIN SELECT * from employees where employees.gender='M'
```
结果示例:
```
id, select_type, table,     type, possible_keys, key,  key_len, ref,  rows,   Extra
1,  SIMPLE,      employees, ALL,  NULL,          NULL, NULL,    NULL, 300252, Using where
```
对上面各个column 的解释

1. id是一组数字,表示查询中执行select子句或操作表的顺序.  
如果id相同,则执行顺序从上至下.  
如果是子查询,id的序号会递增,id越大则优先级越高,越先会被执行.  
id如果相同,则可以认为是一组,从上往下顺序执行,所有组中,id越高,优先级越高,越容易执行  

1. select_type有simple,primary,subquery,derived(衍生),union,unionresult.  

- simple表示查询中不包含子查询或者union.
- 当查询中包含任何复杂的子部分,最外层的查询被标记成primary.
- 在select或where列表中包含了子查询,则子查询被标记成subquery.
- 在from的列表中包含的子查询被标记成derived.
- 若第二个select出现在union后,则被标记成union,若union在from子句的子查询中,外层的select被标记成derived.
- 从union表获取结果的select被标记成union result.
 
1. type叫访问类型,表示在表中找到所需行的方式,常见类型有all,index,range,ref,eq_ref,const,system,NULL 性能从做至右由差至好.

- ALL,即full table scan,mysql将遍历全表来找到所需要的行.
- index为full index scan,只遍历索引树.
- range表示索引范围扫描 ,对索引的扫描开始于一点,返回匹配的值域的行,常见于between,<,>的查询.
- ref为非唯一性索引扫描,返回匹配某个单独值的所有行,常见于非唯一索引即唯一索引的非唯一前缀进行的查找.
- eq_ref表示唯一性索引扫描,对于每个索引键,表中只有一条记录与之匹配,常见于主键或者唯一索引扫描.
- const 表示当对查询部分进行优化,并转化成一个常量时,使用这些类型访问.比如将主键置于where列表中,mysql就能把该查询置成一个常量.
- system是const的一个特例,当查询表中只有一行的情况下使用的是system
- NULL表示在执行语句中,不用查表或索引.
 
1. possiblekey表示能使用哪个索引在表中找到行,查询涉及到的字段上若存在索引,则该索引被列出,但不一定被查询使用.

1. key表示查询时使用的索引.若查询中使用了覆盖索引,则该索引仅出现在key中举个例子

1. keylen表示索引所使用的字节数,可以通过该列结算查询中使用的索引长度

1. ref表示上述表的链接匹配条件,即哪些列或常量可被用于查找索引列上的值.

1. rows表示根据mysql表统计信息及索引选用情况,估算找到所需记录要读取的行数.

1. extra表示不在其他列并且也很重要的额外信息.

- using index表示在相应的select中使用了覆盖索引.
- usingwhere表示存储引擎搜到记录后进行了后过滤(POST-FILTER),如果查询未能使用索引,usingwhere的作用只是提醒我们mysql要用where条件过滤z结果集.
- using temporay表示用临时表来存储结果集,常见于排序和分组查询.
- usingfilesort,mysql中无法用索引完成的排序成为文件排序.

mysq的执行计划有一定局限性:

- EXPLAIN不会告诉你关于触发器,存储过程的信息或用户自定义函数对查询的影响情况
- EXPLAIN不考虑各种Cache
- EXPLAIN不能显示MySQL在执行查询时所作的优化工作
- 部分统计信息是估算的,并非精确值
- EXPALIN只能解释SELECT操作,其他操作要重写为SELECT后查看执行计划

### Backup
备份
```
备份整个数据库
$mysqldump -u 用户名 -p 数据库名 > 导出的文件名

备份一个表
$mysqldump -u 用户名 -p 数据库名 表名 > 导出的文件名
```
恢复
```
mysql>source 导入的文件名
$mysql -u root -p voice<voice.sql
```

## index
在执行CREATE TABLE语句时可以创建索引,也可以单独用CREATE INDEX或ALTER TABLE来为表增加索引
```
INDEX index_name(`field`),
```

### CREATE INDEX
CREATE INDEX可对表增加普通索引或UNIQUE索引

	 CREATE INDEX index_name ON table_name (column_list)
	 CREATE UNIQUE INDEX index_name ON table_name (column_list)

ALTER TABLE用来创建普通索引,UNIQUE索引或PRIMARY KEY索引

	ALTER TABLE table_name ADD INDEX index_name (column_list)
	ALTER TABLE table_name ADD UNIQUE (column_list)
	ALTER TABLE table_name ADD PRIMARY KEY (column_list)

`column_list` 指出对哪些列进行索引,多列时各列之间用逗号分隔.  
索引名`index_name`可选,缺省时,MySQL将根据第一个索引列赋一个名称

### 索引类型
在创建索引时,可以规定索引能否包含重复值,如果不包含,则索引应该创建为PRIMARY KEY或UNIQUE索引,对于单列惟一性索引,这保证单列不包含重复的值;
对于多列惟一性索引,保证多个值的组合不重复.

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

### [性能调优](http://blog.csdn.net/xtdhqdhq/article/details/17582779)
如果我们的查询where条件只有一个,我们完全可以用单列索引,这样的查询速度较快,索引也比较瘦身.
如果我们的业务场景是需要经常查询多个组合列,**不要试图分别基于单个列建立多个单列索引(因为虽然有多个单列索引,但是MySQL只能用到其中的那个它认为似乎最有效率的单列索引)**.  
这是因为当SQL语句所查询的列,全部都出现在复合索引中时,此时由于只需要查询索引块即可获得所有数据,当然比使用多个单列索引要快得多.下面以实际例子说明:
```
CREATE TABLE people(
peopleid SMALLINT NOT NULL AUTO_INCREMENT, 
firstname CHAR(50)　NOT NULL, 
lastname CHAR(50) NOT NULL, 
age SMALLINT NOT NULL, 
townid SMALLINT NOT　NULL, 
PRIMARY KEY (peopleid)
);
```
例如,我们可能需要查找姓名为Mike Sullivan,年龄17岁用户的peopleid
```
SELECT peopleid FROM people WHERE firstname="Mike" AND lastname="Sullivan" AND age=17;
```
由于我们不想让MySQL每次执行查询就去扫描整个表,这里需要考虑运用索引.

首先,我们可以考虑在单个列上创建索引,比如firstname,lastname或者age列.  
如果我们创建firstname列的索引(`ALTER TABLE people ADD INDEX firstname (firstname);`),  
MySQL将通过这个索引迅速把搜索范围限制到那些firstname="Mike"的记录,然后再在这个"中间结果集"上进行其他条件的搜索.
由于建立了firstname列的索引,与执行表的完全扫描相比,MySQL的效率提高了很多,  
但我们要求MySQL扫描的记录数量仍旧远远超过了实际所需要的.虽然我们可以删除firstname列上的索引,再创建lastname或者age列的索引,但总地看来,不论在哪个列上创建索引搜索效率仍旧相似.  
为了提高搜索效率,我们需要考虑运用多列索引.  
如果为firstname,lastname和age这三个列创建一个多列索引,MySQL只需一次检索就能够找出正确的结果!下面是创建这个多列索引的SQL命令:
```
ALTER TABLE people ADD INDEX fname_lname_age (firstname,lastname,age);
```
由于索引文件以B+树格式保存,MySQL能够立即转到合适的firstname,然后再转到合适的lastname,最后转到合适的age.在没有扫描数据文件任何一个记录的情况下,MySQL就正确地找出了搜索的目标记录!

那么,如果在firstname,lastname,age这三个列上分别创建单列索引,效果是否和创建一个firstname,lastname,age的多列索引一样呢?答案是否定的,两者完全不同.  
当我们执行查询的时候,MySQL只能使用一个索引.**如果你有三个单列的索引,MySQL会试图选择一个限制最严格的索引**.
但是,即使是限制最严格的单列索引,它的限制能力也肯定远远低于firstname,lastname,age这三个列上的多列索引.

注意:
继续考虑前面的例子,现在我们有一个firstname,lastname,age列上的多列索引,我们称这个索引为fname_lname_age.  
它相当于我们创建了(firstname,lastname,age),(firstname,lastname)以及(firstname)这些列组合上的索引.

## Cast Functions and Operators
- BINARY	Cast a string to a binary string
- `CAST(expr AS type)` Cast a value as a certain type
- CONVERT()	Cast a value as a certain type

The BINARY operator casts the string following it to a binary string. This is an easy way to force a column comparison to be done byte by byte rather than character by character.
```
mysql> SELECT 'a' = 'A';
        -> 1
mysql> SELECT BINARY 'a' = 'A';
        -> 0
mysql> SELECT 'a' = 'a ';
        -> 1
mysql> SELECT BINARY 'a' = 'a ';
        -> 0
```

`BINARY str` is shorthand for `CAST(str AS BINARY)`.
```
CONVERT(expr,type), CONVERT(expr USING transcoding_name)
```
The CONVERT() and CAST() functions take an expression of any type and produce a result value of a specified type. 

The type for the result can be one of the following values:

- BINARY[(N)]
- CHAR[(N)]
- DATE
- DATETIME
- DECIMAL[(M[,D])]
- SIGNED [INTEGER]
- TIME
- UNSIGNED [INTEGER] 

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

查看权限
```
show grants for 'test'@'%';

select * from mysql.user where user='test'
```

删除刚才创建的用户:  
`DROP USER username@localhost;`

仔细上面几个命令,可以发现不管是授权,还是撤销授权,都要指定响应的host(即 @ 符号后面的内容),因为以上及格命令实际上都是在操作mysql 数据库中的user表,可以用如下命令查看相应用户及对应的host:  
SELECT User, Host FROM user;`  
当然,这个表中还包含很多其它例如用户密码,权限设置等很多内容,操作时候尤其需要小心.

## trigger
```
mysql> help create trigger
Name: 'CREATE TRIGGER'
Description:
Syntax:
CREATE
    [DEFINER = { user | CURRENT_USER }]
    TRIGGER trigger_name
    trigger_time trigger_event
    ON tbl_name FOR EACH ROW
    trigger_body

trigger_time: { BEFORE | AFTER }
trigger_event: { INSERT | UPDATE | DELETE }
```
FOR EACH ROW表示任何一条记录上的操作满足触发事件都会触发该触发器  
当trigger body是多句sql语句时, 需要使用BEGIN END

插入的record 用new 代指, 删除的record 用old代指, 具体见[trigger demo](../demo/sql/trigger.sql)

```
mysql> CREATE TRIGGER trig1 AFTER INSERT
    -> ON work FOR EACH ROW
    -> INSERT INTO time VALUES(NOW());
```
上面创建了一个名为trig1的触发器,一旦在work中有插入动作,就会自动往time表里插入当前时间

tips:一般情况下,mysql默认是以; 作为结束执行语句,与触发器中需要的分行起冲突, 
为解决此问题可用DELIMITER,如:`DELIMITER ||`,可以将结束符号变成||当触发器创建完成后,可以用`DELIMITER ;`来将结束符号变成;
```
mysql> DELIMITER ||
mysql> CREATE TRIGGER trig2 BEFORE DELETE
    -> ON work FOR EACH ROW
    -> BEGIN
    -> INSERT INTO time VALUES(NOW());
    -> INSERT INTO time VALUES(NOW());
    -> END
    -> ||
Query OK, 0 rows affected (0.06 sec)

mysql> DELIMITER ;
```

删除触发器
```
mysql> DROP TRIGGER trig1;
```

## Mysql 的存储引擎,myisam和innodb的区别
MyISAM is the default storage engine. It is based on the older (and no longer available) ISAM storage engine but **has many useful extensions**.  
```sql
CREATE TABLE t(
	i INT)ENGINE = MYISAM;
```

1. **MyISAM 则适合用于频繁查询**的应用, innodb的引擎比较适合于插入和更新操作比较多的应用
1. MyISAM类型不支持事务处理等高级处理, 也不支持外键,而InnoDB类型支持事务, 回滚, 外键和崩溃恢复能力等高级数据库功能(ACID: Atomicity, Consistency, Isolation, Durability)
1. MyISAM类型的表强调的是性能,其执行数度比InnoDB类型更快
1. MyISAM是文件存储的,可以进行直接在不同操作系统间拷贝使用
1. InnoDB不支持FULLTEXT类型的索引
1. InnoDB 中不保存表的具体行数,也就是说,执行`select count(\*) from table`时,InnoDB要扫描一遍整个表来计算有多少行,但是MyISAM只要简单的读出保存好的行数即可.
注意的是,当count(\*)语句包含 where条件时,两种表的操作是一样的.
1. 对于AUTO_INCREMENT类型的字段,InnoDB中必须包含只有该字段的索引,但是在MyISAM表中,可以和其他字段一起建立联合索引.
1. MyISAM 提供表锁, 而InnoDB提供行锁(locking on row level),提供与 Oracle 类型一致的不加锁读取(non-locking read in SELECTs). 
但是InnoDB表的行锁也不是绝对的,如果在执行一个SQL语句时MySQL不能确定要扫描的范围,InnoDB表同样会锁全表,例如`update table set num=1 where name like "%aaa%"`
1. MyISAM将数据与索引放在单独的文件中, 而InnoDB 把数据和索引存都放在表空间同一个数据文件(可能是多个实体文件)里.
1. DELETE FROM table时,InnoDB不会重新建立表,而是一行一行的删除.
1. LOAD TABLE FROM MASTER操作对InnoDB是不起作用的,解决方法是首先把InnoDB表改成MyISAM表,导入数据后再改成InnoDB表,但是对于使用的额外的InnoDB特性(例如外键)的表不适用.

InnoDB:  
InnoDB 是 MySQL 上第一个提供外键约束(FOREIGN KEY constraints)的表引擎.InnoDB 的设计目标是处理大容量数据库系统,它的 CPU 利用率是其它基于磁盘的关系数据库引擎所不能比的.
在技术上,InnoDB 是一套放在 MySQL 后台的完整数据库系统,InnoDB 在主内存中建立其专用的缓冲池用于高速缓冲数据和索引. 

MyISAM  
每张MyISAM 表被存放在三个文件 :frm 文件存放表格定义. 数据文件是MYD (MYData), 索引文件是MYI(MYIndex) 引伸.   
因为MyISAM相对简单所以在效率上要优于InnoDB,小型应用使用MyISAM是不错的选择.   
MyISAM表是保存成文件的形式,在跨平台的数据转移中使用MyISAM存储会省去不少的麻烦

**anto_increment 机制不同**  
InnoDB 如果你为一个表指定AUTO_INCREMENT列,在数据词典里的InnoDB表句柄包含一个名为自动增长计数器的计数器,它被用在为该列赋新值.
自动增长计数器仅被存储在主内存中,而不是存在磁盘上  
MyISAM 每表一个AUTO_INCREMEN列的内部处理.
MyISAM为INSERT和UPDATE操作自动更新这一列.这使得AUTO_INCREMENT列更快(至少10%).
在序列顶的值被删除之后就不能再利用.(当AUTO_INCREMENT列被定义为多列索引的最后一列,可以出现重使用从序列顶部删除的值的情况).
AUTO_INCREMENT值可用ALTER TABLE或myisamch来重置.
对于AUTO_INCREMENT类型的字段,InnoDB中必须包含只有该字段的索引,但是在MyISAM表中,可以和其他字段一起建立联合索引,更好和更快的auto_increment处理.

MySql innodb如果是共享表空间, ibdata1文件越来越大


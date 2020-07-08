# sql
## create
```sql
show create table tb;  // 查看建表语句
```

### 复制数据到新表
- 复制表结构(包括index, key 等)
	1. `CREATE TABLE 新表 LIKE 旧表`
- 复制数据
	1. `CREATE TABLE 新表 SELECT * FROM 旧表`
	1. `INSERT INTO 新表 SELECT * FROM 旧表`

## insert
- `INSERT INTO table_name VALUES(value1, value2, value3, ...);`
- `INSERT INTO table_name(column1, column2, column3, ...) VALUES(value1, value2, value3, ...);`

MySql避免重复插入记录(根据主键判重)

1. `insert ignore into xxx`
1. `replace into xxx`

## update
```sql
UPDATE table_name SET column1 = value1, column2 = value2, ...  WHERE some_column = some_value;
```

如果update 的一条记录在数据库不存在(就是说后面的where语句没有找到record), 不会对数据库产生影响, 同时语句的执行也不会报错.

两个表update

- `update a set age = (select age from b where b.name = a.name)`: 用到了子查询, 当数据量大的时候, 效率非常低
- `update a, b set a.age = b.age where a.name = b.name`: 比上面的子查询效率会高很多, 尤其是在建立有合适的索引时.

## alter
```sql
ALTER TABLE table_name ADD column_name datatype
alter table flows add column ip_prot tinyint(4) null default 0;

ALTER TABLE table_name DROP COLUMN column_name

ALTER TABLE table_name MODIFY COLUMN column_name datatype

ALTER TABLE table_name CHANGE old_col_name new_col_name column_definition
ALTER TABLE table_name RENAME TO new_table_name;
```

## select
mysql中提供了一个G标志,放到sql语句后,可以使一行的每个列打印到单独的行.
```sql
select * from t_goods \G
```

### limit
mysql不支持`select top n`的语法,应该用这个替换:
```sql
select * from tablename order by orderfield desc/asc limit offset, counter;
```
offset 指示从哪里开始查询,如果是0则是从头开始, counter 表示查询的个数

为了检索从某一个偏移量到记录集的结束所有的记录行,可以指定counter 为 -1:

### group by, where, having
可以在包含 GROUP BY 子句的查询中使用 WHERE 子句.在完成任何分组之前,将消除不符合 WHERE 子句中的条件的行.例如:
```sql
SELECT ProductModelID, AVG(ListPrice) AS 'Average List Price'
FROM Production.Product
WHERE ListPrice > $1000
GROUP BY ProductModelID
ORDER BY ProductModelID;
```

having子句与where有相似之处但也有区别,都是设定条件的语句.

1. **having只能用在group by之后,对分组后的结果进行筛选**(即使用having的前提条件是分组).
2. where肯定在group by 之前, 即也在having之前
3. where后的条件表达式里不允许使用聚合函数,而having可以

在查询过程中优先级
```sql
where 子句 > 聚合语句(sum, min, max, avg, count) > having子句
```
eg:

1. `select sum(num) as rmb from order where id>10` 只有对id大于10的记录才对进行聚合语句
1. `select reportsto as manager, count(*) as reports from employees group by reportsto having count(*) > 4`
首先将reportsto相同的group起来, 统计各个group里面成员的个数, 然后再筛选出大于4的groups.
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
为了尽可能地提高平均分,在计算平均分前不包括分数在60分以下的成绩.

分析

1. 要求显示学生姓名和平均分, 因此确定第1步`select s_name,avg(score) from student`
2. 计算平均分前不包括分数在60分以下的成绩,因此确定第2步 `where score>=60`
3. 显示个人平均分, 相同名字的学生(同一个学生)考了多门科目,因此按姓名分组.确定第3步 `group by s_name`
4. 显示个人平均分在70分以上 因此确定第4步 `having avg(s_score)>=70`
5. 按由高到低的顺序, 因此确定第5步 `order by avg(s_score) desc`

合起来就是:
```sql
select s_name,avg(s_score) from student
where score>=60
group by s_name
having avg(s_score)>=70
order by avg(s_score) desc
```

### join
[sql joins](http://www.techonthenet.com/sql/joins.php)

[A Visual Explanation of SQL Joins](http://blog.codinghorror.com/a-visual-explanation-of-sql-joins/)

There are 4 different types of SQL joins:

- SQL INNER JOIN (or sometimes called simple join)
- SQL LEFT OUTER JOIN (or sometimes called LEFT JOIN)
- SQL RIGHT OUTER JOIN (or sometimes called RIGHT JOIN)
- SQL FULL OUTER JOIN (or sometimes called FULL JOIN)

例子中用到的数据
We have a table called suppliers with two fields (supplier_id and supplier_name). It contains the following data:
```sql
supplier_id	supplier_name
10000	IBM
10001	Hewlett Packard
10002	Microsoft
10003	NVIDIA
```
We have another table called orders with three fields (order_id, supplier_id, and order_date). It contains the following data:
```sql
order_id	supplier_id	order_date
500125	10000	2003/05/12
500126	10001	2003/05/13
500127	10004	2003/05/14
```

#### SQL INNER JOIN (SIMPLE JOIN)
It is the most common type of SQL join.
SQL INNER JOINS return all rows from multiple tables where the join condition is met.

The syntax for the SQL INNER JOIN is:
```sql
SELECT columns
FROM table1
INNER JOIN table2
ON table1.column = table2.column;
```

In this visual diagram, the SQL INNER JOIN returns the shaded area:

![sql inner join](http://www.techonthenet.com/sql/images/inner_join.gif)

The SQL INNER JOIN would return the records where table1 and table2 intersect.

ex:
```sql
SELECT s.supplier_id, s.supplier_name, od.order_date
FROM suppliers AS s
INNER JOIN order AS od
ON s.supplier_id = od.supplier_id;
```
result
```sql
supplier_id	name	order_date
10000	IBM	2003/05/12
10001	Hewlett Packard	2003/05/13
```

#### SQL LEFT OUTER JOIN
This type of join returns all rows from the LEFT-hand table specified in the ON condition and only those rows from the other table where the joined fields are equal (join condition is met).

```sql
SELECT columns
FROM table1
LEFT OUTER JOIN table2
ON table1.column = table2.column;
```
In some databases, the LEFT OUTER JOIN keywords are replaced with LEFT JOIN.

![left outer join](http://www.techonthenet.com/sql/images/left_outer_join.gif)

```sql
SELECT suppliers.supplier_id, suppliers.supplier_name, orders.order_date
FROM suppliers
LEFT OUTER JOIN orders
ON suppliers.supplier_id = orders.supplier_id;
```
Our result set would look like this:
```sql
supplier_id	supplier_name	order_date
10000	IBM	2003/05/12
10001	Hewlett Packard	2003/05/13
10002	Microsoft	<null>
10003	NVIDIA	<null>
```

##### 选取 A - B(也就是在A中不在B中)
![A - B](http://blog.codinghorror.com/content/images/uploads/2007/10/6a0120a85dcdae970b012877702754970c-pi.png)

```sql
SELECT * FROM TableA
LEFT OUTER JOIN TableB
ON TableA.name = TableB.name
WHERE TableB.id IS null
```

#### SQL RIGHT OUTER JOIN
![right outer join](http://www.techonthenet.com/sql/images/right_outer_join.gif)

Example
```sql
SELECT orders.order_id, orders.order_date, suppliers.supplier_name
FROM suppliers
RIGHT OUTER JOIN orders
ON suppliers.supplier_id = orders.supplier_id;
```
result
```sql
order_id	order_date	supplier_name
500125	2013/08/12	Apple
500126	2013/08/13	Google
500127	2013/08/14	<null>
```

#### SQL FULL OUTER JOIN
This type of join returns all rows from the LEFT-hand table and RIGHT-hand table **with nulls in place where the join condition is not met**.
```sql
SELECT columns
FROM table1
FULL OUTER JOIN table2
ON table1.column = table2.column;
```

![full outer join](http://www.techonthenet.com/sql/images/full_outer_join.gif)

Example
```sql
SELECT suppliers.supplier_id, suppliers.supplier_name, orders.order_date
FROM suppliers
FULL OUTER JOIN orders
ON suppliers.supplier_id = orders.supplier_id;
```
result
```sql
supplier_id	supplier_name	order_date
10000	IBM	2013/08/12
10001	Hewlett Packard	2013/08/13
10002	Microsoft	<null>
10003	NVIDIA	<null>
<null>	<null>	2013/08/14
```

##### 选取 (A - B) \CUP (B - A)
![set of records unique to Table A and Table B](http://blog.codinghorror.com/content/images/uploads/2007/10/6a0120a85dcdae970b012877702769970c-pi.png)
```sql
SELECT * FROM TableA
FULL OUTER JOIN TableB
ON TableA.name = TableB.name
WHERE TableA.id IS null
OR TableB.id IS null
```

mysql 不支持full outer join, 但是可以使用left join 和right join来模拟.
```sql
SELECT * FROM t1
LEFT JOIN t2 ON t1.id = t2.id
UNION
SELECT * FROM t1
RIGHT JOIN t2 ON t1.id = t2.id
```

# [execution plan](http://my.oschina.net/zimingforever/blog/60233)
mysql的查看执行计划的语句很简单,explain + 你要执行的sql语句就OK了.

举一个例子
```sql
EXPLAIN SELECT * from employees where employees.gender='M'
```
结果示例:
|id |select_type |table     |type |possible_keys |key  |key_len |ref  |rows   |Extra      |
|---|------------|----------|-----|--------------|-----|--------|-----|-------|-----------|
|1  |SIMPLE      |employees |ALL  |NULL          |NULL |NULL    |NULL |300252 |Using where|

1. id是一组数字, 表示查询中执行select子句或操作表的顺序.
	- 如果id相同, 则执行顺序从上至下.
	- 如果是子查询, id的序号会递增, id越大则优先级越高, 越先会被执行.
	- id如果相同, 则可以认为是一组, 从上往下顺序执行, 所有组中, id越高, 优先级越高, 越容易执行
1. select_type有simple, primary, subquery, derived, union, unionresult.
	- simple: 表示查询中不包含子查询或者union.
	- primary: 当查询中包含任何复杂的子部分, 最外层的查询被标记成primary.
	- subquery: 在select或where列表中包含了子查询, 则子查询被标记成subquery.
	- derived: 在from的列表中包含的子查询被标记成derived.
	- union: 若第二个select出现在union后, 则被标记成union, 若union在from子句的子查询中, 外层的select被标记成derived.
	- union result: 从union表获取结果的select被标记成union result.
1. table: 访问哪个表
1. type叫访问类型, 表示在表中找到所需行的方式, 常见类型有all, index, range, ref, eq_ref, const, system, NULL 性能从左至右由差至好.
	- ALL: 即full table scan, mysql将遍历全表来找到所需要的行.
	- index: 为full index scan, 只遍历索引树.
	- range: 表示索引范围扫描, 对索引的扫描开始于一点,返回匹配的值域的行,常见于between, <, > 的查询.
	- ref: 为非唯一性索引扫描, 返回匹配某个单独值的所有行, 常见于非唯一索引即唯一索引的非唯一前缀进行的查找.
	- eq_ref: 表示唯一性索引扫描, 对于每个索引键, 表中只有一条记录与之匹配, 常见于主键或者唯一索引扫描.
	- const: 表示当对查询部分进行优化, 并转化成一个常量时, 使用这些类型访问. 比如将主键置于where列表中, mysql就能把该查询置成一个常量.
	- system: const的一个特例,当查询表中只有一行的情况下使用的是system
	- NULL: 表示在执行语句中,不用查表或索引.
1. possible keys: 表示能使用哪个索引在表中找到行, 查询涉及到的字段上若存在索引, 则该索引被列出, 但不一定被查询使用.
1. key表示查询时使用的索引. 若查询中使用了覆盖索引, 则该索引仅出现在key 中举个例子
1. keylen表示索引所使用的字节数, 可以通过该列结算查询中使用的索引长度
1. ref表示上述表的链接匹配条件, 即哪些列或常量可被用于查找索引列上的值.
1. rows表示根据mysql表统计信息及索引选用情况, 估算找到所需记录要读取的行数.
1. extra表示不在其他列并且也很重要的额外信息.
	- using index: 表示在相应的select中使用了覆盖索引.
	- using where: 表示存储引擎搜到记录后进行了后过滤(POST-FILTER),
		如果查询未能使用索引, using where的作用只是提醒我们mysql要用where条件过滤结果集.
	- using temporay: 表示用临时表来存储结果集, 常见于排序和分组查询.
	- using file sort: mysql中无法用索引完成的排序成为文件排序.

mysq的执行计划有一定局限性:

- EXPLAIN不会告诉你关于触发器,存储过程的信息或用户自定义函数对查询的影响情况
- EXPLAIN不考虑各种Cache
- EXPLAIN不能显示MySQL在执行查询时所作的优化工作
- 部分统计信息是估算的,并非精确值
- EXPALIN只能解释SELECT操作,其他操作要重写为SELECT后查看执行计划

# index
## CREATE INDEX
建表的时候创建索引
```sql
INDEX index_name(`field`),
```

ALTER TABLE用来创建普通索引,UNIQUE索引或PRIMARY KEY索引
```sql
ALTER TABLE table_name ADD INDEX index_name (column_list)
ALTER TABLE table_name ADD UNIQUE (column_list)
ALTER TABLE table_name ADD PRIMARY KEY (column_list)
```

`column_list` 指出对哪些列进行索引,多列时各列之间用逗号分隔.

索引名`index_name`可选,缺省时,MySQL将根据第一个索引列赋一个名称

PRIMARY KEY索引仅是一个名称为 PRIMARY 的 UNIQUE索引. 这表示一个表只能包含一个PRIMARY KEY, 因为一个表中不可能具有两个同名的索引.

## 删除索引
```sql
ALTER TABLE table_name DROP INDEX index_name
ALTER TABLE table_name DROP PRIMARY KEY
```

## 查看索引
```sql
show index from tblname;
show keys from tblname;
```

## [联合索引最左匹配](https://segmentfault.com/a/1190000015416513)
在mysql建立联合索引时会遵循最左前缀匹配的原则,即最左优先,在检索数据时从联合索引的最左边开始匹配,示例:
对列col1,列col2和列col3建一个联合索引
```sql
KEY test_col1_col2_col3 on test(col1,col2,col3);
```
联合索引 test_col1_col2_col3 实际建立了`(col1),(col1,col2),(col,col2,col3)`三个索引.

mysql创建联合索引的规则是首先会对联合合索引的最左边的,也就是第一个字段col1的数据进行排序,在第一个字段的排序基础上
然后再对后面第二个字段col2进行排序.其实就相当于实现了类似 order by col1 col2这样一种排序规则.

对于联合索引(col1,col2,col3),查询语句`SELECT * FROM test WHERE col2=2`;是否能够触发索引?
大多数人都会说NO,实际上却是YES.
原因:
```sql
EXPLAIN SELECT * FROM test WHERE col2=2;
EXPLAIN SELECT * FROM test WHERE col1=1;
```
观察上述两个explain结果中的type字段, 结果分别是 index, ref.
结合前面的execution plan 那一节, index 类型也是使用到了索引,只是是遍历整个索引树.

# Data Type And Functions
## date time
[时间差](http://blog.csdn.net/yzsind/article/details/8831429)

datetime 直接之间作差得到结果不是时间意义上的作差.
实际是mysql的时间相减是做了一个隐式转换操作, 直接把年月日时分秒拼起来, 如`2013-04-21 16:59:33` 直接转换为`20130421165933`, 由于时间不是十进制,所以最后得到的结果没有意义,这也是导致出现坑爹的结果.

要得到正确的时间相减秒值,有以下3种方法:

1. `time_to_sec(timediff(t2, t1))`: timediff 得到的结果是一个时间格式
2. `timestampdiff(second, t1, t2)`
3. `unix_timestamp(t2) - unix_timestamp(t1)`

时间位移: `ADDTIME('2014-05-26 18:26:21', '0:0:2')` 求后2秒的时间.

## str
- `length()`
- `char_length()`
- `locate(substr, str)`: 如果包含, 返回 > 0 的数, 否则返回0

## cast functions and operators
- BINARY: cast a string to a binary string, `BINARY str` is a shorthand for `CAST(str AS BINARY)`.
- `CAST(expr AS type)`: cast a value as a certain type
- CONVERT(): cast a value as a certain type

The BINARY operator casts the string following it to a binary string.
This is an easy way to force a column comparison to be done byte by byte rather than character by character.
```sql
SELECT 'a' = 'A';  // 1
SELECT BINARY 'a' = 'A';  // 0
SELECT 'a' = 'a ';  // 1
SELECT BINARY 'a' = 'a ';  // 0
```

## bit operators
[Bit Functions and Operators](https://dev.mysql.com/doc/refman/8.0/en/bit-functions.html)
|Name	       |Description                           |
|------------|--------------------------------------|
|&	         |Bitwise AND                           |
|>>	         |Right shift                           |
|<<	         |Left shift                            |
|^	         |Bitwise XOR                           |
|BIT_COUNT() |Return the number of bits that are set|
||	         |Bitwise OR                            |
|~	         |Bitwise inversion                     |

优先级: ~ > ^ > & > >> = << > |

# 存储引擎
## MyISAM
每张MyISAM 表被存放在三个文件: frm 文件存放表格定义. 数据文件是MYD (MYData), 索引文件是MYI(MYIndex) 引伸.
因为MyISAM相对简单所以在效率上要优于InnoDB,小型应用使用MyISAM是不错的选择.

MyISAM表是保存成文件的形式,在跨平台的数据转移中使用MyISAM存储会省去不少的麻烦

## Innodb
InnoDB 是 MySQL 上第一个提供外键约束(FOREIGN KEY constraints)的表引擎.InnoDB 的设计目标是处理大容量数据库系统,它的 CPU 利用率是其它基于磁盘的关系数据库引擎所不能比的.
在技术上,InnoDB 是一套放在 MySQL 后台的完整数据库系统,InnoDB 在主内存中建立其专用的缓冲池用于高速缓冲数据和索引.

InnoDB 是聚集索引,MyISAM 是非聚集索引.
聚簇索引的文件存放在主键索引的叶子节点上,因此 InnoDB 必须要有主键,通过主键索引效率很高.
但是辅助索引需要两次查询,先查询到主键,然后再通过主键查询到数据.因此,主键不应该过大,因为主键太大,其他索引也都会很大.
而 MyISAM 是非聚集索引,数据文件是分离的,索引保存的是数据文件的指针.主键索引和辅助索引是独立的.

InnoDB 把数据和索引存都放在表空间同一个数据文件(可能是多个物理文件)里.

1. MyISAM 则适合用于频繁查询的应用, innodb的引擎比较适合于插入和更新操作比较多的应用
1. MyISAM 类型不支持事务处理等高级处理, 也不支持外键,而InnoDB类型支持事务, 回滚, 外键和崩溃恢复能力等高级数据库功能(ACID: Atomicity, Consistency, Isolation, Durability)
1. MyISAM 类型的表强调的是性能, 其执行数度比InnoDB类型更快
1. InnoDB 中不保存表的具体行数,也就是说,执行`select count(*) from table`时,InnoDB要扫描整个表, 但MyISAM只要简单的读出保存好的行数即可. 但当count语句包含 where条件时,两种表的操作是一样的.
1. InnoDB 最小的锁粒度是行锁,MyISAM 最小的锁粒度是表锁.一个更新语句会锁住整张表,导致其他查询和更新都会被阻塞,因此并发访问受限.这也是 MySQL 将默认存储引擎从 MyISAM 变成 InnoDB 的重要原因之一;
1. innodb如果是共享表空间, ibdata1文件越来越大

innodb存储引擎可将所有数据存放于`ibdata*`的共享表空间, 也可将每张表存放于独立的.ibd文件的独立表空间. 共享表空间以及独立表空间都是针对数据的存储方式而言的.

- 共享表空间: 某一个数据库的所有的表数据,索引文件全部放在一个文件中,默认这个共享表空间的文件路径在data目录下. 默认的文件名为:ibdata1 初始化为10M.
- 独立表空间: 每一个表都将会以独立的文件方式来进行存储,每一个表都有一个.frm表描述文件,还有一个.ibd文件. 其中这个文件包括了单独一个表的数据内容以及索引内容,默认情况下它的存储位置也是在表的位置之中.

## RocksDB
### RocksDB与innodb的比较
1. innodb空间浪费, B tree分裂导致page内有较多空闲,尤其是刚分裂完成时, page利用率不高.
	对于顺序插入的场景,块的填充率较高.但对于随机场景,每个块的空间利用率就急剧下降了.
  反映到整体上就是一个表占用的存储空间远大于实际数据所需空间.
1. innodb现有的压缩效率也不高,压缩以block为单位,也会造成浪费.
1. 写入放大:innodb 更新以页为单位,最坏的情况更新N行会更新N个页.RocksDB append only方式
1. RocksDB对齐开销小:SST file (默认2MB)需要对齐,但远大于4k, `RocksDB_block_size`(默认4k) 不需要对齐,因此对齐浪费空间较少
1. RocksDB索引前缀相同值压缩存储
1. RocksDB占总数据量90%的最底层数据,行内不需要存储系统列seqid
1. MyRocks在MySQL 5.6/5.7就实现了逆序索引,基于逆序的列族实现,显然,逆序索引不能使用默认的default列族.
1. 基于LSM特性,MyRocks还以很低的成本实现了TTL索引,类似于HBase.

# 运维
## login
```bash
mysql -u user -p$passwd -h host -P port -D db --default-character-set=utf8
```

## conf
mysql的配置文件: `/etc/my.cnf`.

mysql server 自己带有一些示例配置文件: `rpm -ql mysql-server | grep cnf`,
得到的结果有`my-huge.cnf, my-innodb-heavy-4G.cnf, my-large.cnf, my-medium.cnf, my-small.cnf`,
将这些文件复制为`/etc/my.cnf`, 就可以修改mysql的默认配置, 对于现在的硬件配置, 使用`my-huge.cnf`就可以.

当由于系统的空间不够时, mysql 会在`/var/lib/mysql` 目录下创建一个hostname.error 的文件对错误进行说明,
而不会在 `/var/log/mysqld.log` 中说明.
如果经常出现空间不够的错误提示, 可以再`/etc/my.cnf` 中 修改数据的位置 `datadir=/var/lib/mysql`

## Backup
备份
```bash
## 备份整个数据库
mysqldump -u 用户名 -p 数据库名 > 导出的文件名

## 备份一个表
mysqldump -u 用户名 -p 数据库名 表名 > 导出的文件名
```

恢复
```sql
mysql > source 导入的文件名
```
或者
```bash
mysql -u root -p voice<voice.sql
```

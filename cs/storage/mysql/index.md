- [index](#index)
	- [sql](#sql)
		- [create index](#create-index)
		- [删除索引](#删除索引)
		- [查看索引](#查看索引)
	- [索引种类](#索引种类)
	- [回表](#回表)
	- [覆盖索引](#覆盖索引)
	- [联合索引最左匹配](#联合索引最左匹配)
	- [索引下推](#索引下推)
- [execution plan](#execution-plan)

# index
使用索引时的注意事项

- 索引不会包含有null 值的列: 只要列中包含有null 值都将不会被包含在索引中, 复合索引中只要有一列含有null 值, 那么这一列对
  于此复合索引就是无效的. 所以我们在数据库设计时建议不要让字段的默认值为 null.
- 使用短索引: 对字符串列进行索引, 如果可能应该指定一个前缀长度. 例如, 如果有一个 char(255)的列, 如果在前10 个或20 个字符
  内,多数值是惟一的,那么就不要对整个列进行索引.短索引不仅可以提高查询速度而且可以节省磁盘空间和 I/O 操作.
- 索引列排序: 查询只使用一个索引, 因此如果 where 子句中已经使用了索引的话, 那么order by 中的列是不会使用索引的.因此数据
  库默认排序可以符合要求的情况下不要使用排序操作, 尽量不要包含多个列的排序, 如果需要最好给这些列创建复合索引.
- 不使用 not in 和 <> 操作. 这不属于支持的范围查询条件,不会使用索引.

## sql
### create index
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

PRIMARY KEY 索引仅是一个名称为PRIMARY 的UNIQUE 索引. 这表示一个表只能包含一个 PRIMARY KEY, 因为一个表中不可能具有两个同
名的索引.

### 删除索引
```sql
ALTER TABLE table_name DROP INDEX index_name
ALTER TABLE table_name DROP PRIMARY KEY
```

### 查看索引
```sql
show index from tblname;
show keys from tblname;
```

## 索引种类
B+-tree 索引

show index 得到的结果显示为BTREE, 实际上是B+ Tree.
[Does mysql use B-tree,B+tree or both?](
https://dba.stackexchange.com/questions/204561/does-mysql-use-b-tree-btree-or-both/204573)

哈希索引
```sql
alter table user add index hash_gender using hash(gender);
```
会发现name 的索引类型还是为BTREE, 在innodb 上创建哈希索引, 被称之为伪哈希索引, 和真正的哈希索引不是一回事的.

在Innodb 存储引擎中有一个特殊的功能叫做, 自适应哈希索引, 当索引值被使用的非常频繁时, 它会在内存中基于BTREE 索引之上再创
建一个哈希索引, 那么就拥有了哈希索引的一些特点, 比如快速查找.

哈希索引存储的是哈希值和行指针, 没有存储key 值, 字段值, 但哈希索引多数是在内存完成的, 检索数据是非常快的.

## 回表
还是用上边的案例,id 为主键索引,name 为普通索引.此时查询语句为:
```sql
select id,name,age from table where name = 'kaka'
```
那么这条语句会先在 name 的这颗 B+Tree 中寻找到主键 id,然后在根据主键 id 的索引获取到数据并且返回.

其实这个过程就是从非聚簇索引跳转到聚簇索引中查找数据,被称为回表,也就是说当你查询的字段为非聚簇索引,但是非聚簇索引中没有
将需要查询的字段全部包含就是回表.

在这个案例中,非聚簇索引 name 的叶子节点只有 id,并没有 age,所以会跳转到聚簇索引中,根据 id 在查询整条记录返回需要的字段数据.

## 覆盖索引
覆盖索引,根据名字都能理解的差不多,就是查询的所有字段都创建了索引!
此时查询语句为:
```sql
select id,name from table where name = 'kaka'
```
那么这条语句就是使用了覆盖索引,因为 id 和 name 都为索引字段,查询的字段也是这俩个字段,所以被称为索引覆盖.

也就是说当非覆盖索引的叶子节点中包含了需要查询的字段时就被称为覆盖索引.

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

## 索引下推
还是使用这条 sql 语句:
```sql
select * from table where name = ? and age = ?
```

索引下推是在 MySQL 5.6 及以后的版本出现的.之前的查询过程是,先根据 name 在存储引擎中获取数据,然后在根据 age 在 server 层
进行过滤.

在有了索引下推之后,查询过程是根据 name,age 在存储引擎获取数据,返回对应的数据,不再到 server 层进行过滤.

当你使用 Explain 分析 SQL 语句时,如果出现了Using index condition, 那就是使用了索引下推,索引下推是在组合索引的情况出现几
率最大的.

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
1. type叫访问类型, 表示在表中找到所需行的方式, 常见类型有all, index, range, ref, eq_ref, const, system, NULL 性能从左至
  右由差至好.
	- ALL: 即full table scan, mysql将遍历全表来找到所需要的行.
	- index: 为full index scan, 只遍历索引树.
	- range: 表示索引范围扫描, 对索引的扫描开始于一点,返回匹配的值域的行,常见于between, <, > 的查询.
	- ref: 为非唯一性索引扫描, 返回匹配某个单独值的所有行, 常见于非唯一索引即唯一索引的非唯一前缀进行的查找.
	- eq_ref: 表示唯一性索引扫描, 对于每个索引键, 表中只有一条记录与之匹配, 常见于主键或者唯一索引扫描.
	- const: 表示当对查询部分进行优化, 并转化成一个常量时, 使用这些类型访问. 比如将主键置于where列表中, mysql 就能把该查询
    置成一个常量.
	- system: const的一个特例,当查询表中只有一行的情况下使用的是system
	- NULL: 表示在执行语句中,不用查表或索引.
1. possible keys: 表示能使用哪个索引在表中找到行, 查询涉及到的字段上若存在索引, 则该索引被列出, 但不一定被查询使用.
1. key表示查询时使用的索引. 若查询中使用了覆盖索引, 则该索引仅出现在key 中举个例子
1. keylen表示索引所使用的字节数, 可以通过该列结算查询中使用的索引长度
1. ref表示上述表的链接匹配条件, 即哪些列或常量可被用于查找索引列上的值.
1. rows表示根据mysql表统计信息及索引选用情况, 估算找到所需记录要读取的行数.
1. extra表示不在其他列并且也很重要的额外信息.
	- using index: 表示在相应的select中使用了覆盖索引.
	- using where: 表示存储引擎搜到记录后进行了后过滤(POST-FILTER), 如果查询未能使用索引, using where的作用只是提醒我们
    mysql要用where条件过滤结果集.
	- using temporay: 表示用临时表来存储结果集, 常见于排序和分组查询.
	- using file sort: mysql中无法用索引完成的排序成为文件排序.

mysq的执行计划有一定局限性:

- EXPLAIN不会告诉你关于触发器,存储过程的信息或用户自定义函数对查询的影响情况
- EXPLAIN不考虑各种Cache
- EXPLAIN不能显示MySQL在执行查询时所作的优化工作
- 部分统计信息是估算的,并非精确值
- EXPALIN只能解释SELECT操作,其他操作要重写为SELECT后查看执行计划


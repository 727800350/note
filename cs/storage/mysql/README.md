# create
```sql
show create table tb;  // 查看建表语句
```

必须使用utf8mb4 字符集, mysql 中的utf-8 并非真正的utf-8, 而utf8mb4 才是真正的utf-8.

必须把字段定义为 not null 并且提供默认值

1. null 的列使索引/索引统计/值比较都更加复杂, 对mysql 来说更难优化.
1. null 这种类型mysql 内部需要进行特殊处理, 增加数据库处理记录的复杂性, 同等条件下, 表中有较多空字段的时候, 数据库的处理
  性能会降低很多.
1. null 值需要更多的存储空间, 无论是表还是索引中每行中的null 的列都需要额外的空间来标识.

## 复制数据到新表
- 复制表结构(包括index, key 等)
	1. `create table 新表 like 旧表`
- 复制数据
	1. `create table 新表 select * from 旧表`
	1. `insert into 新表 select * from 旧表`

# insert
```sql
insert into table_name values(value1, value2, value3, ...);

insert into table_name(column1, column2)
values
(v11, v12),
(v21, v22);
```

mysql避免重复插入记录(根据主键判重)

1. `insert ignore into xxx`
1. `replace into xxx`

列设置为AUTO INCREMENT时,如果在表中达到最大值,会发生什么情况?
```plain
> INSERT INTO test (name) VALUES ('d')
[2021-06-16 12:56:30] [23000][1062] Duplicate entry '9223372036854775807' for key 'test.PRIMARY'
```
也就是加到最大后, 再新增一行记录, mysql 还是会尝试使用最大值, 但是因为最大值已经被使用了, 所以出现错误.
(网上搜到的答案还都加了一句"因为密钥已被使用". 但是没明白什么密钥)

`LAST_INSERT_ID` 将返回由auto increment 分配的最后一个值,并且不需要指定表名称.

需要注意的是,如果使用单个INSERT语句将多行插入到表中,则LAST_INSERT_ID函数将返回第一行的ID, 而不是最后一行的.
例如 AUTO_INCREMENT列的当前值为1,并在表中插入3行.当您使用LAST_INSERT_ID函数获取最后一个插入ID时,将获得2而不是4.

LAST_INSERT_ID函数基于客户端独立原则.这意味着特定客户端的LAST_INSERT_ID函数返回的值是该客户端生成的值.这样就确保每个客户
端可以获得自己的唯一ID.

# update
```sql
update table_name set column1 = value1, column2 = value2, ...  where some_column = some_value;
```

如果update 的一条记录在数据库不存在(就是说后面的where语句没有找到record), 不会对数据库产生影响, 同时语句的执行也不会报错.

两个表update

- `update a set age = (select age from b where b.name = a.name)`: 用到了子查询, 当数据量大的时候, 效率非常低
- `update a, b set a.age = b.age where a.name = b.name`: 比上面的子查询效率会高很多, 尤其是在建立有合适的索引时.

# alter
```sql
alter table table_name add column_name datatype
alter table flows add column ip_prot tinyint(4) not null default 0;

alter table table_name drop column column_name

alter table table_name modify column column_name datatype

alter table table_name change old_col_name new_col_name column_definition
alter table table_name rename to new_table_name;
```

# select
mysql中提供了一个G标志,放到sql语句后,可以使一行的每个列打印到单独的行.
```sql
select * from t_goods \G
```

## limit
mysql不支持`select top n`的语法,应该用这个替换:
```sql
select * from tablename order by orderfield desc/asc limit offset, counter;
```
offset 指示从哪里开始查询,如果是0则是从头开始, counter 表示查询的个数

为了检索从某一个偏移量到记录集的结束所有的记录行,可以指定counter 为 -1:

## group by, where, having
可以在包含 group by 子句的查询中使用 where 子句.在完成任何分组之前,将消除不符合 where 子句中的条件的行.例如:
```sql
select model_id, avg(price) as 'average_price'
from product
where price > 1000
group by model_id
order by model_id;
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

## join
[sql joins](http://www.techonthenet.com/sql/joins.php)

There are 4 different types of SQL joins:

- sql inner join (or sometimes called simple join)
- sql left outer join (or sometimes called left join)
- sql right outer join (or sometimes called right join)
- sql full outer join (or sometimes called full join)

### inner join (simple join)
It is the most common type of SQL join.
sql inner joins return all rows from multiple tables where the join condition is met.

```sql
select columns
from table1
inner join table2
on table1.column = table2.column;
```

<img src="./pics/sql/inner_join.gif" alt="inner join" width="20%"/>

The sql inner join would return the records where table1 and table2 intersect.

### left outer join
This type of join returns all rows from the left-hand table specified in the ON condition and only those rows from the
other table where the joined fields are equal (join condition is met).

In some databases, the left outer join keywords are replaced with left join.

<img src="./pics/sql/left_outer_join.gif" alt="left outer join" width="20%"/>

### right outer join
This type of join returns all rows from the right-hand table specified in the ON condition and only those rows from the
other table where the joined fields are equal (join condition is met).

<img src="./pics/sql/right_outer_join.gif" alt="right outer join" width="20%"/>

### full outer join
This type of join returns all rows from the left-hand table and RIGHT-hand table
**with nulls in place where the join condition is not met**.

<img src="./pics/sql/full_outer_join.gif" alt="full outer join" width="20%"/>


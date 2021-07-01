# mysql lock
mysql中有三类锁,分别为行锁,表锁,页锁.首先需要明确的是这三类锁是是归属于哪种存储引擎的:

- 行锁: innodb 存储引擎
- 表锁: myisam, memory, innodb 存储引擎
- 页锁: BDB 存储引擎

myisam 在执行查询语句(select)前, 会自动给涉及的所有表加读锁, 在执行更新操作(update, delete, insert等)前, 会自动给涉及的
表加写锁.
myisam 问题一次获得sql语句所需要的全部锁, 这也正是myisam 表不会出现死锁(deadlock free)的原因.

对于innodb表,在绝大部分情况下都应该使用行级锁,因为事务和行锁往往是我们之所以选择innodb表的理由.但在个别特殊事务中,也可以
考虑使用表级锁.

1. 事务需要更新大部分或全部数据,表又比较大,如果使用默认的行锁,不仅这个事务执行效率低,而且可能造成其他事务长时间锁等待和
  锁冲突,这种情况下可以考虑使用表锁来提高该事务的执行速度.
1. 事务涉及多个表,比较复杂,很可能引起死锁,造成大量事务回滚.这种情况也可以考虑一次性锁定事务涉及的表,从而避免死锁,减少数
  据库因事务回滚带来的开销.

当然,应用中这两种事务不能太多,否则,就应该考虑使用ｍyisaｍ表.

# innodb locking
- 表级锁: 开销小, 加锁快, 不会出现死锁, 锁定粒度大, 发生锁冲突的概率最高, 并发度最低.
- 行级锁: 开销大, 加锁慢, 会出现死锁, 锁定粒度最小, 发生锁冲突的概率最低, 并发度也最高.

## [锁的种类](https://dev.mysql.com/doc/refman/5.6/en/innodb-locking.html)
### Shared Lock And Exclusive Locks
InnoDB implements standard row-level locking where there are two types of locks, shared (S) locks and exclusive (X) locks.

- A shared (S) lock permits the transaction that holds the lock to read a row.
- An exclusive (X) lock permits the transaction that holds the lock to update or delete a row.

### Intention Locks
为了支持不同粒度的锁而设计的一种 表级别锁(但不是通常认为的表锁) which permits coexistence of row locks and table locks.
它表示了表之后将被加上哪种行级锁.意向锁的分类如下:

- Intention Shared Lock,意向共享锁(IS) ,表示事务将要在表上加共享锁,规则是在表中申请某些行的共享锁之前,必须先申请IS锁.
- Intention Exclusive Lock,意向排他锁(IX) ,表示事务将要在表上加排他锁,规则是在表中申请某些行的排他锁之前,必须先申请IX锁.
```sql
SELECT ... LOCK IN SHARE MODE  // 该语句将会在表上加IS锁,同时在对应的记录上加上S锁.
SELECT ... FOR UPDATE  // 该语句将会在表上加上IX锁,同时在对应的记录上加上X锁
```

### Record Locks
A record lock is a lock on an index record. For example, `SELECT c1 FROM t WHERE c1 = 10 FOR UPDATE;`
prevents any other transaction from inserting, updating, or deleting rows where the value of t.c1 is 10.

Record locks always lock index records, even if a table is defined with no indexes.
For such cases, InnoDB creates a hidden clustered index and uses this index for record locking.

### GAP Locks
A gap lock is a lock on a gap between index records, or a lock on the gap before the first or after the last index record.

For example, `SELECT c1 FROM t WHERE c1 BETWEEN 10 and 20 FOR UPDATE;` prevents other transactions from inserting a value of
15 into column t.c1, whether or not there was already any such value in the column, because the gaps between all existing
values in the range are locked.

### Next-key Locks
Next-key锁是记录锁和Gap锁的结合,锁住了记录和记录之前的一段Gap区间. 比如索引包含了10,11,13和20,那么Next-key分出的区间如下:
```info
(negative infinity, 10]
(10, 11]
(11, 13]
(13, 20]
(20, positive infinity)
```

### Insert Intention Locks
An insert intention lock is a type of gap lock set by INSERT operations prior to row insertion.
This lock signals the intent to insert in such a way that multiple transactions inserting into the same index gap
need not wait for each other if they are not inserting at the same position within the gap.

[论 MySql InnoDB 如何通过插入意向锁控制并发插入](https://juejin.im/post/5b865859e51d4538e331ae9a)

## [加锁原则](https://juejin.im/post/5dc6c5325188250b92054dd8)
对于 InnoDB 而言,虽然加锁的类别繁多,加锁形式也灵活多样,但也遵循了一些原则:

- 对于 `select ... from ...` 语句,使用快照读,一般情况下不加锁,仅在Serializable级别会加共享读锁
- 对于 `select ... from ... lock in share mode` 语句使用当前读,加共享读锁(S锁)
- 对于 `select ... from ... for update` 语句,为当前读,加排他写锁(X锁)
- 常见 DML语句(insert, delete, update),使用当前读,加排他写锁(X锁)
- 常见 DDL语句(alter table, create table ...)等,加的是表级锁

```sql
CREATE TABLE `t_user` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `no` char(18) NOT NULL DEFAULT '' COMMENT '身份证',
  `name` varchar(50) NOT NULL DEFAULT '' COMMENT '姓名',
  `age` int(4) NOT NULL DEFAULT '0' COMMENT '年龄',
  PRIMARY KEY (`id`),
  UNIQUE KEY `no` (`no`),
  KEY `name` (`name`)
) ENGINE=InnoDB COMMENT='用户表';
```
默认插入数据如下:

| id | no   | name | age |
|----|------|------|-----|
| 1  | 0001 | 张三 | 20  |
| 3  | 0003 | 李四 | 25  |
| 5  | 0005 | 王五 | 50  |
| 7  | 0007 | 王五 | 23  |
| 9  | 0009 | 赵六 | 28  |

### Read Uncommitted 级别
Read Uncommitted 级别是事务隔离的最低级别,在此隔离级别下会存在脏读的现象,会影响到数据的正确性,因此我们在日常开发过程中很少使用该隔离级别.
在此隔离级别下更新语句采取的是普通的加行锁的机制,Read Committed的加锁过程与Read Uncommitted一致.
由于Read Committed使用范围较Read Uncommitted更广,在Read Committed级别下详细分析.

### Read Committed 级别
Read Committed级别采取了一致性读策略,解决了事务的脏读问题,我们以下简称为RC级别.
在此级别下更新语句加锁与Read Uncommitted一致,可能存在的锁有行锁与意向锁.
加锁过程采取了Semi-consistent read优化策略,对于扫描过的数据如若不匹配,加锁后会立即释放.

#### 使用主键
假设我们需要在上述t_user表格中,删除ID=7的王五这一条记录,语句为:
```sql
delete from t_user where id = 7;
```
由于使用了主键,只需对该条记录加X锁即可,其加锁过程如下:

<img src="./pics/lock/rc_primary_key.png" alt="使用主键删除的加锁过程" width="25%"/>

#### 使用唯一索引
假设我们通过身份证no这个唯一索引来删除id=7这条数据会如何加锁呢?
```sql
delete from t_user where no = '0007';
```
由于唯一索引为二级索引,Innodb 首先通过唯一索引对数据进行过滤,对于0007唯一索引加X锁,然后还需要在聚集索引上对主键=7的数据进行加X锁.

<img src="./pics/lock/rc_unique_index.png" alt="使用唯一索引删除的加锁过程" width="50%"/>

#### 使用非唯一索引
假设我们使用非唯一索引,那么情况又会如何呢?
```sql
delete from t_user where name = '王五';
```
由于唯一索引为二级索引,Innodb 首先通过索引对数据进行过滤,对于王五的两条索引加X锁,然后还需要在聚集索引上对主键=5,7 的数据进行加X锁.

<img src="./pics/lock/rc_ordinary_index.png" alt="使用非唯一索引删除的加锁过程" width="50%"/>

#### 未使用任何索引
如果不使用任何索引,情况会是怎样呢?
```sql
delete from t_user where age = 23;
```
由于删除语句没有使用任何索引,那么 InnoDB 必须进行全表扫描以确定哪条数据需要删除.也就是说首先需要对全表的所有数据进行加锁,
InnoDB 在RC级别下的加锁过程采取了Semi-consistent read优化策略,对于扫描过的数据如若不匹配,加锁后会立即释放.

<img src="./pics/lock/rc_without_index.png" alt="未使用任何索引删除的加锁过程" width="25%"/>

#### 插入过程加锁
那么对于插入过程,RC级别又是如何加锁的呢?
```sql
insert into t_user(id, no, name, age) values(4, '00004', '小灰灰', 8);
```
InnoDB事实上只对主键加了X锁.

<img src="./pics/lock/rc_insert.png" alt="插入的加锁过程" width="25%"/>

### Read Repeatable 级别
Read Repeatable级别引入了间隙锁等一系列机制,来防止其他事务的插入操作,以下简称RR级别.
但与此同时间隙锁的范围也带来了很多额外的开销与问题,其中之一就有由于引入了间隙锁加大了锁的粒度范围,使用不当容易造成死锁.
由于RR级别下可以通过参数innodb_locks_unsafe_for_binlog来配置是否开启gap锁,在此我们讨论的是开启gap锁的情况.

#### 使用主键
假设我们需要在上述t_user表格中,删除ID=7的王五这一条记录,语句为:
```sql
delete from t_user where id = 7;
```
由于使用了主键,可以唯一确认影响的记录,只需对该条记录加X锁即可,其加锁过程与RC级别下的使用主键加锁过程相同.

<img src="./pics/lock/rr_primary_key.png" alt="使用主键删除的加锁过程" width="25%"/>

#### 使用唯一索引
假设我们通过身份证no这个唯一索引来删除id=7这条数据会如何加锁呢?
```sql
delete from t_user where no = '0007';
```
由于唯一索引为二级索引,Innodb 首先通过唯一索引对数据进行过滤,对于0007唯一索引加X锁,然后还需要在聚集索引上对主键=7的数据进行加X锁.

<img src="./pics/lock/rr_unique_index.png" alt="使用唯一索引删除的加锁过程" width="50%"/>

#### 使用非唯一索引
假设我们使用非唯一索引,那么情况又会如何呢?
```sql
delete from t_user where name = '王五';
```
由于使用索引为二级索引,Innodb 首先通过索引对数据进行过滤,由于普通索引不能保证影响数据范围唯一,有可能其他的事务在对二者之间的间隙操作添加新数据,
因此还需要对于王五之间的间隙进行加锁,以防有其他事务在事务提交前在此间隙插入数据,最后还需要在聚集索引上对主键=5,7 的数据进行加X锁.

<img src="./pics/lock/rr_ordinary_index.png" alt="使用非唯一索引删除的加锁过程" width="50%"/>

#### 未使用任何索引
那么在RR级别下,如果不使用索引会导致什么情况呢?
```sql
delete from t_user where age = 23;
```
如若不使用任何索引,InnoDB只能够通过全表扫描以确定需要删除的数据,因此首先会需要对所有数据进行加锁,此外由于需要避免其他事务插入,
还需要对所有的间隙进行加锁,这对InnoDB性能影响非常显著.

<img src="./pics/lock/rr_without_index.png" alt="未使用任何索引删除的加锁过程" width="25%"/>

#### 插入过程
在RR级别下,插入过程是如何加锁的呢?
```sql
insert into t_user(id, no, name, age) values(4, '00004', '小灰灰', 8);
```
插入过程是不需要增加gap锁的,因此RR级别下的加锁过程与RC级别下的加锁过程差不多.
依照官方文档,插入过程隐式的加了插入意向锁,该锁虽然为间隙锁,但大多数时候并不会影响其他行的插入.

<img src="./pics/lock/rr_insert.png" alt="插入的加锁过程" width="25%"/>

### Serializable 级别
Serializable 级别是事务隔离的最高级别,在此级别下所有的请求会进行串行化处理.在InnoDB中该级别下的 更新语句加锁过程与Read Repeatable下一致.

## innodb 行锁实现方式
[mysql 行锁的实现](https://lanjingling.github.io/2015/10/10/mysql-hangsuo/)

InnoDB行锁是通过给索引上的索引项加锁来实现的,这一点MySQL与Oracle不同,后者是通过在数据块中对相应数据行加锁来实现的.
InnoDB这种行锁实现特点意味着:只有通过索引条件检索数据,InnoDB才使用行级锁,否则,InnoDB将使用表锁!
在实际应用中,要特别注意InnoDB行锁的这一特性,不然的话,可能导致大量的锁冲突,从而影响并发性能.

在不通过索引条件查询(select for update)的时候,InnoDB确实使用的是表锁,而不是行锁.


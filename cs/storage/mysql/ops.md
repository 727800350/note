- [login](#login)
- [conf](#conf)
- [backup](#backup)

# login
```bash
mysql -u user -p$passwd -h host -P port -D db --default-character-set=utf8
```

# conf
mysql的配置文件: `/etc/my.cnf`.

mysql server 自己带有一些示例配置文件: `rpm -ql mysql-server | grep cnf`,
得到的结果有`my-huge.cnf, my-innodb-heavy-4G.cnf, my-large.cnf, my-medium.cnf, my-small.cnf`,
将这些文件复制为`/etc/my.cnf`, 就可以修改mysql的默认配置, 对于现在的硬件配置, 使用`my-huge.cnf`就可以.

当由于系统的空间不够时, mysql 会在`/var/lib/mysql` 目录下创建一个hostname.error 的文件对错误进行说明,
而不会在 `/var/log/mysqld.log` 中说明.
如果经常出现空间不够的错误提示, 可以再`/etc/my.cnf` 中 修改数据的位置 `datadir=/var/lib/mysql`

MySQL 登录情况下可以通过show variables 命令查看,以下介绍两个研发需要关注的MySQL配置参数.
```sql
show variables like '%max_connections%'
show variables where variable_name = 'version';
```

`innodb_flush_log_at_trx_commit`: 用于设置InnoDB 在事务提交后的日志写入频率,需要根据具体的业务场景配置此参数的值.

- 0: 表示log buffer 每秒都会写入到日志文件并刷新到磁盘.在事务提交的时候,不会主动触发写入磁盘的操作,也就是log buffer的刷
  新操作和事务没有关系.这种情况下MySQL的性能最好,但如果mysqld 进程崩溃,通常会导致最后1秒的日志丢失.
- 1: 这是默认值,表示每次事务提交时,log buffer 会被写入到日志文件并刷新到磁盘.这种配置最安全,但由于每次事务都需要进行磁盘
  I/O,所以性能最差.
- 2: 表示每次事务提交时log buffer都会写入到日志文件,但是每隔1秒才会将日志文件内容刷新到磁盘.该场景下,如果mysqld进程奔溃,
  不会造成日志的丢失,但如果操作系统奔溃,通常会导致最后 1秒的日志丢失.

对于一些数据一致性要求不高的场景,设置为2就行了. 如果要求最高的性能则可以设置为0. 像一些对数据一致性要求很严格的业务场景(
如金融支付),即使性能最差,也最好设置为1保证数据没有丢失.

`sync_binlog`: 用于控制MySQL binlog同步到磁盘的策略,需要根据具体的业务场景配置此参数的值.

- 0: 表示MySQL依赖操作系统不定期把binlog刷新到磁盘.
- n(n>0): 表示MySQL每完成n次事务,就调用文件系统的刷新操作将缓存刷到磁盘.(如果启用了autocommit,那么每个语句都会写一次
  binlog,否则每个事务写一次.)

`sync_binlog`设置0 可以保证最高的抗并发能力,但是数据库可能会出现阻塞(binlog日志量太大,操作系统同步磁盘时占用过多资源导致
业务出现阻塞), 如果业务要求数据库合适的抗并发能力,且不希望出现阻塞, 设置n(n>1, 根据具体业务和压测结果设置), 对于金融支付
这类场景,为保证安全性,还是推荐设置为1.

# backup
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


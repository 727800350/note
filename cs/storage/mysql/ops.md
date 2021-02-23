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


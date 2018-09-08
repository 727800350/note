# mysql
## mysql-connector-python
安装包: `pip install --alow-all-external mysql-connector-python`

demo
```Python
import mysql.connector
conn = mysql.connector.connect(host="localhost", user="test", passwd="123456", db="test", buffered=True)
cursor = conn.cursor()
cursor.execute("show databases;")
r = cursor.fetchone()
print r
cursor.close()
conn.close()
```

process a query result
```Python
# Using a while-loop
cursor.execute("SELECT * FROM employees")
row = cursor.fetchone()
while row is not None:
	print(row)
	row = cursor.fetchone()
				
# Using the cursor as iterator 
cursor.execute("SELECT * FROM employees")
for row in cursor:
	print(row)
```

Inserting or updating data is also done using the handler structure known as a cursor. 
When you use a transactional storage engine such as InnoDB (the default in MySQL 5.5 and later), 
you must commit the data after a sequence of INSERT, DELETE, and UPDATE statements.

[insert demo](/../demo/python/db/insert.py)

update 也可以像上面的demo 一样使用.

bind 的使用
```Python
add_user = """INSERT INTO DB.tbluser 
              (username, department, startyear, currentpos, link) 
              VALUES (%s, %s, %s, %s, %s)"""
```

当需要插入mysql 中的`null` 时, 在python 中将变量赋值为 `None`.

transaction
```Python
cnx.start_transaction(consistent_snapshot=bool, isolation_level=level, readonly=access_mode)

con.start_trsansaction()
cursor.execute(sql)
con.commit()
cursor.close()
con.close()
```
- The default `consistent_snapshot` value is False. If the value is True, Connector/Python sends WITH CONSISTENT SNAPSHOT with the statement.
	MySQL ignores this for isolation levels for which that option does not apply.
- The default `isolation_level` value is None, and permitted values are 'READ UNCOMMITTED', 'READ COMMITTED', 'REPEATABLE READ', and 'SERIALIZABLE'. 
	If the isolation_level value is None, no isolation level is sent, so the default level applies.
- The `readonly` argument can be True to start the transaction in READ ONLY mode or False to start it in READ WRITE mode. 


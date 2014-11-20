library(RMySQL) # will load DBI as well
## 打开一个MySQL数据库的连接
> con <- dbConnect(dbDriver("MySQL"), dbname = "test")
## 列出数据库中表
> dbListTables(con)
## 把一个数据框导入到数据库，删除任何已经存在的拷贝
> data(USArrests)
> dbWriteTable(con, "arrests", USArrests, overwrite = TRUE)
TRUE
> dbListTables(con)
[1] "arrests"
## 获得整个表
> dbReadTable(con, "arrests")
               Murder Assault UrbanPop Rape
Alabama          13.2     236       58 21.2
Alaska           10.0     263       48 44.5
Arizona           8.1     294       80 31.0
Arkansas          8.8     190       50 19.5
...
## 从导入的表中查询
> dbGetQuery(con, paste("select row_names, Murder from arrests", "where Rape > 30 order by Murder"))
   row_names Murder
1   Colorado    7.9
2    Arizona    8.1
3 California    9.0
4     Alaska   10.0
5 New Mexico   11.4
6   Michigan   12.1
7     Nevada   12.2
8    Florida   15.4
> dbRemoveTable(con, "arrests")
> dbDisconnect(con)

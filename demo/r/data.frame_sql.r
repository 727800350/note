library(sqldf)

newdf <- sqldf("select * from mtcars where carb=1 order by mpg", row.names = TRUE)
## 参数row.names = TRUE 将原始数据框中的行名延续到新的数据框中

newdf <- sqldf("select avg(mpg) as avg_mpg, avg(disp) as avg_disp, gear from mtcars where cyl in (4, 6) group by gear")


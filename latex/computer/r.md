**R语言的启动**

- R语言启动后会首先查找有无.Rprofile文档,用户可通过编辑.Rprofile文档来自定义R启动环境,该文件可放在工作目录或安装目录中.
- 之后R会查找在工作目录有无.RData文档,若有的话将自动加载恢复之前的工作内容.
- 在R中所有的默认输入输出文件都会在工作目录中.getwd() 报告工作目录,setwd() 负责设置工作目录.在win窗口下也可以点击Change Working Directory来更改.
- Sys.getenv('R_HOME') 会报告R主程序安装目录
- ?Startup可以得到更多关于R启动时的帮助

退出
`> q()`
At this point you will be asked whether you want to save the data from your R session.
Data which is saved will be available in future R sessions.

注释: R 语言里面没有像C 语言那样的多行注释

# Objects
R是一种基于对象(Object)的语言,所以你在R语言中接触到的每样东西都是一个对象,一串数值向量是一个对象,一个函数是一个对象,一个图形也是一个对象.
基于对象的编程(OOP)就是在定义类的基础上,创建与操作对象.

- `attributes()` 获得对象属性
- `str()`,它能以简洁的方式显示对象的数据结构及其内容

## NA
Missing values are represented by the symbol NA (not available).   
Impossible values (e.g., dividing by zero) are represented by the symbol NaN (not a number).   
R uses the same symbol for character and numeric data.对于factor 用 <NA> 表示, 但是仍然可以使用is.na(f)进行检测

Testing for Missing Values  
对于一般的数据类型使用`is.na` 进行检测, 而对于data.frame 需要使用complete.cases 进行检测(注意complete.cases输出中为TRUE表示这个record 是完整的)

测试函数
```
is.finite(x)
is.infinite(x)
is.nan(x)
is.na(x)
```
几个特殊例子
```
pi / 0 ## = Inf a non-zero number divided by zero creates infinity
0 / 0  ## =  NaN

1/0 + 1/0 # Inf
1/0 - 1/0 # NaN, infinity - infinity 的结果不定, 所以是NaN
```

不同类型NA的判断
```
## numeric
y <- c(1,2,3,NA)
is.na(y) # returns a vector (F F F T)

## charactor
ch <- c("test", " ", "", NA)
is.na(ch) # return FALSE FALSE FALSE  TRUE

## factor
head(flows$app_name)
[1] MICROSOFT-DS MICROSOFT-DS <NA> <NA> <NA> <NA>
Levels: DOMAIN FTP HTTP MICROSOFT-DS NETBIOS-NS SMTP
is.na(head(flows$app_name))
[1] FALSE FALSE  TRUE  TRUE  TRUE  TRUE

## data.frame
head(flows[,c('app_name', 'srcaddr')])
##       app_name      srcaddr
## 1 MICROSOFT-DS 10.68.106.21
## 2 MICROSOFT-DS  10.231.2.89
## 3         <NA> 10.192.24.50
## 4         <NA>    10.1.17.1
## 5         <NA>   10.2.88.12
## 6         <NA>    10.1.17.1
is.na(head(flows[,c('app_name', 'srcaddr')]))
##   app_name srcaddr
## 1    FALSE   FALSE
## 2    FALSE   FALSE
## 3     TRUE   FALSE
## 4     TRUE   FALSE
## 5     TRUE   FALSE
## 6     TRUE   FALSE
complete.cases(head(flows[,c('app_name', 'srcaddr')]))
[1]  TRUE  TRUE FALSE FALSE FALSE FALSE
!complete.cases(head(flows[,c('app_name', 'srcaddr')]))
[1] FALSE FALSE  TRUE  TRUE  TRUE  TRUE
```

Recoding Values to Missing
```
# recode 99 to missing for variable v1
mydata$v1[mydata$v1==99] <- NA
```

Excluding Missing Values from Analyses
Arithmetic functions on missing values yield missing values.
```
x <- c(1,2,NA,3)
mean(x) # returns NA
mean(x, na.rm=TRUE) # returns 2
```

The function na.omit() returns the object with listwise deletion of missing values.
```
# create new dataset without missing data 
newdata <- na.omit(mydata)
```

## namespace
同一个环境只能存在一个唯一的名字,不同环境可以存在相同名字,R寻找一个名字,
会站在当前环境沿着search() path(`".GlobalEnv"     "package:base"   "namespace:base"`)往之后的环境中找名字,如果当前名字不符合就依次找后面的环境.  
可以参见[demo namespace](../../demo/r/namespace.r)

## object types
R语言中最为基本的类包括了数值(numeric),逻辑(logical),字符(character),列表(list),
在此基础上构成了一些复合型的类,包括矩阵(matrix),数组(array),因子(factor),数据框(dataframe).
除了这些内置的类外还有很多其它的,用户还可以自定义新的类,但所有的类都是建立在这些基本的类之上的

overview of the type of objects representing data

| object                                     | modes                                     | several modes possible in the same object? |
|--------------------------------------------|-------------------------------------------|--------------------------------------------|
| vector                                     | numeric, character, complex or logical 	 | No  
| factor                                     | numeric or character 					 | No
| array                                      | numeric, character, complex or logical 	 | No  
| matrix                                     | numeric, character, complex or logical 	 | No  
| data frame                                 | numeric, character, complex or logical 	 | No  
| ts                                         | numeric, character, complex or logical 	 | No  
| list                                       | numeric, character, complex or logical, function, expression, ... 	 | No  

- A factor is a categorical variable
- A data frame is a table composed with one or several vectors and/or factors all of the same length but possibly of different modes. dataframe是一种R的数据格式,可以将它想象成类似统计表格,每一行都代表一个样本点,而每一列则代表了样本的不同属性或特征
- A 'ts' is a time series data set and so contains additional attributes such as frequency and dates. 
- A list a general form of vector in which the various elements need not be of the same type. 
They can contain any type of object, included lists!
- matrices or more generally arrays are multi-dimensional generalizations of vectors.

a matrix with 2 lines and 2 columns has for dim the pair of values `[2, 2]`, but its length is 4.

list objects in memory  
`ls()`: only the names of objects
`ls(pattern = "m")`: list the objects with the name containing "m", or using `pat` for short
`ls.str()`: display some details

delete objects in memory  
`rm(x)`, `rm(x,y)` remove the object x and y from memory

**Conversion**

|                 | to one long vector | to matrix           | to data frame        |                         |
|-----------------|--------------------|---------------------|----------------------|-------------------------|
| from vector     |                    | c(x,y)              | cbind(x,y), rbind(x,y)| data.frame(x,y)         |
| from matrix     |                    | as.vector(mymatrix) |                      | as.data.frame(mymatrix) |
| from data frame |                    |                     | as.matrix(myframe)   |                         |

- cbind() 把矩阵横向合并成一个大矩阵(列方式), 将参数当成列矩阵来处理
- rbind() 是纵向合并(行方式), 将参数当行矩阵来处理

[cbind and rbind demo](../../demo/r/bind.r)

## vector
Vectors are the most important type of object in R.

选择特定的元素
```
> a <- c(1:5)
> a>3
[1] FALSE FALSE FALSE  TRUE  TRUE
> a[a>3]
[1] 4 5
> sum(a[a>3])
[1] 9
> sum(a>3)  ## 统计满足条件的元素的个数
[1] 2
```

`x[1:10]` selects the first 10 elements of x

`> y <- x[-(1:5)]` gives y all but the first five elements of x

`> y <- x[!is.na(x)]`  
creates (or re-creates) an object y which will contain the non-missing values of x, in the same order.

`> z <- (x+1)[(!is.na(x)) & x>0]`  
creates an object z and places in it the values of the vector x+1 for which 
the corresponding value in x was both non-missing and positive.

`x[is.na(x)] <- 0` replaces any missing values in x by zeros and  
`> y[y < 0] <- -y[y < 0]` has the same effect as `> y <- abs(y)`

```
> fruit <- c(5,10,1,20)
> fruit
[1]  5 10  1 20
> names(fruit) <- c("orange", "banana", "apple", "peach")
> fruit
orange banana  apple  peach 
     5     10      1     20 
> lunch <- fruit[c("apple","orange")]
> lunch
 apple orange 
     1      5 
> fruit["apple"]
apple 
    1 
```

`as.matrix(vector)`生成的矩阵是一个column matrice

## array
```
> z <- c(1:24)
> dim(z) <- c(3,4,2)
> dim(z)
[1] 3 4 2
> dim(z[,,]) ## z[,,]表示整个array
[1] 3 4 2
> dim(z[1,,])
[1] 4 2
> dim(z[,1,])
[1] 3 2
> dim(z[,,1])
[1] 3 4
```

`> x <- array(1:20, dim=c(4,5))` generates a 4 by 5 array, a matrix  
`> Z <- array(1:24, dim=c(3,4,2))` generates a 3 by 4 by 2 array

## matrix
```
> x <- matrix(c(1,2,3,4,5,6), c(2,3))
> x
     [,1] [,2] [,3]
[1,]    1    3    5
[2,]    2    4    6

```
## factor
因子提供了一个简单并且紧凑的形式来处理**分类数据(不能进行数字计算)**.
因子用"水平level"来表示所有可能的取值.
如果数据集有取值个数固定的名字变量,因子就特别有用.

```
> g<-c("f","m","f","f","m") 
> g 
[1] "f" "m" "f" "f" "m" 
> g<-factor(g)#因子化 
> g 
[1] f m f f m 
Levels: f m #有几种可选的值
```

## list
- unlist: 将list转换为非list格式, 为向量格式

```
> a <- list(1,2, "test")
> a
[[1]]
[1] 1

[[2]]
[1] 2

[[3]]
[1] "test"
```

[如何高效的append an element to a list in R](http://stackoverflow.com/questions/17046336/here-we-go-again-append-an-element-to-a-list-in-r)

## data.frame
在数据导入R语言后,会以数据框(dataframe)的形式储存.
dataframe是一种R的数据格式,可以将它想象成类似统计表格,每一行都代表一个样本点,而每一列则代表了样本的不同属性或特征.
```
mydata <- data.frame(col1, col2, col3, ...)
```
例如

```
books <- data.frame(
    title = c("harry potter", "war and peace", "lord of the rings"), # column named "title"
    author = c("rowling", "tolstoy", "tolkien"),
    num_pages = c("350", "875", "500")
)

> books
              title  author num_pages
1      harry potter rowling       350
2     war and peace tolstoy       875
3 lord of the rings tolkien       500
```

如果在像数据库那样, 获得的都是一个个的record(具有不同数据类型的fields), 然后想把很多的records组成一个data.frame, 
现在我能想到的方法就是用list来存储一个record, 然后用rbind 函数将这个list 放到data.frame里面.(循环实现将所以的records放入data.frame中)

下面的命令可以让你有机会修改数据并存入到新的变量newdata中:
```
newdata=edit(data)
```

## [date and time](http://www.cyclismo.org/tutorial/R/time.html)
Part of the difficulty with time data types is that R prints them out in a way that is different from how it stores them internally. 
This can make type conversions tricky, and you have to be careful and test your operations to insure that R is doing what you think it is doing.

### Time
the POSIXct and POSIXlt data types: 两种不同的表示时间的方式

POSIXct: # seconds since the start of January 1, 1970. 
Negative numbers represent the number of seconds before this time, 
and positive numbers represent the number of seconds afterwards.

POSIXlt a vector, and the entries in the vector have the following meanings:

- sec 0–61: seconds.
- min 0–59: minutes.
- hour 0–23: hours.
- mday 1–31: day of the month
- mon 0–11: months after the first of the year.
- year years since 1900.
- wday 0–6 day of the week, starting on Sunday.
- yday 0–365: day of the year.
- isdst Daylight Saving Time flag. Positive if in force, zero if not, negative if unknown.
- zone (Optional.) The abbreviation for the time zone in force at that time: "" if unknown (but "" might also be used for UTC).
- gmtoff (Optional.) The offset in seconds from GMT: positive values are East of the meridian. Usually NA if unknown, but 0 could mean unknown.

Sys.time():  get the current time
as.POSIXct and as.POSIXlt convert the time value into the different formats.
strftime command is used to take a time data type and convert it to a string
```
timeString <-  strftime(t,"%Y-%m-%d %H:%M:%S")
```
strptime command is used to take a string and convert it into a form that R can use for calculations
```
time <- strptime("2014-01-23 14:28:21", "%Y-%m-%d %H:%M:%S")
```

从系统与字符串转换来的时间类型是不一样的
```
> now <- Sys.time()
> now
[1] "2015-01-23 22:47:11 CST"
> class(now)
[1] "POSIXct" "POSIXt"
> mode(now)
[1] "numeric"
> typeof(now)
[1] "double"

> t <- strptime("2005-06-24 10:05:35","%Y-%m-%d %H:%M:%S")
> class(t)
[1] "POSIXlt" "POSIXt"
> mode(t)
[1] "list"
> typeof(t)
[1] "list"

## 但是还是可以直接作差的
> now - t
Time difference of 3500.529 days
> difftime(now, t)
Time difference of 3500.529 days
```

### Date
The difference is that the date data type keeps track of numbers of days rather than seconds. 
You can cast a string into a date type using the `as.Date` function. The `as.Date` function takes the same arguments as the time data types discussed above.
```
> theDates <- c("1 jan 2012","1 jan 2013","1 jan 2014")
> d <- as.Date(theDates,"%d %b %Y")
> d 
[1] "2012-01-01" "2013-01-01" "2014-01-01"
> class(d)
[1] "Date"
> mode(d)
[1] "numeric"
> typeof(d)
[1] "double"

> infamy <- as.Date(-179,origin="1942-06-04")
```

### Time Operations
```
> earlier <- strptime("2000-01-01 00:00:00","%Y-%m-%d %H:%M:%S")
> later <- strptime("2000-01-01 00:00:20","%Y-%m-%d %H:%M:%S")
> later-earlier
Time difference of 20 secs
> as.double(later-earlier)
[1] 20

> earlier <- strptime("2000-01-01 00:00:00","%Y-%m-%d %H:%M:%S")
> later <- strptime("2000-01-01 01:00:00","%Y-%m-%d %H:%M:%S")
> later-earlier
Time difference of 1 hours
> as.double(later-earlier)
[1] 1

> up <- as.Date("1961-08-13")
> down <- as.Date("1989-11-9")
> down-up
Time difference of 10315 days
```
以上说明时间直接作差的结果取决于差值的大小.
使用`difftime`可以避免这个问题
```
difftime(time1, time2, 'arg')
```
arg should be one of "auto", "secs", "mins", "hours", "days", "weeks"

例子
```
> earlier <- strptime("2000-01-01 00:00:00","%Y-%m-%d %H:%M:%S")
> later <- strptime("2000-01-01 01:00:00","%Y-%m-%d %H:%M:%S")
> difftime(later,earlier)
Time difference of 1 hours
> difftime(later,earlier,units="secs")
Time difference of 3600 secs
```

difftime 的另外一种用法
```
> diff <- as.difftime("00:30:00","%H:%M:%S",units="hour")
> diff
Time difference of 0.5 hours
> Sys.time()
[1] "2014-01-23 16:45:39 EST"
> Sys.time()+diff
[1] "2014-01-23 17:15:41 EST"
```

The last thing to mention is that once a time stamp is cast into one of R's internal formats comparisons can be made in a natural way.

```
> diff <- as.difftime("00:30:00","%H:%M:%S",units="hour")
> now <- Sys.time()
> later <- now + diff
> now
[1] "2014-01-23 16:47:48 EST"
> later
[1] "2014-01-23 17:17:48 EST"
>
> if(now < later)
  {
     cat("there you go\n")
  }
there you go
```

## class, mode and typeof
All objects have two intrinsic attributes: mode and length. 

The mode is the basic type of the elements of the object; there are four main modes:
**numeric, character, complex , and logical** (FALSE or TRUE). 
Other modes exist but they do not represent data, for instance function or expression. 
The **length** is the number of elements of the object. 

```
>f <- gl(2,5)
>f 
 [1] 1 1 1 1 1 2 2 2 2 2
Levels: 1 2
> class(f)   #查看变量的类,显示为因子
[1] "factor"
> mode(f)  #查看数据大类,显示为数值型
[1] "numeric"
> typeof(f)  #查看数据细类,显示为整数型
[1] "integer"
> storage.mode(f)
[1] "integer"
```

The class() is used to define/identify what "type" an object is from the point of view of object-oriented programming in R. So for
```
> x <- 1:3
> class(x)
[1] "integer"
```
typeof() gives the "type" of object from R's point of view,   
whilst mode() gives the "type" of object from the point of view of Becker, Chambers Wilks (1988). 
The latter may be more compatible with other S implementations according to the R Language Definition manual.

'mode' is a mutually exclusive classification of objects according to their basic structure.  
'class' is a property assigned to an object that determines how generic functions operate with it. It is not a mutually exclusive classification.

```
> z <- 0:9
> z
 [1] 0 1 2 3 4 5 6 7 8 9
> digits <- as.character(z)
> digits
 [1] "0" "1" "2" "3" "4" "5" "6" "7" "8" "9"
> d <- as.integer(digits)
> d
 [1] 0 1 2 3 4 5 6 7 8 9
```

`> e <- numeric()` makes e an empty vector structure of mode numeric. 
Once an object has been created, new components may be added to it simply by giving it an index value outside its previous range.  
`> e[3] <- 17`  
now makes e a vector of length 3, (the first two components of which are at this point both NA).

# 基本数据管理
## 创建新变量
```
x1 <- c(2, 2, 6, 4)
x2 <- c(3, 4, 2, 8)
mydata <- data.frame(x1,x2)

## 第一种方式
mydata$sumx <- mydata$x1 + mydata$x2
mydata$meanx <- (mydata$x1 + mydata$x2)/2

## 第二种方式
attach(mydata)
mydata$sumx <- x1 + x2
mydata$meanx <- (x1 + x2)/2
detach(mydata)

## 第三种方式
mydata <- transform(mydata, 
	sumx = x1 + x2, 
	meanx = (x1 + x2)/2)
## 注意, 其中的 = 不能换成 <-
```

## 数据集取子集
Dropping variables
```
myvars <- names(leadership) %in% c("q3", "q4")
newdata <- leadership[!myvars]

newdata <- leadership[c(-7, -8)]
```

Selecting observations
```
newdata <- leadership[which(leadership$gender == "M" & leadership$age > 30), ]

attach(leadership)
newdata <- leadership[which(gender == "M" & age > 30), ]
detach(leadership)
```

Selecting observations based on dates
```
leadership$date <- as.Date(leadership$date, "%m/%d/%y")
startdate <- as.Date("2009-01-01")
enddate <- as.Date("2009-10-31")
newdata <- leadership[leadership$date >= startdate & leadership$date <= enddate, ]
```

Using the subset() function
```
## 选择所有age >= 35 或者 age <24的行, 保留了fields q1 到 q4
newdata <- subset(leadership, age >= 35 | age < 24, select = c(q1, q2, q3, q4))
## 选择所有满足条件的行, 保留了gender 到 q4之前所有的fields(包括两者)
newdata <- subset(leadership, gender == "M" & age > 25, select = gender:q4)
```

随机抽样 sample
```
## 随机抽取大小为3的样本
mysample <- leadership[sample(1:nrow(leadership), 3, replace=FALSE), ]
```

我们想从1到10中随机抽取5个数字,那么这样来做:首先产生一个序列,然后用sample函数进行无放回抽取.
```
x=1:10
sample(x,size=5)
```
有放回抽取则是
```
sample(x,size=5,replace=T)
```
sample函数在建模中经常用来对样本数据进行随机的划分,一部分作为训练数据,另一部分作为检验数据.

`sample(x, size, replace = FALSE, prob = NULL)`
replace 表示取样的时候能够重复, 也就是说一个元素可以不可以被多次取到  
prob 的和可以不为1, 只要保证每个元素非负就可以了

```
> x <- 1:5
> sample(x, length(x),replace=T, prob=c(0.1,0.2,0.3,0.25,0.25))
[1] 3 4 4 2 5
> replicate(3, sample(x, length(x),replace=F))  ## repalce=FALSE 表示元素不能重复
     [,1] [,2] [,3]
[1,]    4    4    4
[2,]    3    5    2
[3,]    2    1    5
[4,]    1    2    1
[5,]    5    3    3
> replicate(3, sample(x, length(x),replace=T))
     [,1] [,2] [,3]
[1,]    4    2    4
[2,]    5    3    1
[3,]    1    2    4
[4,]    3    4    2
[5,]    1    1    1
```

特殊的简化
```
sample(x, n)  ## 当length(x) = 1 且 x > 1, 那么这句话就是从序列1:x中取出n个值
```

描述统计是一种从大量数据中压缩提取信息的工具,最常用的就是summary命令
对于数值变量计算了五个分位点和均值,对于分类变量则计算了频数

## 使用sql 操作data.frame
使用sqldf的前提是本地有数据库服务, 因为sqldf 会调用本地的数据库来执行语句
```
library(sqldf)

newdf <- sqldf("select * from mtcars where carb=1 order by mpg", row.names = TRUE)
## 参数row.names = TRUE 将原始数据框中的行名延续到新的数据框中

newdf <- sqldf("select avg(mpg) as avg_mpg, avg(disp) as avg_disp, gear from mtcars where cyl in (4, 6) group by gear")
```

## reshape2
### transform data between wide and long formats
[An Introduction to reshape2](http://seananderson.ca/2013/10/19/reshape.html)

[What makes data wide or long?](../../demo/r/reshape2.r)

It is easier to record their data in wide format, but it is easy to process in long format

reshape2 is based around two key functions: melt and cast:

- melt takes wide-format data and melts it into long-format data.
- cast takes long-format data and casts it into wide-format data.

In reshape2 there are multiple cast functions. Since you will most commonly work with data.frame objects, we will explore the dcast function. 
(There is also acast to return a vector, matrix, or array.)

dcast uses a formula to describe the shape of the data. The arguments on the left refer to the ID variables and the arguments on the right refer to the measured variables.

[reshape demo](../../demo/r/reshape2.r)

**melt 错误提示**  
Warning message: attributes are not identical across measure variables; they will be dropped  
This warning is basically telling you that variables that you are trying to put in the "value" column (the measure variables) are different types 
(some may be character, others may be factors, others may be numeric).  
@MrFlick suggestion to treat those columns as keys even if they might not be would solve that problem.

**cast 错误提示**  
Aggregation function missing: defaulting to length  
This warning is a warning that you usually get when the combination of IDs is not unique.  
you would need to add another column to make the ID variables unique to avoid having dcast automatically use length as its fun.aggregate function.

### 变量的重命名
```
names(dataframe)[index] <- "newname"

## 通过reshape 重命名
library(reshape)
rename(dataframe, c(oldname = "newname", oldname = "newname", ...))
```

Reshape2 is a reboot of the reshape package. 
It is been over five years since the first release of the package, and in that time I have learned a tremendous amount about R programming, and how to work with data in R. 
Reshape2 uses that knowledge to make a new package for reshaping data that is much more focussed and much much faster.

This version improves speed at the cost of functionality, so I have renamed it to reshape2 to avoid causing problems for existing users. 
Based on user feedback I may reintroduce some of these features.

What's new in reshape2:

- considerably faster and more memory efficient thanks to a much better underlying algorithm that uses the power and speed of subsetting to the fullest extent, 
in most cases only making a single copy of the data.
- cast is replaced by two functions depending on the output type:  dcast produces data frames, and acast produces matrices/arrays.
- multidimensional margins are now possible: grand_row and  grand_col have been dropped: now the name of the margin refers to the variable that has its value set to (all).
- some features have been removed such as the | cast operator, and the ability to return multiple values from an aggregation function. 
I'm reasonably sure both these operations are better performed by plyr.
- a new cast syntax which allows you to reshape based on functions
- of variables (based on the same underlying syntax as plyr):
- better development practices like namespaces and tests.

# 常用统计函数运算
在R语言中经常会用到函数,例如上节中讲到的求样本统计量就需要均值函数(mean)和标准差函数(sd).对于二元数值数据还用到协方差(cov),对于二元分类数据则可以用交叉联列表函数(table).
下文讲述在初级统计学中最常用到的三类函数.

## 描述性统计分析
**偏度(Skew)**衡量实数随机变量概率分布的不对称性.
偏度的值可以为正,可以为负或者甚至是无法定义.
偏度分为两种:

- 负偏态或左偏态:左侧的尾部更长,分布的主体集中在右侧.
- 正偏态或右偏态:右侧的尾部更长,分布的主体集中在左侧.
- 如果分布对称,那么平均值=中位数,偏度为零(此外,如果分布为单峰分布,那麽平均值=中位数=众数).

![偏度](http://upload.wikimedia.org/wikipedia/commons/thumb/f/f8/Negative_and_positive_skew_diagrams_%28English%29.svg/400px-Negative_and_positive_skew_diagrams_%28English%29.svg.png)

**峰度(Kurtosis)**衡量实数随机变量概率分布的峰态.
峰度高就意味着方差增大是由低频度的大于或小于平均值的极端差值引起的.
表征概率密度分布曲线在平均值处峰值高低的特征数.直观看来,峰度反映了尾部的厚度

基础包中summary

Hmisc包中的describe
变量和观察的数量, 缺失值和唯一值的数量, 平均值, 分位数以及5个最大的值和最小的值

pastecs 中的stat.desc
```
stat.desc(x, basic=TRUE, desc=TRUE, norm=FALSE, p=0.95)
x: a data frame or a time series

basic: do we have to return basic statistics (by default, it is TRUE)? 
	These are: the number of values (nbr.val), the number of null values (nbr.null), the number of missing values (nbr.na), the minimal value (min), the maximal value (max), the range (range, that is, max-min) and the sum of all non-missing values (sum)

desc: do we have to return various descriptive statistics (by
          default, it is TRUE)? These are: the median (median), the
          mean (mean), the standard error on the mean (SE.mean), the
          confidence interval of the mean (CI.mean) at the 'p' level,
          the variance (var), the standard deviation (std.dev) and the
          variation coefficient (coef.var) defined as the standard
          deviation divided by the mean

norm: do we have to return normal distribution statistics (by default, it is FALSE)? 
	the skewness coefficient g1 (skewness), its significant criterium (skew.2SE, that is, g1/2.SEg1; if skew.2SE > 1, then skewness is significantly different than zero), kurtosis coefficient g2 (kurtosis), its significant criterium (kurt.2SE, same remark than for skew.2SE), the statistic of a Shapiro-Wilk test of normality (normtest.W) and its associated probability (normtest.p)

p: 置信区间
```

psych中的describe
```
describe(x, na.rm = TRUE, interp=FALSE,skew = TRUE, ranges = TRUE,trim=.1, type=3,check=TRUE,fast=NULL)
describeData(x,head=4,tail=4)
trim: trim=.1 - trim means by dropping the top and bottom trim fraction
```

## 分组计算描述性统计量
aggregate

```
> vars <- c("mpg", "hp", "wt")
> aggregate(mtcars[vars], by = list(am=mtcars$am), mean)
  am      mpg       hp       wt
1  0 17.14737 160.2632 3.768895
2  1 24.39231 126.8462 2.411000

> aggregate(mtcars[vars], by = list(am=mtcars$am), sd)
  am      mpg       hp        wt
1  0 3.833966 53.90820 0.7774001
2  1 6.166504 84.06232 0.6169816
```
遗憾的是, aggregate 仅允许每次调用mean, sd 这样的单值返回函数.
要返回多值, 可以使用by 函数
```
by(data, INDICES, FUN)
## Apply a Function to a Data Frame Split by Factors
```
ex:
```
dstats <- function(x)(c(mean=mean(x), sd=sd(x)))
by(mtcars[vars], mtcars$am, dstats)
## 以am为分组, 对mtcars中的vars三个列作用dstats函数
```

doBy
```
summaryBy(formula, data = data.frame, FUN = function)
```
ex
```
library(doBy)
summaryBy(mpg + hp + wt ~ am, data = mtcars, FUN = mystats)
```

还可以通过reshape包
```
library(reshape)
dstats <- function(x) (c(n = length(x), mean = mean(x), sd = sd(x)))
dfm <- melt(mtcars, measure.vars = c("mpg", "hp", "wt"), id.vars = c("am", "cyl"))
## 只取出c("mpg", "hp", "wt")三列
cast(dfm, am + cyl + variable ~ ., dstats)
```

## 频数表与列联表
- table(var1, var2, ...varN) 使用N个类别变量(因子)创建一个N维列联表
- Xtabs(formula, data) 根据一个公式和一个矩阵或数据框创建一个列联表
- prop.table(table, margins) 依据margins定义的边界将表中条目表示为分数形式
- Margin.table(table, margins) 依据margins定义的边界计算表中条目的和
- Addmargins(table, margins) 将概述边margins(默认是求和结果)放入表中
- ftable(table) 创建一个紧凑的平铺式列联表

[一维列联表](../../demo/r/table.r)
### 二维列联表
```
mytable <- table(A,B)
mytable <- xtabs(~A+B, data = mydata)
```
[二维列联表](../../demo/r/table.r)

table和xtabs 都可以基于三个或者更多个的类型变量生成多维列联表.  
margin.table, prop.table, addmargins 可以自然地推广到高于二维的情况.  
ftable 可以以一种紧凑而吸引人的方式输出多维列联表

## 数据汇总函数
apply族函数包括了apply,sapply,lappy,tapply等函数,这些函数在不同的情况下能高效的完成复杂的数据处理任务,但角色定位又有所不同

[A brief introduction to "apply" in R](https://nsaunders.wordpress.com/2010/08/20/a-brief-introduction-to-apply-in-r/)

apply()可将任意函数应用到矩阵,数组或者数据框的任何维度上,
它逐行或逐列的处理数据,其输出的结果将是一个向量或是矩阵.
```
apply(x, MARGIN, FUN,...)
```
x 是数据对象, 
margin 是维度的下标(margin = 1 表示行, margin = 2表示列, margin = c(1,2)表示每个格), 
fun是函数, 
而 ... 则包括了任何想传递给fun的参数

```
m.data <- matrix(rnorm(100),ncol=10)
apply(m.data,1,mean)  ## 对行求平均值
apply(m.data,2,mean)  ## 对列求平均值
apply(iris[,1:4], 2, mean) ## 由于iris 有些列不是numeric, 不能求mean
```

lappy() 与sapply 的处理对象是向量,列表或其它对象,
lapply returns a list of the same length as X, each element of which is the result of applying FUN to the corresponding element of X.
```
# create a list with 2 elements
l <- list(a = 1:10, b = 11:20)
l
$a
 [1]  1  2  3  4  5  6  7  8  9 10
$b
 [1] 11 12 13 14 15 16 17 18 19 20

# the mean of the values in each element
lapply(l, mean)
$a
[1] 5.5
$b
[1] 15.5
 
# the sum of the values in each element
lapply(l, sum)
$a
[1] 55
$b
[1] 155
```

sapply is a user-friendly version of lapply by default returning a vector or matrix if appropriate.
```
> sapply(l, mean)
   a    b
 5.5 15.5
> mode(lapply(l,mean))
[1] "list"
> mode(sapply(l,mean))
[1] "numeric"
```

在R中数据框是一种特殊的列表,所以数据框的列也将作为函数的处理对象.下面的例子即对一个数据框按列来计算中位数与标准差.
```
f.data <- data.frame(x=rnorm(10),y=runif(10))
lapply(f.data,FUN=function(x) list(median=median(x),sd=sd(x)))
```

sapply()可能是使用最为频繁的向量化函数了,它和lappy()是非常相似的,但其输出格式则是较为友好的矩阵格式.
```
f.data <- data.frame(x=rnorm(10),y=runif(10))
sapply(f.data,FUN=function(x) list(median=median(x),sd=sd(x)))
```

tapply()的功能则又有不同,它是专门用来处理分组数据的.
Apply a function to each cell of a ragged array, that is to each (non-empty) group of values given by a unique combination of the levels of certain factors.

我们以iris数据集为例,可观察到Species列中存放了三种花的名称,我们的目的是要计算三种花瓣萼片宽度的均值.其输出结果是数组格式.
```
attach(iris)
# mean petal length by species
tapply(iris$Petal.Length, Species, mean)
    setosa versicolor  virginica 
     1.462      4.260      5.552
```
与tapply功能非常相似的还有aggregate(),其输出是更为友好的数据框格式.而by()和上面两个函数是同门师兄弟.

## 概率计算函数
如果给定一种概率分布,通常会有四类计算问题:

1. 计算其概率密度density (d)
1. 计算其概率分布probability(p)
1. 计算其百分位数quantile (q)
1. 随机数模拟random (r)

![概率类型](http://1.bp.blogspot.com/-h1-vKZMKEh4/TrfKPr_3QOI/AAAAAAAAAhQ/2Gs77XvCBxI/s400/%25E6%258D%2595%25E8%258E%25B7.JPG)

另外还有一个非常有用的函数replicate(),它可以将某个函数重复运行N次,常常用来生成较复杂的随机数.  
下面的例子即先建立一个函数,模拟扔两个骰子的点数之和,然后重复运行10000次.
```
game <- function() {
    n <- sample(1:6,2,replace=T)
    return(sum(n))
}
replicate(n=10000,game())
```

# 字符串str
- 获取字符串长度:nchar()能够获取字符串的长度,它也支持字符串向量操作.注意它和length()的结果是有区别的.
- 字符串粘合:paste()负责将若干个字符串相连结,返回成单独的字符串.其优点在于,就算有的处理对象不是字符型也能自动转为字符型.
- 字符串分割:strsplit()负责将字符串按照某种分割形式将其进行划分,它正是paste()的逆操作.
- 字符串选取[: 在用strsplit之后, 得到一个字符串的list, 可以用[ 来进行选取操作
```
> str <- "this is a test"
> str
[1] "this is a test"
> r <- strsplit(str, " ") ## "this" "is"   "a"    "test"
> length(r)  ## 不知道为什么length 是1 
[1] 1
> mode(r)
[1] "list"
> sapply(r,"[",1)
[1] "this"
> r[1]
[[1]]
[1] "this" "is"   "a"    "test"
> r[1,]
Error in r[1, ] : incorrect number of dimensions
> r[,1]
Error in r[, 1] : incorrect number of dimensions
> dim(r)
NULL
```
- 字符串截取:substr()能对给定的字符串对象取出子集,其参数是子集所处的起始和终止位置.
- 字符串替代:gsub()负责搜索字符串的特定表达式,并用新的内容加以替代.
- sub()函数是类似的,但只替代第一个发现结果.
- 字符串匹配:grep()负责搜索给定字符串对象中特定表达式 ,并返回其位置索引.
- grepl()函数与之类似,但其后面的"l"则意味着返回的将是逻辑值.

# IO
`print(x, ...)`

For more customizable (but cumbersome) printing, see `cat`, `format` or also `write`.   
For a simple prototypical print method, see `.print.via.format` in package **tools**.

`source("commands.R")` 读取文件并执行  

重定向输出
```
sink("文件名", append = FALSE, split = FALSE)
sink()   #取消显示到文件
```
使用参数append=TRUE可以将文本追加到文件后  
参数split=TRUE可将输出同时发送到屏幕和输出文件中

## file
[data import](http://www.r-tutor.com/r-introduction/data-frame/data-import)

[R数据的导入与导出](http://www.biosino.org/R/R-doc/onepage/R-data_cn.html), 这个比较全面, 包含

- Spreadsheet-like data: 电子表格类似的数据
- Importing from other statistical systems: 导入其它统计软件的数据
- Relational databases: 关系数据库
- Binary files: 二进制文件
- Connections: 连接
- Network interfaces: 网络接口
- Reading Excel spreadsheets: 读取Excel表格文件
- References: 参考文献
- Function and variable index: 函数和变量索引
- Concept index: 概念索引

```
read.table(file, header = FALSE, sep = "", quote = "\"'")
read.csv(file, header = TRUE, sep = ",", quote = "\"", dec = ".", fill = TRUE, comment.char = "", ...)
```

excel
```
> library(gdata)                   # load gdata package 
> help(read.xls)                   # documentation 
> mydata = read.xls("mydata.xls")  # read from first sheet
```
Alternatively, we can use the function loadWorkbook from the XLConnect package to read the entire workbook, 
and then load the worksheets with readWorksheet. 
The XLConnect package requires Java to be pre-installed.
```
> library(XLConnect)               # load XLConnect package 
> wk = loadWorkbook("mydata.xls") 
> df = readWorksheet(wk, sheet="Sheet1")
```

**Working Directory**  
文件不在当前目录时  
```
> getwd()              # get current working directory
```
You can select a different working directory with the function setwd(), and thus avoid entering the full path of the data files.
```
> setwd("<new path>")   # set working directory  
```
Note that the forward slash should be used as the path separator even on Windows platform.  
```
> setwd("C:/MyDoc")
```

## database
dbDriver("MySQL") 的调用会返回一个数据库连接管理对象,然后调用dbConnect打开一个数据库连接,随后会调用泛型函数 dbDisconnect 来关闭这个连接.
对于,ORACLE和SQLITE系统,分别使用ROracle 或 RSQLite 里面的dbDriver("Oracle") 函数或 dbDriver("SQLite") 函数.

SQL 查询可以通过dbSendQuery或 dbGetQuery 传给数据库管理系统.
dbGetQuery 传送查询语句, 把结果以数据框形式返回.
dbSendQuery 传送查询,返回的结果是 继承"DBIResult"的一个子类的对象."DBIResult" 类 可用于取得结果,而且还可以通过调用 dbClearResult 清除结果.

函数 fetch 用于获得查询结果的部分或全部行,并以列表返回. 函数 dbHasCompleted 确定是否所有行已经获得了, 而 dbGetRowCount 返回结果中行的数目.

这些是数据库中读/写/测试/删除表的方便接口. dbReadTable 和 dbWriteTable 实现一个 R 数据框的复制进和复制出数据库, 把数据框的行名字映射到 MySQL 表的 row_names 字段.

[database demo](../../demo/r/db.r)

# 流程
```
> if (expr_1) expr_2 else expr_3
> for (name in expr_1) expr_2
> while (condition) expr
```
exp 中可以用{}

break, next

`prediction = ifelse(post.yes >= post.no, "Yes", "No")` 类似于C 语言中的`? :`运算符.

# function
## math
Vectors occurring in the same expression need not all be of the same length. 
If they are not, the value of the expression is a vector with the same length as the longest vector which occurs in the expression. 
**Shorter vectors in the expression are recycled** as often as need be until they match the length of the longest vector.

近似

- ceiling(x)
- floor(x)
- round(x, digits = 0)

### matrix
- `A %*% B`: 矩阵相乘
- `t(A)`: 矩阵转置

### sort
#### order
By default, sorting is ASCENDING. Prepend the sorting variable by a minus sign to indicate DESCENDING order. Here are some examples.
```
# sorting examples using the mtcars dataset
attach(mtcars)

# sort by mpg
newdata <- mtcars[order(mpg),] 

# sort by mpg and cyl
newdata <- mtcars[order(mpg, cyl),]

#sort by mpg (ascending) and cyl (descending)
newdata <- mtcars[order(mpg, -cyl),] 

detach(mtcars)
```
#### order and rank
```
> a <- c(45, 50, 10, 96)
> order(a)  
[1] 3 1 2 4
> rank(a)
[1] 2 3 1 4
> a[order(a)]
[1] 10 45 50 96  
> sort(a)
[1] 10 45 50 96  
```
order(a) is saying, 'put the third element first when you sort... ', 
whereas rank(a) is saying, 'the first element is the second lowest... '. 
(Note that they both agree on which element is lowest, etc.; they just present the information differently.)''''

## construct sequence
### c
This is a generic function which combines its arguments.

The default method combines its arguments to form a vector.
All arguments are coerced to a common type which is the type of the returned value, and all attributes except names are removed.  
The output type is determined from the highest type of the components in the hierarchy 
`NULL < raw < logical < integer < double < complex < character < list < expression`.

```
> c(1,7:9)
[1] 1 7 8 9
> c(1:5, 10.5, "next")
[1] "1"    "2"    "3"    "4"    "5"    "10.5" "next"

构造list ll
> ll <- list(A = 1, c = "C")
> ll
$A
[1] 1

$c
[1] "C"

由于1:3 生成的三个元素时integer类型的数据, 比list类型的ll层级低, 所以c会把1:3生成的三个元素都分别转化为list类型
> c(ll, d=1:3)
$A
[1] 1

$c
[1] "C"

$d1
[1] 1

$d2
[1] 2

$d3
[1] 3

这里d才是一个list, 和ll的层级一样
> c(ll, d=list(1:3))
$A
[1] 1

$c
[1] "C"

$d
[1] 1 2 3

```

### seq
`seq()` is a more general facility for generating sequences.

`seq(from = 1, to = 1, by = ((to - from)/(length.out - 1)), length.out = NULL, along.with = NULL, ...)`

typical usage
```
seq(from, to)
seq(from, to, by= ) ## from, from+by, from+2*by...
seq(from, to, length.out= )  ## equally spaced
```

### rep
`rep()` which can be used for replicating an object in various complicated ways. The simplest form is  
`> s5 <- rep(x, times=5)` which will put five copies of x end-to-end in s5.  
`> s6 <- rep(x, each=5)` which repeats each element of x five times before moving on to the next.

## 自定义
```
myfunction <- function(arg1, arg2, ... ){
	statements
	return(object)
}
```
例如下面的归一化函数
```
autonorm <- function(data){
	min <- min(data)
	max <- max(data)
	for(i in 1:length(data))
		data[i] <- (data[i] - min)/(max - min)
	return(data)
}
```
# package and model
`fitted` is a generic function which extracts fitted values from objects returned by modeling functions.
All object classes which are returned by model fitting functions should provide a 'fitted' method.

`data`: Loads specified data sets, or list the available data sets.

## install package
- `install.packages()`会将package安装到默认的目录(.libPaths()指定), 类似于yum, 会自动查找依赖关系
- `remove.packages()`会将package移除  
- `update.packages()`: update package

```
> .libPaths()
[1] "/usr/lib64/R/library" "/usr/share/R/library"
```

package doc  
文档中的Value 部分是返回值, 可以通过$来获取, 例如

## K-近邻算法
用到kknn package
```
kknn(formula = formula(train), train, test, na.action = na.omit(),
k = 7, distance = 2, kernel = "optimal", ykernel = NULL, scale=TRUE,
contrasts = c( unordered = "contr.dummy", ordered = "contr.ordinal"))

kknn.dist(learn, valid, k = 10, distance = 2)
```
demo
```
library(kknn)
data(iris)
m <- dim(iris)[1]
val <- sample(1:m, size = round(m/3), replace = FALSE, prob = rep(1/m, m))
iris.learn <- iris[-val,]
iris.valid <- iris[val,]
iris.kknn <- kknn(Species~., iris.learn, iris.valid, distance = 1,
kernel = "triangular")
summary(iris.kknn)
fit <- fitted(iris.kknn)
table(iris.valid$Species, fit)
```

## svm(support vector machine)
### e1071
`classAgreement()` computes several coefficients of agreement between the columns and rows of a 2-way contingency table.  
diag: Percentage of data points in the main diagonal of `tab`, 也就是分类正确率

# graph
## save graph

|Format	|	Driver	|	Notes|
|-------|-----------|--------|
|JPG	|	jpeg	|	Can be used anywhere, but doesn't resize|
|PNG	|	png	|	Can be used anywhere, but doesn't resize|
|WMF	|	win.metafile	|	Windows only; best choice with Word; easily resizable|
|PDF	|	pdf	|	Best choice with pdflatex; easily resizable|
|Postscript	|	postscript	|	Best choice with latex and Open Office; easily resizable|


So if I wanted to save a jpg file called "rplot.jpg" containing a plot of x and y, I would type the following commands:
```
> jpeg('rplot.jpg')
> plot(x,y)
> dev.off()
```
Another Approach
```
> plot(x,y)
## When you are happy with the way it looks, call dev.copy
> dev.copy(jpeg,'rplot.jpg')
> dev.off()
```

## scatter plot
[ref](http://www.statmethods.net/graphs/scatterplot.html)

## graphical parameters
One way is to specify these options in through the par( ) function. 
If you set parameter values here, the changes will be in effect for the rest of the session or until you change them again. 
The format is `par(optionname=value, optionname=value, ...)`
```
# Set a graphical parameter using par()
par()              # view current settings
opar <- par()      # make a copy of current settings
par(col.lab="red") # red x and y labels 
hist(mtcars$mpg)   # create a plot with these new settings 
par(opar)          # restore original settings
```
A second way to specify graphical parameters is by providing the `optionname=value` pairs directly to a high level plotting function. 
In this case, the options are only in effect for that specific graph.

```
# Set a graphical parameter within the plotting function 
hist(mtcars$mpg, col.lab="red")
```

## color
[color ref](http://research.stowers-institute.org/efg/R/Color/Chart/index.htm)  
[quick color value ref](http://research.stowers-institute.org/efg/R/Color/Chart/ColorChart.pdf)

`colors()`, or with the British spelling, `colours()`, returns a **vector of 657 color names** in R.
The names "gray" and "grey" can be spelled either way

Options that specify colors include the following.

- col	Default plotting color. Some functions (e.g. lines) accept a vector of values that are recycled.
- col.axis	color for axis annotation
- col.lab	color for x and y labels
- col.main	color for titles
- col.sub	color for subtitles
- fg	plot foreground color (axes, boxes - also sets col= to same)
- bg	plot background color

You can specify colors in R by index, name, hexadecimal, or RGB.  
For example `col=1, col="white"`, and `col="#FFFFFF"` are equivalent.

```
> colors()[c(552,254,26)]
[1] "red" "green" "blue"

> grep("red",colors())
[1] 100 372 373 374 375 376 476 503 504 505 506 507 524 525 526 527 528 552 553
[20] 554 555 556 641 642 643 644 645

> colors()[grep("red",colors())]
[1] "darkred" "indianred" "indianred1" "indianred2" ...

> col2rgb("yellow")
[,1]
red 255
green 255
blue 0
```

Create a vector of 'n' contiguous colors, 比如当需要同时使用多种颜色时, 就可以通过这些函数来自动选择.
```
rainbow(n, s = 1, v = 1, start = 0, end = max(1, n - 1)/n, alpha = 1)
heat.colors(n, alpha = 1)
terrain.colors(n, alpha = 1)
topo.colors(n, alpha = 1)
cm.colors(n, alpha = 1)
```

## Plotting Symbols/Chracters
Use the pch= option to specify symbols to use when plotting points. For symbols 21 through 25, specify border color (col=) and fill color (bg=).

![symbols](http://www.statmethods.net/advgraphs/images/points.png)

plot中默认的pch为1, 也就是"空心圆圈"

- '0:18': S-compatible vector symbols.
- '19:25': further R vector symbols.
- '26:31': unused (and ignored).
- '32:127': ASCII characters.

## lines
- lty	line type. see the chart below.
- lwd	line width relative to the default (default=1). 2 is twice as wide.

![line types](http://www.statmethods.net/advgraphs/images/lines.png)

## [Axes and Text](http://www.statmethods.net/advgraphs/axes.html)
### legend
```
legend(location, title, legend, ...)
```
ex: `legend("topleft", c('Positive','Negative'), col=seq(2), pch=1, text.col=seq(2))`

- location	There are several ways to indicate the location of the legend.  
You can give an x,y coordinate for the upper left hand corner of the legend.   
You can use locator(1), in which case you use the mouse to indicate the location of the legend.   
You can also use the keywords "bottom", "bottomleft", "left", "topleft", "top", "topright", "right", "bottomright", or "center".   
If you use a keyword, you may want to use `inset=` to specify an amount to move the legend into the graph (as fraction of plot region).
- title	A character string for the legend title (optional)
- legend	A character vector with the labels
- bty for box type
- bg for background color
- cex for size, 放大倍数 1=default, 1.5 is 50% larger,
- text.col for text color
- Setting horiz=TRUE sets the legend horizontally rather than vertically.

Other options

- If the legend labels colored lines, specify col= and a vector of colors. 
- If the legend labels point symbols, specify pch= and a vector of point symbols. 
- If the legend labels line width or line style, use lwd= or lty= and a vector of widths or styles. 
- To create colored boxes for the legend (common in bar, box, or pie charts), use fill= and a vector of colors.

# help
`?command`, `help(command)`, `help("command")`.  
The last one must be used to access help with non-conventional characters:
```
> ?*
Error: syntax error
> help("*")
Arithmetic package:base R Documentation
Arithmetic Operators
...
```

`help.start()`: start the help system in browser

The `help.search` command (alternatively `??`) allows searching for help in various ways. For example,  
`> ??solve`

# 调试debug
`stop('your message here.')`

对函数进行调试的重要工具是`browser()`,它可以使我们进入调试模式逐行运行代码.
在函数中的某一行插入browser()后,在函数执行时会在这一行暂停中断,并显示一个提示符.此时我们可以在提示符后输入任何R语言的交互式命令进行检查调试.

- 输入n则会逐行运行程序,并提示下一行将运行的语句.
- 输入c会直接跳到下一个中断点.
- 而输入Q则会直接跟出调试模式.

debug()函数和browser()是相似的,如果你认为某个函数,例如`fx(x)`,有问题的话,使用`debug(fx(x))`即可进入调试模式.
它本质上是在函数的第一行加入了browser,所以其它提示和命令都是相同的.

其它与程序调试有关的函数还包括:trace(),setBreakpoint(),traceback(),recover()

# 处理大数据集  
由于R在内存中存储对象, 往往会受限于可用的内存量. 举例来说, 在服役了5的2G 内存的Winodws PC上, 可以轻松地处理含有1000万个元素的数据集(100个变量 * 100 000个观测).
在一台4G内存的iMac上, 可以不费力的处理含有上亿个元素的数据.  
但是也要考虑到两个问题, 数据集的大小和要应用的统计方法. R 可以处理GB级别到TB级别数据分析问题, 但需要专门的手段.

Memory limits will depend primarily on the R build (32 versus 64-bit) and for 32-bit Windows, on the  OS version involved. 
Error messages starting with  `cannot allocate vector of size`  typically indicate a failure to obtain sufficient contiguous memory, 
while error messages starting with  `cannot allocate vector of length` indicate that an address limit has been exceeded. 
When working with large datasets, try to use a 64-bit build if at all possible. 
For all builds, the number of elements in a vector is limited to 2,147,483,647 (see  ?Memory for more information).
There are three issues to consider when working with large datasets: 

- (a) efficient programming to speed execution, 
- (b) storing data externally to limit memory issues, and 
- (c) using specialized statistical routines designed to efficiently analyze massive amounts of data. We will briefly consider each.

## Efficient programming
There are a number of programming tips that improve performance when working with large datasets.

- Vectorize calculations when possible. Use R's built-in functions for manipulating vectors, matrices, and lists (for example, s apply , lappy , and mapply ) and avoid loops ( for and  while ) when feasible.
- Use matrices rather than data frames (they have less overhead).
- When using the  read.table()  family of functions to input external data into data frames, specify the  colClasses  and  nrows options explicitly, set  comment.  
char = "" , and specify  "NULL" for columns that aren't needed. This will decrease memory usage and speed up processing considerably. 
When reading external data into a matrix, use the  scan() function instead.
- Test programs on subsets of the data, in order to optimize code and remove bugs, before attempting a run on the full dataset.
- Delete temporary objects and objects that are no longer needed. 
The call rm(list=ls())  will remove all objects from memory, providing a clean slate.  Specific objects can be removed with  rm( object )  .
- Use the function . ls.objects() described in Jeromy Anglim's blog entry "Memory Management in R: A Few Tips and Tricks" (jeromyanglim.blogspot.com), 
to list all workspace objects sorted by size ( MB ). This function will help you find and deal with memory hogs.
- Profile your programs to see how much time is being spent in each function.  
You can accomplish this with the  Rprof()  and  summaryRprof() functions. The system.time()  function can also help. 
The  profr  and  prooftools packages provide functions that can help in analyzing profiling output.
- The  Rcpp package can be used to transfer R objects to C++ functions and back when more optimized subroutines are needed.
With large datasets, increasing code efficiency will only get you so far. 
When bumping up against memory limits, you can also store our data externally and use specialized analysis routines.

## Storing data outside of RAM
There are several packages available for storing data outside of R's main memory. 
The strategy involves storing data in external databases or in binary flat files on disk, and then accessing portions as they are needed. 

- ff Provides data structures that are stored on disk but behave as if they were in RAM.
- bigmemory Supports the creation, storage, access, and manipulation of massive matrices. Matrices are allocated to shared memory and memory mapped files.
- filehash Implements a simple key-value database where character string keys are associated with data values stored on disk.
- ncdf , ncdf 4  Provides an interface to Unidata netCDF data files.
- RODBC , RMySQL , ROracle , RPostgreSQL , RSQ Lite Each provides access to external relational database management systems.

## Analytic packages for large datasets
R provides several packages for the analysis of large datasets:

- The  biglm  and  speedglm packages fit linear and generalized linear models to large datasets in a memory efficient manner. 
This offers  lm()  and  glm()  type functionality when dealing with massive datasets.
- Several packages offer analytic functions for working with the massive matrices produced by the  bigmemory package . 
The  biganalytics package offers k-means clustering, column statistics, and a wrapper to  biglm . 
The  bigtabulate package provides  table()  ,  split()  , and  tapply() functionality and the bigalgebra package provides advanced linear algebra functions.
- The  biglars package offers least-angle regression, lasso, and stepwise regression for datasets that are too large to be held in memory, when used in conjunction with the  ff package .
- The  Brobdingnag package can be used to manipulate large numbers (numbers larger than 2^1024).
Working with datasets in the gigabyte to terabyte range can be challenging in any language. 
For more information on the methods available within R, see the CRAN Task View: High-Performance and Parallel Computing with R (cran.r-project.org/web/views/).


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

执行r的脚本文件: `R -f file.r`

[批量运行R代码](http://book.2cto.com/201305/21969.html)
```
#!/usr/bin/Rscript --slave
argv <- commandArgs(TRUE)
x <- as.numeric(argv[1]) # 默认的参数是字符串类型
y <- as.integer(argv[2])
```
执行: `$ Rscript scriptfile arg1 arg2`

下面是一些其他批处理模式下的实用命令:

- --slave 类似于--quiet,它禁止回送输入的信息,使R软件输出的信息更为简洁.
- --no-restore 在R启动时不还原工作空间.对于希望以空白工作空间启动R的脚本而言,这个选项很有必要.
- --no-save 在退出R时,不保存工作空间,否则,R会保存当前工作空间并覆盖原有工作目录中的.RData文件.
- --no-init-file 不读取.Rprofile文件或者~/. Rprofile文件.

注意: 

1. R 语言里面没有像C 语言那样的多行注释
1. R 中时没有续行符的, 所以要注意[ref](http://yihui.name/en/2007/12/be-careful-with-the-value-returned-in-r-functions/), [demo](../demo/r/line_continuation.r)

# Data Types
## every is object
R是一种基于对象(Object)的语言,在R语言中接触到的每样东西都是一个对象,一串数值向量是一个对象,一个函数是一个对象,一个图形也是一个对象.
基于对象的编程(OOP)就是在定义类的基础上,创建与操作对象.

- `attributes()` 获得对象属性
- `str()`,它能以简洁的方式显示对象的数据结构及其内容

list objects in memory  

- `ls()`: only the names of objects
- `ls(pattern = "m")`: list the objects with the name containing "m", or using `pat` for short
- `ls.str()`: display some details

delete objects in memory  

- `rm(x)`, `rm(x,y)` remove the object x and y from memory  
- `rm(list=ls())` remove all objects

save objects to File System  

- `save(x, file = 'x.RData')` 保存一个对象x 到文件 x.RData 中,  
- `load('x.RData')`, 将文件 x.RData 中的对象加载到内存中, 这里也就是x 对象.  
- `save.image()` is just a short-cut for 'save my current workspace', i.e., `save(list = ls(all = TRUE), file = ".RData")`.

## namespace
同一个环境只能存在一个唯一的名字,不同环境可以存在相同名字,R寻找一个名字,
会站在当前环境沿着search() path(`".GlobalEnv"     "package:base"   "namespace:base"`)往之后的环境中找名字,如果当前名字不符合就依次找后面的环境.  
可以参见[demo namespace](../demo/r/namespace.r)

## data types
最为基本的类包括了数值(numeric),逻辑(logical),字符(character),列表(list),
在此基础上构成了一些复合型的类,包括矩阵(matrix),数组(array),因子(factor),数据框(dataframe).

overview of the type of objects representing data

| object       | comment |
|--------------|---------|
| vector       | 
| array        | 
| factor       | a categorical variable, 类似于C 语言中enum 类型 | 
| data frame   | 类似于数据库中的table, 每一行都代表a record,而每一列则代表了不同属性 |
| list         | 可以包含任何的object |
| matrix       | 
| ts           |a time series data set |

a matrix with 2 lines and 2 columns has for dim the pair of values `[2, 2]`, but its length is 4.

- cbind() 把矩阵横向合并成一个大矩阵(列方式), 将参数当成列矩阵来处理
- rbind() 是纵向合并(行方式), 将参数当行矩阵来处理

[cbind and rbind demo](../demo/r/bind.r)

## 类型的判断
[is 与 inherits](http://stackoverflow.com/questions/27923345/whats-the-difference-between-is-and-inherits)
```
class(letters) ## [1] "character"
is(letters, "character") ## [1] TRUE
inherits(letters, "character") ## [1] TRUE
```
Is there a preference for which one I should use, and do they ever return different values?

Use inherits, but be careful with numbers and S4 classes.

The most obvious place where the two functions differ is when checking if integers are numeric.
```
class(1L) ## [1] "integer"
is.numeric(1L) ## [1] TRUE
is(1L, "numeric") ## [1] TRUE
inherits(1L, "numeric") ## [1] FALSE
```

## vector
Vectors are the most important type of object in R.

追加元素
```
> x <- c(1:3)
> c(x,5)
[1] 1 2 3 5
```

选择特定的元素
```
> a <- c(1:5)
> a>3
[1] FALSE FALSE FALSE  TRUE  TRUE
> a[a>3]
> sum(a>3)  ## 统计满足条件的元素的个数
```

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

logical vectors

- any: Given a set of logical vectors, is at least one of the values true?
- all: Given a set of logical vectors, are all of the values true?
```
> x <- c(1:5)
> y <- c(2:4)
> x %in% y [1] FALSE  TRUE  TRUE  TRUE FALSE
> all(x %in% y) [1] FALSE
> any(x %in% y) [1] TRUE
```

## array
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

`levels(factor)`: 获取不同的levels, 返回类型为character 的数据类型

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

有时后从数据源处获得的都是一个个的record(具有不同数据类型的fields), 然后想把很多的records组成一个data.frame, 
现在我能想到的方法就是用list来存储一个record, 然后用rbind 函数将这个list 放到data.frame里面.(循环实现将所以的records放入data.frame中)

下面的命令可以让你有机会修改数据并存入到新的变量newdata中:
```
newdata=edit(data)
```

根据已有列同时增加多列数据
```
mydata <- transform(mydata, 
	sumx = x1 + x2, 
	meanx = (x1 + x2)/2)
```

同时减少多列
```
## 方式1
myvars <- names(leadership) %in% c("q3", "q4")
newdata <- leadership[!myvars]

## 方式2
newdata <- leadership[c(-7, -8)]
```
也可以采用另外一种思路, 用集和运算将要保留的cols 计算出来

### 取子集
Using the subset() function
```
## 选择所有age >= 35 或者 age <24的行, 保留了fields q1 到 q4
newdata <- subset(leadership, age >= 35 | age < 24, select = c(q1, q2, q3, q4))

## 选择所有满足条件的行, 保留了gender 到 q4之间所有的fields(包括两者)
newdata <- subset(leadership, gender == "M" & age > 25, select = gender:q4)
```

Selecting observations
```
newdata <- leadership[which(leadership$gender == "M" & leadership$age > 30), ]

attach(leadership)
newdata <- leadership[which(gender == "M" & age > 30), ]
detach(leadership)
```

涉及到日期
```
leadership$date <- as.Date(leadership$date, "%m/%d/%y")
startdate <- as.Date("2009-01-01")
enddate <- as.Date("2009-10-31")
newdata <- leadership[leadership$date >= startdate & leadership$date <= enddate, ]
```

## 随机抽样 sample
`sample(x, size, replace = FALSE, prob = NULL)`  
replace(FALSE) 表示无放回抽样; prob 的和可以不为1, 只要保证每个元素非负就可以了

随机抽取大小为3的样本: `mysample <- leadership[sample(1:nrow(leadership), 3, replace=FALSE), ]`

我们想从1到10中随机抽取5个数字,那么这样来做:首先产生一个序列,然后用sample函数进行无放回抽取: `sample(c(1:10),size=5)`

`sample(x, n)`  ## 当length(x) = 1 且 x > 1, 那么这句话就是从序列1:x中取出n个值

和replicate 结合可以很方便的产生**matrix**, 一个sample 构成一列
```
> x <- 1:5
> sample(x, length(x),replace=T, prob=c(0.1,0.2,0.3,0.25,0.25))
[1] 3 4 4 2 5
> replicate(3, sample(x, length(x),replace=F))
     [,1] [,2] [,3]
[1,]    4    4    4
[2,]    3    5    2
[3,]    2    1    5
[4,]    1    2    1
[5,]    5    3    3
```

### 更改列名
1. 直接通过index 来修改: `names(dataframe)[index] <- "newname"`
1. 不需要知道列名情况下修改列名:`names(df)[names(df)=="site"]="position";`
1. 通过reshape 重命名: `library(reshape); rename(dataframe, c(oldname = "newname", oldname = "newname", ...))`

### rearrange columns of a data frame
[ref](http://stackoverflow.com/questions/3369959/moving-columns-within-a-data-frame-without-retyping/18540144#18540144).
use functio [moveme](../demo/r/moveme.r)

using it to reorder the columns in your data.frame is straightforward: `df[moveme(names(df), "g first")]`

```
moveme(names(df), "g first")
moveme(names(df), "g first; a last; e before c")
```

使用sql 操作data.frame, 使用sqldf的前提是本地有数据库服务, 因为sqldf 会调用本地的数据库来执行语句
[manipulate data.frame using sql demo](../demo/r/data.frame_sql.r)

## hash
在Python中有这样一个神通广大的数据类型,它叫Dictionary.
而长久以来在R中想要实现类似的Hash存储只能依靠environment类型,用起来非常不友好.
hash它对environment进行了封装,使用户可以很方便的利用Hash表进行存储.

其中有几个地方需要特别注意的:

1. Hash表的Key必须为字符类型的,而且不能是空字符串
1. 引用传递.在R中environment和hash对象只存在一份全局拷贝,因此如果在函数内改变它的值将会影响到外部访问的结果.如果需要复制hash对象,需调用它的copy方法
1. 内存释放.通过rm销毁hash对象时,其占用的内存不会自动释放,因此需在rm前调用clear,以防内存泄露

[hash 与 list 性能比较](http://equation85.github.io/blog/hash-table-for-r/)

[hash demo](../demo/r/hash_demo.r)

## NA
- **Missing values** are represented by the symbol **NA** (not available).
- **Impossible values**(e.g., dividing by zero) are represented by the symbol **NaN**(not a number).

Testing for Missing Values  
对于一般的数据类型使用`is.na` 进行检测, 
而对于data.frame 需要使用complete.cases 进行检测(注意complete.cases输出中为TRUE表示这个record 是完整的)

测试函数: `is.finite(x), is.infinite(x), is.nan(x), is.na(x)`

[不同类型NA的判断](../demo/r/na_judge.r)

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

- Sys.time():  get the current time
- as.POSIXct and as.POSIXlt convert the time value into the different formats.
- strftime: string from time, convert time to string, eg `timeString <-  strftime(t,"%Y-%m-%d %H:%M:%S")`
- strptime: convert string into a form that R can use for calculations, eg `time <- strptime("2014-01-23 14:28:21", "%Y-%m-%d %H:%M:%S")`

[从系统与字符串转换来的时间类型是不一样的, 但是还是可以直接作差的](../demo/r/time_type.r)

### Date
The difference is that the date data type keeps track of numbers of days rather than seconds. 
You can cast a string into a date type using the `as.Date` function. The `as.Date` function takes the same arguments as the time data types discussed above.

[日期类型的数据](../demo/r/date_type.r)

[Time Operations](../demo/r/time_operation.r)

`difftime(time1, time2, 'arg')`, arg should be one of "auto", "secs", "mins", "hours", "days", "weeks"

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
typeof() gives the "type" of object from R point of view,   
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

描述统计是一种从大量数据中压缩提取信息的工具,最常用的就是summary命令
对于数值变量计算了五个分位点和均值,对于分类变量则计算了频数

## reshape2
### transform data between wide and long formats
[An Introduction to reshape2](http://seananderson.ca/2013/10/19/reshape.html)

[What makes data wide or long?](../demo/r/reshape2.r)

It is easier to record their data in wide format, but it is easy to process in long format

reshape2 is based around two key functions: melt and cast:

- melt takes wide-format data and melts it into long-format data.
- cast takes long-format data and casts it into wide-format data.

In reshape2 there are multiple cast functions. Since you will most commonly work with data.frame objects, we will explore the dcast function. 
(There is also acast to return a vector, matrix, or array.)

dcast uses a formula to describe the shape of the data. The arguments on the left refer to the ID variables and the arguments on the right refer to the measured variables.

[reshape demo](../demo/r/reshape2.r)

**melt 错误提示**  
Warning message: attributes are not identical across measure variables; they will be dropped  
This warning is basically telling you that variables that you are trying to put in the "value" column (the measure variables) are different types 
(some may be character, others may be factors, others may be numeric).  
@MrFlick suggestion to treat those columns as keys even if they might not be would solve that problem.

**cast 错误提示**  
Aggregation function missing: defaulting to length  
This warning is a warning that you usually get when the combination of IDs is not unique.  
you would need to add another column to make the ID variables unique to avoid having dcast automatically use length as its fun.aggregate function.

Reshape2 is a reboot of the reshape package. 
It is been over five years since the first release of the package, and in that time I have learned a tremendous amount about R programming, and how to work with data in R. 
Reshape2 uses that knowledge to make a new package for reshaping data that is much more focussed and much much faster.

This version improves speed at the cost of functionality, so I have renamed it to reshape2 to avoid causing problems for existing users. 
Based on user feedback I may reintroduce some of these features.

What is new in reshape2:

- considerably faster and more memory efficient thanks to a much better underlying algorithm that uses the power and speed of subsetting to the fullest extent, 
in most cases only making a single copy of the data.
- cast is replaced by two functions depending on the output type:  dcast produces data frames, and acast produces matrices/arrays.
- multidimensional margins are now possible: grand_row and  grand_col have been dropped: now the name of the margin refers to the variable that has its value set to (all).
- some features have been removed such as the | cast operator, and the ability to return multiple values from an aggregation function. 
I am reasonably sure both these operations are better performed by plyr.
- a new cast syntax which allows you to reshape based on functions
- of variables (based on the same underlying syntax as plyr):
- better development practices like namespaces and tests.

# 常用统计函数运算
在R语言中经常会用到函数,例如上节中讲到的求样本统计量就需要均值函数(mean)和标准差函数(sd).对于二元数值数据还用到协方差(cov),对于二元分类数据则可以用交叉联列表函数(table).
下文讲述在初级统计学中最常用到的三类函数.

比如, 对于某个数据集, 使用with 可以省去写data$var1, data$var2 这样的重复性修饰.
```
with(iris, c(mean(Sepal.Length), sum(Sepal.Length)))
```

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

用count = 1, 在利用sum 函数聚合后, 可以求出by 变量的出现次数.
> aggregate(c(mtcars[vars], count=1), by = list(am=mtcars$am), sum)
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

[一维列联表](../demo/r/table.r)
### 二维列联表
```
mytable <- table(A,B)
mytable <- xtabs(~A+B, data = mydata)
```
[二维列联表](../demo/r/table.r)

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

## 集合运算
- 求A和B的并集 `union(A, B)`
- 求A和B的交集 `intersect(A, B)`
- 求A-B `setdiff(A, B)`
- 求B-A `setdiff(B, A)`
- 检验集合A,B是否相同 `setequal(A, B)`
- 检验元素12是否属于集合A `is.element(12, A)`
- 检验集合A是否包含B `all(B %in% A)`

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
- 字符串连接: paste(sep=" "), 默认用空格进行连接
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
> length(r[[1]])
[1] 4
> r[[1]][4]
[1] "test"
```
- 字符串截取:substr()能对给定的字符串对象取出子集,其参数是子集所处的起始和终止位置.
- 字符串替代:gsub()负责搜索字符串的特定表达式,并用新的内容加以替代.
- sub()函数是类似的,但只替代第一个发现结果.
- 字符串匹配:grep()负责搜索给定字符串对象中特定表达式 ,并返回其位置索引.
- grepl()函数与之类似,但其后面的"l"则意味着返回的将是逻辑值.
- tolower(): 将全部字母转换为小写
- toupper()
- capitalize(y): 首字母大写, 需要`library(Hmisc)`

# IO
`print(x, ...)`

For more customizable (but cumbersome) printing, see `cat`, `format` or also `write`.   
For a simple prototypical print method, see `.print.via.format` in package **tools**.

`cat(x, y, ...)`: 可以输出多个变量, eg: `cat("x = ", x, "\n")`

`source("commands.R")` 读取文件并执行  

重定向输出
```
sink("文件名", append = FALSE, split = FALSE)
sink()   #取消显示到文件
```
使用参数append=TRUE可以将文本追加到文件后  
参数split=TRUE可将输出同时发送到屏幕和输出文件中

- 'write' is a wrapper for 'cat', which gives further details on the format used.
- 'save' for writing any R objects, 
- 'write.table' for data frames, and 
- 'scan' for reading data.

```
write.table(x, file = "", append = FALSE, quote = TRUE, sep = " ", eol = "\n", na = "NA", dec = ".", row.names = TRUE, col.names = TRUE, qmethod = c("escape", "double"), fileEncoding = "")
write.csv(...)
write.csv2(...)
```

- quote: a logical value ('TRUE' or 'FALSE') or a numeric vector.  
If 'TRUE', any character or factor columns will be surrounded by double quotes.  
If a numeric vector, its elements are taken as the indices of columns to quote.  In both cases, row and column names are quoted if they are written.  
If 'FALSE', nothing is quoted.
- sep: the field separator string. Values within each row of x are separated by this string.
- eol: end of line
- na: the string to use for missing values in the data
- dec: the string to use for decimal points in numeric or complex columns: must be a single character.
- row.names: either a logical value indicating whether the row names of x are to be written along with x, or a character vector of row names to be written.
- col.names: either a logical value indicating whether the column names of x are to be written along with x, or a character vector of column names to be written. 

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
read.table(file, header = FALSE, sep = "")
read.csv(file, header = TRUE, sep = ",", dec = ".", fill = TRUE, comment.char = "", ...)
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

[database demo](../demo/r/db.r)

# 流程
```
> if (expr_1) expr_2 else expr_3
> for (name in expr_1) expr_2
> while (condition) expr
```
exp 中可以用{}

```
if(condition){
	...
}else{
	...
}
```
一定要注意: else必须放在} 的后面, 而不能另起一行.

break, next

`prediction = ifelse(post.yes >= post.no, "Yes", "No")` 类似于C 语言中的`? :`运算符.

# function
函数调用会产生所谓的 call stack,这个结构也就产生了 environment 的树状结构.
我们可以用 sys.* 函数访问这个 call stack, 如 

- sys.call() 返回当前(或者通过 which 参数表示更上几个层次的)函数,
- sys.frame() 返回当前 environment 的 frame,
- sys.function() 返回的是当前函数,
- sys.parent(0) 返回的是上级 environment,
- 对应还有复数版本,比如 sys.functions() 就是获得调用栈里面所有函数.

## 自定义
当要返回多值, 可以返回一个list, eg: `return(list(min = value_min, max = value_max))`

```
myfunction <- function(arg1, arg2, ... ){
	...
	return(object)
}
```

## Default arguments and lazy evaluation in R
[ref](http://www.johndcook.com/blog/2008/10/16/default-arguments-and-lazy-evaluation-in-r/)

In C++, default function arguments must be constants, but in R they can be much more general. For example, consider this R function definition.
```
f <- function(a, b=log(a)) { a*b }
```
If f is called with two arguments, it returns their product. 
If f is called with one argument, the second argument defaults to the logarithm of the first. That is convenient, but it gets more surprising. Look at this variation.
```
f <- function(a, b=c) {c = log(a); a*b}
```
Now the default argument is a variable that does not exist until the body of the function executes! 
If f is called with one argument, the R interpreter chugs along until it gets to the last line of the function and says 
Hmm. What is b? Let me go back and see. Oh, the default value of b is c, and now I know what c is.

This behavior is called lazy evaluation. 
Expressions are not evaluated unless and until they are needed. It is a common feature of functional programming languages.

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

`library(packagename)` 或者`library("packagename")`.  
当var 是一个字符串变量呃时候, `library(var)` 会失败, 需要使用`library(var, character.only = TRUE)`

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

- Vectorize calculations when possible. Use R built-in functions for manipulating vectors, matrices, and lists (for example, s apply , lappy , and mapply ) and avoid loops ( for and  while ) when feasible.
- Use matrices rather than data frames (they have less overhead).
- When using the  read.table()  family of functions to input external data into data frames, specify the  colClasses  and  nrows options explicitly, set  comment.  
char = "" , and specify  "NULL" for columns that are not needed. This will decrease memory usage and speed up processing considerably. 
When reading external data into a matrix, use the  scan() function instead.
- Test programs on subsets of the data, in order to optimize code and remove bugs, before attempting a run on the full dataset.
- Delete temporary objects and objects that are no longer needed. 
The call rm(list=ls())  will remove all objects from memory, providing a clean slate.  Specific objects can be removed with  rm( object )  .
- Use the function . ls.objects() described in Jeromy Anglim blog entry "Memory Management in R: A Few Tips and Tricks" (jeromyanglim.blogspot.com), 
to list all workspace objects sorted by size ( MB ). This function will help you find and deal with memory hogs.
- Profile your programs to see how much time is being spent in each function.  
You can accomplish this with the  Rprof()  and  summaryRprof() functions. The system.time()  function can also help. 
The  profr  and  prooftools packages provide functions that can help in analyzing profiling output.
- The  Rcpp package can be used to transfer R objects to C++ functions and back when more optimized subroutines are needed.
With large datasets, increasing code efficiency will only get you so far. 
When bumping up against memory limits, you can also store our data externally and use specialized analysis routines.

## Storing data outside of RAM
There are several packages available for storing data outside of R main memory. 
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

# Writing R Extensions
A package consists of a subdirectory containing a file 'DESCRIPTION' and the subdirectories 'R', 'data', 'demo', 'exec', 'inst', 'man', 'po', 'src', and 'tests' (some of which can be missing).
The package subdirectory may also contain files 'INDEX', 'NAMESPACE', 'configure', 'cleanup', 'LICENSE', 'LICENCE', and 'COPYING'. 
Other files such as 'README', 'NEWS' or 'ChangeLog' will be ignored by R, but may be useful to end-users.

一个最简单的包结构如下(括号中为相应解释):
```
pkg (包的名字,请使用一个有意义的名字,不要照抄这里的pkg三个字母)
|
|--DESCRIPTION (描述文件,包括包名,版本号,标题,描述,依赖关系等)
|--R (函数源文件)
	|--function1.R
	|--function2.R
	|--...
|--man (帮助文档)
	|--function1.Rd
	|--function2.Rd
	|--...
|--...
```

'package.skeleton' automates some of the setup for a new source package.  
It creates directories, saves functions, data, and R code files to appropriate places, and creates skeleton help files and a 'Read-and-delete-me' file describing further steps in packaging.

制作package 的步骤[ref](http://blog.fens.me/r-build-package/):

1. 生成骨架: `package.skeleton(name="package-name", code_files="./sayHello.R")`
1. 打包: `R CMD build package-name`
1. 安装: `R CMD INSTALL package-name_version.tar.gz`
1. 卸载: 进入sudo R环境, `remove.package("package-name")`
1. 如果要提交R包和CRAN,必须要执行check检查.如果有任何的error和warning都将不被通过: `R CMD check package-name_version.tar.gz`

## DESCRIPTION
- Package: 包的名字
- Version: 版本(介绍语义版本命名法,主要.次要.补丁:http://semver.org/,让版本号变得有意义,除非你是Knuth,用pi做版本号)
- Date: 日期
- Title: 标题
- Description: 描述(详细说明)
- Author: 作者(可以多人)
- Maintainer: 维护者(一个人,可以不同于作者,**必须要有邮箱**)
- 依赖关系
	- Depends 加载这个包会依赖加载进来的包
	- Imports 只是导入命名空间,不直接加载(被导入的包中的函数对用户不直接可见)
	- Suggests 推荐安装的包,通常不涉及到本包的核心功能,但如果有这些包的话,本包会更强大
- License: 许可证(发布到CRAN的包必须用开源许可证,不限于GPL)
- URL: 网址
- BugReports: Bug报告地址
- R源文件列表(指定用哪些R代码来创建本包)

## 其他目录
data文件夹放R数据,扩展名为rda,通常可以用save()函数生成.  
对每一个数据,都必须有相应的Rd文档,它可以通过roxygen生成

demo文件夹里可以放一些演示,这些演示文件将来可以用demo()函数来调用

inst文件夹下的所有文件都会被原封不动复制到安装包的路径下,这个文件夹下可以放任意文件,但有一个例外是doc,它用来放R包的手册(Vignette),后文详述

## NAMESPACE
命名空间(NAMESPACE)是R包管理包内对象的一个途径,它可以控制哪些R对象是对用户可见的,哪些对象是从别的包导入(import),哪些对象从本包导出(export).

为什么要有这么个玩意儿存在?主要是为了更好管理你的一堆对象.写R包时,有时候可能会遇到某些函数只是为了另外的函数的代码更短而从中抽象,独立出来的.
这些小函数仅仅供你自己使用,对用户没什么帮助,他们不需要看见这些函数,
这样你就可以在包的根目录下创建一个NAMESPACE文件,里面写上export(函数名)来导出那些需要对用户可见的函数.
自R 2.14.0开始,命名空间是R包的强制组成部分,所有的包必须有命名空间,如果没有的话,R会自动创建.

Imports,这里设置的包通常是你只需要其部分功能的包,
例如我只想在我的包中使用foo包中的bar()函数,NAMESPACE中则需要写`importFrom(foo, bar)`,
在自己的包的源代码中则可以直接调用bar()函数,R会从NAMESPACE看出这个bar()对象是从哪里来的.

### S3泛型函数
S3泛型函数的核心思想是基于对象的类去匹配函数
S3函数可以用UseMethod()去定义,然后**函数.类名**就是具体的子函数,
例如hello()这个函数([ref](https://github.com/yihui/rmini/blob/master/R/S3.R))有两个子函数hello.default()和hello.character(),分别对应它的默认方法以及对字符对象应用的方法.
NAMESPACE 中写
```
S3method(hello, character)
S3method(hello, default)
```
使用
```
library(rmini)
hello(1) ## hello, numeric
hello("a") ## Hi! I love characters!
hello(structure(1, class = "world")) ## hello, world
```

## 嵌入其它语言
R可以与其它语言沟通,常见的如C和Fortran.  
其它语言的源代码都放在src文件夹底下(ex: [reverse.c](https://github.com/yihui/rmini/blob/master/src/reverse.c)).
这个c文件将来在R CMD INSTALL过程中会被编译成一个动态链接库,供R调用.
R函数(ex: [reverse()](https://github.com/yihui/rmini/blob/master/R/C.R))中我们使用`.C()`调用前面提到的C函数.  
注意这里在调用之前我们必须告诉R加载编译好的动态链接库,所以我们在NAMESPACE文件中生成相应的useDynLib()命令,当R包加载的时候,动态链接库也会被加载.

## C interface
1. C functions called by R must all return void, which means they need to return the results of the computation in their arguments.
2. All arguments passed to the C function are passed by reference, which means we pass a pointer to a number or array.
3. Each file containing C code to be called by R should include the `R.h` header file.
If you are using special functions (e.g. distribution functions), you need to include the `Rmath.h` header file.
1. When compiling your C code, you use R to do the compilation rather than call the C compiler directly.
Use `R CMD SHLIB foo.c` or `R CMD SHLIB -o foo.so foo.c`
1. load the library, `> dyn.load('foo.so')`

[demo C](../demo/r/hello.c)  
[demo R](../demo/r/hello.r)

在R中定义的函数是可以和C中的函数用一样的名字

Notice that we convert n to integer type using as.integer and 
in the C function we have set n to be of type int * (remember that variables are always passed as pointers when using .C)

.C returns a list containing the (possibly modified) arguments which were passed into your C function.

输出用, Rprintf.
Rprintf is exactly like the standard printf function in C except that Rprintf sends its output to the R console so that you can see it when running R.

Another ex:
```
cconv.c
*s is the result
void cconv(int *l, double *x, int *n, double *s){
	double *y = x + (*n - *l), *z = x + *l, *u = x;
	while ( u < y)
		*s += *u++ * *z++;
}

rconv.r
rconv <- function(lag,x) {
	.C("cconv", as.integer(lag), as.double(x), as.integer(length(x)), as.double(0.0))[[4]]
}
```
The .C function returns a list with all its arguments, we only need the last (fourth) argument.  
In this case we did not name the last argument but extracted it using a numeric index.

R has many matrix manipulation routines that are highly optimized.
所以要尽量避免在C里面操作矩阵, 但是如果确实需要的话, 
it is important to remember that matrices are represented as just very long vectors (of length nrows * ncols) in C.


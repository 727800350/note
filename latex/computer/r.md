`> q()`
At this point you will be asked whether you want to save the data from your R session.
Data which is saved will be available in future R sessions.

**Executing commands from or diverting output to a file**
`> source("commands.R")` 读取文件并执行  
`> sink("record.lis")` will divert all subsequent output from the console to an external file, record.lis.   
The command `> sink()` restores it to the console once again.

**assign operator**
```
n <- 15
15 -> n
```
在多数场合下, 可以用`=`来代替`<-`  
或者使用`assign("x", c(10.4, 5.6, 3.1, 6.4, 21.7))`

# Objects
## namespace
同一个环境只能存在一个唯一的名字,不同环境可以存在相同名字,R寻找一个名字,
会站在当前环境沿着search() path(`".GlobalEnv"     "package:base"   "namespace:base"`)往之后的环境中找名字,如果当前名字不符合就依次找后面的环境.  
可以参见[demo namespace](../../demo/r/namespace.r)

## object types
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
- A data frame is a table composed with one or several vectors and/or factors all of the same length but possibly of different modes. 
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

### Missing values
In general any operation on an NA becomes an NA.
`is.na(x)` 进行判断, gives a logical vector of the same size as x with value TRUE if and only if the corresponding element in x is NA

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

计算每个元素出现的次数,使用table()函数
> table(g) 
g 
f m 
3 2
table()函数还可以用于获得多个因子的交叉表
>  age<-factor(c('adult','adult','adult','adult','juvenile')) 
> table(age,g)
age        f m 
  adult    3 1 
  juvenile 0 1

计算表的边际(margin)和频率.
计算边际
> t<-table(age,g)

#按照年龄查看边际
> margin.table(t,1) 
age 
   adult juvenile 
       4        1 
> margin.table(t,2) 
g 
f m 
3 2

计算频率
> prop.table(t,1)#
age           f    m 
  adult    0.75 0.25

#查看整个表的频率 
> prop.table(t)
age          f   m 
  adult    0.6 0.2 
  juvenile 0.0 0.2
```

## list
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

## mode
All objects have two intrinsic attributes: mode and length. 
The mode is the basic type of the elements of the object; there are four main modes:
**numeric, character, complex , and logical** (FALSE or TRUE). 
Other modes exist but they do not represent data, for instance function or expression. 
The **length** is the number of elements of the object. 
`mode(x)`, `length(x)`

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

# IO
`print(x, ...)`

For more customizable (but cumbersome) printing, see `cat`, `format` or also `write`.   
For a simple prototypical print method, see `.print.via.format` in package **tools**.

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
break, next

`prediction = ifelse(post.yes >= post.no, "Yes", "No")` 类似于C 语言中的`? :`运算符.

# function
## math
Vectors occurring in the same expression need not all be of the same length. 
If they are not, the value of the expression is a vector with the same length as the longest vector which occurs in the expression. 
**Shorter vectors in the expression are recycled** as often as need be until they match the length of the longest vector.

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

### sample
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

### rnorm
`rnorm(n, mean = 0, sd = 1)`  
n: number of observations. If ‘length(n) > 1’, the length is taken to be the number required.  
random generation for the normal distribution with mean equal to ‘mean’ and standard deviation equal to ‘sd’.

### \*apply
#### apply
returns a vector or array or list of values obtained by applying a function to margins of an array or matrix.
```
apply(X, MARGIN, FUN, ...)
X: an array, including a matrix
for a matrix ‘1’ indicates rows, ‘2’ indicates columns, ‘c(1, 2)’ indicates rows and columns.
```

demo
```
> ma <- matrix(c(1:4, 1, 6:8), nrow = 2)
> ma
     [,1] [,2] [,3] [,4]
[1,]    1    3    1    7
[2,]    2    4    6    8
> apply(ma, 1, sum)  ## 对行求和
[1] 12 20
> apply(ma, 2, sum)  ## 对列求和
[1]  3  7  7 15
```

#### tapply

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
# model
`summary` is a generic function used to produce result summaries of the results of various model fitting functions.  
The function invokes particular ‘methods’ which depend on the ‘class’ of the first argument.

`fitted` is a generic function which extracts fitted values from objects returned by modeling functions.
All object classes which are returned by model fitting functions should provide a ‘fitted’ method.

`data`: Loads specified data sets, or list the available data sets.

## install package
- `install.packages()`会将package安装到默认的目录(.libPaths()指定), 类似于yum, 会自动查找依赖关系
- `remove.packages()`会将package移除  
- `update.packages()`: update package

```
> .libPaths()
[1] "/usr/lib64/R/library" "/usr/share/R/library"
```

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


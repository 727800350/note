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

`seq()` is a more general facility for generating sequences.

`rep()` which can be used for replicating an object in various complicated ways. The simplest form is  
`> s5 <- rep(x, times=5)` which will put five copies of x end-to-end in s5.  
`> s6 <- rep(x, each=5)` which repeats each element of x five times before moving on to the next.

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


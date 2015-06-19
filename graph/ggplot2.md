# R中绘图系统的介绍
ggplot2:elegant graphics for data analysis 一书是ggplot软件包2的系统介绍,包括其语法结构的详细介绍以及绘图实例,也包括了plyr,reshape等包的强大的数据处理能力,是数据分析和统计图形的优秀著作.

[online doc](http://docs.ggplot2.org/current/)

R的基础图形系统基本上是一个"纸笔模型",即:一块画布摆在面前,你可以在这里画几个点,在那里画几条线,指哪儿画哪儿.

后来lattice包的出现稍微改善了这种情况,你可以说,我要画散点图或直方图,并且按照某个分类变量给图中的元素上色,
此时数据才在画图中扮演了一定的中心角色,我们不用去想具体这个点要用什么颜色(颜色会根据变量自动生成).
然而,lattice继承了R语言的一个糟糕特征,就是参数设置铺天盖地,足以让人窒息,光是一份xyplot()函数的帮助文档,恐怕就够我们消磨一天时间了,
更重要的是,lattice仍然面向特定的统计图形,像基础图形系统一样,有直方图,箱线图,条形图等等,它没有一套可以让数据分析者说话的语法.

那么数据分析者是怎样说话的呢?他们从来不会说这条线用#FE09BE颜色,那个点用三角形状,他们只会说,把图中的线用数据中的职业类型变量上色,或图中点的形状对应性别变量.
有时候他们画了一幅散点图,但马上他们发现这幅图太拥挤,最好是能具体看一下里面不同收入阶层的特征,所以他们会说,把这幅图拆成七幅小图,每幅图对应一个收入阶层.
然后发现散点图的趋势不明显,最好加上回归直线,看看回归模型反映的趋势是什么,或者发现图中离群点太多,最好做一下对数变换,减少大数值对图形的主导性.

从始至终,数据分析者都在数据层面上思考问题,而不是拿着水彩笔和调色板在那里一笔一划作图,而计算机程序员则倾向于画点画线.
Leland Wilkinson的著作在理论上改善了这种状况,他提出了一套图形语法,让我们在考虑如何构建一幅图形的时候不再陷在具体的图形元素里面,而是把图形拆分为一些互相独立并且可以自由组合的成分.
这套语法提出来之后他自己也做了一套软件,但显然这套软件没有被广泛采用,幸运的是,Hadley Wickham在R语言中把这套想法巧妙地实现了.

为了说明这种语法的想法,我们考虑图形中的一个成分:坐标系.常见的坐标系有两种:笛卡尔坐标系和极坐标系.
在语法中,它们属于一个成分,可自由拆卸替换.笛卡尔坐标系下的条形图实际上可以对应极坐标系下的饼图,因为条形图的高可以对应饼图的角度,本质上没什么区别.
因此在ggplot2中,从一幅条形图过渡到饼图,只需要加极少量的代码,把坐标系换一下就可以了.
如果我们用纸笔模型,则可以想象,这完全是不同的两幅图,一幅图里面要画的是矩形,另一幅图要画扇形.

Without the grammer, there is no underlying theory and existing graphics packages are just a big collection of special cases.

## ggplot2 绘图概述
A statistical graphic is a **mapping** from **data** to **aesthetic** attributes(colour, shape, size) of **geometric** objects(points, lines, bars)
此外, 图形中还可能包含数据的统计变换(statistical transformation, 缩写为stats),
最后绘制在某个特性的坐标系(coordinate system, 缩写为coord)中, 
而分面(facet, 指将绘图窗口划分为若干个子窗口)则可以用来生成数据不同子集的图形.
总而言之, 一个统计图形就是由上述这些独立的图形部件所组成的.

- 最基础的部分是想要可视化的**数据(data)**, 以及一系列将数据中的变量对应到图形属性的**映射(mapping)**
- **几何对象(geom)**: 代表你在图中实际看到的图形元素, 如点, 线, 多边形等
- **统计变换(stats)**: 是对数据进行的某种汇总. 例如将数据分组计数以创建直方图, 或将一个二维的关系用线性模型来进行解释
- **标度(scale)**: 将数据的取值(values in the data space)映射到图形控件(values in an aesthetic space), 例如用颜色, 大小或形状来表示不同的取值. 展现标度的常见做法是绘制图例和坐标轴---他们实际上是图形到数据的一个映射, 使读者可以从图中读取原始的数据.
- **坐标系(coord)**: 描述了数据是如何映射到图形所在的平面的, 它同时提供了看图所需的坐标轴和网格线. 通常使用笛卡尔坐标系, 但也用极坐标系和地图投影
- **分面(facet)**: 描述了如何将数据分解为各个子集, 记忆如何对子集作图并联合进行展示. 分面也叫做条件作图或网格作图

1. scale transformation 在 statistical transformation 之前  
This ensures that a plot of log(x) vs. log(y) on linear scales looks the same as x vs. y on log scales.
1. After the statistics are computed, each scale is trained on every dataset from all the layers and facets. 
The training operation combines the ranges of the individual datasets to get the range of the complete data
1. Finally the scales map the data values into aesthetic values producing a new dataset that can then be rendered by the geoms.

Coordinate transformation 在 statistical transformation 之后

A scale is a function, and its inverse, along with a set of parameters.
The inverse function is used to draw a guide so that you can read values from the graph. 
Guides are either axes (for position scales) or legends (for everything else).

绘图有两种方式:

1. 一种是一步到位, 即利用qplot
1. 另一种是逐层叠加式, 即利用ggplot()函数和图层叠加逐步作图.
Each layer can come from a different dataset and have a different aesthetic mapping

当我们得到一个图形对象时, 可以对它进行如下处理.

- 用print() 将其打印到屏幕上. 在交互操作时, print 会自动被调用, 但是在循环或函数里, 我们需要手动输入print
- 用ggsave()将其保存在磁盘上  
`ggsave("plot.png", width=5, height=5)`
- summary() 简单查看其结构
- save() 把图像的缓存副本保存到磁盘; 这样可以保存图像的完整副本(包括图形中的数据), 可以调用load()来重现该图  
`save(p, file = "plot.rdata")`

# aesthetic
`unique(mtcars$cyl)` 得到 4 6 8

- `ggplot(mtcars, aes(cyl))`, 那么x 就是从4 到 8
- `ggplot(mtcars, aes(factor(cyl)))`, x 就是只有4, 6, 8三个

# geom
对于 geom_bar
color 改变的只是框的颜色
fill 才是填充色

geom_bar(width = 0.5)  控制宽度

# group
Oxboys records the heights(height) and centered ages(age) of 26 boys(Subject), measured on nine occasions(Occasion).
```
library(nlme)
h <- ggplot(Oxboys, aes(age, height))  ## 默认的就是group = 1
## A single line tries to connect all the observations
h + geom_line()
```
[group = 1 result](http://docs.ggplot2.org/current/aes_group_order-18.png)  
从图中可以看到得到的线是杂乱无章的.

```
## The group aesthetic maps a different line for each subject
h + geom_line(aes(group = Subject))
```
[group = Sbuject result](http://docs.ggplot2.org/current/aes_group_order-20.png)  
每个subject 画一条线

```
# Different groups on different layers
h <- h + geom_line(aes(group = Subject))
# Using the group aesthetic with both geom_line() and geom_smooth()
# groups the data the same way for both layers
h + geom_smooth(aes(group = Subject), method = "lm", se = FALSE)
```
[smooth group = Subject result](http://docs.ggplot2.org/current/aes_group_order-22.png)  
对于每个subject, 画出来的本来就是一条线, 再对每个subject, 来进行数据拟合, 没有太大的意义.

```
# Changing the group aesthetic for the smoother layer
# fits a single line of best fit across all boys
h + geom_smooth(aes(group = 1), size = 2, method = "lm", se = FALSE)
```
[smooth group = 1 result](http://docs.ggplot2.org/current/aes_group_order-24.png)  
把所有的subjects, 作为数据, 来拟合出一条直线

# scale
格式为: `scale_element_option`

- element 可以为x,y 与 color, fill, size, shape, linetype, alpha
- option 可以为 continuous, discrete, manual 等

eg:
```
scale_x_continuous
scale_x_discrete
scale_colour_manual(..., values)
```

## 设置边界 limits
- xlim(10, 20): a continuous scale from 10 to 20, 为下面的简写形式
- scale_x_continuous(limits=c(10, 20))
- ylim(20, 10): a reversed continuous scale from 20 to 10
- xlim("a", "b", "c"): a discrete scale
- xlim(as.Date(c("2008-05-01", "2008-08-01"))): a date scale from May 1 to August 1 2008.

## 设置 lab
- xlab("x") 相当于`scale_x_continuous("x")`, 当不想显示xlab时, 需要设置为`NULL`
- 使用expression, scale_y_continuous(expression(votes^alpha))

- labs(title = "title")

## 设置ticks, breaks
- `+ scale_x_continuous(breaks=1:10)`
- `+ scale_x_continuous(breaks=c(1,3,7,9))`
- `+ scale_x_continuous(breaks=c(2,5,8), labels=c("two", "five", "eight"))`
[ex](http://docs.ggplot2.org/current/scale_continuous-24.png)
- `+ scale_x_continuous(breaks=c(2,5,8), labels=expression(Alpha, Beta, Omega))`

NULL: do not display any breaks

## 变换 transformation
scale_x_log10() is equivalent to scale_x_continuous(trans = "log10")

- plot x using scale_x_log(): the axes will be labelled in the original data space, 也就是x
- plot directory log10(x): 坐标轴是用 log10(x)进行标记的
That produces an identical result inside the plotting region, 
but the the axis and tick labels are not the same.

- `+ scale_y_log10()`
- `+ scale_y_sqrt()`
- `+ scale_y_reverse()`
[ex](http://docs.ggplot2.org/current/scale_continuous-34.png)

## manuel
```
p <- qplot(mpg, wt, data = mtcars, colour = factor(cyl))
cols <- c("8" = "red","4" = "blue","6" = "darkgreen", "10" = "orange")
p + scale_colour_manual(values = cols, breaks = c("4", "6", "8"), labels = c("four", "six", "eight"))
```
[figure](http://docs.ggplot2.org/current/scale_manual-14.png)

```
# And limits to control the possible values of the scale
 p + scale_colour_manual(values = cols, limits = c("4", "8"))
```
[figure](http://docs.ggplot2.org/current/scale_manual-16.png)

# stat
A stat takes a dataset as input and returns a dataset as output, and so a stat can add new variables to the original dataset. 
It is possible to map aesthetics to these new variables.  
For example, stat_bin, the statistic used to make histograms, produces the following variables:

- count, the number of observations in each bin
- density, the density of observations in each bin (percentage of total / bar width)
- x, the centre of the bin

```
ggplot(diamonds, aes(carat)) + geom_histogram(aes(y = ..density..), binwidth = 0.1)
```
generated variables must be surrounded with ..  to prevents confusion in case the original dataset includes a variable with the same name.

# position
## facet
[facet grid ref](http://docs.ggplot2.org/current/facet_grid.html)

- facet_grid( . ~ .), 默认的, 不用facet
- facet_grid( . ~ a), 单行多列
- facet_grid( b ~ .): 多行单列
- facet_grid( a ~ b): 多行多列的矩阵
- facet_grid( . ~ a + b) 或者 facet_grid( a + b ~ .): multiple variable in the rows or columns(or both), 有点像 facet_grid( a ~ b)的一维排列效果. 
[. ~ a+b ex](http://docs.ggplot2.org/current/facet_grid-14.png)
[a+b ~. ex](http://docs.ggplot2.org/current/facet_grid-16.png)

In contingency table, it is often usefull to display marginal totals(totals over a row or column), 在facet_grid 中同样适用  
三种方式

1. margins = TRUE: all margins[ex](http://docs.ggplot2.org/current/facet_grid-10.png)
1. list the names of variables that you want margins for, eg: margins = c("sex", "age")
1. grand_row, grand_col

**facet_grid and facet_wrap**

- facet_grid produces a 2d grid of panels, 严格的矩阵样式, 即使有的grid 是空的, 也会显示出来.
- facet_warp( ~ a) or facet_wrap(a ~ b) 产生的是一个1d 的panels, 但是系统会为了节约纸张, 会自己在合适的位置进行换行, 也就是会产生一个2d. 但是不显示没有内容的grid.
注意, 第一个没有那一点

If you have a single variable with many levels and want to arrange the plots in a more space efficient manner, 那么就应当适用facet_wrap.
如果使用facet_grid, 那么plots 会很长, 而不便于在纸上显示出来.

For both types of faceting you can control whether the position scales are the same in all panels (fixed) or allowed to vary between panels (free). 

- scales = "fixed": x and y scales are fixed across all panels.
- scales = "free": x and y scales vary across panels.
- scales = "free_x": the x scale is free, and the y scale is fixed.
- scales = "free_y": the y scale is free, and the x scale is fixed

```
mt <- ggplot(mtcars, aes(mpg, wt, colour = factor(cyl))) + geom_point()
mt + facet_grid(. ~ cyl, scales = "free")
```
[scale free ex](http://docs.ggplot2.org/current/facet_grid-27.png)
从图中可以看到, 4, 6, 8 个grid 的横坐标的标度都不一样, 这就是scale 为 free 的效果.

除了可以对scales 设置为free 外, 还可以对space 设置为free(不同的grid 占用不同的面积)
```
mt + facet_grid(. ~ cyl, scales = "free", space = "free")
```
[space free ex](http://docs.ggplot2.org/current/facet_grid-29.png)

## dodge
```
ggplot(mtcars, aes(x=factor(cyl), fill=factor(vs))) + geom_bar(position="dodge")
```
或者采用position_dodge的方式: `position = position_dodge(width = 0.9)`

facet vs dodge

- `geom_bar(position="dodge")`: 画到一张图上
- ` + facet_grid(. ~ cut)` facet 画到不同的小图上

## coordinate system
coord_flip() x 轴与y轴换着显示, 但是逻辑上x 还是原来的x, y 还是原来的y, 只是在渲染的时候改变了显示效果
[ex](http://docs.ggplot2.org/current/geom_bar-6.png)  
如果要对原来的x 轴进行设置, 仍然使用 scale_x_continuous 等.

`coord_cartesian(ylim=c(1,4)) + coord_flip()`: 后面的coord_flip 会冲掉前面设置的ylim, 因为coord_cartesian and coord_flip are actually the same sort of thing, 
需要将ylim 直接设置在coord_flip 里面`coord_flip(ylim=c(1,4))`

## 仅仅更换数据的快捷方式
You can replace the old dataset with `%+%`
```
p <- ggplot(mtcars, aes(mpg, wt, colour = cyl)) + geom_point()
p
mtcars <- transform(mtcars, mpg = mpg ^ 2)
p %+% mtcars
```

# annotate
[annotate](http://docs.ggplot2.org/current/annotate.html)

p <- ggplot(mtcars, aes(x = wt, y = mpg)) + geom_point()
p + annotate("text", x = 4, y = 25, label = "Some text")

# experssion
`?plotmath`

Output of experssion will be formatted according to TeX-like rule

`+ xlab(expression(frac(miles, gallon)))`

# qplot
```
qplot(x, y, data=, color=, shape=, size=, alpha=, geom=, method=, formula=, facets=, 
		xlim=, ylim= xlab=, ylab=, main=, sub=)
```
**x, y**  
Specifies the variables placed on the horizontal and vertical axis. 
For univariate plots (for example, histograms), omit y

**data**  
Specifies a data frame

**color, shape, size, fill**  
Associates the levels of variable with symbol color, shape, or size.   
For line plots, color associates levels of a variable with line color.   
shape 必须是离散的, color 可以是连续的, 也可以是离散的, size 适合连续量
除了可以自动指定颜色外, 也可以手动指定, eg: `color = I("red"), size = I(2)`
For density and box plots, fill associates fill colors with a variable(用color来填充)., 例如
[ggplotdensity.png](http://www.statmethods.net/advgraphs/images/ggplotdensity.png)  
Legends are drawn automatically.

**alpha**  
transparency for overlapping elements expressed as a fraction between 0 (complete transparency) and 1 (complete opacity)

**facets**  
Creates a trellis(格子) graph by specifying conditioning variables.   
Its value is expressed as rowvar ~ colvar. 
To create trellis graphs based on a single conditioning variable, use rowvar~. or .~colvar)

**geom**  
Specifies the geometric objects that define the graph type.   
The geom option is expressed as a character vector with one or more entries.   
geom values include:

- "point"(散点图), 
- "smooth"(拟合曲线), 
- "path" 和 "line"(数据点之间绘制连线): line 只能创建从左到右的连线, 而path图可以使任意的方向
- "histogram"(直方图)
- "freqpoly"(频率多边形)
- "density"(密度曲线)
- "bar"(条形图)
- "boxplot"(箱线图), 
- "jitter"(扰动点图)

`geom=c('point','smooth')`: 将多个几何对象组成一个向量传递给geom, 几何对象会按照指定的顺序进行堆叠

**main, sub**  
Character vectors specifying the title and subtitle

If the text argument to one of the text-drawing functions (text, mtext, axis, legend) in R is an expression, 
the argument is interpreted as a mathematical expression and the output will be formatted according to TeX-like rules. 
Expressions can also be used for titles, subtitles and x- and y-axis labels (but not for axis labels on persp plots).

**method, formula**  
在图中添加平滑曲线

**xlab, ylab**  
Character vectors specifying horizontal and vertical axis labels

**xlim,ylim**  
Two-element numeric vectors giving the minimum and maximum values for the horizontal and vertical axes, respectively  
eg: `xlim = c(0,15)`

**log**  
一个字符向量, 说明哪一个坐标轴应该取对数.
例如, `log = "x"` 表示对 x 轴取对数, `log = "xy"` 表示对x轴和y轴都取对数

[ggplot2 demo](../demo/r/ggplot2.r)

[ggplot2 tutoraial](https://github.com/echen/ggplot2-tutorial/blob/master/README.md)

[Graphics with ggplot2](http://www.statmethods.net/advgraphs/ggplot2.html)

## 平滑曲线
If geom="smooth", a **loess fit line** and **confidence limits** are added by default.   
se = FALSE 可以去掉 confidence limits  
曲线的平滑程度由span参数控制, 0(很不平滑) 到 1(很平滑)  
当n较小时, loess是默认选项, 但是loess复杂度是O(n^2), 对于大数据不适用.
When the number of observations is greater than 1,000, a more efficient smoothing algorithm is employed. 

Methods include "lm" for regression, "gam" for generalized additive models, and "rlm" for robust regression. 
The formula parameter gives the form of the fit.   
For example, to add simple linear regression lines, you would specify `geom="smooth", method="lm", formula=y~x`. 
Changing the formula to `y~poly(x,2)` would produce a quadratic fit.   
Note that the formula uses the letters x and y, not the names of the variables.   
For `method="gam"`, be sure to load the mgcv package. For `method="rml"`, load the MASS package.

```
library(ggplot2)
head(diamonds)
##   carat       cut color clarity depth table price    x    y    z
## 1  0.23     Ideal     E     SI2  61.5    55   326 3.95 3.98 2.43
## 2  0.21   Premium     E     SI1  59.8    61   326 3.89 3.84 2.31
## 3  0.23      Good     E     VS1  56.9    65   327 4.05 4.07 2.31
## 4  0.29   Premium     I     VS2  62.4    58   334 4.20 4.23 2.63
## 5  0.31      Good     J     SI2  63.3    58   335 4.34 4.35 2.75
## 6  0.24 Very Good     J    VVS2  62.8    57   336 3.94 3.96 2.48

nrow(diamonds)
## [1] 53940

set.seed(1410)
dsmall <- diamonds[sample(nrow(diamonds),100),]

qplot(carat, price, data = dsmall, geom=c('point','smooth'))
qplot(carat, price, data = dsmall, geom=c('point','smooth'),se=FALSE)

qplot(carat, price, data = dsmall, geom=c('point','smooth'),se=FALSE)

library(mgcv)
qplot(carat, price, data = dsmall, geom=c('point','smooth'), method = 'gam', formula = y ~ s(x))
qplot(carat, price, data = dsmall, geom=c('point','smooth'), method = 'gam', formula = y ~ s(x, bs = "cs"))
## method = 'gam', formula = y ~ s(x, bs = "cs") 是数据量超过1000时默认的选项
```

## 箱线图与扰动点图
一个分类变量和一个或多个连续变量, 想知道连续变量会如何随着分类变量的变化而变化

箱线图  
用了5个数字对分布进行概括
```
qplot(color, price/carat, data=diamonds, geom = "boxplot")
```
![boxplot](http://i.imgbox.com/kWos4O2E.png)

扰动点图  
```
qplot(color, price/carat, data=diamonds, geom = "jitter")
```
![jitter](http://i.imgbox.com/GLkp5p6R.png)

扰动图可以像散点图那样对点的属性(大小, 颜色, 形状等)进行设置

```
qplot(color, price/carat, data=diamonds, geom = "jitter", alpha=I(0.1))
```
![jitter alpha](http://i.imgbox.com/QajYxjdO.png)


## 直方图与密度曲线图
直方图  
binwidth: 设定组距来调整平滑度  
breaks:切分位置也可以通过breaks 显示的设定  
在直方图中, 当组距较大时, 图形能反应数据的总体特征; 当组距较小时, 能能显示出更多的细节.

绘制直方图和密度曲线对平滑程度进行实验非常重要.

## 条形图

```
qplot(color, data = diamonds, geom = "bar")
nrow(diamonds[diamonds$color == "J",])
## [1] 2808
nrow(diamonds[diamonds$color == "F",])
## [1] 9542
```
![bar](http://i.imgbox.com/Ks90NwGZ.png)

使用weight, 对连续变量进行分组求和, 这里是将carat 对color分组求和
```
qplot(color, data = diamonds, geom = "bar", weight= carat)
sum(diamonds$carat[diamonds$color == "H"])
## [1] 7571.58
```
![bar weight](http://i.imgbox.com/miTsdwvy.png)

## 时间序列中的线条图和路径图

- 线条图: 将点从左到右进行连接
- 路线图: 按照点在数据集中的顺序对其进行连接

线条图的x轴一般是时间, 它展示了单个变量随时间变化的情况.  
路径图则展示了两个变量随时间联动的情况, 时间反映在点的顺序上

```
qplot(date, unemploy / pop, data = economics, geom = "line")
```

可以将两个时间序列花在同一张图中, 又尽管我们可以用散点图来表示失业率和失业时间长度之间的关系, 但是我们并不能看出变量随时间变化的情况.
对此, 解决的办法是将临近时点的散点连接起来, 形成一张**路径图**
```
year <- function(x){as.POSIXlt(x)$year + 1900}
qplot(unemploy / pop, uempmed, data = economics, geom = c("path"), color = year(date))
```

## 将多图绘制到一页
ggplot2在生成一页多图方面,有个facet,分面的命令,可以自动根据分组,每组对应一幅图出来.
但是,如果是不同类型的图,想拼到一起,就比较麻烦了.
在这里,Hiu给大家提供两种方法.

第一种,使用grid包的viewport功能.
```
library(ggplot2)
#generate two ggplot2 objects.
a <- qplot(TMPRSS4, ECADHERIN, data=spear)
b <- qplot(TMPRSS4, ECADHERIN, data=spear, geom="jitter")

library(grid)
#generate a new page, or erase the current one.
grid.newpage()
#maintain a Viewport tree, which describe the regions and coordinate systems.
pushViewport(viewport(layout=grid.layout(1,2)))
#print them into the Viewport, or the page.
print(a, vp=viewport(layout.pos.row=1,layout.pos.col=1))
print(b, vp=viewport(layout.pos.row=1,layout.pos.col=2))
```

第二种,使用一个新包,叫做gridExtra.
```
library(gridExtra)
a <- qplot(TMPRSS4, ECADHERIN, data=spear)
b <- qplot(TMPRSS4, ECADHERIN, data=spear, geom="jitter")
grid.arrange(a,b,ncol=2)
```
于是,我们就得到了二合一的图片.
当然你也可以按照你的意愿随意控制图的位置.

Customizing ggplot2 Graphs
```
library(ggplot2)
p <- qplot(hp, mpg, data=mtcars, shape=am, color=am, facets=gear~cyl, 
	main="Scatterplots of MPG vs. Horsepower", xlab="Horsepower", ylab="Miles per Gallon")

# White background and black grid lines
p <- p + theme_bw()

# Large brown bold italics labels and legend placed at top of plot
p <- p + theme(axis.title=element_text(face="bold.italic", size="12", color="brown"), legend.position="top")
```


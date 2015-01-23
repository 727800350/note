ggplot2:elegant graphics for data analysis 一书是ggplot软件包2的系统介绍，包括其语法结构的详细介绍以及绘图实例，也包括了plyr、reshape等包的强大的数据处理能力，是数据分析和统计图形的优秀著作.

R的基础图形系统基本上是一个“纸笔模型”，即：一块画布摆在面前，你可以在这里画几个点，在那里画几条线，指哪儿画哪儿。后来lattice包的出现稍微改善了这种情况，你可以说，我要画散点图或直方图，并且按照某个分类变量给图中的元素上色，此时数据才在画图中扮演了一定的中心角色，我们不用去想具体这个点要用什么颜色（颜色会根据变量自动生成）。然而，lattice继承了R语言的一个糟糕特征，就是参数设置铺天盖地，足以让人窒息，光是一份xyplot()函数的帮助文档，恐怕就够我们消磨一天时间了，更重要的是，lattice仍然面向特定的统计图形，像基础图形系统一样，有直方图、箱线图、条形图等等，它没有一套可以让数据分析者说话的语法。
那么数据分析者是怎样说话的呢？他们从来不会说这条线用#FE09BE颜色，那个点用三角形状，他们只会说，把图中的线用数据中的职业类型变量上色，或图中点的形状对应性别变量。有时候他们画了一幅散点图，但马上他们发现这幅图太拥挤，最好是能具体看一下里面不同收入阶层的特征，所以他们会说，把这幅图拆成七幅小图，每幅图对应一个收入阶层。然后发现散点图的趋势不明显，最好加上回归直线，看看回归模型反映的趋势是什么，或者发现图中离群点太多，最好做一下对数变换，减少大数值对图形的主导性。
从始至终，数据分析者都在数据层面上思考问题，而不是拿着水彩笔和调色板在那里一笔一划作图，而计算机程序员则倾向于画点画线。Leland Wilkinson的著作在理论上改善了这种状况，他提出了一套图形语法，让我们在考虑如何构建一幅图形的时候不再陷在具体的图形元素里面，而是把图形拆分为一些互相独立并且可以自由组合的成分。这套语法提出来之后他自己也做了一套软件，但显然这套软件没有被广泛采用；幸运的是，Hadley Wickham在R语言中把这套想法巧妙地实现了。
为了说明这种语法的想法，我们考虑图形中的一个成分：坐标系。常见的坐标系有两种：笛卡尔坐标系和极坐标系。在语法中，它们属于一个成分，可自由拆卸替换。笛卡尔坐标系下的条形图实际上可以对应极坐标系下的饼图，因为条形图的高可以对应饼图的角度，本质上没什么区别。因此在ggplot2中，从一幅条形图过渡到饼图，只需要加极少量的代码，把坐标系换一下就可以了。如果我们用纸笔模型，则可以想象，这完全是不同的两幅图，一幅图里面要画的是矩形，另一幅图要画扇形。

```
qplot(x, y, data=, color=, shape=, size=, alpha=, geom=, method=, formula=, facets=, xlim=, ylim= xlab=, ylab=, main=, sub=)
```
- x, y	Specifies the variables placed on the horizontal and vertical axis. 
For univariate plots (for example, histograms), omit y

- data	Specifies a data frame

- color, shape, size, fill	Associates the levels of variable with symbol color, shape, or size.   
For line plots, color associates levels of a variable with line color.   
shape 必须是离散的, color 可以是连续的, 也可以是离散的
For density and box plots, fill associates fill colors with a variable(用color来填充)., 例如[ggplotdensity.png](http://www.statmethods.net/advgraphs/images/ggplotdensity.png)  
Legends are drawn automatically.

- alpha	Alpha transparency for overlapping elements expressed as a fraction between 0 (complete transparency) and 1 (complete opacity)

- facets	Creates a trellis(格子) graph by specifying conditioning variables.   
Its value is expressed as rowvar ~ colvar. 
To create trellis graphs based on a single conditioning variable, use rowvar~. or .~colvar)

- geom	Specifies the geometric objects that define the graph type.   
The geom option is expressed as a character vector with one or more entries. geom values include "point", "smooth", "boxplot", "line", "histogram", "density", "bar", and "jitter".

- main, sub	Character vectors specifying the title and subtitle

- method, formula	If geom="smooth", a loess fit line and confidence limits are added by default. 
When the number of observations is greater than 1,000, a more efficient smoothing algorithm is employed. 
Methods include "lm" for regression, "gam" for generalized additive models, and "rlm" for robust regression. 
The formula parameter gives the form of the fit. 
For example, to add simple linear regression lines, you would specify geom="smooth", method="lm", formula=y~x. Changing the formula to y~poly(x,2) would produce a quadratic fit. Note that the formula uses the letters x and y, not the names of the variables. 
For method="gam", be sure to load the mgcv package. For method="rml", load the MASS package.

- xlab, ylab	Character vectors specifying horizontal and vertical axis labels

- xlim,ylim	Two-element numeric vectors giving the minimum and maximum values for the horizontal and vertical axes, respectively

[ggplot2 demo](../../demo/r/ggplot2.r)

[ggplot2 tutoraial](https://github.com/echen/ggplot2-tutorial/blob/master/README.md)
[Graphics with ggplot2](http://www.statmethods.net/advgraphs/ggplot2.html)

Customizing ggplot2 Graphs
```
library(ggplot2)
p <- qplot(hp, mpg, data=mtcars, shape=am, color=am, facets=gear~cyl, main="Scatterplots of MPG vs. Horsepower", xlab="Horsepower", ylab="Miles per Gallon")

# White background and black grid lines
p <- p + theme_bw()

# Large brown bold italics labels and legend placed at top of plot
p <- p + theme(axis.title=element_text(face="bold.italic", size="12", color="brown"), legend.position="top")
```

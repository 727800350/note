# basic plots
## curve
[example](./curve.py)

plt.plot(x, y)

The marker style is also accessible for pyplot.plot() using the same marker parameter.
Using one marker for each data point can be a problem as it will display more points than we want to.
The markevery parameter allows you to display only one marker for every N points.

## scatter point
[example](./scatter.py)
plt.scatter(x, y)

### color in scatter
pyplot.scatter() offers two options to control the colors of dots

- Common color for all the dots: all the dots will appear in that color.
- Individual color for each dot: If the color parameter is a sequence of colors, the ith dot will appear in the ith color.

具体是通过下面的两个变量

- color parameter controls the color of the dots
- edgecolor parameter controls the color of the edge of the dots.

## bar chart
[example](./bar.py)
plt.bar(x, y, width = 0.8)
x 为各个bar的起始x坐标, 也就是左边界的位置
y 为各个bar的高度
By default, a bar will have a thickness of 0.8 units. 这样在各个bar之间就会有0.2 units的间隔

plt.barh(x, y)
横着的bar

## pie chart
[example](./pie.py)

plt.pie(x)

The pyplot.pie() function simply takes a list of values as the input.
Note that the input data is a list; it could be a NumPy array.
You do not have to adjust the data so that it adds up to 1 or 100.
You just have to give values to matplolib and it will automatically compute the relative areas of the pie chart.

## histogram
[example](./hist.py)

plt.hist(x, bins = 10)
The pyplot.hist() function will generate a bar chart, one bar for one bin.
The height of one bar is the number of values following in the corresponding bin.
The number of bins is determined by the optional parameter bins. The range of the values will be divided into equal-sized bins (10 bins by default).
By setting the optional parameter normed to True , the bar height is normalized and the sum of all bar heights is equal to 1

## boxplot
[example](./boxplot.py)

plt.boxplot(x)
Boxplot allows you to compare distributions of values by conveniently showing the median, quartiles, maximum, and minimum of a set of values.

- The red bar is the median of the distribution.
- The blue box includes 50 percent of the data from the lower quartile to the upper quartile. Thus, the box is centered on the median of the data.
- The lower whisker extends to the lowest value within 1.5 IQR from the lower quartile.
- The upper whisker extends to the highest value within 1.5 IQR from the upper quartile.
- Values further from the whiskers are shown with a cross marker.

## triangulation
[example](./triplot.py)
Triangulations arise when dealing with spatial locations.
Apart from showing distances between points and neighborhood relationships, triangulation plots can be a convenient way to represent maps.

# Customizing the Color and Styles
## color, c
- Triplets: RGB
- Quadruplets: These work as triplets, and the fourth component defines a transparency value(also be in the [0, 1] interval).
- Predefined names: "red" will be interpreted as a bright red. A few colors have a one-letter alias
	- b Blue
	- g Green
	- r Red
	- c Cyan
	- m Magenta
	- y Yellow
	- k Black
	- w White
- Gray-level strings: matplotlib will interpret a string representation of a floating point value as a shade of gray, such as 0.75 for a medium light gray.

## line styles, ls
The linestyle parameter is available for all the commands that involve line rendering.

- Solid
- Dashed
- Dotted
- Dashdot

## line width, lw
Likewise, the linewidth parameter will change the thickness of lines.
By default, the thickness is set to 1 unit.

## fill pattern
Rendering function filling volumes, such as pyplot.bar(), accept an optional parameter, hatch. 
This parameter can take the following values:

- /
- \
- |
- -
- +
- x
- o
- O
- .
- *

## marker style
Markers can be specified in various ways as follows:

- Predefined markers: They can be predefined shapes, represented as a number in the [0, 8] range, or some strings
- Vertices list: This is a list of value pairs, used as coordinates for the path of a shape
- Regular polygon: It represents a triplet (N, 0, angle) for an N sided regular polygon, with a rotation of angle degrees
- Start polygon: It represents a triplet (N, 1, angle) for an N sided regular star, with a rotation of angle degrees

通过变量s可以控制marker的大小

# Annotations
LaTeX-notation feature can be used for any annotation.

The LaTeX language relies heavily on the escape character, \ , which also happens to be the string escape character for Python.
Thus, where you would use one \ character in a LaTeX text, put two in your Python string.
To avoid fumbling with escape characters, you can prefix your string with r and you will not need any escape characters.
Thus, '$f(x)=\\frac{1}{4} (x+4)(x+1)(x-2)$' and r'$f(x)=\frac{1}{4}(x+4)(x+1)(x-2)$' are equivalent.

- plt.title()
- plt.xlabel()
- plt.ylabel()
- plt.legend(): 需要在plt.plot(lable = 'str'), 其他的plot也一样
	- 'loc': location 'best'(default), 'upper left', 'lower left', 'lower right', 'right', 'center left', 'center right', 'lower center', 'upper center', 'center'
	- 'shadow': either True or False, and it renders the legend with a shadow effect.
	- 'fancybox': either True or False and renders the legend with a rounded box.
	- 'title': This renders the legend with the title passed as a parameter.
	- 'ncol': This forces the passed value to be the number of columns for the legend.
- plt.grid(True, lw = 2, ls = '--', c = '.75')
- plt.text(x, y, str)
	- The position is given in the graphic coordinates, specifying the position of the left border and the vertical baseline of the text.
- plt.ylim(-.5, 1.5)

## add line
```
N = 16
for i in range(N):
	plt.gca().add_line(plt.Line2D((0, i), (N - i, 0), color = '.75'))
plt.grid(True)
plt.axis('scaled')
plt.show()
```
- pyplot.Line2D(): function creates a new Line2D object.
- pyplot.gca().add_line(): render the line object
- The pyplot.axis('scaled') function is required to ensure that the figure uses a uniform scale: the same scale as the one used on the x and y axes.

## add shapes
```
import matplotlib.patches as patches
import matplotlib.pyplot as plt

# Circle
shape = patches.Circle((0, 0), radius = 1., color = '.75')
plt.gca().add_patch(shape)

# Rectangle
shape = patches.Rectangle((2.5, -.5), 2., 1., color = '.75')
plt.gca().add_patch(shape)

# Ellipse
shape = patches.Ellipse((0, -2.), 2., 1., angle = 45., color = '.75')
plt.gca().add_patch(shape)

# Fancy box
shape = patches.FancyBboxPatch((2.5, -2.5), 2., 1., boxstyle = 'sawtooth', color = '.75')
plt.gca().add_patch(shape)

# Display all
plt.grid(True)
plt.axis('scaled')
plt.show()
```

- Circle: This takes the coordinates of its center and the radius as the parameters
- Rectangle: This takes the coordinates of its lower-left corner and its size as the parameters
- Ellipse: This takes the coordinates of its center and the half-length of its two axes as the parameters
- FancyBox: like a rectangle but takes an additional boxstyle parameter (either 'larrow' , 'rarrow' , 'round' , 'round4' , 'roundtooth' , 'sawtooth' , or 'square' )

# Working with Figures
Compositing multiple figures
[example](./subplot2grid.py)

pyplot.subplot2grid(grid, coordinate, [rowspan], [colspan])

- grid: the number of rows and columns of the grid, passed as a tuple. If we want a grid of R rows and C columns, we would pass (R, C).
- coordinate: coordinate in the grid, in rows and columns, also passed as a tuple.
- rowspan: defines how many rows the figure will span.
- colspan: defines how many columns the figure will span.

By default, matplotlib 对于x, y轴会使用不一样的标度.
通过`plt.axes().set_aspect('equal')` 可以强制设置一样的标度.
The pyplot.axes() function returns an instance of the Axes object, the object in charge of the axes.
[example](./axe_scale.py)

Setting the aspect ratio
应该是调整图片的大小 figure size
plt.figure(figsize=(10.24, 2.56))
默认为figsize=(8, 6)

Inserting subfigures
create a subregion on the figure as follows: `sub_axes = plt.axes([.6, .6, .25, .25])`
The region is in figure-wise coordinates; that is, (0, 0) is the bottom-left corner and (1, 1) is the top-right corner of the overall figure.
The subregion is defined by four values: the coordinates of the bottom-left corner of the region and its dimensions.
Once the subregion is defined, we have an Axes instance in which we create a figure.
Then, we need to call pyplot.setp() on our Axes instance as follows: `plt.setp(sub_axes)`

- axe.set_xlabel()
- axe.set_ylabel()
- axe.set_title()

plt.xscale('log')
plt.yscale('log')
[example](./log_scale.py)

突出细节, 又能观察全局
Using a logarithmic scale can also be useful to zoom in on one small range over data with a very large range.
```
X = np.linspace(-100, 100, 4096)
plt.xscale('symlog', linthreshx=6.)
plt.plot(X, np.sinc(X), c = 'k')
plt.show()
```
The central part of the curve (the [-6, 6] range) is shown with a linear scale, while the other parts are shown with a logarithmic scale.

polaire
`plt.axes(polar = True)`
[example](./polaire.py)

## 保存图片
plt.savefig('sinc.png')
支持PNG, EPS, SVG, and PDF, 可以根据后缀自己选择相应的引擎.
transparent = True: 背景透明
dpi = 300: (figsize 为默认的8,6时)2400 x 1800 pixels, 默认情况下为100dpi,  800 x 600 pixels

By default, matplotlib will output a figure of 8 x 6 spatial units—a 4/3 aspect ratio.
In matplotlib, 1 spatial unit equals to 100 pixels.
Thus, by default, matplotlib will give a picture file of 800 x 600 pixels.
If we use dpi = 300, the picture size will be 8 * 300 x 6 * 300, that is, 2400 x 1800 pixels.


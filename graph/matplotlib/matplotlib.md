point
plt.scatter(x, y)

bar chart
[example](./bar.py)
plt.bar(x, y)
x 为各个bar的起始x坐标, 也就是左边界的位置
y 为各个bar的高度
By default, a bar will have a thickness of 0.8 units. 这样在各个bar之间就会有0.2 units的间隔

plt.barh(x, y)
横着的bar

pie chart
[example](./pie.py)

The pyplot.pie() function simply takes a list of values as the input.
Note that the input data is a list; it could be a NumPy array.
You do not have to adjust the data so that it adds up to 1 or 100.
You just have to give values to matplolib and it will automatically compute the relative areas of the pie chart.

histogram
[example](./hist.py)
The pyplot.hist() function will generate a bar chart, one bar for one bin.
The height of one bar is the number of values following in the corresponding bin.
The number of bins is determined by the optional parameter bins. The range of the values will be divided into equal-sized bins (10 bins by default).
By setting the optional parameter normed to True , the bar height is normalized and the sum of all bar heights is equal to 1

boxplot
[example](./boxplot.py)
Boxplot allows you to compare distributions of values by conveniently showing the median, quartiles, maximum, and minimum of a set of values.

- The red bar is the median of the distribution.
- The blue box includes 50 percent of the data from the lower quartile to the upper quartile. Thus, the box is centered on the median of the data.
- The lower whisker extends to the lowest value within 1.5 IQR from the lower quartile.
- The upper whisker extends to the highest value within 1.5 IQR from the upper quartile.
- Values further from the whiskers are shown with a cross marker.

triangulation
[example](./triplot.py)
Triangulations arise when dealing with spatial locations.
Apart from showing distances between points and neighborhood relationships, triangulation plots can be a convenient way to represent maps.


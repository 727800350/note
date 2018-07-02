numpy can perform operations on whole arrays at once. 就像Matlab一样,因此可以避免显式的循环.

array 的属性

- ndarray.ndim: number of dimensions
- ndarray.shape: the dimensions of the array, a tuple of integers 
- ndarray.size: the total number of elements of the array. This is equal to the product of the elements of shape.
- ndarray.dtype: an object describing the type of the elements in the array. numpy.int32, numpy.int16 and numpy.float64 are some examples.
- ndarray.itemsize: the size in bytes of each element of the array, equivalent to ndarray.dtype.itemsize.
- ndarray.data: the buffer containing the actual elements of the array.

array 的创建

- np.array([1, 2, 3])
- np.zeros(3), np.ones([3,2]), np.empty([2,2]), empty 的元素值是随机的
- np.zeros_like([1,2,3]), np.ones_like([[1,2,3], [2,3,4]]), empty_like()
- np.arange(x) 相当于 np.array(range(x))
- mp.linspace(start, stop, num=50, endpoint=True): endpoint(true) 表示最后一个点为stop
- np.fromfunction(): Construct an array by executing a function over each coordinate, therefore has a value `fn(x, y, z)` at coordinate `(x, y, z)`

numpy.random

- np.random.shuffle(): 随机化
- np.random.permutation(x): 返回x shuffle 的结果; 若x 是整数, 则相当于操作range(x)
- np.random.randn(n, m): 返回一个 `n * m` 的矩阵, 元素都是标准正态分布的结果
- np.random.randint(low, high=None, size=None): [low, high), size 可以是整数也可以是一个shape, 比如(3, 4) 表示一个矩阵

array 的操作

- np.argmax(x): Returns the indices of the maximum values along an axis. 也可以使用 x.argmax()
- np.reshape(x, shape): Gives a new shape to an array without changing its data, 例如把[3, 4] 的shape变为[2, 6]的
- np.dot(x, y): dot product
- np.transpose(x): 矩阵转置
- `x[:,1]`: 取第二列
- `x[x>1]`: 取所有大于1的值
- `x.cumsum()`: 累积和, axis = 0 or 1 控制横纵方向
- np.copy(x): makes a complete copy of the array and its data.
	```python
	>>> x = np.array([1, 2, 3])
	>>> y = x
	>>> z = np.copy(x)
	```
	Note that, when we modify x, y changes, but not z:

- `np.loadtxt(fname, dtype=<type 'float'>, delimiter=None, converters=None, skiprows=0)`
	- delimiter : str, optional. By default, this is any whitespace.
	- converters : optional, A dictionary mapping column number to a function that will convert that column to a float. 
		E.g. if column 0 is a date string: converters = {0: datestr2num}.
		Converters can also be used to provide a default value for missing data, converters = {3: lambda s:float(s.strip() or 0)}. Default: None.
		```python
		XR21 32.789 1
		XR22 33.091 2
		table = np.loadtxt('example.txt',dtype={'names':('ID', 'Result', 'Type'),'formats': ('S4', 'f4', 'i2')})
		# [('XR21', 32.78900146484375, 1) ('XR22', 33.090999603271484, 2)]
		```
	- for row in data, data 为 numpy array
	- for column in data.T
- np.vstack(tup): Stack arrays in sequence vertically (row wise), 也就是上下放置

boolean
when using NumPy arrays, you can only use & and | as this allows fast comparisons of boolean values.

broadcasting

- 一个 (m, n) 的matrix 和 一个 (1, n) 的row vector 进行 `+ - * /`, row vector 会被copy m 份, 也变成一个 (m, n) 的matrix, 然后进行对应位的四则运算
- 一个 (m, n) 的matrix 和 一个 (m, 1) 的column vector 进行 `+ - * /`, column vector 会被copy n 份, 也变成一个 (m, n) 的matrix, 然后进行对应位的四则运算


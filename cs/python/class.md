python 中类里的属性变量以:

- `_xxx`: 单下划线开头, protected, 意思是受保护的, 只能在本类以及子类中访问
- `__xxx`: 双下划线, private, 只能本类中访问, 连子类都无法访问
- 没有下划线的是public


类拥有两种操作
1. 类属性 attribute references 
类属性就相当于专属于一个类的变量(即某些语言中的类的静态公共变量static public),使用方法是: 类名称.类属性名称
2. 实例化instantiation
实例化则是创建一个类的实例的方法,使用方法是:类名称()
在使用实例化方法后,将会创建一个空的类实例,一般的python类的定义中会有一个特殊的方法来初始化, 
这个方法就是`__init__()`,当调用了类的实例化方法后,`__init__()`方法会立刻被这个类的实例调用.也就是说,`__init__()`不是构造函数,而是一个普通的方法.

class 中特殊方法

- `__init__` initialize an instance  每边都是两个下划线
- `__str__` print an instance out
- `__cmp__` compare the instances
- `_init__(self,...)` 这个方法在新建对象恰好要被返回使用之前被调用.
- `__del__(self)` 恰好在对象要被删除之前调用.
- `__str__(self)` 在我们对对象使用print语句或是使用str()的时候调用.
- `__lt__(self,other)` 当使用 小于 运算符(<)的时候调用.类似地,对于所有的运算符(+,>等等)都有特殊的方法.
- `__getitem__(self,index)` 使用x[index]索引操作符的时候调用)
- `__len__(self)` 对序列对象使用内建的len()函数的时候调用

Note: Remember that a double underscore (`__`) around a method name indicates that the method has special meaning to Python, 
even though it can be used and overridden like any other method. 
Python has some special conventions for calling such methods; for example, `len(x)` makes Python call `x.__len__()`.


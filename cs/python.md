- [data types](#data-types)
  - [mutable vs immutable](#mutable-vs-immutable)
  - [list](#list)
    - [列表推导式](#列表推导式)
  - [dict](#dict)
  - [set](#set)
  - [heapq](#heapq)
  - [Numeric](#numeric)
  - [string](#string)
    - [原始字符串](#原始字符串)
- [FP](#fp)
- [反射(自省)](#反射自省)
  - [访问对象的元数据](#访问对象的元数据)
- [多线程](#多线程)

```plain
help, dir, type, id, hasattr, getattr, callable, isinstance
```
在python终端输入help()进入帮助终端, 然后输入"topic" 进行查询, 之后输入quit退出help

常用自省函式有

- help , help("topic")
- dir 列出对象的所有属性,
- type 返回对象的类型,
- id 返回对象的"唯一序号"
- hasattr( ) 和 getattr( ) 分别判断对象是否有某个属性及获得某个属性值
- hasattr (object, "split" )
- callable() 判断对象是否可以被调用
- isinstance() 可以确认某个变量是否有某种类型 isinstance(a, str)

`sys.argv` 获取参数列表, `len(sys.argv)`

```python
def fun(x):
  global a # 说明a是全局变量,可以直接使用, 需要确保在使用之前a 确实是存在的一个全局变量
  return a + x
```

# data types
- **list and dictionary is mutable**
- two non-scalar type: **tuple and string are immutable**

## mutable vs immutable
lists and dictionaries are mutable, meaning you can change their content without changing their identity.
Other objects like integers, floats, strings and tuples ... are objects that can not be changed.

在使用`for x in seq`结构循环中, 不能改变seq, 否则会出现不可预知的问题.

## list
list 不是线程安全的, dequeue 是线程安全的

要注意列表中的列表不会被打散,如 `a=[1,2], b=[a,3,4]`,结果就是 `b=[[1,2],3,4]`

列表赋值是引用赋值, 要想拷贝, 必须用切片:`b = a[:]`

list是对所指向对象的一个引用
list2 = list1; 那么这两个list指向的是同一个list对象, 当我们改变一个中的元素的时候, 另一个也随之改变

- list.append(x)
- list.extend(L): 将list L中所有member 追加到list中
- list.insert(i, x)
- list.remove(x)
- list.pop(i)
- list.index(x): Return the index in the list of the first item whose value is x. It is an error if there is no such item.
- list.count(x): Return the number of times x appears in the list.
- list.reverse(): Reverse the elements of the list, in place. 会改变原列表.

list 的排序

- list.sort(): Sort the items of the list, in place. 会改变原列表. 还可以加参数 list.sort(reverse=True)
- sorted(list): 返回一个排好序的对象, 原来的list不会改变

[list sort](http://www.educity.cn/wenda/353925.html)
```python
L = [('b',6),('a',1),('c',3),('d',4)]
L.sort(lambda x,y:cmp(x[1],y[1]))
L.sort(key=lambda x:x[1])
import operator; L.sort(key=operator.itemgetter(1))
L.sort(key=lambda x:(x[1],x[0])): 多关键字排序, 先用第二列的数字, 然后再用第一列的字母
```

### 列表推导式
```python
>>>vec = [2, 4, 6] 
>>>[3*x for x in vec if x>3]  # 大于 3 的元素乘上 3 作为新列表元素 
[12, 18] 

>>>[(x, x**2) for x in vec] # 循环变量要是一个 sequence, 而[ x, x* * 2 f or x i n vec] 是错误的 
[(2, 4), (4, 16), (6, 36)] 

>>>vec2 = [4, 3, - 9] 
>>>[x*y for x in vec for y in vec2] # vec 与 vec2 元素相乘 
[8, 6, -18, 16, 12, -36, 24, 18, -54] 
```

## dict
定义方式为 `c={'a':1, 'b':2}`

定义和访问时,key必须用引号引起来. 使用{}定义,使用[]访问,即`c['a']`

```python
for key in dictonary.keys():
  do something
```
[for key in dictionary 会出错](http://www.cnblogs.com/codeape/archive/2012/11/21/2780534.html)

对字典key 的要求:  
dictionaries are indexed by keys, which can be any immutable type; strings and numbers can always be keys.

- `key in d`: Return True if d has a key key, else False.
- `key not in d`: Equivalent to not key in d
- `len(d)`
- `get(key[, default])`: Return the value for key if key is in the dictionary, else default.
  If default is not given, it defaults to None, so that this method **never raises a KeyError**.
- `pop(key[, default])`
- `del d[key]`: Raises a KeyError if key is not in the map

## set
集合是无序的,不重复的元素集,类似数学中的集合,可进行逻辑运算和算术运算.
[set demo](../demo/python/set.py)

## heapq
[heapq](https://docs.python.org/2/library/heapq.html)

需要 `import heapq`

- heapq.heappush(heap, item): item 可以是一个tuple, eg: (key, value)
- heapq.heappop(heap) 弹出最小元素, 同时这个弹出的元素会从heap中被删除
- heapq.heappushpop(heap, item): The combined action runs more efficiently than heappush() followed by a separate call to heappop().
- heapq.heapify(x) Transform list x into a heap, in-place, in linear time.
- heapq.heapreplace(heap, item): Pop and return the smallest item from the heap, and also push the new item. more efficient than a heappop() followed by heappush()
- heapq.nlargest(n, iterable[, key]) Return a list with the n largest elements from the dataset defined by iterable.
- heapq.nsmallest(n, iterable[, key])

heapq 底层使用的就是一个list, 所以初始化, 清空都使用`heap = []`, 元素个数用 `len(heap)`

## Numeric
- `int(String)`: convert a string to integer
- `float`: 将字符串或者整数转换为浮点数, 百分号是不能识别的
- `chr`: 将ascii值转换为ascii字符
- `hex`: 将整数转换为十六进制的字符串
- `long`: 转化为长整型
- `oct`: 将整数转化为八进制的字符串
- `ord`: 将ascii字符转化为ascii值
- `str(object)`  转化为字符串
- `len(object)` 长度
- `divmod(x, y)` which returns a tuple `(x / y, x % y)`
- `round(2.675, 2)`: 小数位数
- `math.floor()`
- `math.ceil()`

## string
**以下函数并不改变字符串本身, 而是返回修改后的新的字符串 string immutable**.
一旦声明了一个字符串, 则该字符串中的每个字符都有了自己固定的位置,可以使用`[index]`来访问.
python还允许以负数来访问字符串中字符,负数表示从字符串的尾部开始计算,此时最后一个字符的序号为-1.

字符

- chr(num): 将num 转换为对应的字符;
- ord(c): c 语言中可以直接使用数字来表示char, 但是python 不行, 与chr 相对的一个函数为 ord(c)

**计数**
- `str.count(sub[, start[, end]])`
Return the number of non-overlapping occurrences of substring sub in the range [start, end]. Optional arguments start and end are interpreted as in slice notation.

[编码](http://in355hz.iteye.com/blog/1860787)
因为 Python 认为 16 位的 unicode 才是字符的唯一内码,而大家常用的字符集如 gb2312,gb18030/gbk,utf-8,以及 ascii 都是字符的二进制(字节)编码形式.
把字符从 unicode 转换成二进制编码,当然是要 encode
[encoding demo](../demo/python/encoding.py)

- `str.decode([encoding[, errors]])`: Decodes the string using the codec registered for encoding.
- `str.encode([encoding[, errors]])`: Return an encoded version of the string.
eg: '我'.decode('utf8').encode('gbk')

**查找**
- `str.startswith(prefix)`: Return True if the string ends with the specified suffix, otherwise return False.
- `str.endswith(suffix)`: Return True if the string ends with the specified suffix, otherwise return False.
- `str.find(sub[, start[, end]])`: Return the lowest index in the string where substring sub is found, 否则返回-1.

**boolean**
- `str.isalnum()` Return true if all characters in the string are alphanumeric and there is at least one character, false otherwise.
- `str.isalpha()` Return true if all characters in the string are alphabetic and there is at least one character, false otherwise.
- `str.isdigit()` Return true if all characters in the string are digits and there is at least one character, false otherwise.
- `str.islower()` Return true if all cased characters [4] in the string are lowercase and there is at least one cased character, false otherwise.
- `str.isspace()` Return true if there are only whitespace characters in the string and there is at least one character, false otherwise.
- `str.isupper()` Return true if all cased characters [4] in the string are uppercase and there is at least one cased character, false otherwise.

**大小写**
- `str.capitalize()` Return a copy of the string with its first character capitalized and the rest lowercased.
- `str.lower()` Return a copy of the string with all the cased characters [4] converted to lowercase.
- `str.upper()`

**分隔合并**
- `str.strip()`: 去掉两边的空字符(包括末尾的换行符)
- `str.lstrip([chars])` Return a copy of the string with leading characters removed. The chars argument is a string specifying the set of characters to be removed.

- `str.replace(old, new[, count])`: Return a copy of the string with all occurrences of substring old replaced by new.
  If the optional argument count is given, only the firstcount occurrences are replaced.

- `str.split([sep[, maxsplit]])`: Return a list of the words in the string, using sep as the delimiter string. 但是split只能实现一次采用一个字符分割
  - re模块中的split可实现一次多个字符分割, 不同字符之间使用 | 隔开, 其中 如果是 . 需要转义
  ```Python
  e='aa@dd.com'
  re.split('@|\.', e) # 得到 aa dd com
  ```

- `str.join(iterable)`: Return a string which is the concatenation of the strings in the iterable iterable.
  The separator between elements is the string providing this method.
  string.join() 的参数是一个iterable的object, 例如一个list
  `":".join(["this","is"])` 生成 `this:is`

### 原始字符串
原始字符串是python中一类比较特殊的字符串, 以大写字母R或者小写字母r开始.
在原始字符串中,\不再表示转义字符的含义
原始字符串是为正则表达式而设计的, 但是可以用其来方便的表示windows系统下的路径

# FP
在函式编程中,最著名的特色就是高序(High Order).简单地说,就是定制一个算法,
按规则来指定容器中的每一个元素.最常用的 High Order 为:  

- map
映射,也就是将算法施于每个元素,将返回值合并为一个新的容器.  
- filter
过滤,将算法施于每个元素,将返回值为真的元素合并为一个新的容器.  
- reduce
合并,将算法(可能携带一个初值)依次施于每个元素,将返回值作为下一步计算的参数之一,与下一个元素再计算,直至最终获得一个总的结果.  

**map**
`map(f, iterA, iterB, ...) returns a list containing f(iterA[0], iterB[0]), f(iterA[1], iterB[1]), f(iterA[2], iterB[2]), ....`
函式 map 至少需要两个参数,第一个是一个函式,第二个是传入函式的参数.例如
```python
def foo(x):
  return x*x
print map(foo,range(10))
```

```python
def foo(x,y):
  return x+y
print map(foo,range(10),range(10))
## 得到[0,2,4,....18]
```

**filter**
`filter(predicate, iter)` returns a list that contains all the sequence elements that meet a certain condition, and is similarly duplicated by list comprehensions.
A predicate is a function that returns the truth value of some condition;
for use with filter(), the predicate must take a single value.
例如可以用下面的方法得到 100以内的偶数列:
```python
def foo(x):
  return x%2==0 
print filter(foo,range(100))
```

**reduce**
`reduce(func, iter, [initial_value])`
func must be a function that takes two elements and returns a single value.
reduce() takes the first two elements A and B returned by the iterator and calculates func(A, B).
It then requests the third element, C, calculates func(func(A, B), C),
combines this result with the fourth element returned, and continues until the iterable is exhausted.
If the iterable returns no values at all, a TypeError exception is raised.
If the initial value is supplied, it is used as a starting point and func(initial_value, A) is the first calculation.

**lambda**
lambda 参数列表: 表达式
```python
fun=lambda x: x*x-x
fun(3)  #get 6
print map(lambda x: x* * 2, range(10)) 
```

# 反射(自省)
有时候我们会碰到这样的需求,需要执行对象的某个方法,或是需要对对象的某个字段赋值,而方法名或是字段名在编码代码时并不能确定,需要通过参数传递字符串的形式输入.
举个具体的例子:当我们需要实现一个通用的DBM框架时,可能需要对数据对象的字段赋值,但我们无法预知用到这个框架的数据对象都有些什么字段,
换言之,我们在写框架的时候需要通过某种机制访问未知的属性.

这个机制被称为反射(反过来让对象告诉我们他是什么),或是自省(让对象自己告诉我们他是什么,好吧我承认括号里是我瞎掰的),用于实现在运行时获取未知对象的信息.
反射是个很吓唬人的名词,听起来高深莫测,在一般的编程语言里反射相对其他概念来说稍显复杂,一般来说都是作为高级主题来讲,
但在Python中反射非常简单,用起来几乎感觉不到与其他的代码有区别,使用反射获取到的函数和方法可以像平常一样加上括号直接调用,获取到类后可以直接构造实例,
不过获取到的字段不能直接赋值,因为拿到的其实是另一个指向同一个地方的引用,赋值只能改变当前的这个引用而已.

**访问对象的属性**
[demo](../demo/python/reflection.py)

- `dir([obj])`:
调用这个方法将返回包含obj大多数属性名的列表(会有一些特殊的属性不包含在内).obj的默认值是当前的模块对象.
当你对一个你构造的对象使用dir()时,可能会发现列表中的很多属性并不是你定义的.这些属性一般保存了对象的元数据,比如类的__name__属性保存了类名.
大部分这些属性都可以修改,不过改动它们意义并不是很大
- `hasattr(obj, attr)`:
这个方法用于检查obj是否有一个名为attr的值的属性,返回一个布尔值.
- `getattr(obj, attr)`:
调用这个方法将返回obj中名为attr值的属性的值,例如如果attr为'bar',则返回obj.bar.
- `setattr(obj, attr, val)`:
调用这个方法将给obj的名为attr的值的属性赋值为val.例如如果attr为'bar',则相当于obj.bar = val.

## 访问对象的元数据
**确定对象的类型**
在types模块中定义了全部的Python内置类型,结合内置方法isinstance()就可以确定对象的具体类型了.
```python
isinstance(object, classinfo):
```
检查object是不是classinfo中列举出的类型,返回布尔值.classinfo可以是一个具体的类型,也可以是多个类型的元组或列表.
types模块中仅仅定义了类型,而inspect模块中封装了很多检查类型的方法,比直接使用types模块更为轻松,所以这里不给出关于types的更多介绍,
如有需要可以直接查看types模块的文档说明.本文第3节中介绍了inspect模块.

模块(module)

- `__doc__`: 文档字符串.如果模块没有文档,这个值是None.
- `*__name__`: 始终是定义时的模块名,即使你使用import .. as 为它取了别名,或是赋值给了另一个变量名.
- `*__dict__`: 包含了模块里可用的属性名-属性的字典,也就是可以使用模块名.属性名访问的对象.
- `__file__`: 包含了该模块的文件路径.需要注意的是内建的模块没有这个属性,访问它会抛出异常!

类(class)

- `__doc__`: 文档字符串.如果类没有文档,这个值是None.
- `*__name__`: 始终是定义时的类名.
- `*__dict__`: 包含了类里可用的属性名-属性的字典,也就是可以使用类名.属性名访问的对象.
- `__module__`: 包含该类的定义的模块名,需要注意,是字符串形式的模块名而不是模块对象.
- `*__bases__`: 直接父类对象的元组,但不包含继承树更上层的其他类,比如父类的父类.

实例(instance)

- `*__dict__`: 包含了可用的属性名-属性字典.
- `*__class__`: 该实例的类对象.对于类Cat,cat.__class__ == Cat 为 True.

# 多线程
Python的线程虽然是真正的线程, 但解释器执行代码时, 有一个GIL锁:Global Interpreter Lock, 任何Python线程执行前, 必须先获得
GIL锁,然后,每执行100条字节码,解释器就自动释放GIL锁,让别的线程有机会执行.这个GIL全局锁实际上把所有线程的执行代码都给上了
锁,所以,多线程在Python中只能交替执行,即使100个线程跑在100核CPU上,也只能用到1个核.

对面向I/O的(会调用内建的操作系统C代码的)程序来说,GIL会在这个I/O调用之前被释放,以允许其他线程在这个线程等待I/O的时候运行.
如果某线程并未使用很多I/O操作,它会在自己的时间片内一直占用处理器和GIL.
也就是说,I/O密集型的Python程序比计算密集型的Python程序更能充分利用多线程的好处.

GIL是Python解释器设计的历史遗留问题,通常我们用的解释器是官方实现的CPython,要真正利用多核,除非重写一个不带GIL的解释器.


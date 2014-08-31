# Python sequence
sequence在python不是一种特定的类型,而是泛指一系列的类型

list, tuple, 字符串都属于sequence

# IO
## 重定向

	import sys
	out = open("out.txt","w")
	sys.stdout = out  //之后的print语句就会输出到out 指向的文件中
	print .....
	out.close()

输出到文件

	out = open("out.txt","w")  //w is write, a+ 追加
	print>>out,"string",integer //输出到out中
	sys.stdout  //sys模块中
或者
```
f.write(string)  将一个字符串写入文件,如果写入结束,必须在字符串后面加上"\n"
f.close()
```

## 文件
读取文件
```
f = open("d:\test.txt", "w")
for line in f:
    print line
```

`f.read(size)`   //参数size表示读取的数量,可以省略.如果省略size参数,则表示读取文件所有内容.
`f.readline()`  读取文件一行的内容
`f.readlines()`  读取所有的行到数组里面[line1,line2,...lineN].在避免将所有文件内容加载到内存中,这种方法常常使用,便于提高效率.

文件中定位
这个函数的格式如下(单位是bytes):`f.seek(offset, from_what)`
from_what表示开始读取的位置,offset表示从from_what再移动一定量的距离,比如`f.seek(10, 3)`表示定位到第三个字符并再后移10个字符.from_what值为0时表示文件的开始,它也可以省略,缺省是0即文件开头
```
f = open('/tmp/workfile', 'r+')
f.write('0123456789abcdef')
f.seek(5) # Go to the 6th byte in the file
f.read(1) // read '5'
f.seek (-3, 2) # Go to the 3rd byte before the end
f.read(1) // read 'd'
```
# random 随机数生成
- `random.random()`用于生成一个0到1的随机符点数: 0 <= n < 1.0
-  `random.uniform(a, b)`,指定范围内的随机符点数. 如果a > b,则生成的随机数n: a <= n <= b.如果 a <b, 则 b <= n <= a.
- `random.randint(a, b)`,指定范围内的整数, 生成的随机数n: a <= n <= b
- `random.randrange([start], stop[, step])`,从指定范围内,按指定基数递增的集合中 获取一个随机数  
如:random.randrange(10, 100, 2),结果相当于从[10, 12, 14, 16, ... 96, 98]序列中获取一个随机数.random.randrange(10, 100, 2)在结果上与 random.choice(range(10, 100, 2) 等效.
- `random.choice(sequence)`从序列中获取一个随机元素  
- `random.shuffle(x[, random])`,Shuffle the sequence x in place.   
The optional argument random is a 0-argument function returning a random float in [0.0, 1.0); by default, this is the function random().
- `random.sample(sequence, k)`,从指定序列中随机获取指定长度的片断.sample函数不会修改原有序列

# Data types
list and dictionary is mutable

two non-scalar type: tuple and string are immutable

## mutable vs immutable
immutable
```
x = something
func(x)
print x # prints the same thing 
```

mutable
```
x = something # 
func(x)
print x # might print something different, that means func can change the real x
```

```
x = something # immutable type 
y = x 
print x 
# some statement that operates on y
print x # prints the same thing 
```

```
x = something # mutable type 
y = x 
print x 
# some statement that operates on y
print x # might print something different
```

Python represents all its data as objects.
Some of these objects like lists and dictionaries are mutable, meaning you can change their content without changing their identity.
Other objects like integers, floats, strings and tuples ... are objects that can not be changed. An easy way to understand that is if you have a look at an objects ID.

Below you see a string that is immutable. You can not change its content. It will through an error at you, if you try to change it. 
Also, if we assign new content, a new object is created instead of the contents being modified.

```
>>> s = "abc"
>>>id(s) 4702124
>>> s[0] 'a'
>>> s[0] = "o"
Traceback (most recent call last): File "<stdin>", line 1, in <module>TypeError: 'str' object does not support item assignment 
```

```
>>> s = "xyz"
>>>id(s) 4800100
>>> s += "uvw"
>>>id(s)4800500
```

### Iterate
file, list, tuple, dict 的迭代, 可以使用for 来进行统一形式的迭代, 其中dict迭代的是key

	for i in seq:
	    do_something_to(i)
实际上是这样工作的:

	fetch = iter(seq)
	while True:
		try:
			i = fetch.next()
		except StopIteration:
			break
		do_something_to(i)

## List
list 不是线程安全的, dequeue 是线程安全的

要注意列表中的列表不会被打散,如 `a=[1,2], b=[a,3,4]`,结果就是 `b=[[1,2],3,4]`

列表赋值是引用赋值.要想拷贝,必须用切片:`b = a[:]`

list是对所指向对象的一个引用
list2=list1,那么这两个list指向的是同一个list对象, 当我们改变一个中的元素的时候, 另一个也随之改变

### List API
- list.append(x)
- list.extend(L): 将list L中所有member 追加到list中
- list.insert(i, x)
- list.remove(x)
- list.pop(i)
- list.index(x): Return the index in the list of the first item whose value is x. It is an error if there is no such item.
- list.count(x): Return the number of times x appears in the list.
- list.sort(): Sort the items of the list, in place. 会改变原列表.
- list.reverse(): Reverse the elements of the list, in place. 会改变原列表.

### 列表推导式
	>>>vec = [ 2, 4, 6] 
	>>>[ 3* x for x in vec if x>3]  # 大于 3 的元素乘上 3 作为新列表元素 
	[ 12, 18] 
	
	>>>[ ( x, x**2) for x in vec] # 循环变量要是一个 sequence, 而[ x, x* * 2 f or x i n vec] 是错误的 
	[ ( 2, 4) , ( 4, 16) , ( 6, 36) ] 
	
	>>>vec2 = [ 4, 3, - 9] 
	>>>[ x* y for x in vec for y in vec2] # vec 与 vec2 元素相乘 
	[ 8, 6, - 18, 16, 12, - 36, 24, 18, - 54] 

## Dict
定义方式为 `c={'a':1, 'b':2}`

定义和访问时,key必须用引号引起来. 使用{}定义,使用[]访问,即`c['a']`

对字典key 的要求:  
dictionaries are indexed by keys, which can be any immutable type; strings and numbers can always be keys.

### Dict API
- `key in d`: Return True if d has a key key, else False.
- `key not in d`: Equivalent to not key in d
- `len(d)`
- `get(key[, default])`: Return the value for key if key is in the dictionary, else default. 
If default is not given, it defaults to None, so that this method **never raises a KeyError**.
- `pop(key[, default])`
- `del d[key]`: Raises a KeyError if key is not in the map

## Numeric
`int(String)`: convert a string to integer
`float`: 将字符串或者整数转换为浮点数
`chr`: 将ascii值转换为ascii字符
`hex`: 将整数转换为十六进制的字符串
`long`: 转化为长整型
`oct`: 将整数转化为八进制的字符串
`ord`: 将ascii字符转化为ascii值
`str(object)`  转化为字符串
`len(object)` 长度

`divmod(x, y)` which returns a tuple `(x / y, x % y)`

### 小数位数

	>>> round(2.675,2)
	2.6800000000000002
	>>> round(2.675,2) == 2.68
	True
	>>> round(2.675,2) == 2.67
	False

保留小数位要用字符替换 

	>>> "%.0f" % 3.45
	'3'
	>>> "%.1f" % 3.45
	'3.5'
	>>> "%.2f" % 3.45
	'3.45'
	>>> "%.3f" % 3.45
	'3.450'

### math module
- `math.floor()`
- `math.ceil()`

## string
**以下函数并不改变字符串本身, 而是返回修改后的新的字符串 string immutable**
一旦声明了一个字符串, 则该字符串中的每个字符都有了自己固定的位置,可以使用`[index]`来访问
python还允许以负数来访问字符串中字符,负数表示从字符串的尾部开始计算,此时最后一个字符的序号为-1, 例如 `string[-2] #倒数第2个字符`

**计数**
- `str.count(sub[, start[, end]])`
Return the number of non-overlapping occurrences of substring sub in the range [start, end]. Optional arguments start and end are interpreted as in slice notation.

**编码**
- `str.decode([encoding[, errors]])`
Decodes the string using the codec registered for encoding. encoding defaults to the default string encoding. errors may be given to set a different error handling scheme. The default is 'strict', meaning that encoding errors raise UnicodeError. Other possible values are 'ignore','replace' and any other name registered via codecs.register_error(), see section Codec Base Classes.
- `str.encode([encoding[, errors]])`
Return an encoded version of the string. Default encoding is the current default string encoding. errors may be given to set a different error handling scheme. The default for errors is 'strict', meaning that encoding errors raise a UnicodeError. Other possible values are 'ignore','replace', 'xmlcharrefreplace', 'backslashreplace' and any other name registered via codecs.register_error(), see section Codec Base Classes. For a list of possible encodings, see section Standard Encodings.

**查找**
- `str.endswith(suffix[, start[, end]])`
Return True if the string ends with the specified suffix, otherwise return False. suffix can also be a tuple of suffixes to look for. With optional start, test beginning at that position. With optional end, stop comparing at that position.

- `str.find(sub[, start[, end]])`
Return the lowest index in the string where substring sub is found, such that sub is contained in the slice s[start:end]. Optional argumentsstart and end are interpreted as in slice notation. Return -1 if sub is not found.
Note: **The find() method should be used only if you need to know the position of sub. To check if sub is a substring or not, use the `in` operator**

- `str.index(sub[, start[, end]])` Like find(), but raise ValueError when the substring is not found.

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
- `str.lstrip([chars])` Return a copy of the string with leading characters removed. The chars argument is a string specifying the set of characters to be removed. If omitted or None, the chars argument defaults to removing whitespace. The chars argument is not a prefix; rather, all combinations of its values are stripped:

- `str.partition(sep)`
Split the string at the first occurrence of sep, and return a 3-tuple containing the part before the separator, the separator itself, and the part after the separator. If the separator is not found, return a 3-tuple containing the string itself, followed by two empty strings.

- `str.replace(old, new[, count])`
Return a copy of the string with all occurrences of substring old replaced by new. If the optional argument count is given, only the firstcount occurrences are replaced.

- `str.split([sep[, maxsplit]])`
Return a list of the words in the string, using sep as the delimiter string. If maxsplit is given, at most maxsplit splits are done. 
当要对一个str 对象按照多个分隔符分隔时, 最好是进行多次分隔, 即一次指用一个字符分隔, 不知道为什么不能一次性处理

- `str.join(iterable)` Return a string which is the concatenation of the strings in the iterable iterable. The separator between elements is the string providing this method.
string.join() 的参数是一个iterable的object, 例如一个list
":".join(["this","is"]) 生成 "this:is"

### 原始字符串
原始字符串是python中一类比较特殊的字符串, 以大写字母R或者小写字母r开始.
在原始字符串中,\不再表示转义字符的含义
原始字符串是为正则表达式而设计的, 但是可以用其来方便的表示windows系统下的路径

# Database
## mysql-connector-python
install

	pip install --alow-all-external mysql-connector-python

demo

	import mysql.connector
	conn = mysql.connector.connect(host="localhost", user="test", passwd="123456", db="test", buffered=True)
	cursor = conn.cursor()
	cursor.execute("show databases;")
	r = cursor.fetchone()
	print r
	cursor.close()
	conn.close()

process a query result

	# Using a while-loop
	cursor.execute("SELECT * FROM employees")
	row = cursor.fetchone()
	while row is not None:
		print(row)
		row = cursor.fetchone()
				
	# Using the cursor as iterator 
	cursor.execute("SELECT * FROM employees")
	for row in cursor:
		print(row)

# Internet
## Address
	import socket
	import struct
	>>> socket.ntohl(struct.unpack('I', socket.inet_aton("201.119.123.249"))[0])
	3380050937L
	;; 如果用i, 而不是I, 可能会出现负值

	>>> socket.inet_ntoa(struct.pack('I',socket.htonl(980942848 )))
	'58.120.0.0'

# General
应该尽量避免使用Python全局变量.
在文件开头声明Python全局变量variable, 在具体函数中使用该变量时,需要事先声明 global variable,否则系统将该变量视为局部变量. CONSTANT = 0  (将全局变量大写便于识别)

## 基本程序结构
条件
```
if condition:
    do something
```

循环
```
for sth in sths:
    do something
while condition:
     do something
```

switch
python中不支持switch, 可以通过if来完成, 当然还有一种更加优雅的方式
```
sw = {
    'a': lambda x: x,
    'b': lambda x: x + 1,
    'c': lambda x: x * x,
}
print sw['a'](2) ## 输出2
print sw['b'](2) ## 输出3
print sw['c'](2) ## 输出4
```
不管从可读性(这是显然的),性能(哈希表 vs 普通查找)上都更好.另外最后一种做法将参数与行为的映射完全独立出来了,一来修改起来极其方便,到时候也很容易将它们分离到配置文件中去. 

```
sw= {
  '10':'age 10',
  '20':'age 20',
  '30':'30',
  'default':'age 10'
  }
age = '10'
print  sw.get(age,'age is deault') ## 如果sw中不含有age, 则sw[age] = 'age is deault'
```

函数
```
def functionname():    
    do something
    return something
```

异常处理
```
try:    suite
except:    suite
    except IOError: suite
finally:    suite
```

`assert expression, 'text'`  #如果expression为假,则将text输出,并且报AssertionError
例如:
```
a=4
assert a>0, 'a must be positive'  # do nothing
assert a<0, 'a must be negative' #输出a must be negative
```

## 模块
```
import module  #将整个模块导入
import module as alias
from module import function #只导入module中的一个函数,使用的时候可以直接使用, 而不需要用 module.function
from module import *  #导入所有函数
```

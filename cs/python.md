脚本里面退出: `sys.exit(1)`

# Python sequence
sequence在python不是一种特定的类型,而是泛指一系列的类型

list, tuple, 字符串都属于sequence

# 参数
## 系统参数
`sys.argv` 获取参数列表  
如果需要获取参数列表的长度, 需要通过`len(sys.argv)`.  
参数依次为: `sys.argv[0]`(第一个参数为本文件的名字), `sys.argv[1]`

## 可变长参数
使用可变长参数的函数, 将其所有参数保存在一个元组里, 在函数中可以使用for循环来处理
以 `*` 开头定义一个参数即可
```
def mylistappend(L,*list):
    for i in list:
        L.extend(i)
    return L
```

## 参数引用
在C语言中,可以通知在参数中使用指针已达到改变参数值的作用
在python中,可以在参数中使用可变对象,如列表和字典来达到改变参数的目的
```
传值
def changevalue(x):
	x=x**2

传引用
def changevalue2(x):
	x[0]=x[0]**2
```

在Python中,无论你把对象做为参数传递,做为函数返回值,都是引用传递的.  
你想复制一个对象.  
标准库中的copy模块提供了两个方法来实现拷贝.一个方法是copy,它返回和参数包含内容一样的对象.
```
import copy
new_list = copy.copy(existing_list)
```

有些时候,你希望对象中的属性也被复制, 尤其是当有list 等对象时,可以使用deepcopy方法:
```
import copy
new_list_of_dicts = copy.deepcopy(existing_list_of_dicts)
```
[demo](../demo/python/copy_deepcopy.py)


## global
```
def fun(x):
	global a #说明a是全局变量,可以直接使用, 需要确保在使用之前a 确实是存在的一个全局变量
	return a+x
```

# IO
```
print >>sys.stdout, 'sth'
print >>sys.stderr, 'sth'
print >> sys.stdout, ("pass: %2d, avg_cost: %f"%(num, avg_cost))
```

print 默认输出的时候会自动换行, 在末尾加一个逗号可以避免换行
```
print 'sth',
sys.stdout.write('sth')  ## 用这个也可以达到同样的效果
```

pprint 模块(pretty printer)
用于打印 Python 数据结构.
当你在命令行下打印特定数据结构时你会发现它很有用(输出格式比较整齐, 便于阅读).

[(非二进制)从stdin 读入, 输出到stdout](../demo/python/io/stdin_stdout.py)  
[(二进制)从stdin 读入, 输出到stdout](../demo/python/io/stdin_stdout_binary.py)

If the end of the file has been reached, f.read() will return an empty string ("").  
`a = ""` 可以使用 `a == ""` 或者 `not a` 来进行判断, 两者都为True

## 重定向

	import sys
	out = open("out.txt","w")
	sys.stdout = out  //之后的print语句就会输出到out 指向的文件中
	print .....
	out.close()

记住,如果你还想在控制台打印一些东西的话,最好先将原始的控制台对象引用保存下来,向文件中打印之后再恢复 sys.stdout

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

`f.read(size)`   //参数size表示读取的字节数,可以省略.如果省略size参数,则表示读取文件所有内容.
`f.readline()`  读取文件一行的内容
`f.readlines()`  读取所有的行到一个数组list里面.在避免将所有文件内容加载到内存中,这种方法常常使用,便于提高效率.

文件中定位
这个函数的格式如下(单位是bytes):`f.seek(offset, from_what)`
from_what表示开始读取的位置,
offset表示从from_what再移动一定量的距离,比如`f.seek(10, 3)`表示定位到第三个字符并再后移10个字符.
from_what值为0时表示文件的开始,它也可以省略,缺省是0即文件开头
```
f = open('/tmp/workfile', 'r+')
f.write('0123456789abcdef')
f.seek(5) # Go to the 6th byte in the file
f.read(1) // read '5'
f.seek (-3, 2) # Go to the 3rd byte before the end
f.read(1) // read 'd'
```

混合使用file.readline() and file.next()要注意:  
When a file is used as an iterator, typically in a for loop (for example, `for line in f: print line.strip()`), the next() method is called repeatedly.
In order to make a for loop the most efficient way, the next() method uses a **hidden read-ahead buffer**.
也就是说,next()方法会预加载后面的内容, 这时如果使用readline()方法则会与next()方法的预加载产生冲突.
However, using seek() to reposition the file to an absolute position will flush the read-ahead buffer.

## [二进制](http://www.cnblogs.com/gala/archive/2011/09/22/2184801.html)
有的时候需要用python处理二进制数据,比如,存取文件,socket操作时.这时候,可以使用python的struct模块来完成.可以用 struct来处理c语言中的结构体.

binascii模块包含很多在二进制和ASCII编码的二进制表示转换的方法

- binascii.hexlify(data): Return the hexadecimal representation of the binary data. 
	Every byte of data is converted into the corresponding 2-digit hex representation(因此变为原来的两倍长)
- binascii.unhexlify(hexstr): Return the binary data represented by the hexadecimal string hexstr. hexstr 必须为偶数个16 进制digits, otherwise a TypeError is raised.
```
bin = struct.pack(i, 23) ## bin 为 '\x17\x00\x00\x00' 占4 个bytes
binascii.hexlify(bin) ## 返回'17000000', 占 8 个bytes
```
 
struct模块中最重要的三个函数是pack(), unpack(), calcsize()

- `pack(fmt, v1, v2, ...)`: 按照给定的格式(fmt),把数据封装成字符串(实际上是类似于c结构体的字节流)
- `unpack(fmt, string)`: 按照给定的格式(fmt)解析字节流string,返回解析出来的tuple
- `calcsize(fmt)`: 计算给定的格式(fmt)占用多少字节的内存

[binary struct demo](../demo/python/format/pack_unpack.py)

[PyMOTW: Struct](https://pymotwcn.readthedocs.org/en/latest/documents/struct.html)

struct中支持的格式如下表:

big endian or little endian

| Character | Byte order             | Size     | Alignment |
|-----------|------------------------|----------|-----------|
| @         | native                 | native   | native    |
| =         | native                 | standard | none      |
| <         | little-endian          | standard | none      |
| >         | big-endian             | standard | none      |
| !         | network (= big-endian) | standard | none      |

数据是二进制的, 按照big endian 进行的编码, 但是本纪是litte endian, 则按照下面的方法可以正确读取出值
```
value = sys.stdin.read(4)
value = struct.unpack('>i', value)[0]
```

data type

| Format | C Type             | Python             | 字节数 |
|--------|--------------------|--------------------|--------|
| x      | pad byte           | no value           | 1      |
| c      | char               | string of length 1 | 1      |
| b      | signed char        | integer            | 1      |
| B      | unsigned char      | integer            | 1      |
| ?      | _Bool              | bool               | 1      |
| h      | short              | integer            | 2      |
| H      | unsigned short     | integer            | 2      |
| i      | int                | integer            | 4      |
| I      | unsigned int       | integer or long    | 4      |
| l      | long               | integer            | 4      |
| L      | unsigned long      | long               | 4      |
| q      | long long          | long               | 8      |
| Q      | unsigned long long | long               | 8      |
| f      | float              | float              | 4      |
| d      | double             | float              | 8      |
| s      | char[]             | string             | 1      |
| p      | char[]             | string             | 1      |
| P      | void *             | long               |        |

注意

1. q和Q只在机器支持64位操作时有意思
2. 每个格式前可以有一个数字,表示个数
3. s格式表示一定长度的字符串,4s表示长度为4的字符串,但是p表示的是pascal字符串
4. P用来转换一个指针,其长度和机器字长相关
5. 最后一个可以用来表示指针类型的,占4个字节

注意:二进制文件处理时会碰到的问题

我们使用处理二进制文件时,需要用如下方法
```
binfile=open(filepath,'rb')    读二进制文件
binfile=open(filepath,'wb')    写二进制文件
```
不同之处有两个地方:

1. 使用'r'的时候如果碰到'0x1A',就会视为文件结束,这就是EOF.
使用'rb'则不存在这个问题.即,如果你用二进制写入再用文本读出的话,如果其中存在'0X1A',就只会读出文件的一部分.使用'rb'的时候会一直读到文件末尾.
1. 对于字符串x='abc\ndef',我们可用len(x)得到它的长度为7,\n我们称之为换行符,实际上是'0X0A'.
当我们用'w'即文本方式写的时候,在windows平台上会自动将'0X0A'变成两个字符'0X0D','0X0A',即文件长度实际上变成8.当用'r'文本方式读取时,又自动的转换成原来的换行符.
如果换成'wb'二进制方式来写的话,则会保持一个字符不变,读取时也是原样读取.
所以如果用文本方式写入,用二进制方式读取的话,就要考虑这多出的一个字节了.
'0X0D'又称回车符.linux下不会变.因为linux只使用'0X0A'来表示换行.
 
# random 随机数生成
- `random.random()`用于生成一个0到1的随机符点数: 0 <= n < 1.0
- `random.uniform(a, b)`,指定范围内的随机符点数. 如果a > b,则生成的随机数n: a <= n <= b.如果 a <b, 则 b <= n <= a.
- `random.randint(a, b)`,指定范围内的整数, 生成的随机数n: a <= n <= b
- `random.randrange([start], stop[, step])`,从指定范围内,按指定基数递增的集合中 获取一个随机数.
	如:random.randrange(10, 100, 2),结果相当于从[10, 12, 14, 16, ... 96, 98]序列中获取一个随机数.
	random.randrange(10, 100, 2)在结果上与 random.choice(range(10, 100, 2) 等效.
- `random.choice(sequence)`从序列中获取一个随机元素  
- `random.shuffle(x[, random])`,Shuffle the sequence x in place.
	The optional argument random is a 0-argument function returning a random float in [0.0, 1.0); by default, this is the function random().
- `random.sample(sequence, k)`,从指定序列中随机获取指定长度的片断.sample函数不会修改原有序列

```
list = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]  
slice = random.sample(list, 5)  #从list中随机获取5个元素,作为一个片断返回  
print slice  
print list #原有序列并没有改变
```

# Data types
- **list and dictionary is mutable**
- two non-scalar type: **tuple and string are immutable**

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
Other objects like integers, floats, strings and tuples ... are objects that can not be changed. 
An easy way to understand that is if you have a look at an objects ID.

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
在使用`for x in seq`结构循环中, 不能改变seq, 否则会出现不可预知的问题.

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

### set
集合是无序的,不重复的元素集,类似数学中的集合,可进行逻辑运算和算术运算.
[set demo](../demo/python/set.py)

## [heapq](https://docs.python.org/2/library/heapq.html)
需要 `import heapq`

- heapq.heappush(heap, item)
- heapq.heappop(heap) 弹出最小元素, 同时这个弹出的元素会从heap中被删除
- heapq.heappushpop(heap, item): The combined action runs more efficiently than heappush() followed by a separate call to heappop().
- heapq.heapify(x) Transform list x into a heap, in-place, in linear time.
- heapq.heapreplace(heap, item): Pop and return the smallest item from the heap, and also push the new item. more efficient than a heappop() followed by heappush() 
- heapq.nlargest(n, iterable[, key]) Return a list with the n largest elements from the dataset defined by iterable.
- heapq.nsmallest(n, iterable[, key])

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

[编码](http://in355hz.iteye.com/blog/1860787)
因为 Python 认为 16 位的 unicode 才是字符的唯一内码,而大家常用的字符集如 gb2312,gb18030/gbk,utf-8,以及 ascii 都是字符的二进制(字节)编码形式.
把字符从 unicode 转换成二进制编码,当然是要 encode
[encoding demo](../demo/python/encoding.py)

- `str.decode([encoding[, errors]])`: Decodes the string using the codec registered for encoding.
- `str.encode([encoding[, errors]])`: Return an encoded version of the string.

**查找**
- `str.endswith(suffix[, start[, end]])`: Return True if the string ends with the specified suffix, otherwise return False.
- `str.find(sub[, start[, end]])`: Return the lowest index in the string where substring sub is found, such that sub is contained in the slice s[start:end].
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
- `str.lstrip([chars])` Return a copy of the string with leading characters removed. The chars argument is a string specifying the set of characters to be removed.

- `str.partition(sep)`: Split the string at the first occurrence of sep, 
	and return a 3-tuple containing the part before the separator, the separator itself, and the part after the separator. 
	If the separator is not found, return a 3-tuple containing the string itself, followed by two empty strings.

- `str.replace(old, new[, count])`: Return a copy of the string with all occurrences of substring old replaced by new. 
	If the optional argument count is given, only the firstcount occurrences are replaced.

- `str.split([sep[, maxsplit]])`: Return a list of the words in the string, using sep as the delimiter string.
	当要对一个str 对象按照多个分隔符分隔时, 最好是进行多次分隔, 即一次指用一个字符分隔, 不知道为什么不能一次性处理

- `str.join(iterable)`: Return a string which is the concatenation of the strings in the iterable iterable. 
	The separator between elements is the string providing this method.
	string.join() 的参数是一个iterable的object, 例如一个list
	`":".join(["this","is"])` 生成 `this:is`

### 原始字符串
原始字符串是python中一类比较特殊的字符串, 以大写字母R或者小写字母r开始.
在原始字符串中,\不再表示转义字符的含义
原始字符串是为正则表达式而设计的, 但是可以用其来方便的表示windows系统下的路径

# class
[class inheritance demo](../demo/python/school.py)

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

Inserting or updating data is also done using the handler structure known as a cursor. 
When you use a transactional storage engine such as InnoDB (the default in MySQL 5.5 and later), 
you must commit the data after a sequence of INSERT, DELETE, and UPDATE statements.

[insert demo](/../demo/python/db/insert.py)

update 也可以像上面的demo 一样使用.

bind 的使用
```
add_user = """INSERT INTO DB.tbluser 
              (username, department, startyear, currentpos, link) 
              VALUES (%s, %s, %s, %s, %s)"""
```

当需要插入mysql 中的`null` 时, 在python 中将变量赋值为 `None`.

transaction
```
cnx.start_transaction(consistent_snapshot=bool, isolation_level=level, readonly=access_mode)

con.start_trsansaction()
cursor.execute(sql)
con.commit()
cursor.close()
con.close()
```
The default consistent_snapshot value is False. If the value is True, Connector/Python sends WITH CONSISTENT SNAPSHOT with the statement. 
MySQL ignores this for isolation levels for which that option does not apply.

The default isolation_level value is None, and permitted values are 'READ UNCOMMITTED', 'READ COMMITTED', 'REPEATABLE READ', and 'SERIALIZABLE'. 
If the isolation_level value is None, no isolation level is sent, so the default level applies.

The readonly argument can be True to start the transaction in READ ONLY mode or False to start it in READ WRITE mode. 

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
不管从可读性(这是显然的),性能(哈希表 vs 普通查找)上都更好.
另外最后一种做法将参数与行为的映射完全独立出来了,一来修改起来极其方便,到时候也很容易将它们分离到配置文件中去. 

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

class

python中类里的属性变量以:
`_xxx` 单下划线开头,类似于php中的protected,意思是受保护的,只能在本类以及子类中访问
`__xxx` 双下划线,类似于private,私有的,只能本类中访问,连子类都无法访问
没有下划线的是public

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

## Help
```
help, dir, type, id, hasattr, getattr, callable, isinstance
```

在python终端输入help()进入帮助终端
    然后输入"topic" 进行查询
    之后输入quit退出help

常用自省函式有

- help , help("topic")
- dir 列出对象的所有属性,
- type 返回对象的类型,
- id 返回对象的"唯一序号"
- hasattr( ) 和 getattr( ) 分别判断对象是否有某个属性及获得某个属性值
- hasattr (object, "split" )
- callable() 判断对象是否可以被调用
- isinstance() 可以确认某个变量是否有某种类型 isinstance(a, str)

# 模块
包管理工具[PIP](http://lesliezhu.github.io/public/2014-11-08-pip.html)

- $ pip search SomePackage
- $ pip install SomePackage
- $ pip show SomePackage ## 可以查看软件版本
- $ pip list --outdated
- $ pip install --upgrade SomePackage
- $ pip uninstall SomePackage
- $ pip show --files SomePackage: 查看具体的安装文件

## import os
- os.listdir(path) #列出当前路径下的文件
- os.getcwd() #get current working directory, 绝对目录
- os.path.dirname(path):返回文件所在目录
- os.path.basename(path):返回文件名

- os.sep可以取出操作系统特定的路径分割符, `linux /`, `windows \\`, `MacOS :`
- os.linesep字符串给出当前平台使用的行终止符.例如,Windows使用'\r\n', Linux使用'\n'而Mac使用'\r'.
- os.name字符串指示你正在使用的平台.比如对于Windows,它是'nt',而对于Linux/ Unix用户,它是'posix'.
- os.getcwd()函数得到当前工作目录,即当前Python脚本工作的目录路径.
- os.getenv()和os.putenv()函数分别用来读取和设置环境变量.
- os.listdir()返回指定目录下的所有文件和目录名.
- os.system()函数用来运行shell命令.
- os.path.split()函数返回一个路径的目录名和文件名.["/dir","file.ext"]
- os.path.splitext() 得到文件的其他部分和后缀, 例如"/dir/file.ext", ["/dir/file",".ext"]
- os.path.getsize(path) Return the size, in bytes, of path. Raise os.error if the file does not exist or is inaccessible.

对文件,文件夹的操作需要涉及到os模块和shutil模块.

- os.mknod("test.txt"): 创建空文件
- open("test.txt",w): 直接打开一个文件,如果文件不存在则创建文件
- os.mkdir("file"): 创建目录
- shutil.copyfile("oldfile","newfile"): oldfile和newfile都只能是文件, 不能同名
- shutil.copy("oldfile","newfile"): oldfile只能是文件夹,newfile可以是文件(可以同名, 会覆盖),也可以是目标目录
- shutil.copy2(src, dst): Similar to shutil.copy(), but metadata is copied as well – in fact, this is just shutil.copy() followed by copystat()
- shutil.copytree("olddir","newdir"): 复制文件夹, olddir和newdir都只能是目录,且newdir必须不存在
- os.rename("oldname","newname")       文件或目录都是使用这条命令
- shutil.move("oldpos","newpos")   移动文件(目录)
- os.remove("file") 删除文件
- os.rmdir("dir") 只能删除空目录
- shutil.rmtree("dir")    空目录,有内容的目录都可以删
- os.chdir("path")    换路径

判断目标

- os.path.exists("goal")    判断目标是否存在
- os.path.isdir("goal")     判断目标是否目录
- os.path.isfile("goal")    判断目标是否文件

## import sys
- sys.path #系统目录
- sys.argv  #可执行文件名是第一个参数

## import time
- `time.time()`用ticks计时单位返回从12:00am, January 1, 1970(epoch) 开始的记录的当前操作系统时间

上述也就是struct_time元组.这种结构具有如下属性:
```
序号	属性	值
0	tm_year	2008
1	tm_mon	1 到 12
2	tm_mday	1 到 31
3	tm_hour	0 到 23
4	tm_min	0 到 59
5	tm_sec	0 到 61 (60或61 是闰秒)
6	tm_wday	0到6 (0是周一)
7	tm_yday	1 到 366(儒略历)
8	tm_isdst	-1, 0, 1, -1是决定是否为夏令时的旗帜
```

- `time.localtime(time.time())` 将秒数转化为struct_time 形式的时间
- `time.asctime(time.localtime(time.time()))` 获取格式化的时间
- `import calendar` 日历处理模块

## pickle 序列化反序列化
[ref](http://www.cnblogs.com/pzxbc/archive/2012/03/18/2404715.html)

python的pickle模块实现了基本的数据序列和反序列化.
通过pickle模块的序列化操作我们能够将程序中运行的对象信息保存到文件中去,永久存储,
通过pickle模块的反序列化操作,我们能够从文件中创建上一次程序保存的对象.

`pickle.dump(obj, file, [,protocol])` :将对象obj保存到文件file中去.

- protocol为序列化使用的协议版本
	- 0: ASCII协议,所序列化的对象使用可打印的ASCII码表示,
	- 1: 老式的二进制协议,
	- 2: 2.3版本引入的新二进制协议,较以前的更高效.其中协议0和1兼容老版本的python.protocol默认值为0.
- file:对象保存到的类文件对象. file必须有write()接口, file可以是一个以'w'方式打开的文件或者一个StringIO对象或者其他任何实现write()接口的对象.
如果protocol>=1,文件对象需要是二进制模式打开的.

`pickle.load(file)`: 从file中读取一个字符串,并将它重构为原来的python对象.

- file:类文件对象,有read()和readline()接口.

可以将多个对象dump 到同一个文件中(追加的形式)
依次进行load可以得到原来dump的对象

## import numpy as np
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

array 的操作

- np.reshape(x, shape): Gives a new shape to an array without changing its data, 例如把[3, 4] 的shape变为[2, 6]的
- np.dot(x, y): dot product
- np.transpose(x): 矩阵转置
- np.copy(x): makes a complete copy of the array and its data.
	```
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
		```
		XR21 32.789 1
		XR22 33.091 2
		table = np.loadtxt('example.txt',dtype={'names':('ID', 'Result', 'Type'),'formats': ('S4', 'f4', 'i2')})
		# [('XR21', 32.78900146484375, 1) ('XR22', 33.090999603271484, 2)]
		```
	- for row in data, data 为 loadtxt 的结果
	- for column in data.T

boolean
when using NumPy arrays, you can only use & and | as this allows fast comparisons of boolean values.

## scipy
SciPy是一款方便,易于使用,专为科学和工程设计的Python工具包.它包括统计,优化,整合,线性代数模块,傅里叶变换,信号和图像处理,常微分方程求解器等等.

There are several ways to fit data with a linear regression. In this section we will use `curve_fit`, which is a χ2-based method (in other words, a best-fit method)

[solve funtion](../demo/python/scipy_solve_function.py)

[interpolation](../demo/python/scipy_interpolation.py)

## import sympy
SymPy是Python的一个数学符号计算库.
它目的在于成为一个富有特色的计算机代数系统.
它保证自身的代码尽可能的简单,且易于理解,容易扩展.SymPy完全由Python写成,不需要额外的库.

## import subprocess as sp
[ref1](https://docs.python.org/2/library/subprocess.html)
[ref2](http://blog.chinaunix.net/uid-14833587-id-76547.html)

Execute a child program in a new process:
`subprocess.Popen(args, bufsize=0, stdin=None, stdout=None, stderr=None, preexec_fn=None, close_fds=False, shell=False, cwd=None,env=None,universal_newlines=False)`

- args: should be a sequence of program arguments or else a single string
- shell: (defaults to False) specifies whether to use the shell as the program to execute. If True, pass args as a string rather than as a sequence.
- bufsize: If you experience performance issues, it is recommended that you try to enable buffering by setting bufsize to either -1 or a large value (such as 4096).
- stdin, stdout and stderr: valid values are PIPE, an existing file descriptor (a positive integer), an existing file object, and None. 
- cwd: 工作目录
- universal_newlines: if True, '\n' for the Unix end-of-line convention; '\r' for the old Macintosh convention or '\r\n' for the Windows convention,都被作为'\n'处理

```
p = sp.Popen("./subprocess_demo", stdin = sp.PIPE, stdout = sp.PIPE, stderr = sp.PIPE, shell = False)
## 这里将p的stdin, stdout, sterr都设置为pipe,稍后我们就可以通过往这些pipe写数据往subprocess_demo程序的输入写入数据

## write to p.stdin, so that app can read from stdin
p.stdin.write('3\n')
p.stdin.write('4\n')
sys.stdout.write(p.stdout.read())
```
[subprocess_demo.cpp](../demo/python/subprocess_demo.cpp)

## xlutils, xlrd, xlwt
module for excel
rd: read, wt: write

xlrd: 读取excel 内容
```
workbook = xlrd.open_workbook(path)
sheet = workbook.sheets()[0]
value = sheet.cell(1,2).value
```
[xlrd demo](../demo/python/excel.py)

xlwt: 创建编辑新的excel
```
workbook = xlwt.Workbook()
sheet = workbook.add_sheet('sheet name')
sheet.write(0,0,'test')
workbook.save('demo.xls')
```

如果对一个单元格重复操作,会引发
returns error:
```
# Exception: Attempt to overwrite cell:
# sheetname=u'sheet 1' rowx=0 colx=0
```
所以在打开时加cell_overwrite_ok=True解决
```
table = file.add_sheet('sheet name',cell_overwrite_ok=True)
```

使用style
```
style = xlwt.XFStyle() #初始化样式
font = xlwt.Font() #为样式创建字体
font.name = 'Times New Roman'
font.bold = True
style.font = font #为样式设置字体
table.write(0, 0, 'some bold Times text', style) # 使用样式

设置日期格式
style.num_format_str = "mm/dd/yyyy"
```

如果要编辑现有的excel 表格, 只能采取迂回的策略, 
即用xlrd 打开workbook, 然后用xlutils 提供的copy 工具将打开的workbook 复制一份, 被复制的是xlwt格式, 可以进行编辑, 最后再将复制的workbook保存下来.

```
rb = xlrd.open_workbook(path)
rs = rb.sheets()[0]
wb = copy(rb)
ws = wb.get_sheet(0)
```
对于excel 03的格式, 可以使用 `rb = xlrd.open_workbook(path, formatting_info = True)`, 这样可以将path的格式保留下来, 但是这个功能在excel 07还没有实现

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
```
def foo(x):
	return x*x
print map(foo,range(10))
```

```
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
```
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
```
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
```
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

# 胶水语言
它常被昵称为胶水语言,能够把用其他语言制作的各种模块(尤其是C/C++)很轻松地联结在一起.
[如何实现 C/C++ 与 Python 的通信?](http://www.zhihu.com/question/23003213/answer/56121859)
## Python call C
比如说,我们有一个功能强大的C函数:
```
int great_function(int a) {
	return a + 1;
}
```
期望在Python里这样使用:
```
>>> import great_module
>>> great_module.great_function(2)
3
```
[Python call C 实现方式](../demo/python/great_module.c)

除了功能强大的函数great_function外,这个文件中还有以下部分:

- 包裹函数_great_function.它负责将Python的参数转化为C的参数(PyArg_ParseTuple),调用实际的great_function,并处理great_function的返回值,最终返回给Python环境.
- 导出表GreateModuleMethods. 它负责告诉Python这个模块里有哪些函数可以被Python调用.
导出表的名字可以随便起,每一项有4个参数:Python环境的函数名称;_great_function,即包裹函数;参数变长;说明性的字符串.
导出表总是以{NULL, NULL, 0, NULL}结束.
- 导出函数initgreat_module.这个的名字不是任取的,是你的module名称添加前缀init.导出函数中将模块名称与导出表进行连接.

## C call Python
```
// c_call_python_simple.c
#include <Python.h>
int main(int argc, char *argv[]){
	Py_SetProgramName(argv[0]);
	Py_Initialize();
	PyRun_SimpleString("print 'Hello Python!'\n");
	Py_Finalize();
	return 0;
}
```
编译生成可执行文件: `gcc c_call_python_simple.c -I/usr/include/python2.7/ -lpython2.7`

虽然非常简单,但这段代码除了能用C语言动态生成一些Python代码之外,并没有什么用处.我们需要的是C语言的数据结构能够和Python交互.

下面举个例子,比如说,有一天我们用Python写了一个功能特别强大的函数:
```
## great_module.py
def great_function(a):
	return a + 1
```
接下来要把它包装成C语言的函数.我们期待的C语言的对应函数应该是这样的:
```
int great_function_from_python(int a){
    int res; 
    // some magic
    return res;
}
```

[C call Python 实现方式](../demo/python/c_call_python.c)

编译生成可执行文件: `gcc c_call_python.c -I/usr/include/python2.7/ -lpython2.7`

在Linux/Mac OSX运行此示例之前,可能先需要设置Python环境变量:`export PYTHONPATH=.:$PYTHONPATH`

从上述代码可以窥见Python内部运行的方式:

- 所有Python元素,module,function,tuple,string等等,实际上都是PyObject.C语言里操纵它们,一律使用PyObject *.
- Python的类型与C语言类型可以相互转换.Python类型XXX转换为C语言类型YYY要使用PyXXX_AsYYY函数,C类型YYY转换为Python类型XXX要使用PyXXX_FromYYY函数.
- 也可以创建Python类型的变量,使用PyXXX_New可以创建类型为XXX的变量.
- 若a是Tuple,则a[i] = b对应于 PyTuple_SetItem(a,i,b),有理由相信还有一个函数PyTuple_GetItem完成取得某一项的值.
- 不仅Python语言很优雅,Python的库函数API也非常优雅.


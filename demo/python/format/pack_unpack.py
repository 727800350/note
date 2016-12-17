[二进制](http://www.cnblogs.com/gala/archive/2011/09/22/2184801.html)
有的时候需要用python处理二进制数据,比如,存取文件,socket操作时.这时候,可以使用python的struct模块来完成.可以用 struct来处理c语言中的结构体.

struct模块中最重要的三个函数是pack(), unpack(), calcsize()

- `pack(fmt, v1, v2, ...)`: 按照给定的格式(fmt),把数据封装成字符串(实际上是类似于c结构体的字节流)
- `unpack(fmt, string)`: 按照给定的格式(fmt)解析字节流string,返回解析出来的tuple
- `calcsize(fmt)`: 计算给定的格式(fmt)占用多少字节的内存

比如有一个结构体

struct Header{
    unsigned short id;
    char[4] tag;
    unsigned int version;
    unsigned int count;
}

通过socket.recv接收到了一个上面的结构体数据,存在字符串s中,现在需要把它解析出来,可以使用unpack()函数.

import struct
id, tag, version, count = struct.unpack("!H4s2I", s)

上面的格式字符串中,
!表示我们要使用网络字节顺序解析,因为我们的数据是从网络中接收到的,在网络上传送的时候它是网络字节顺序的.
后面的H表示 一个unsigned short的id,
4s表示4字节长的字符串,
2I表示有两个unsigned int类型的数据.

就通过一个unpack,现在id, tag, version, count里已经保存好我们的信息了.

同样,也可以很方便的把本地数据再pack成struct格式.

ss = struct.pack("!H4s2I", id, tag, version, count);

pack函数就把id, tag, version, count按照指定的格式转换成了结构体Header,
ss现在是一个字符串(实际上是类似于c结构体的字节流),
可以通过 socket.send(ss)把这个字符串发送出去.
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
binfile = open(filepath, 'rb')    读二进制文件
binfile = open(filepath, 'wb')    写二进制文件
```
不同之处有两个地方:

1. 使用'r'的时候如果碰到'0x1A',就会视为文件结束,这就是EOF.
使用'rb'则不存在这个问题.即,如果你用二进制写入再用文本读出的话,如果其中存在'0X1A',就只会读出文件的一部分.使用'rb'的时候会一直读到文件末尾.
1. 对于字符串x='abc\ndef',我们可用len(x)得到它的长度为7,\n我们称之为换行符,实际上是'0X0A'.
当我们用'w'即文本方式写的时候,在windows平台上会自动将'0X0A'变成两个字符'0X0D','0X0A',即文件长度实际上变成8.当用'r'文本方式读取时,又自动的转换成原来的换行符.
如果换成'wb'二进制方式来写的话,则会保持一个字符不变,读取时也是原样读取.
所以如果用文本方式写入,用二进制方式读取的话,就要考虑这多出的一个字节了.
'0X0D'又称回车符.linux下不会变.因为linux只使用'0X0A'来表示换行.
 

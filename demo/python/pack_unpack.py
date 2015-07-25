示例一:

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


示例二:

import struct
a=12.34

#将a变为二进制
bytes=struct.pack('i',a)
此时bytes就是一个string字符串,字符串按字节同a的二进制存储内容相同.

再进行反操作
现有二进制数据bytes,(其实就是字符串),将它反过来转换成python的数据类型:
a,=struct.unpack('i',bytes)
注意,unpack返回的是tuple

所以如果只有一个变量的话:
bytes=struct.pack('i',a)

那么,解码的时候需要这样
a,=struct.unpack('i',bytes) 或者 (a,)=struct.unpack('i',bytes)

如果直接用a=struct.unpack('i',bytes),那么 a=(12.34,) ,是一个tuple而不是原来的浮点数了.

如果是由多个数据构成的,可以这样:

a='hello'
b='world!'
c=2
d=45.123
bytes=struct.pack('5s6sif',a,b,c,d)

此时的bytes就是二进制形式的数据了,可以直接写入文件比如 binfile.write(bytes)
然后,当我们需要时可以再读出来,bytes=binfile.read()
再通过struct.unpack()解码成python变量
a,b,c,d=struct.unpack('5s6sif',bytes)

'5s6sif'这个叫做fmt,就是格式化字符串,由数字加字符构成,
5s表示占5个字符的字符串,
2i,表示2个整数等等

当某一个格式符经常用到时, 可以先为这个格式符创建一个对象
import struct
import binascii

values = (1, 'ab', 2.7)
s = struct.Struct('I 2s f')
packed_data = s.pack(*values)

print 'Original values:', values
print 'Format string  :', s.format
print 'Uses           :', s.size, 'bytes'
print 'Packed Value   :', binascii.hexlify(packed_data)


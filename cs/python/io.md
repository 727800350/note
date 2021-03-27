往 stdout 输出中文的时候一定要注意编码, 比如 decode('utf8'), encode(‘utf8’)
否则会报下面的错误: 'ascii' codec can't encode character in position 20: ordinal not in range(128)

```python
sys.stdout.write()
sys.stderr.write()
print >> sys.stdout, 'sth'
print >> sys.stderr, 'sth'
print >> sys.stdout, 'pass: %2d, avg_cost: %f' % (num, avg_cost)
```

pprint 模块(pretty printer), 打印 Python 数据结构, 输出格式比较整齐, 便于阅读
```python
from pprint import pprint as pretty
```

输出到文件
```python
out = open("out.txt", "w")  //w is write, a+ 追加
print >> out, "string", integer

out.write(string)
out.close()
```

- `sys.stdin.read(size)`: 读取size个字节, 文件结束, read() will return an empty string (""). 如果省略参数,则读取所有内容.

    ```python
    // kv example
    while True:
        // decode
        kl_byte = sys.stdin.read(4)
        if kl_byte == '':
            break
        kl = st.unpack('i', kl_byte)[0]
        k = sys.stdin.read(kl)
        vl = st.unpack('i', sys.stdin.read(4))[0]
        v = sys.stdin.read(vl)

        // encode
        sys.stdout.write(st.pack('i', kl))
        sys.stdout.write(k)
        sys.stdout.write(st.pack('i', vl))
        sys.stdout.write(v)
    ```
- `f.readline()`: 读取文件一行的内容
- `f.readlines()`: 读取所有的行到一个数组list里面.在避免将所有文件内容加载到内存中,这种方法常常使用,便于提高效率.

## python3 format
```Python
print('{0}, {1}'.format('zhangk', 32))
print('{}, {}, {}'.format('zhangk', 'boy', 32))
print('{name}, {sex}, {age}'.format(age=32, sex='male', name='zhangk'))
```

## 文件
读取文件
```python
f = open("./data.txt", "r")
for line in f:
    line = line.strip()
    print line
f.close()
```

文件中定位
这个函数的格式如下(单位是bytes):`f.seek(offset, from_what)`
from_what表示开始读取的位置,
offset表示从from_what再移动一定量的距离,比如`f.seek(10, 3)`表示定位到第三个字符并再后移10个字符.
from_what值为0时表示文件的开始,它也可以省略,缺省是0即文件开头
```python
f = open('/tmp/workfile', 'r+')
f.write('0123456789abcdef')
f.seek(5) # Go to the 6th byte in the file
f.read(1) // read '5'
f.seek (-3, 2) # Go to the 3rd byte before the end
f.read(1) // read 'd'
```

混合使用file.readline() and file.next()要注意:  
When a file is used as an iterator, typically in a for loop (for example, `for line in f: print line.strip()`), the
next() method is called repeatedly.
In order to make a for loop the most efficient way, the next() method uses a **hidden read-ahead buffer**.
也就是说,next()方法会预加载后面的内容, 这时如果交叉使用readline()方法则会与next()方法的预加载产生冲突.
However, using seek() to reposition the file to an absolute position will flush the read-ahead buffer.


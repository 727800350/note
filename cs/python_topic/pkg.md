- [import argparse](#import-argparse)
- [import logging](#import-logging)
- [import os](#import-os)
- [import sys](#import-sys)
- [import urllib](#import-urllib)
- [import time](#import-time)
- [pickle 序列化反序列化](#pickle-序列化反序列化)
- [scipy](#scipy)
- [import sympy](#import-sympy)
- [import subprocess as sp](#import-subprocess-as-sp)
- [xlutils, xlrd, xlwt](#xlutils-xlrd-xlwt)

## [import argparse](http://blog.xiayf.cn/2013/03/30/argparse/)
argparse内置6种动作可以在解析到一个参数时进行触发:

- store 保存参数值,可能会先将参数值转换成另一个数据类型.若没有显式指定动作,则默认为该动作.
- `store_ture/store_false` 保存相应的布尔值.这两个动作被用于实现布尔开关.
- `store_const`: 类似于`store_true/false`, 有了这个标记, 就把dest 设置为 const,
- append 将值保存到一个列表中.若参数重复出现,则保存多个值.
- `append_const` 将一个定义在参数规格中的值保存到一个列表中.
- version 打印关于程序的版本信息,然后退出

```python
import argparse
parser = argparse.ArgumentParser(description='Short sample app')
parser.add_argument('-a', action="store_true", default=False, help = 'used as a flag')
parser.add_argument('-c', action="store", dest="c", type=int, help = 'store a int')
parser.add_argument('-c', action='store_const', dest='constant_value', const='value-to-store', help='constant value')
arg = parser.parse_args() 的返回值是一个命名空间,包含传递给命令的参数
```

而argparse是一个全面的命令行参数解析工具,也处理非选项参数.
```python
parser.add_argument('count', action="store", type=int, help = 'number of apples')
parser.add_argument('units', action="store")
python ./cmd.py 3 inches
Namespace(count=3, units='inches')
```

## import logging
- `logging.basicConfig(level = logging.DEBUG, format = "%(levelname)s %(asctime)s [%(filename)s][%(lineno)d][%(funcName)s] %(message)s")`
  用默认日志格式Formatter为日志系统建立一个默认的流处理器StreamHandler, 设置基础配置(如日志级别等)并加到root logger
  (根Logger)中这几个logging模块级别的函数
- log = logging.getLogger(name):返回一个logger对象,如果没有指定名字将返回root logger
- log.debug(), log.info(), log.warnning(), log.error(), log.critical(), eg: log.debug('this is %s', 'Eric Wang')

默认的log 会打印到stderr, 也可以设置 file|stream handler, formatter, filter etc.
参考[Python logging模块详解](http://blog.csdn.net/zyz511919766/article/details/25136485)

## import os
- os.sep可以取出操作系统特定的路径分割符, `linux /`, `windows \\`, `MacOS :`
- os.linesep字符串给出当前平台使用的行终止符.例如,Windows使用'\r\n', Linux使用'\n'而Mac使用'\r'.

- os.listdir(path) #列出当前路径下的文件
- os.getcwd() #get current working directory, 绝对目录
- os.path.dirname(path):返回文件所在目录
- os.path.basename(path):返回文件名
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
- shutil.copy2(src, dst): Similar to shutil.copy(), but metadata is copied as well – in fact, this is just shutil.copy()
  followed by copystat()
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
- sys.path: 系统目录
- sys.argv: 可执行文件名是第一个参数

## import urllib
- urllib.unquote(): url解码
- urllib.urlencode(): 可以把key-value这样的键值对转换成我们想要的格式, 返回的是`a=1&b=2`这样的字符串

## import time
- `time.time()`用ticks计时单位返回从12:00am, January 1, 1970(epoch) 开始的记录的当前操作系统时间
- `time.localtime(time.time())` 将秒数转化为`struct_time` 形式的时间
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
- file:对象保存到的类文件对象. file必须有write()接口, file可以是一个以'w'方式打开的文件或者一个StringIO对象或者其他任何
  实现write()接口的对象. 如果protocol>=1,文件对象需要是二进制模式打开的.

`pickle.load(file)`: 从file中读取一个字符串,并将它重构为原来的python对象.

- file:类文件对象,有read()和readline()接口.

可以将多个对象dump 到同一个文件中(追加的形式), 依次进行load可以得到原来dump的对象

## scipy
SciPy是一款方便,易于使用,专为科学和工程设计的Python工具包.它包括统计,优化,整合,线性代数模块,傅里叶变换,信号和图像处理,常
微分方程求解器等等.

There are several ways to fit data with a linear regression. In this section we will use `curve_fit`, which is a
χ2-based method (in other words, a best-fit method)

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
- shell: (defaults to False) specifies whether to use the shell as the program to execute. If True, pass args as a
  string rather than as a sequence.
- bufsize: If you experience performance issues, it is recommended that you try to enable buffering by setting bufsize
  to either -1 or a large value (such as 4096).
- stdin, stdout and stderr: valid values are PIPE, an existing file descriptor (a positive integer), an existing file
  object, and None. 
- cwd: 工作目录
- universal_newlines: if True, '\n' for the Unix end-of-line convention; '\r' for the old Macintosh convention or '\r\n'
  for the Windows convention,都被作为'\n'处理

```python
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
```python
workbook = xlrd.open_workbook(path)
sheet = workbook.sheets()[0]
value = sheet.cell(1,2).value
```
[xlrd demo](../demo/python/excel.py)

xlwt: 创建编辑新的excel
```python
workbook = xlwt.Workbook()
sheet = workbook.add_sheet('sheet name')
sheet.write(0,0,'test')
workbook.save('demo.xls')
```

如果对一个单元格重复操作,会引发
returns error:
```python
# Exception: Attempt to overwrite cell:
# sheetname=u'sheet 1' rowx=0 colx=0
```
所以在打开时加cell_overwrite_ok=True解决
```python
table = file.add_sheet('sheet name',cell_overwrite_ok=True)
```

使用style
```python
style = xlwt.XFStyle() #初始化样式
font = xlwt.Font() #为样式创建字体
font.name = 'Times New Roman'
font.bold = True
style.font = font #为样式设置字体
table.write(0, 0, 'some bold Times text', style) # 使用样式

# 设置日期格式
style.num_format_str = "mm/dd/yyyy"
```

如果要编辑现有的excel 表格, 只能采取迂回的策略, 
即用xlrd 打开workbook, 然后用xlutils 提供的copy 工具将打开的workbook 复制一份, 被复制的是xlwt格式, 可以进行编辑, 最后再
将复制的workbook保存下来.

```python
rb = xlrd.open_workbook(path)
rs = rb.sheets()[0]
wb = copy(rb)
ws = wb.get_sheet(0)
```
对于excel 03的格式, 可以使用 `rb = xlrd.open_workbook(path, formatting_info = True)`, 这样可以将path的格式保留下来, 但是
这个功能在excel 07还没有实现


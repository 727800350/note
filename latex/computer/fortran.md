# General introduction
.f 则为fortran 77, .f90 为fortran 90

对于fortran 77, 每一行程序的最前面, 最少都会留有6 个空格. 因为前6 个字符不能用作程序代码.
可以使用输出两个tab键, 在.f文件中, 按下tab键会被自动替换为4个空格.  
对于fortran 90 及之后, 则没有这个要求.

fortran 90 可以使用双引号或者单引号来封装字符串.  
fortran 77 标准中只能使用单引号, 不过大部分的fortran 77 编译器还是可以接受双引号.

- fortran 程序中空格是无意义的字符
- fortran **不区分大小写**, 不管是程序关键字还是变量
- 变量定义必须放在函数的开始位置, 而不能向C语言那样在需要用到的地方再定义

## 固定格式
- 第1 个字符: 如果是字母C,c或*, 这一行文本会被作为注释
- 第1 ~ 5 个字符: 如果是数字, 就是用来给这一行代码取个代号, 不然只能是空格. 之后可以用goto 跳转到这里
- 第6 个字符: 如果是"0" 以外的任何字符, 表示这一行程序会持续上一行
- 第7 ~ 72 个字符, Fortran 程序代码的编写区域
- 第73 个字符之后: 不使用, 超出的部分会被忽略, 有的编译器会发出错误警告

前面空6个空格
```fortran
      program main
      write(*,*) "hello"
      write(*,*) 
     1"hello"
100   write(*,*) "hello"
10    stop
      end
```

前面空8个空格
```fortran
        program main
        write(*,*) 
CC 		用于续行的1 只能放在第6 个字符位置处
     1  "hello"
  10    stop
        end
```

## 自由格式
free format 基本上允许非常自由的编写格式, 它没有规定每一行的第几个字符有什么作用.
需要注意的事项只有以下几点:

1. ! 后面是注释
1. 每行可以编写132 个字符
1. 行号放在每行程序的最前面, 当然也可以不放
1. 一行程序代码的最后如果是符号 &, 为续行符; 如果一行代码的开头是 &, 表示和上一行程序连接
```fortran
program main
write(*,*) "hello" ! 这也是注释
write(*,*) &
"hello"
wri& ! 这一行和下一行的 & 都不能去掉, 不清楚为什么
&te(*,*) "hello"
stop
end
```

# IO
- `write(*, *)`: 第一个* 表示输出的位置为默认值(也就是标准输出), 第二个* 则代表不特别设置输出格式.
- `print *,"string"`: * 表示不限定输出格式, print 只能输出到stdout

## File
**打开文件**
```
open(UNIT = number, FILE = filename, FROM = '...', STATUS = '...', ACCESS = '...')
```
- number 必须是一个正整数, 可以使用变量或常量来赋值.
number 最好避开1,2,5,6(因为2,6 是默认的输出位置, 1,5 是默认的输入位置)
- FROM = 'formated'(表示文本文件, 默认值) or 'unformated'(二进制文件)
- STATUS = 'NEW' or 'OLD' or 'REPLACE' or 'UNKNOWN'(编译器自定义, 通常取replace 的效果, default value)
- ACCESS = 'SEQUENTIEL'(顺序读取, 默认值) or 'DIRECT'(读写文件的操作可以任意指定位置)

以下是fortran 90 添加的功能

- POSITION = 'ASIS'(通常是文件开头, 默认值) or 'REWIND'(打开文件时, 移动到文件开头) or 'APPEND'(移动到文件末尾)

**关闭文件**
```
close(unit = number, status = string)
```
status = 'keep' or 'delete' 文件关闭后是保存还是删除这个文件

**查询文件的状态 inquire**
```
inquire(UNIT = number, FILE = filename, IOSTAT = stat, ERR = label, EXIST = exist, OPENED = opened, 
		NUMBER = number, NAMED = named, ACCESS = access, FROM = from, 
		FORMATTED = formatted, UNFORMATTED = unfomatted)
```
- unit: 根据文件代号查询
- file: 根据文件名字查询
- iostat: >0, 读取操作发生错误; = 0, 正常; < 0, 到达文件末尾
- exist: 检查文件是否存在, logical
- opened: 检查文件是否打开
- number: 由文件名查询这个文件的代号
- named: 查询文件是否取了名字

在使用open 命令打开文件的前后, 都可以通过inquire 命令来查询文件目前的状态.

[检查文件是否存在的demo](../../demo/fortran/file_exist.f90)

# 数据类型 data type
[ref](https://docs.oracle.com/cd/E19205-01/821-0391/aevmb/index.html)

| **Fortran 数据类型**   | **大小** | **缺省对齐** | **COMMON 中的对齐** |
|------------------------|----------|--------------|---------------------|
| BYTE X                 | 1        | 1            | 1                   |
| CHARACTER X            | 1        | 1            | 1                   |
| CHARACTER* n X         | n        | 1            | 1                   |
| **COMPLEX** X          | 8        | 4            | 4                   |
| COMPLEX*8 X            | 8        | 4            | 4                   |
| DOUBLE COMPLEX X       | 16       | 8            | 4                   |
| COMOPLEX*16 X          | 16       | 8            | 4                   |
| COMPLEX*32 X           | 32       | 8/16         | 4                   |
| **DOUBLE PRECISION** X | 8        | 8            | 4                   |
| REAL X                 | 4        | 4            | 4                   |
| REAL*4 X               | 4        | 4            | 4                   |
| REAL*8 X               | 8        | 8            | 4                   |
| REAL*16 X              | 16       | 8/16         | 4                   |
| **INTEGER** X          | 4        | 4            | 4                   |
| INTEGER*2 X            | 2        | 2            | 2                   |
| INTEGER*4 X            | 4        | 4            | 4                   |
| INTEGER*8 X            | 8        | 8            | 8                   |
| **LOGICAL** X          | 4        | 4            | 4                   |
| LOGICAL*1 X            | 1        | 1            | 1                   |
| LOGICAL*2 X            | 2        | 2            | 2                   |
| LOGICAL*4 X            | 4        | 4            | 4                   |
| LOGICAL*8 X            | 8        | 8            | 4                   |

[data type demo](../../demo/fortran/data_type.f90)

请注意以下事项:

REAL*16 和 COMPLEX*32:在 64 位环境(使用 -m64 进行编译)中,缺省对齐位置是 16 字节(而非 8 字节)边界,如表中 8/16 所示.该数据类型通常称为四精度.

数组和结构按照其元素或字段对齐.
数组对齐方式与数组元素相同.
结构对齐方式与具有最宽对齐的字段相同.

## 隐含说明语句IMPLICIT语句
IMPLICIT可以将程序中以某一字母开头的所有变量指定为所需类型.例如: 
```
IMPLICIT  INTEGER(A,C),REAL(I—K) 
```
指定了以A与C字母开头的所有变量都是整型变量,以字母I至K开头的所有变量为实型变量. 

类型说明语句和隐含说明语句只在本程序中有效,他们必须放在程序单位所有可执行语句的前面,而IMPLICIT语句又必须在所有的类型说明语句的前面.

确定变量类型的三种方法中,类型说明语句的优先级最高,I-N规则的优先级最低.

implicit none即设计任何和隐含说明语句无效,
这个时候所有变量都要显式地人工声明,不能未声明就直接使用,有效地避免了可能的大量错误.(比如,你可以注意到implicit none后,I-N规则是无效的)

```fortran
program main
integer a ! 声明
a = 3 ! 赋值
print *,"a = ", a
stop
end
```

## 全局变量 global variable
[全局变量 demo](../../demo/fortran/global.f90)

## 数组 array
定义:
```
类型说明 [::] 数组名([下标下界:]下标上界[,…])[,…]
```
当维说明符省略下标下界时,默认所在维的下标从1开始
```fortran
REAL :: A(1:2,2:4)
INTEGER B(10)
```

或者:
```
DIMENSION [::] 数组名([下标下界:]下标上界[,…])[,…]
[类型说明 [::] 数组名[,…]]
```
```fortran
DIMENSION :: A(10), B(2:11)
INTEGER :: A
```

数组赋值
```
数组名 = (/取值列表/)
```
其中,取值列表可以是标量,隐式DO循环或者任意秩的数组.  
取值列表中所有数值的类型都应该相同,数值之间以逗号分隔.  
如果取值列表中出现了数组,则它的值是按**列元素优先**的规则来赋给目标数组变量.

数组构造器的标识"(/"和"/)"在书写时要注意,括弧和撇号之间不能有空格.

下面来看一些实例
```fortran
MN = (/1, 3, 5, 7, 9/)         	! 标量表示
AB = (/B(2,1:5),B(3:7,7:9)/)   	! 数组表示
CC = (/(I, I=1,4)/)				! 隐DO循环
DE = (/10,A(2:7),(I,I=1,4),7/)	! 混合表示
```

**fortran 与 C 等对数组存储方式的不同**  
FORTRAN is different from languages such as C and Pascal in the way that it stores two-dimensional arrays in memory. 
Although array elements are referenced in row major notation, i.e., (row, column), just as in C or Pascal, 
FORTRAN stores two-dimensional arrays as one-dimensional **column arrays**. 

For example, in C, a two-dimensional array with 2 rows and 3 columns will be stored in memory in the following sequence:
```
(1,1),(1,2),(1,3),(2,1),(2,2),(2,3)
```
However, FORTRAN will store the same array in the following sequence:
```
(1,1),(2,1),(1,2),(2,2),(1,3),(2,3)
```

## 指针
[demo](../../demo/fortran/pointer.f90)

## 数学表达式
** 表示乘幂

# 流程控制
## if else
```fortran
if (condition 1) then
end if

if (condition 1) then
else if(condition 2) then
else
end if
```

## select-case
select-case 在fortran 90 中出现, 但是大部分fortran 77 的编译器也都支持
```fortran
select case(var)
case(case 1)
	do 1
case default
end select
```

case 的写法

- case(1): = 1
- case(1:5): [1, 5]
- case(1:): [1, \infity]
- case(:5): [-\infity, 5]
- case(1, 3, 5): {1,3,5}

select-case 的限制

1. var 只能使用integer, character, logical, 不能使用浮点数及复数
1. 每个case 所使用的数值必须是固定的常量, 不能使用变量

## goto
不建议使用

## pause, continue, stop

## 循环 loop
```fortran
do i = from, to, step
	...
end do
```

```fortran
do label i = from, to, step
	...
label continue
```
label continue 才是 fortran 77 的标准中的.

Many Fortran 77 compilers allow do-loops to be closed by the enddo statement. 
The advantage of this is that the statement label can then be omitted since it is assumed that an enddo closes the nearest previous do statement.
The enddo construct is widely used, but it is not a part of ANSI Fortran 77. 

```fortran
do while(condition)
	...
end do
```

[loop demo](../../demo/fortran/loop.f90)

**提前跳出循环**  
Fortran中跳出循环的两个命令为exit和cycle,
exit表示跳出离命令最近的循环,
cycle表示结束此次循环并开始下一次循环

# 函数 function

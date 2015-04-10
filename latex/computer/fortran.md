# General introduction
.f 则为fortran 77, .f90 为fortran 90

对于fortran 77, 每一行程序的最前面, 最少都会留有6 个空格. 因为前6 个字符不能用作程序代码.
可以使用输出两个tab键, 在.f文件中, 按下tab键会被自动替换为4个空格.  
对于fortran 90 及之后, 则没有这个要求.

fortran 90 可以使用双引号或者单引号来封装字符串.
fortran 877 标准中只能使用单引号, 不过大部分的fortran 77 编译器还是可以接受双引号.

fortran 程序中空格是无意义的字符

fortran 不区分大小写, 不管是程序关键字还是变量

## 固定格式
- 第1 个字符: 如果是字母C,c或*, 这一行文本会被作为注释
- 第1 ~ 5 个字符: 如果是数字, 就是用来给这一行代码取个代号, 不然只能是空格. 之后可以用goto 跳转到这里
- 第6 个字符: 如果是"0" 以外的任何字符, 表示这一行程序会持续上一行
- 第7 ~ 72 个字符, Fortran 程序代码的编写区域
- 第73 个字符之后: 不使用, 超出的部分会被忽略, 有的编译器会发出错误警告

```
前面空6个空格
      program main
      write(*,*) "hello"
      write(*,*) 
     1"hello"
100   write(*,*) "hello"
10    stop
      end

前面空8个空格
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
```
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

请注意以下事项：

REAL*16 和 COMPLEX*32:在 64 位环境(使用 -m64 进行编译)中,缺省对齐位置是 16 字节(而非 8 字节)边界,如表中 8/16 所示.该数据类型通常称为四精度。

数组和结构按照其元素或字段对齐.
数组对齐方式与数组元素相同.
结构对齐方式与具有最宽对齐的字段相同.

```
program main
integer a ! 声明
a = 3 ! 赋值
print *,"a = ", a
stop
end
```

## 数组 array

## 指针
[demo](../../fortran/pointer.f90)

## 数学表达式
** 表示乘幂

# 流程控制
## if else
```
if (condition 1) then
end if

if (condition 1) then
else if(condition 2) then
else
end if
```

## select-case
select-case 在fortran 90 中出现, 但是大部分fortran 77 的编译器也都支持
```
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
```
do i = from, to, step
	...
end do
```

```
do while(condition)
	...
end do
```

# 函数 function

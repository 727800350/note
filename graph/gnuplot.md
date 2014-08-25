# General
## 画图命令
```
gnuplot>plot sin(x)
gnuplot>set xrange [xmin:xmax]
gnuplot>set yrange [ymin:ymax] 以后的图形应该都被改变

gnuplot>plot [0:5]  [-2:2] sin(x)  # 只设定这个图形

gnuplot>plot [0:5]  sin(x)  只设置x范围
gnuplot>plot [] [0:1]  sin(x) 只设置y范围

只设置一边,如设置y为小于0
gnuplot>plot [] [:0]  sin(x)
```

坐标的数值精度
```
set xtics format "%.2f" #设置x轴的刻度数值保留小数点后2位
set ytics format "%.0f" #整数
```

可以在一行写多个命令,命令间用分号隔开
```
gnuplot>set xlabel 'xlabel'; set ylabel 'ylabel'; plot sin(x)
gnuplot>unset xlabel
gnuplot>unset ylabel
```

设置对数坐标
```
gnuplot>set logscale y
gnuplot>plot exp(x)
gnuplot>unset logscale
```

同时绘制多个函数
```
gnuplot>plot f1(x),f2(x),......fn(x),例如:
gnuplot>plot sin(x),x
```

为曲线设置标题
```
gnuplot>plot f(x) title 'title'
gnuplot>plot sin(x) title 'y=sin(x)',x title 'y=x'
```

title的位置
`set key right top` #这也是默认设置, 设置说明文字的位置为右上角, 同样地, 也可以设置为其他地方, 比如 left center bottom  
将曲线标题放置到默认位置,使用命令:`gnuplot>set key default`  
可设置绘图标题框 `gnuplot>set key box`

```
gnuplot>set grid 添加网格
gnuplot>unset grid
```

输出状态
```
gnuplot>set terminal <类型> 其中<类型>为图片的类型,如png,jpeg,gif
gnuplot>set output '<文件名>' 设置输出文件名
```
例如:
```
gnuplot>set terminal  gif
gnuplot>set output 'sinx.gif'
gnuplot>plot sin(x)
```
即会生成名为sinx.gif的图片

设置回窗口输出(即默认状态)
```
gnuplot>set terminal  windows
```
可简写为 `se  t  w`
也可设置输出为其他类型文件,如(ps,tex等),也可直接输出到打印机
```
set term post eps color enh solid
datafile="MPE 8?1 MR.data"
set output  "MPE 8?1 MR 1.eps"
plot datafile using 6:7  #第6, 7列, 第一列为1, 而不是0
```
所有gnuplot设置均已set开头  
term 即terminal 只是图片输出的格式用
post eps 指定输出eps格式图片　
color 可以输出彩色图片和曲线 
enh 即enhance, 加强, 可以输入各种符号, 例如希腊符号  
solid 可使用实体线

图片大小
```
set size 1,1 #这是默认设置, 设置图的大小为1
set size m,n 
```
m,n为放大(缩小)倍数(相对于当前大小),如:  
`set size 1.2, 0.5` 将长设置为当前的1.2倍,宽设为当前的 0.5倍

多图
```
set multiplot layout 2, 2 title "title text"  #将整个画布分为4份
plot    左上角
plot    右上角
plot    左下角
plot    右下角
```

三维绘图
命令splot用于三维绘图,如:
```
gnuplot>splot x*y
```

`pwd`, `cd`, `exit`, `quit`, `print`

clear  清空输出的图像

`load`: 读取输入文件的每一行并执行,

`call` 命令  
命令格式:`call "<input-file>" <parameter-0> <parm-1> ... <parm-9>`  
call命令与load命令有相同的功能,但是call命令允许传入参数(最多10个),
例如有一个名为calltest.gp的文件,内容为:
	
	print "argc=$# p0=$0 p1=$1 p2=$2 p3=$3 p4=$4 p5=$5 p6=$6 p7=x$7x"
在命令行调用该文件:

	gnuplot>call 'calltest.gp' "abcd" 1.2 + "'quoted'" -- "$2"
将会显示:
	
	argc=7 p0=abcd p1=1.2 p2=+ p3='quoted' p4=- p5=- p6=$2 p7=xx
说明:`$#`的值为参数的个数 ,$n表示第(n+1)个参数,n为0-9的数字

`set`:
```
set encoding utf8
```

条件判断命令 if
语法:`if (<condition>) <command-line> [; else if (<condition>) ...; else …]`
	
	if(a>0)   print a;a=a-1;
if后还可接else ,else if命令,也必须和if写在同一行

# Style
in gnuplot prompt, Use the command `test` to see what is available.

[ref](http://gnuplot.sourceforge.net/docs_4.2/node231.html)

Default plotting styles are chosen with the set style data and set style function commands.
```
     set style function <style>
     set style data <style>
     show style function
     show style data
```

Default styles for specific plotting elements may also be set.
```
     set style arrow <n> <arrowstyle>
     set style fill <fillstyle>
     set style histogram <histogram style options>
     set style line <n> <linestyle>
```

- line types: solid, dash-dot, color, etc
- point types: circles, squares, crosses, etc

## Set style data

The set style data command changes the default plotting style for data plots.
```
     set style data <plotting-style>
     show style data
```

## Set style line
[demo](http://gnuplot.sourceforge.net/demo_canvas/dashcolor.html)
```
     set style line <index> default
     set style line <index> {{linetype  | lt} <line_type> | <colorspec>}
                            {{linecolor | lc} <colorspec>}
                            {{linewidth | lw} <line_width>}
                            {{pointtype | pt} <point_type>}
                            {{pointsize | ps} <point_size>}
                            {palette}
     unset style line
     show style line
```

# 函数
## 数学库函数 
```
函数     参数类型   返回
abs(x)       any          x的绝对值|x|,
abs(x)       complex     x的长度, sqrt( real(x)^2 + imag(x)^2 ) 
acos(x)     any         cos(x) 的反函数
acosh(x)     any cosh(x)的反函数
arg(x)       complex     the phase of x in radians
asin(x)      any         sin(x)的反函数
asinh(x)      any         sinh(x)的反函数
atan(x)      any         tan(x)的反函数
atan2(y,x)  int or real tan?1 (y/x) (inverse tangent)
atanh(x)       any tanh(x)的反函数
besj0(x)     radians     J0 Bessel function of x 
besj1(x)     radians     J1 Bessel function of x 
besy0(x)     radians     Y0 Bessel function of x 
besy1(x)     radians     Y1 Bessel function of x 
ceil(x)      any         不小于x的最小整数
cos(x)       radians     余弦
cosh(x)      radians     双曲余弦
erf(x)       any         Erf(real(x)), error function of real(x) 
erfc(x)      any         Erfc(real(x)), 1.0 - error function of real(x) 
exp(x)       any         e的x次方
floor(x)     any         不大于x的最大整数
gamma(x)     any         Gamma(real(x)),  gamma function of real(x) 
ibeta(p,q,x) any         Ibeta(real(p,q,x)), ibeta function of real(p,q,x) 
inverf(x)    any         inverse error function of real(x) 
igamma(a,x)  any         Igamma(real(a,x)), igamma function of real(a,x) 
imag(x)      complex     x的虚部,返回一个实数
invnorm(x)   any         inverse normal distribution function of real(x) 
int(x)       real        实数的整数部分,小数后面的部分被去除(非四舍五入)
lambertw(x)    real     Lambert W function
lgamma(x)    any         Lgamma(real(x)),  lgamma function of real(x) 
log(x)       any         ln(x),x的自然对数 ( 基为e)
log10(x)     any         log(x),x的以十为基的对数
norm(x)      any         normal distribution (Gaussian) function of real(x) 
rand(x)      any        Rand(real(x)),  pseudo random number generator 
real(x)      any        x的实部
sgn(x)       any       符号函数,x>0返回1,x<0返回－1,x=0返回0 若x为复数,则x的虚部(imag(x))将被忽略
sin(x)       radians     x的正弦
sinh(x)      radians     x的双曲正弦
sqrt(x)      any        x的开方
tan(x)       radians     x的正切
tanh(x)      radians     x的双曲正切
```

## 字符串函数
```
gprintf("format",x)        any        string result from applying gnuplot's format parser
sprintf("format",x,...)  multiple      string result from C-language sprintf
strlen("string")       string      int length of string
strstrt("string","key")   strings      int index of first character of substring "key"
substr("string",beg,end)  multiple      string "string"[beg:end]
system("command")        string       string containing output stream of shell command
word("string",n)    string, int returns the nth word in "string"
words("string")        string      returns the number of words in "string"
```

## 其他函数
```
column(x)         int        column x during datafile manipulation.
defined(X)    variable name    [DEPRECATED] returns 1 if X is defined, 0 otherwise.
exists("X")  "variable name" returns 1 if a variable named X is defined, 0 otherwise.
stringcolumn(x)      int        content of column x as a string.  timecolumn(x)        int        
timecolumn x during datafile manipulation.
tm_hour(x)          int        the hour
tm_mday(x)          int        the day of the month
tm_min(x)          int        the minute
tm_mon(x)          int        the month
tm_sec(x)         int        the second
tm_wday(x)          int        the day of the week
tm_yday(x)          int        the day of the year
tm_year(x)         int        the year
valid(x)         int        test validity of column(x) during datafile manip.
```

## 自定义函数
在gnuplot中可直接用f(x)=<expression> 来定义函数,<expression>为表达式,如:
```
gnuplot>f(x)=x*x;plot f(x)
```

也可以定义二维函数,如:
```
gnuplot>p(x,y)=sin(x)*cos(y);splot p(x,y)
```

# 
暂停命令 pause
命令语法:
pause <time> {"<string>"}
pause mouse {<endcondition>}^{ <endcondition>} {"<string>"}
对于第一种用法,<time>是时间,单位为秒,<string>为暂停是打印的字符串.
对于第二种用法,如果当前终端支持鼠标,pause mouse执行后,点击鼠标和按下ctrl+C都会使暂停结束,<endcondition>可以是 keypress, button1, button2, button3,any,keypress 为按键动作,
button1为鼠标的左键,button2为中键,button3为右键,any为任意动作,即按键动作或鼠标点击.
说明:对于pause <time> 若time为负值,则暂停直到点击 Enter键(return键),若time为0,则不暂停,
举例:
pause 1 'pause 1 second'  #暂停1秒,并打印 pause 1 second
pause 0 'no paue'  #此命令功能与print 'no paue' 功能相同
pause  -1 'Hit  return to continue '
pause mouse button1  #点击鼠标左键程序继续执行
pause mouse any "Any key or button will terminate"   #按任意键或点击鼠标程序将继续

# latex
将gnuplot与latex结合起来使用

文件sin.plt 的内容如下
```
set terminal latex size 7cm, 5cm
set output "sin.tex"  # 生成一个tex文件
set format xy "$%g$"
set title 'This is a plot of $y=\sin(x)$'
set xlabel 'This is the $x$ axis'
set ylabel 'This is\\the\\$y$ axis'
plot [0:6.28] [0:1] sin(x)
```
在gnuplot 的命令窗口执行 `load sin.plt`, 会生成`sin.tex` 文件.

在latex 中使用下面的代码来插入上面生成的图片
```
\begin{figure}
	\centering
	\input{sin}
\end{figure}
```

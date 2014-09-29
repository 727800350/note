保存变量var1, var2 ... 到文件filename.mat 中

	save filename var1 var2 ...
	load filename

# Script
和Matlab 一样, 使用m 文件
Octave 假定一个脚本文件的头几行注释是该脚本的描述,这几行注释也是你使用 help 命令时获得信息,因
此在你每个脚本的头几行写上有关该脚本的帮助信息是一个很好的习惯.

对于函数文件, 在`function out = name(in)`下一行写上注释

## Octave 可执行程序
	#!/usr/bin/octave -qf
	# a sample Octave program
	printf ("Hello, world!\n");

- '-q' 选项能够屏蔽程序在普通情况下执行时输出到标准输出的一些介绍性的信息, 
- '-f' 选项能够让程序忽略用户的在文件~/.octaverc 中的一些设置

Octave 程序在系统终端执行的时候, 还可以为其添加一些参数. 这样的参数通过 Octave 的内部变量 argv 存储. 例如一个例子:

	#!/bin/octave -qf
	printf ("%s\n", program_name ());
	arg_list = argv();
	for i = 1:nargin
		printf (" %s, ", arg_list{i});
	endfor
将上述内容保存为 test 文件中, 并为其添加可执行权限并执行之:

	kasion@tmp:$ ./test arg1 arg2
	那么程序的输出为:
	test, arg1, arg2

# Plot Figure
plot 命令中的颜色和样式选项 (见 help plot),(† 仅在 Matlab 中存在的选项)

|颜色	 |		| 		|
|--------|------|-------|
|w 白色 |. 点 |- 实线|
|m 品红 o| 圆圈 |: 虚线 †|
|c 青色 |xx形| -. 点划线 †|
|r 红色 +|+号 |– 虚线 †|
|g 绿色 |* 星号 ||
|b 蓝色 |s 正方形 † ||
|y 黄色 † |d 菱形 † ||
|k 黑色 † |v 下三角 † ||
||< 左三角 †||
||> 右三角 †||
||p 五角星 †||
||h 六边形 †||

Octave/Gnuplot 没有提供鼠标或者快捷键操作来实现图片的打印操作.然而 Octave 提供了 print 命令
来将图片打印到默认的打印机上.  
`help print` 命令给出所有的 print 命令提供的选项,包括

	octave:##> print('graph1.eps','-deps')
来保存当前图像到一个 eps 文件或者

	octave:##> print('graph1.png','-dpng')
将当前图像保存为一个 png 文件.

## 2D
### ezplot

	ezplot('sin(x)')
	ezplot('x^2 - y^2 - 1') // 双曲线, 不是z = x^2-y^2-1

## Function
Matlab 中数组下标从1开始, 而不是像很多种编程语言一样的从0 开始.

	function [y1,...,yN] = myfun(x1,...,xM)
The name of the file should match the name of the first function in the file.  
如果是多个返回值, 调用的时候必须用多个变量来接收函数的返回值.  
例如:`function [x,y]=fun(a,b)`, 使用的时候: `[x,y]=fun(2,3)`.

可以通过多个维度来返回多个值, 例如
	
	function y=f(a,b)
		y(1)=a+b;
		y(2)=a-b;
		y(3)=a*b
使用的时候就是`y=f(2,3)`  
参数也可以使用这种方式

## Solve Equation
### solve
exact solution

### fzero
get a numeric solution from the start value

### fsolve
	fsolve('m file', [initial], option)

### dsolve
get the exact solution of differential equation

	dsolve('eq1', 'eq2',...,'initial', 'variables')
`Dy, D2y` represents `y',y''`

If initial condition is not given, it means to get the general solution

### ode
	[t,x]=solver('f', ts, x0, options)

	t: variable
	x: function
	f: the m file for function to solve
	ts: the time interval
	x0: the initial value of x
solver can be `ode45, ode23, ode15s`

The first thing to try is use `ode45`, then try `ode15s`

## Optimization
fmincon: Find minimum of constrained nonlinear multivariable function

linprog: Solve linear programming problems

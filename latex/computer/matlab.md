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
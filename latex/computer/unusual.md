[adding two numbers without using +](http://stackoverflow.com/questions/11244510/adding-two-numbers-without-using)  
I have this code which does the trick:

	#include <stdio.h>
	int main(){
		int a = 30000, b = 20,sum;
		char *p;
		p=(char *)a;
		sum = (int)&p[b]; // adding a & b
		printf("%d",sum);
		return 0;
	}
When excuting to line 9, 

	(gdb) p p
	$1 = 0x7530 <Address 0x7530 out of bounds>
	(gdb) p *p
	Cannot access memory at address 0x7530
	(gdb) p (int)&p[0]
	$2 = 30000
	(gdb) p (int)&p[-1]
	$3 = 29999
	(gdb) p (int)&p[1]
	$4 = 30001
	(gdb) p (int)&p[20]
	$5 = 30020
	(gdb) p (int)&p[20000]
	$6 = 50000
	(gdb) p (int)&p[200000]
	$7 = 230000
我们可以看到即使我们不能够access `*p`, 我们仍然能够完成计算.

<br/>
I'm not sure if the following code can cause redundant calculations, or is it compiler-specific? 
	for(int i = 0; i < strlen(ss); i++){ 
		// blabla
	} 

So will `strlen()` be calculated everytime when i increases? 
Yes, `strlen()` will be evaluated on each iteration. 
It's possible that, under ideal circumstances, the optimiser might be able to deduce that the value won't change, but I personally wouldn't rely on that. 
I'd do something like 

	for(int i = 0, n = strlen(ss); i < n; i++) 
	or possibly 
	for(int i = 0; ss[i]; i++)


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

<br/>
[Why does sizeof(x++) not increment x?](http://stackoverflow.com/questions/8225776/why-does-sizeofx-not-increment-x)

	#include <stdio.h>
	int main(){
		int x = 5;
		printf("%d and ", sizeof(x++)); // note 1
		printf("%d\n", x); // note 2
		return 0;
	}
I expect x to be 6 after executing note 1. However, the output is:  
`4 and 5`  
Can anyone explain why x does not increment after note 1?

`sizeof` is a compile-time operator, 
so at the time of compilation sizeof and its operand get replaced by the result value. 
The operand is not evaluated (except when it is a variable length array) at all; only the type of the result matters.

	short func(short x) {  // this function never gets called !!
		printf("%d", x);    // this print never happens
		return x;
	}
	int main(){
		printf("%d", sizeof(func(3))); // all that matters to sizeof is the return type of the function.
		return 0;
	}
Output: `2`  
as short occupies 2 bytes on my machine.  
Changing the return type of the function to double:

	double func(short x) {// rest all same}
will give 8 as output.}

<br/>
[With C arrays, why is it the case that `a[5] == 5[a]`?](http://stackoverflow.com/questions/381542/with-c-arrays-why-is-it-the-case-that-a5-5a)  
The C standard defines the [] operator as follows:

	a[b] == *(a + b)

Therefore a[5] will evaluate to:

	*(a + 5)
and 5[a] will evaluate to:

	*(5 + a)
This is the direct artifact of arrays behaving as pointers.

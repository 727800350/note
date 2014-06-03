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

<br/>
[`int a[] = {1,2,};` Weird comma allowed. Any particular reason?](http://stackoverflow.com/questions/7043372/int-a-1-2-weird-comma-allowed-any-particular-reason)  
It makes it easier to generate source code, and also to write code which can be easily extended at a later date. Consider what's required to add an extra entry to:

	int a[] = {
	   1,
	   2,
	   3
	};
you have to add the comma to the existing line and add a new line.   
Compare that with the case where the three already has a comma after it, where you just have to add a line. Likewise if you want to remove a line you can do so without worrying about whether it's the last line or not, and you can reorder lines without fiddling about with commas. Basically it means there's a uniformity in how you treat the lines.

Now think about generating code. Something like (pseudo-code):

	output("int a[] = {");
	for (int i = 0; i < items.length; i++) {
	    output("%s, ", items[i]);
	}
	output("};");
No need to worry about whether the current item you're writing out is the first or the last. Much simpler.

It's useful if you do something like this:

	int a[] = {
	  1,
	  2,
	  3, //You can delete this line and it's still valid
	};

<br/>
[`int *ptr = (int*)(&a + 1);`](http://stackoverflow.com/questions/15141450/int-ptr-inta-1)  

	#include <stdio.h>
	int main(void){
	   int a[5] = { 1, 2, 3, 4, 5 };
	   int *ptr = (int*)(&a + 1);  // what happen here ?
	   printf("%d %d\n", *(a + 1), *(ptr - 1));
	   return 0;
	}
I expected the answer to be 1 but 'm getting 5 .. Why ?

在gdb中对上述程序进行调试, 得到:

	(gdb) p a
	$1 = {1, 2, 3, 4, 5}
	(gdb) p &a
	$2 = (int (*)[5]) 0x7fffffffe3d0 // 可以看出`&a` 是一个指向5个元素的数组
	(gdb) p &a[0]
	$3 = (int *) 0x7fffffffe3d0
	(gdb) p &*a
	$4 = (int *) 0x7fffffffe3d0
	(gdb) p *a
	$5 = 1

`a` = address of first element of array: `a[0]` (address of int)  
`&a` = address of array a,same value with "a", but **type is a 5 element array**,so expression "(&a + 1)" is pointer to next array of array a.  
`ptr` points to the first element of the next array of array a, whose value is `a+5`.  
`(ptr - 1)`: pointer to previous int of ptr, that means the pointer of last element of array "a".

<br/>
[Will the prototype of `a[1][2]` be this: `int **a`?](http://stackoverflow.com/questions/5643753/will-the-prototype-of-a12-be-this-int-a)  

	type **a; (pointer to pointer to type)
	type *a[n]; (array of n pointers to type)
	type (*a)[n]; (pointer to **array of n** type)
	type a[m][n]; (array of m arrays of n type)

Precisely how the expression is evaluated depends on which of these types a actually has.

First `a + 1` is calculated.  
If a is itself a pointer (either case 1 or case 3), then the value of a is directly loaded.  
If a is an array (case 2 or case 4), then the address of the first element of a is loaded (which is identical to the address of a itself).

This pointer is now offset by 1 object of the type that it points to.   
In case 1 and case 2, it would be offset by 1 "pointer to type" object;   
in case 3 and case 4, it would be offset by 1 "array of n type" object, which is the same as ofsetting by n type objects.

The calculated (offset) pointer is now dereferenced.   
In cases 1 and 2, the result has type "pointer to type",   
in cases 3 and 4 the result has type "array of n type".

Next `*(a + 1) + 2` is calculated.   
As in the first case, if `*(a + 1)` is a pointer, then the value is used directly (this time, cases 1 and 2).   
If `*(a + 1)` is an array (cases 3 and 4), then the address of the first element of that array is taken.

The resulting pointer (which, at this point, always has type "pointer to type") is now offset by 2 type objects.   
The final offset pointer is now dereferenced, and the type object is retrieved.

<br/>
[How do you set, clear and toggle a single bit in C/C++?](http://stackoverflow.com/questions/47981/how-do-you-set-clear-and-toggle-a-single-bit-in-c-c)  

Setting a bit to 1  
Use the bitwise OR operator (|) to set a bit.

	number |= 1 << x; // 将1 左移x 位
That will set bit x.

Clearing a bit  
Use the bitwise AND operator (&) to clear a bit.

	number &= ~(1 << x);
That will clear bit x. You must invert the bit string with the bitwise NOT operator (~), then AND it.

Toggling a bit, 将第x个(从右往左)二进制位取反  
The XOR operator (^) can be used to toggle a bit.

	number ^= 1 << x;
That will toggle bit x.

Checking a bit  
You didn't ask for this but I might as well add it.

To check a bit, AND it with the bit you want to check:

	bit = number & (1 << x);
That will put the value of bit x into the variable bit.

Nobody mentioned the Standard C++ Library: std::bitset<N>.  
Or the boost version: boost::dynamic_bitset.

No need to roll your own:

	#include <bitset>
	#include <iostream>
	int main(){
	    std::bitset<5> x;
	    x[1] = 1;
	    x[2] = 0;
	    // Note x[0-4]  valid
	    std::cout << x << std::endl;
	}
	[Alpha:] > ./a.out
	00010
Boost version allows a runtime sized bitset compared with STL compile time sized bitset.

<br/>
**Hello world in C with no semi-colons?**  

	#include <stdio.h>
	int main(){
		if(printf("Hello world.\n")){}
	}

<br/>
[fork() branches more than expected?](http://stackoverflow.com/questions/11132868/fork-branches-more-than-expected)  
	
	#include <stdio.h>
	#include <sys/types.h>
	#include <unistd.h>
	int main(void){
	    int i;
	    for(i = 0; i < 2; i++){
			fork();
	        printf(".");
	    }
	    return 0;
	}
This program outputs 8 dots. How can that be possible? Should not there be 6 dots instead?

at first, there is one process. That creates a second process, both of which print a dot and loop. On their second iteration, each creates another copy, so there are four processes print a dot, and then exit. So we can easily account for six dots, like you expect.

However, **what printf() really does is buffer its output**. So the first dot from when there were only two processes does not appear when written. Those dots remain in the buffer—which is duplicated at fork(). It is not until the process is about to exit that the buffered dot appears. Four processes printing a buffered dot, plus the new one gives 8 dots.

If you wanted to avoid that behavior, call fflush(stdout); after printf().

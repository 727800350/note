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

<br/>
[Solve the memory alignment in C interview question that stumped me](http://stackoverflow.com/questions/227897/solve-the-memory-alignment-in-c-interview-question-that-stumped-me/)  
I just finished a test as part of a job interview, and one question stumped me - even using google for reference. I'd like to see what the stackoverflow crew can do with it:

The “memset_16aligned” function requires a 16byte aligned pointer passed to it, or it will crash.  
a) How would you allocate 1024 bytes of memory, and align it to a 16 byte boundary?  
b) Free the memory after the memset_16aligned has executed.

	{
	   void *mem;
	   void *ptr;
	   // answer a) here
	   memset_16aligned(ptr, 0, 1024);
	   // answer b) here
	}

解决这个的思想: 多申请一点内存, 然后将内存向前移动15位(当malloc的返回值)
**Original answer**

	{
		void *mem = malloc(1024+16);
		void *ptr = ((char *)mem+16) & ~ 0x0F;
		memset_16aligned(ptr, 0, 1024);
		free(mem);
	}
& 报错
转化为int 来运算, 可以

	int(~0x0f): 0xfffffff0 前面补零, 是因为~0x0f 代表的十进制数是负的
	mem = 0x601010
	(char *)mem + 16: 0x601020
	(int)((char *)mem + 16): 0x601020 = 0x00601020
	(int)((char *)mem + 16) & (int)(~0x0f): 0x601020  
在16进制表示中, 最后一位是0, 所以是这个16进制数是16的倍数, 也就是说是对齐的

使用1024+15就可以满足要求

- 如果返回的地址是0x601011, 那么我们申请的内存区域就是[0x601011, 0x601011 + 1024 +15 -1]  
ptr = (int)((char *)0x601011 + 15) & 0xfffffff0 = 0x601020 & 0xfffffff0 = 0x601020,  
那么ptr 所指向的内存的长度就是: 0x601011 + 1024 + 15 - 1 - 0x601020 + 1 = 1024, 满足要求
- 如果返回的地址是0x60101f, 那么我们申请的内存区域就是[0x60101f, 0x60101f + 1024 +15 -1]  
ptr = (int)((char *)0x60101f + 15) & 0xfffffff0 = 0x60102e & 0xfffffff0 = 0x601020,  
那么ptr 所指向的内存的长度就是: 0x60101f + 1024 + 15 - 1 - 0x601020 + 1 = 1024 + 14, 满足要求
todo: 不是刚好1024, 有点问题

这里使用 0x0f, 是因为要求16Byte aligned, 这样取反之后, 并& 之后, 就可以保证以16进制表示时, 最后一位是0, 也就是整个数是16的倍数.  
如果是要求8, 那么应该是000111(o07)

**Fixed answer**

	{
		void *mem = malloc(1024+15);
		void *ptr = ((uintptr_t)mem+15) & ~ (uintptr_t)0x0F;
		memset_16aligned(ptr, 0, 1024);
		free(mem);
	}
Explanation as requested

The first step is to allocate enough spare space, just in case. Since the memory must be **16-byte aligned (meaning that the leading byte address needs to be a multiple of 16)**, adding 16 extra bytes guarantees that we have enough space.  
Somewhere in the first 16 bytes, there is a 16-byte aligned pointer. 
(Note that malloc() is supposed to return a pointer that is sufficiently well aligned for any purpose. 
However, the meaning of 'any' is primarily for things like basic types - long, double, long double, long long. 
When you are doing more specialized things, like playing with graphics systems, 
they can need more stringent alignment than the rest of the system - hence questions and answers like this.)

The next step is to convert the void pointer to a char pointer; 
GCC notwithstanding, you are not supposed to do pointer arithmetic on void pointers (and GCC has warning options to tell you when you abuse it).
Then add 16 to the start pointer. Suppose malloc() returned you an impossibly badly aligned pointer: 0x800001. Adding the 16 gives 0x800011. 
Now I want to **round down to the 16-byte boundary - so I want to reset the last 4 bits to 0**. 
0x0F has the last 4 bits set to one; therefore, ~ 0x0F has all bits set to one except the last four. 
Anding that with 0x800011 gives 0x800010. You can iterate over the other offsets and see that the same arithmetic works.

The last step, free(), is easy: you always, and only, return to free() a value that one of malloc(), calloc() or realloc() returned to you - 
anything else is a disaster. You correctly provided mem to hold that value - thank you. The free releases it.

Finally, if you know about the internals of your system's malloc package, you could guess that it might well return 16-byte aligned data 
(or it might be 8-byte aligned). 
If it was 16-byte aligned, then you'd not need to dink with the values. 
However, this is dodgy and non-portable -- other malloc packages have different minimum alignments, 
and therefore assuming one thing when it does something different would lead to core dumps. Within broad limits, this solution is portable.

Someone else mentioned posix_memalign() as another way to get the aligned memory; 
that isn't available everywhere, but could often be implemented using this as a basis. 
Note that it was convenient that the alignment was a power of 2; other alignments are messier.

One more comment - this code does not check that the allocation succeeded.

**Amendment**  
Windows Programmer pointed out that you can't do bit mask operations on pointers, and, indeed, GCC (3.4.6 and 4.3.1 tested) does complain like that. 
So, an amended version of the basic code - converted into a main program, follows.
I've also taken the liberty of **adding just 15 instead of 16**, as has been pointed out. 
I'm using `uintptr_t` since C99 has been around long enough to be accessible on most platforms. 
If it wasn't for the use of PRIXPTR in the printf() statements, it would be sufficient to `#include <stdint.h>` instead of using `#include <inttypes.h>`. 
[This code includes the fix pointed out by C.R., which was reiterating a point first made by Bill K a number of years ago, which I managed to overlook until now.]

	#include <assert.h>
	#include <inttypes.h>
	#include <stdio.h>
	#include <stdlib.h>
	
	static void memset_16aligned(void *space, char byte, size_t nbytes){
	    assert((nbytes & 0x0F) == 0);
	    assert(((uintptr_t)space & 0x0F) == 0);
	}
	
	int main(void){
	    void *mem = malloc(1024+15);
	    void *ptr = (void *)(((uintptr_t)mem+15) & ~ (uintptr_t)0x0F);
	    printf("0x%08" PRIXPTR ", 0x%08" PRIXPTR "\n", mem, ptr);
	    memset_16aligned(ptr, 0, 1024);
	    free(mem);
	    return(0);
	}

And here is a marginally more generalized version, which will work for sizes which are a power of 2:
	
	#include <assert.h>
	#include <inttypes.h>
	#include <stdio.h>
	#include <stdlib.h>
	
	static void memset_16aligned(void *space, char byte, size_t nbytes){
	    assert((nbytes & 0x0F) == 0);
	    assert(((uintptr_t)space & 0x0F) == 0);
	}
	
	static void test_mask(size_t align){
	    uintptr_t mask = ~(uintptr_t)(align - 1);
	    void *mem = malloc(1024+align-1);
	    void *ptr = (void *)(((uintptr_t)mem+align-1) & mask);
	    assert((align & (align - 1)) == 0);
	    printf("0x%08" PRIXPTR ", 0x%08" PRIXPTR "\n", mem, ptr);
	    memset_16aligned(ptr, 0, 1024);
	    free(mem);
	}
	
	int main(void){
	    test_mask(16);
	    test_mask(32);
	    test_mask(64);
	    test_mask(128);
	    return(0);
	}
To convert `test_mask()` into a general purpose allocation function, 
the single return value from the allocator would have to encode the release address, as several people have indicated in their answers.

Problems with interviewers

Uri commented: Maybe I am having [a] reading comprehension problem this morning, but if the interview question specifically says: 
"How would you allocate 1024 bytes of memory" and you clearly allocate more than that. Wouldn't that be an automatic failure from the interviewer?

My response won't fit into a 300-character comment...

It depends, I suppose. I think most people (including me) took the question to mean 
"How would you allocate a space in which 1024 bytes of data can be stored, and where the base address is a multiple of 16 bytes". 
If the interviewer really meant how can you allocate 1024 bytes (only) and have it 16-byte aligned, then the options are more limited.

Clearly, one possibility is to allocate 1024 bytes and then give that address the 'alignment treatment'; 
the problem with that approach is that the actual available space is not properly determinate (the usable space is between 1008 and 1024 bytes, 
but there wasn't a mechanism available to specify which size), which renders it less than useful.  
Another possibility is that you are expected to write a full memory allocator and ensure that the 1024-byte block you return is appropriately aligned. 
If that is the case, you probably end up doing an operation fairly similar to what the proposed solution did, but you hide it inside the allocator.  
However, if the interviewer expected either of those responses,
I'd expect them to recognize that this solution answers a closely related question, 
and then to reframe their question to point the conversation in the correct direction. 
(Further, if the interviewer got really stroppy, then I wouldn't want the job; 
if the answer to an insufficiently precise requirement is shot down in flames without correction, then the interviewer is not someone for whom it is safe to work.)

<br/>
[How to initialize an array in C](http://stackoverflow.com/questions/201101/how-to-initialize-an-array-in-c)  
I have a large array in C (not C++ if that makes a difference). I want to initialize all members to the same value. I could swear I once knew a simple way to do this. I could use memset() in my case, but isn't there a way to do this that is built right into the C syntax?

Unless that value is 0 (in which case you can omit some part of the initializer and the corresponding elements will be initialized to 0), there's no easy way.

Don't overlook the obvious solution, though:

	int myArray[10] = { 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 };
Elements with missing values will be initialized to 0:

	int myArray[10] = { 1, 2 }; //initialize to 1,2,0,0,0...
So this will initialize all elements to 0:

	int myArray[10] = { 0 }; //all elements 0
In C++, an empty initialization list will also initialize every element to 0:

	int myArray[10] = {}; //all elements 0 in C++
Remember that objects with static storage duration will initialize to 0 if no initializer is specified:

	static int myArray[10]; //all elements 0
**And that "0" doesn't necessarily mean "all-bits-zero"**, so using the above is better and more portable than memset(). (Floating point values will be initialized to +0, pointers to null value, etc.)

<br/>
[How dangerous is it to compare floating point values?](http://stackoverflow.com/questions/10334688/how-dangerous-is-it-to-compare-floating-point-values)  

First of all, floating point values are not "random" in their behavior. 
Exact comparison can and does make sense in plenty of real-world usages. 
But if you're going to use floating point you need to be aware of how it works. 
Erring on the side of assuming floating point works like real numbers will get you code that quickly breaks. 
Erring on the side of assuming floating point results have large random fuzz associated with them (like most of the answers here suggest) will get you code that appears to work at first but ends up having large-magnitude errors and broken corner cases.

First of all, if you want to program with floating point, you should read this:  
[What Every Computer Scientist Should Know About Floating-Point Arithmetic](http://docs.oracle.com/cd/E19957-01/806-3568/ncg_goldberg.html)  
Yes, read all of it. If that's too much of a burden, you should use integers/fixed point for your calculations until you have time to read it.

Edit: In particular, a magnitude-relative epsilon check should look something like:

	if (fabs(x-y) < K * FLT_EPSILON * fabs(x+y))
Where `FLT_EPSILON` is the constant from float.h (replace it with `DBL_EPSILON` for doubles or `LDBL_EPSILON` for long doubles) 
and K is a constant you choose such that the accumulated error of your computations is definitely bounded by K units in the last place 
(and if you're not sure you got the error bound calculation right, make K a few times bigger than what your calculations say it should be).

Finally, note that if you use this, some special care may be needed near zero, since FLT_EPSILON does not make sense for denormals. A quick fix would be to make it:

	if (fabs(x-y) < K * FLT_EPSILON * fabs(x+y) || fabs(x-y) < FLT_MIN)
and likewise substitute DBL_MIN if using doubles.

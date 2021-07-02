```c
/* helloworld.c */
#include <stdio.h>
int main() {
  printf("hello, world!\n");
}
```

1. Preprocessing

preprocessing the directives (e.g., #include and #define).
```bash
cpp helloworld.c > helloworld.i
```
The result is a file helloworld.i that contains the source code with all macros expanded.

2. Compilation

The compiler (ccl) translates helloworld.i into helloworld.s. File helloworld.s contains assembly code. 
```bash
gcc -S helloworld.i
```
The command line option `-S` tells the compiler to convert the preprocessed code to assembly language without creating
an object file. After having created helloworld.s you can see the content of this file. While looking at assembly code
you may note that the assembly code contains a call to the external function printf.

3. Assembly

the assembler (as) translates helloworld.s into machine language instructions, and generates an object file helloworld.o.
```bash
as helloworld.s -o helloworld.o
```
the resulting file contains the machine instructions for the classic "Hello World!" program, with an undefined reference
to printf.

4. Linking

This phase links object files to produce final executable file.
```bash
ld -dynamic-linker /lib64/ld-linux-x86-64.so.2 /usr/lib64/crt1.o /usr/lib64/crti.o /usr/lib64/crtn.o
/usr/lib/gcc/x86_64-redhat-linux/4.1.2/crtbegin.o
-L /usr/lib/gcc/x86_64-redhat-linux/4.1.2/ -lgcc -lgcc_eh -lc -lgcc -lgcc_eh \
/usr/lib/gcc/x86_64-redhat-linux/4.1.2/crtend.o
helloworld.o
-o helloworld
```

And, you can greet the universe as follows:
```bash
./helloworld
```


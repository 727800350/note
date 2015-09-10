# Intro
## FP
there was 2 main mindsets in terms of design and implementation of programming languages:

1. Start from the Von Neumann Architecture and add abstraction. 这个就是 imperative language, C家族
1. Start from Mathematics and remove abstraction. FP 家族.

Here is a (non-exhaustive) list of FP features:

1. First-Class Functions: mean that you can store functions into a variable
1. High-Order Functions: functions can return functions or receive other functions as params
1. Pure Functions: mean that the function does not change any value, it just receives data and output data, just like our beloved functions from Mathematics.
1. Closures: mean that you can save some data inside a function that is only accessible to a specific returning function, i.e the returning function keeps its execution environment.
1. Immutable State: means that you can not change any state at all (even though you can get a new state).

Lisp: List Processing

Guile implements Scheme as described in the Revised5Report on the Algorithmic Language Scheme (usually known as R5RS), providing clean and general data and control structures.
Guile goes beyond the rather austere language presented in R5RS, extending it with a module system, full access to POSIX system calls, networking support, multiple threads,
dynamic linking, a foreign function call interface, powerful string processing, and many other features needed for programming in the real world.

It is easy to call Scheme code from C code and vice versa, giving the application designer full control of how and when to invoke the interpreter. 
Applications can add new functions, data types, control structures, and even syntax to Guile, creating a domain-specific language tailored to the task at hand,
but based on a robust language design.

Since the 2.0, Guile architecture supports compiling any language to its core virtual machine bytecode, and Scheme is just one of the supported languages. 
Other supported languages are Emacs Lisp, ECMAScript (commonly known as Javascript) and Brainfuck, and work is under discussion for Lua, Ruby and Python.

## Start and exit
guile 的启动方式

- -s script arg...
- -c expr arg... ;; Evaluate expr as Scheme code, and then exit.
- -l file ;; Load Scheme source code from file, and continue processing the command line.
- -e function ;; Make function the entry point of the script. the return value of `(command-line)` will be the arguments of the function.

退出:`(quit)` or `(exit)`

## guile script

	#!/usr/bin/guile -s
	!#
	(display "Hello, world!")
	(newline)

当有多个options时:  
the meta switch `\` allows the Guile programmer to specify an arbitrary
number of options

	#!/usr/local/bin/guile \
	-e main -s
	!#

Start ex:

	guile -e main -s /u/jimb/ex4 foo
Load the file /u/jimb/ex4, and then call the function main, passing it the list `("/u/jimb/ex4" "foo")`

还可以用load过程来直接调用Scheme语言源文件并执行它,格式为:`(load "filename")`

注释:
标准的Scheme语言定义中没有多行注释,不过在它的实现中几乎都有.
在Guile中就有多行注释,以符号组合"#!"开始,以相反的另一符号组合"!#"结束,其中内容为注释,如:
```
#!
there are scheme comment area.
you can write mulity lines here.
!#
```
注意的是,符号组合"#!"和"!#"一定分做两行来写.

## Tips
Guile initialization file: `~/.guile`

To make it easier for you to repeat and vary previously entered expressions, or to edit
the expression that you are typing in, Guile can use the GNU Readline library. This is not
enabled by default because of licensing reasons, but all you need to activate Readline is the
following pair of lines.

	scheme@(guile-user)> (use-modules (ice-9 readline))
	scheme@(guile-user)> (activate-readline)
Put 
	(use-modules (ice-9 readline))
	(activate-readline)
in `~/.guile`

History

	(use-modules (ice-9 history))

# Data Types
The term **latent typing** is used to describe a computer language, such as Scheme, 
for which you cannot, in general, simply look at a program source code and determine what type of data will be associated with a particular variable, 
or with the result of a particular expression. 
Instead, **the types of variables and expressions are only known in general at run time**

**all values in Scheme – carry their type with them**.   
In other words, every value "knows," at runtime, what kind of value it is, a number, a string, a list, whatever.

- (define 变量名 值): 定义变量
- (set! 变量名 值): 改变变量的值

Scheme语言的变量类型不是固定的,可以随时改变.

数据比较

- eq?: 两个变量正好是同一个对象
- eqv?: 同 eq?
- equal?: 值相同

ex:

- `(eqv? null ''()) => #t`
- `(eqv? null (list)) => #t`

## 基本类型
include characters, strings, numbers and procedures. 

### boolean

- #t: true
- #f: false  
- not: 对 boolean 类型取反
- (boolean? value)

It is important to note that `#f` is not equivalent to any other Scheme value. 
In particular, `#f` is not the same as the number 0 (like in C and C++), and not the same as the "empty list" (like in some Lisp dialects).

### number
它又分为四种子类型:整型(integer),有理数型(rational),实型(real),复数型(complex)

`#b, #o, #d, #x`(二进制,八进制,十进制和十六进制), 来作为表示数字进制类型的前缀,其中表示十进制的#d可以省略不写,

- `(complex? (define c 3+2i))`
- `(real? (define f 22/7))`: 就是 22/7, 不会把它近似
- `(retaional? (define p 3.1415))`
- `(integer? (define i 123))`
- `(number? value)`

数学运算

- max, min, abs
- reminder, modulo
- gcd, lcm
- (complex? arg)
- (odd? arg)
- (positive? arg)
- (negative? arg)
- (pair? arg)

### char
字符型数据均以符号组合"#\" 开头, 表示单个字符
例如, `#\A` 表示大写字母A, `#\0`表示字符0, `#\space` 表示空格符和 `#\newline` 表示换行符

- `(char? value)`

### symbol 符号型
符号类型是Scheme语言中有多种用途的符号名称,它可以是单词,用括号括起来的多个单词,也可以是无意义的字母组合或符号组合,
它在某种意义上可以理解为C中的枚举类型

- `(define a (quote xyz))`: 定义变量a为符号类型,值为xyz
- `(define xyz ''a)`: 定义变量xyz为符号类型,值为a, 单引号'' 与quote是等价(**注意, 这些''都表示单引号, 由于vim 高亮的缺陷**)
- `(symbol? value)`
- `(null? '()) => #t`, null意为空类型,它表示为 '() ,即括号里什么都没有的符号

符号类型与字符串不同的是符号类型不能象字符串那样可以取得长度或改变其中某一成员字符的值,但二者之间可以互相转换.

## 复合类型
### String

- (string-append str1 ... strn)

- (string-length str)

- (string-set! string index char): 更改string 中index 位置的字符为char( string 中下标从 0 开始)
- (string-ref! string index): 取得string 中index 位置的字符

- (string=? argument)

- (number->string argument)
- (string->number argument)

### 点对(pair)
是由一个点和被它分隔开的两个所值组成的.形如: (1 . 2) 或 (a . b), 注意的是点的两边有空格.
这是最简单的复合数据类型,同是它也是其它复合数据类型的基础类型, 如列表类型(list)就是由它来实现的.
按照Scheme语言说明中的惯例,以下我们用符号组合 "=>" 来表示表达式的值.

它用cons来定义,如: `(cons 8 9) => (8 . 9)`

其中在点前面的值被称为 car ,在点后面的值被称为 cdr ,car和cdr同时又成为取pair的这两个值的过程,如:
```
(define p (cons 4 5)) => (4 . 5)
(car p)	=> 4
(cdr p)	=> 5
```
还可以用set-car! 和 set-cdr! 来分别设定这两个值:
```
(set-car! p "hello")
(set-cdr! p "good")
```
如此,以前定义的 p 又变成了 `("hello" . "good")` 这个样子了

### 列表(list)
列表是由多个相同或不同的数据连续组成的数据类型,它是编程中最常用的复合数据类型之一,很多过程操作都与它相关

- `(list)` 表示一个空的列表, 可以简写为 `''()`
- `(define liste (list 1 2 3 4 ))`
- `(length liste)`
- `(list-ref liste index)`
- `(list-set! liste index value)`
- `(define y (make-list 5 6)) => (6 6 6 6 6)`: 创建列表, make-list用来创建列表,第一个参数是列表的长度,第二个参数是列表中添充的内容
- `(append arg list)`: append arg to list, note the difference between cons and append from the example below

(apply procedure list)

map的功能和apply有些相似,它的第一个参数也必需是一个过程,随后的参数必需是多个列表,返回的结果是此过程来操作列表后的值
(map + (list 1 2 3) (list 4 5 6)) => (5 7 9)
(map car (list (a b) (c d) (e f))) => (a c e)


还可以实现多重列表,即列表的元素也是列表,如:(list (list 1 2 3) (list 4 5 6)).

list 和 pair 的关系

```
(define a (cons 1 (cons 2 (cons 3 (list)))))
a => (1 2 3)
(list? a) => #t
(pair? a) => #t
```
由上可见,a本来是我们上面定义的点对,最后形成的却是列表.事实上列表是在点对的基础上形成的一种特殊格式.
事实上列表是在点对的基础上形成的一种特殊格式.
list是pair的子类型,list一定是一个pair,而pair不是list

list
```
(define liste (list 1 2 3))
(car liste) => 1
(caar liste) => fail
(cdr liste) => ''(2 3)
(cddr liste) => ''(3)
(cdddr liste) => ''()
(cddddr liste) => fail
```

pair
```
(define p (cons 1 (cons 2 3)))
p => ''(1 2 . 3)
(car p) => 1
(caar p) => fail
(cdr p) => ''(2 . 3)
(cddr p) => 3
(cdddr p) => fail
```
cadr,cdddr等过程是专门对PAIR型数据再复合形成的数据操作的过程,最多可以支持在中间加四位a或d,如cdddr,caaddr等

### 向量(vector)
可以说是一个非常好用的类型, 是一种元素按整数来索引的对象, 异源的数据结构, 在占用空间上比同样元素的列表要少.
vector是一种比较常用的复合类型,它的元素索引从0开始,至第 n-1 结束,这一点有点类似C语言中的数组.

创建vector

- (define v (vector 3 4 5))
- (define v #(3 4 5))

操作

- (vector-ref vecteur index)
- (vector-length vecteur)
- (vector-set! vecteur index value)
- (make-vector num value)

## 类型转换

- `(number->string 123) => "123"`: 数字转换为字符串
- `(string->number "456") => 456`: 字符串转换为数字
- `(char->integer #\a) => 97`: 字符转换为整型数,小写字母a的ASCII码值为96
- `(char->integer #\A) => 65`: 大写字母A的值为65
- `(integer->char 97) => #\a`: 整型数转换为字符
- `(string->list "hello") => (#\h #\e #\l #\l #\o)`: 字符串转换为列表
- `(list->string (make-list 4 #\a)) => "aaaa"`: 列表转换为字符串
- `(string->symbol "good") => good`: 字符串转换为符号类型
- `(symbol->string ''better) => "better"`: 符号类型转换为字符串

## 过程 procedure
在Scheme语言中, 过程相当于C语言中的函数, 不同的是Scheme语言过程是一种数据类型, 这也是为什么Scheme语言将程序和数据作为同一对象处理的原因

Scheme语言中可以用lambda来定义过程,其格式如下:
```
(define 过程名 (lambda (参数 ...) (操作过程 ...)))
```
我们可以自定义一个简单的过程,如下: `(define add5 (lambda (x) (+ x 5)))`.  
`(lambda (x) (+ x 5))` 实际上就是一个匿名过程.

既然过程是一种数据类型,所以将过程作为过程的参数是完全可以的
```
(define isp
	(lambda (x)
		(if (procedure? x) 'isaprocedure 'notaprocedure)))
```

过程的嵌套定义
在Scheme语言中,过程定义也可以嵌套,一般情况下,过程的内部过程定义只有在过程内部才有效,相当C语言中的局部变量.
如下面的代码的最终结果是50:
```
(define fix 
	(lambda (x y z)
		(define add 
			(lambda (a b) (+ a b)))
		(- x (add y z))))
(display (fix 100 20 30))
```

此时过程add只在fix过程内部起做用,这事实上涉及了过程和变量的绑定,可以参考下面的关于过程绑定(let,let* 和letrec)的介绍.

# 控制结构
块(form)是Scheme语言中的最小程序单元,一个Scheme语言程序是由一个或多个form构成.
没有特殊说明的情况下 form 都由小括号括起来

Scheme语言中只有if结构是系统原始提供的, 其它的cond, case, and, or, 另外还有do,when,unless等都是可以用宏定义的方式来定义的.
这一点充分体现了Scheme的元语言特性, 关于do,when等结构的使用可以参考R5RS.

## 顺序结构
也可以说成由多个form组成的form, 用begin来将多个form放在一对小括号内, 最终形成一个form.

格式为:`(begin form1 form2 ...)`

如用Scheme语言写成的经典的helloworld程序是如下样子的:
```
(begin 
	(display "Hello world!")
	(newline))
```

## if cond and case
- `(if condition action-true action-false)`
- `(if condition action-true)`

Scheme语言中的cond结构类似于C语言中的switch结构
`(cond ((测试) 操作) ... (else 操作))`

case结构和cond结构有点类似
`(case (表达式) ((值) 操作))	... (else 操作)))`

## and 与 or
- `(and arg1 ... argn)`: 后面的参数的表达式的值都为#t时,它的返回值才为#t,否则为#f
- `(or arg1 ... argn)`: 后面的参数的表达式的值有一个为#t时, 返回值为#t,否则为#f

## loop
在Scheme语言中没有循环结构, 不过循环结构可以用递归来很轻松的实现(在Scheme语言中只有通过递归才能实现循环)
```
(define loop
	(lambda(x y)
		(if (<= x y)
			(begin 
				(display x)
				(display " ")
				(set! x (+ x 1))
				(loop x y)))))

(loop 1 10) ;; print from 1 to 10
```

## let let* letrec
(let ((...)...) ...)
(let ((x 2) (y 4)) (+ x y)))

# IO
`(command-line)`: 得到a list of strings  
eg: `./choose 1 4`: `(command-line)` 返回的是a **list of strings**: `("./choose" "1" "4")`, 然后`cadr` 得到 "1", `caddr`得到"4"

## input
打开一个输入文件后,返回的是输入端口,可以用read过程来输入文件的内容:

- read: 读入一个 s-expression, `(define x (read))`
- read-char: 读入一个 char
- read-line: 读入一行

## output
- (display x)
- (newline)
- (write 对象 端口)
- write-char

## File
Scheme语言中输入输出中用到了端口的概念,相当于C中的文件指针,也就是Linux中的设备文件,请看下面的操作
	
- (current-input-port): stdin
- (current-output-port): stdout
- input-port?, output-port?: 判断是否为输入或者输出文件
- open-input-file, open-output-file
- close-input-port, close-output-port

```
(define input (open-input-file "input"))
(read input)
(close-input-port input)

(define output (open-output-file "output"))
(display var output)
(newline output)
(close-output-port output)
```

- delete-file

## Procedures
The value of the procedure invocation expression is the value of the last evaluated expression
in the procedure body.

The side effects of calling the procedure are the combination of the
side effects of the sequence of evaluations of expressions in the procedure body.

One of the great simplifications of Scheme is that a procedure is just 
another type of value,
and that procedure values can be passed around and stored in variables in exactly the same
way as, for example, strings and lists.

**Note** that this is quite different from many dialects of Lisp — including Emacs Lisp —
in which a program can use the same name with two quite separate meanings: one meaning
identifies a Lisp function, while the other meaning identifies a Lisp variable, whose value
need have nothing to do with the function that is associated with the first meaning. In
these dialects, functions and variables are said to live in different namespaces.  
**In Scheme, on the other hand, all names belong to a single unified namespace**, and the
variables that these names identify can hold any kind of Scheme value, including procedure
values.

top level variable

### Creating and Using a New Procedure
	(lambda (argument1 argument2) expression ...)
创建的lambda 表达式是一个procedure, 只不过没有名字, 例如:

	(lambda (name address)
		(string-append "Name=" name ":Address=" address))
定义之后可以直接作用在参数上, 例如:	
	
	((lambda (name address)
		(string-append "Name=" name ":Address=" address))
	"FSF" "Cambridge")
	
**store the procedure value in a variable**

	(define make-combined-string
		(lambda (name address)
			(string-append "Name=" name ":Address=" address)))
然后就可以使用: `(make-combined-string "FSF" "Cambridge")`

#### Lambda Alternatives
Since it is so common in Scheme programs to want to create a procedure and then store it
in a variable, there is an alternative form of the define syntax that allows you to do just
that.
A define expression of the form

	(define (name [arg1 [arg2 ...]])
		expression ...)
is exactly equivalent to the longer form

	(define name
		(lambda ([arg1 [arg2 ...]])
			expression ...))
例如对上面的进行重定义:

	(define (make-combined-string name address)
		(string-append "Name=" name ":Address=" address))
		

**variable number of arguments:**

	(lambda (arg1 ... . args) expression ...)
	(lambda args expression ...)
The corresponding forms of the alternative define syntax are:

	(define (name arg1 ... . args) expression ...)
	(define (name . args) expression ...)

## Side effect
Of the expressions that we have met so far, `define` and `set!` expressions have side
effects 
— the creation or modification of a variable — but no value;   
`lambda` expressions
have values — the newly constructed procedures — but no side effects;   
and procedure invocation expressions, in general, have either values, or side effects, or both.

**Variables**

	(define x 5)
	(display "original x: ")(display x)(newline) ;; x = 5
	
	(define y x)
	(display y)(newline) ;; y = 5
	
	(set! x "now, a string")
	(display "resetting x: ")(display x)(newline) ;; x = "now, a string"
	
	(display "y: ")(display y)(newline) ;; y = 5
	
**Porcedure**:	
	
	(display (string-append "/home" "/" "eric"))(newline) ;; /home/eric
	
	(define t string-append)
	(display (t "/home" "/" "eric"))(newline) ;; /home/eric
	
	(define string-append string-length)
	(display (string-append "/home"))(newline) ;; 5
	
	(display (t "/home" "/" "eric"))(newline) ;; /home/eric
从上面的结果我们可以看到, procedure 的改变也没有side effect

# Expressions and Evaluation
## Evaluating Special Syntactic Expressions
When a procedure invocation expression is evaluated, the procedure and all the argument
expressions must be evaluated before the procedure can be invoked.  
Special syntactic expressions 不遵循这个规定.

Why is this needed? Consider a program fragment that asks the user whether or not to
delete a file, and then deletes the file if the user answers yes.

	(if (string=? (read-answer "Should I delete this file?") "yes")
		(delete-file file))
这里, 如果最外面的if expression 是一个procedure invocation的话, 那么他的参数就必须在它之前执行, 也就是说`(delete-file file)`要执行, 
如果是这样的话, 就起不到向用户确认删除文件的作用.  
Therefore `if` must be special syntax, not a procedure. 

Other special syntaxes that we have already met are `define, set!` and `lambda`. 
define and set! are syntax because they need to know the variable name that is given as the first argument in a define or set!
expression, not that variable's value. 
lambda is syntax because it does not immediately evaluate the expressions that define the procedure body;

## Tail calls
获取list 中的最后一个元素

	(define my-last
	  (lambda (lst)
		(if (null? (cdr lst))
		  (car lst)
		  (my-last (cdr lst)))))

	(print (my-last '(1 2 3)))

A proper tail call is only available from certain contexts, namely the following special form positions,

- and — last expression
- begin — last expression
- case — last expression in each clause
- cond — last expression in each clause, and the call to a => procedure is a tail call
- do — last result expression
- if — "true" and "false" leg expressions
- lambda — last expression in body
- let, let*, letrec, let-syntax, letrec-syntax — last expression in body
- or — last expression

The following core functions make tail calls,

- apply — tail call to given procedure
- call-with-current-continuation — tail call to the procedure receiving the new continuation
- call-with-values — tail call to the values-receiving procedure
- eval — tail call to evaluate the form
- string-any, string-every — tail call to predicate on the last character (if that point is reached)

# The Concept of Closure
The concept of closure is the idea that a lambda expression "captures" the variable bindings
that are in lexical scope at the point where the lambda expression occurs. The procedure
created by the lambda expression can refer to and mutate the captured bindings, and the
values of those bindings persist between procedure calls.

"creating a variable" is in fact establishing an association between a
name, or identifier, that is used by the Scheme program code, and the variable location to
which that name refers.

A collection of associations between names and locations is called an **environment**. When
you create a top level variable in a program using define, the name-location association
for that variable is added to the **top level environment**. The "top level" environment also
includes name-location associations for all the procedures that are supplied by standard
Scheme.

It is often useful to create variables that are more limited in their
scope, typically as part of a procedure body. In Scheme, this is done using the let syntax,
or one of its modified forms let* and letrec.

## Closure
Consider a let expression that does not contain any lambdas:

	(let ((s (/ (+ a b c) 2)))
		(sqrt (\* s (- s a) (- s b) (- s c))))
When the Scheme interpreter evaluates this, it

- creates a new environment with a reference to the environment that was current when it encountered the let
- creates a variable binding for s in the new environment, with value given by (/ (+ a b c) 2)
- evaluates the expression in the body of the let in the context of the new local envi- ronment, and remembers the value V
- forgets the local environment
- continues evaluating the expression that contained the let, using the value V as the value of the let expression, in the context of the containing environment.

After the `let` expression has been evaluated, the local environment that was created is
simply forgotten, and there is no longer any way to access the binding that was created in
this environment. If the same code is evaluated again, it will follow the same steps again,
creating a second new local environment that has no connection with the first, and then
forgetting this one as well.

If the **`let` body contains a lambda expression**, however, the local environment is not
forgotten. Instead, it becomes associated with the procedure that is created by the lambda
expression, and is reinstated every time that that procedure is called. In detail, this works
as follows.

- When the Scheme interpreter evaluates a lambda expression, to create a procedure object, it stores the current environment as part of the procedure definition.
- Then, whenever that procedure is called, the interpreter reinstates the environment that is stored in the procedure definition and evaluates the procedure body within the context of that environment.

The result is that the procedure body is always evaluated in the context of the environ-
ment that was current when the procedure was created.
This is what is meant by closure.

## Examples
**Example 1: A Serial Number Generator**

	(define (make-serial-number-generator)
		(let ((current-serial-number 0))
			(lambda ()
				(set! current-serial-number (+ current-serial-number 1))
					current-serial-number)))
	(define entry-sn-generator (make-serial-number-generator))

	(define entry-sn-generator (make-serial-number-generator))
	(print (entry-sn-generator)) ;; 1
	(print (entry-sn-generator)) ;; 2
	
	(define entry-generator (make-serial-number-generator))
	(print (entry-generator)) ;; 1
	(print (entry-generator)) ;; 2
	(print (entry-generator)) ;; 3


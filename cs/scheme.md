Lisp: List Processing

Guile implements Scheme as described in the Revised5Report on the Algorithmic Language
Scheme (usually known as R5RS), providing clean and general data and control structures.
Guile goes beyond the rather austere language presented in R5RS, extending it with a
module system, full access to POSIX system calls, networking support, multiple threads,
dynamic linking, a foreign function call interface, powerful string processing, and many
other features needed for programming in the real world.

It is easy to call Scheme code 
from C code and vice versa, giving the application designer full control of how and when to
invoke the interpreter. Applications can add new functions, data types, control structures,
and even syntax to Guile, creating a domain-specific language tailored to the task at hand,
but based on a robust language design.

Since the 2.0, Guile's architecture supports compiling any language to its core virtual machine bytecode, and Scheme is just one of the supported languages. 
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

## Tips
Guile initialization file: `~/.guile`

To make it easier for you to repeat and vary previously entered expressions, or to edit
the expression that you’re typing in, Guile can use the GNU Readline library. This is not
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

# API
## Syntax
- #t: true
- #f: false  
It is important to note that `#f` is not equivalent to any other Scheme value. In particular,
`#f` is not the same as the number 0 (like in C and C++), and not the same as the “empty
list” (like in some Lisp dialects).

	(if condition action-true action-false)
	(if condition action-true)
	
	(cond ((测试) 操作) ... (else 操作))
	(case (表达式) ((值) 操作))	... (else 操作)))

	(and arg1 ... argn)

	(let ((...)...) ...)
	(let ((x 2) (y 4)) (+ x y)))

在Scheme语言中没有循环结构, 不过循环结构可以用递归来很轻松的实现(在Scheme语言中只有通过递归才能实现循环)
	
	(define loop
		(lambda(x y)
			(if (<= x y)
				(begin 
					(display x) (display " ") (set! x (+ x 1)) (loop x y)))))
	(loop 1 10) ;; print from 1 to 10

## IO
`(command-line)`: 得到a list of strings  
eg: `./choose 1 4`: `(command-line)` 返回的是a **list of strings**: `("./choose" "1" "4")`, 然后`cadr` 得到 "1", `caddr`得到"4"

	(display x)
	(newline)
	
	;; 定义一个print, 在末尾添加一个换行符
	;; todo: 变长参数
	(define print
  		(lambda (para)
        	(display para)(newline)))

Scheme语言中也提供了相应的输入输出功能,是在C基础上的一种封装.

**端口**  
Scheme语言中输入输出中用到了端口的概念,相当于C中的文件指针,也就是Linux中的设备文件,请看下面的操作
	
	guile> (current-input-port)
	#<input: standard input /dev/pts/0> ;当前的输入端口
	guile> (current-output-port)
	#<output: standard output /dev/pts/0> ;当前的输出端口

判断是否为输入输出端口,可以用:`input-port?` 和`output-port?`

`open-input-file,open-output-file,close-input-port,close-output-port`这四个过程用来打开和关闭输入输出文件,其中打开文件的参数是文件名字符串,关闭文件的参数是打开的端口.

### 输入
打开一个输入文件后,返回的是输入端口,可以用read过程来输入文件的内容:

	guile> (define port (open-input-file "readme"))
	guile> port
	#<input: readme 4>
	guile> (read port)
	GUILE语言
上面的操作打开了readme文件,并读出了它的第一行内容.

此外还可以直接用read过程来接收键盘输入,如下面的操作:

	guile> (read)  ; 执行后即等待键盘输入
	12345
	12345
	guile> (define x (read))  ; 等待键盘输入并赋值给x
	12345
	guile> x
	12345
以上为用read来读取键入的数字,还可以输入字符串等其它类型数据:

	guile> (define name (read))
	tomson
	guile> name
	tomson
	guile> (string? name)
	#f
	guile> (symbol? name)
	#t
此时输入的tomson是一个符号类型,因为字符串是用引号引起来的,所以出现上面的情况.下面因为用引号了,所以`(string? str)`返回值为`#t`.

	guile> (define str (read))
	"Johnson"
	guile> str
	"Johnson"
	guile> (string? str)
	#t

还可以用load过程来直接调用Scheme语言源文件并执行它,格式为:`(load "filename")`,还有`read-char`过程来读单个字符等等.

### 输出
常用的输出过程是display,还有write,它的格式是
	
	(write 对象 端口)
这里的对象是指字符串等常量或变量,端口是指输出端口或打开的文件.下面的操作过程演示了向输出文件temp中写入字符串"helloworld",并分行的实现.
	guile> (define port1 (open-output-file "temp"))  ; 打开文件端口赋于port1
	guile> port1
	#<output: temp 3> 
	guile> (output-port? port1)
	#t                     ; 此时证明port1为输出端口
	guile> (write "hello world" port1)
	guile> (close-output-port port1)
	guile> (exit)               ; 写入数据并关闭退出
	[root@toymouse test]# more temp          显示文件的内容,达到测试目的
	"hello world"

## String
	(string-append str1 ... strn)
	(string-length str)
	(string=? argument)
	(number->string argument)
	(string->number argument) ;; 但是没有string->integer 等

## List
构造list的方式举例:

- `(list 1 2 3)`
- `'(1 2 3)`

	(car list) ;; get the first element
	(cdr list) ;; get the rest of the list except the first element
	(cadr list) ;; (car (cdr list))
	(caddr list) ;; (car (cdr (cdr list)))
	(cons arg1 arg2) ;; concatenate
	(append arg list) ;; append arg to list, note the difference between cons and append from the example below
	(length list) ;; get the length of the list, eg:(length (list 2 3 4)) ;; return 3

	(apply procedure list) ;; 

	map的功能和apply有些相似,它的第一个参数也必需是一个过程,随后的参数必需是多个列表,返回的结果是此过程来操作列表后的值
	guile> (map + (list 1 2 3) (list 4 5 6))
	(5 7 9)
	guile> (map car '((a b)(c d)(e f)))
	(a c e)

**Demo**

	guile> (define list '(1 2 3))
	guile> (car list)
	1
	guile> (cdr list)
	(2 3)
	guile> (cdr '(4))
	()
	guile> (cdr '())
	error: Wrong type (expecting pair): ()

	guile> (cons 0 list)
	(0 1 2 3)
	guile> (cons '(7 8) list)
	((7 8) 1 2 3)
	guile> (cons 1 2)
	(1 . 2)
	guile> (cons 1 2 3)
	error: Wrong number of arguments to #<primitive-procedure cons>
	guile> (cons (cons 1 2) 3)
	((1 . 2) . 3)

	guile> (append '(7 8) list)
	(7 8 1 2 3)

## ?

	(null? arg)
	(list? arg)
	(number? arg)
	(equal? arg) (eq? arg)

	(integer? arg) Return #t if x is an exact or inexact integer number, else return #f.
Ex:

	(integer? 487) #t
	(integer? 3.0) #t
	(integer? -3.4) #f

	(odd? arg)
	(positive? arg)
	(negative? arg)
	(pair? arg)

## Math

	max, min
	abs
	reminder, modulo
	gcd, lcm
	(complex? arg)

## File system
	(delete-file file)

## Other

# Data Types, Values and Variables
**latent typing**  
The term latent typing is used to describe a computer language, such as Scheme, for which
you cannot, in general, simply look at a program's source code and determine what type
of data will be associated with a particular variable, or with the result of a particular
expression.  
Instead, the types of variables and expressions are only known – in general – at run time.

## Values and Variables
**Primitive types**:
include characters, strings, numbers and procedures. 

**Compound types**: allow *a group of primitive and compound values* to be stored together,
include lists, pairs, vectors and multi-dimensional arrays. 

	#(1 2 3) is a three-element vector

In addition, Guile allows applications to define their own data types, 
with the same status as the built-in standard Scheme types.

**all values in Scheme – carry their type with them**.   
In other words, every value "knows," at runtime, what kind of value it is, 
a number, a string, a list, whatever.

### Defining and Setting Variables
	(define variable-name value)
	
For example(;; 表示注释):

	;; Make a variable 'x' with initial numeric value 1.
	(define x 1)
	
	;; Make a variable 'organization' with an initial string value.
	(define organization "Free Software Foundation")
	
	;; change value, new-value can have different data type
	(set! variable-name new-value)
	
In general, however, **value and new-value can be any Scheme expression**.

	(set! x (+ x 1))

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
Consider a let expression that doesn't contain any lambdas:

	(let ((s (/ (+ a b c) 2)))
		(sqrt (* s (- s a) (- s b) (- s c))))
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

# History
The Emacs thesis is that it is delightful to create composite
programs based on an orthogonal kernel written in a low-level language together with a
powerful, high-level extension language.

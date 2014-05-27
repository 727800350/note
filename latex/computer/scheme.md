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

Since the 2.0 release, Guile’s architecture 
supports compiling any language to its core virtual
machine bytecode, and Scheme is just one of the supported languages. 
Other supported languages are 
Emacs Lisp, ECMAScript (commonly known as Javascript) and Brainfuck,
and work is under discussion for Lua, Ruby and Python.


**guile script**

	#!/usr/bin/guile -s
	!#
	(display "Hello, world!")
	(newline)

Linking Guile into Programs
\todo{to be tested}

# API

## IO
	(display x)
	(newline)
	
	;; 定义一个print, 在末尾添加一个换行符
	;; todo: 变长参数
	(define print
  		(lambda (para)
        	(display para)(newline)))

## String
	(string-append str1 ... strn)
	(string-length str)
	(string=? argument)

## List
	(car list) ;; get the first element
	(cdr list) ;; get the rest of the list except the first element
	(cons arg1 arg2) ;; concatenate
	(append arg list) ;; append arg to list, note the difference between cons and append from the example below

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

## File system
	(delete-file file)

## Other
	(and arg1 ... argn)

# Data Types, Values and Variables
**latent typing**  
The term latent typing is used to describe a computer language, such as Scheme, for which
you cannot, in general, simply look at a program’s source code and determine what type
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
In other words, every value “knows,” at runtime, what kind of value it is, 
a number, a string, a list, whatever.

### Defining and Setting Variables
	(define variable-name value)
	
For example(;; 表示注释):

	;; Make a variable ‘x’ with initial numeric value 1.
	(define x 1)
	
	;; Make a variable ‘organization’ with an initial string value.
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
expression, not that variable’s value. 
lambda is syntax because it does not immediately evaluate the expressions that define the procedure body;

## Tail calls

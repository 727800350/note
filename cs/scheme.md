# Intro
## FP
there was 2 main mindsets in terms of design and implementation of programming languages:

1. Start from the Von Neumann Architecture and add abstraction. 这个就是 imperative language, C家族
1. Start from Mathematics and remove abstraction. FP 家族.

Here is a (non-exhaustive) list of FP features:

1. **First-Class Functions**: mean that you can store functions into a variable
1. **High-Order Functions**: functions can return functions or receive other functions as params
1. **Pure Functions**: mean that the function does not change any value, it just receives data and output data, just like our beloved functions from Mathematics.
1. **Closures**: mean that you can save some data inside a function that is only accessible to a specific returning function, i.e the returning function keeps its execution environment.
1. **Immutable State**: means that you can not change any state at all (even though you can get a new state).

## Start and exit
guile 的启动方式

- -s script arg...
- -c expr arg... ;; Evaluate expr as Scheme code, and then exit.
- -l file ;; Load Scheme source code from file, and continue processing the command line.
- -e function ;; Make function the entry point of the script. the return value of `(command-line)` will be the arguments of the function.

退出:`(quit)` or `(exit)`

## guile script
```scheme
#!/usr/bin/guile -s
!#
(display "Hello, world!")
(newline)
```
当有多个options时:  
```scheme
#!/usr/local/bin/guile -e main -s
!#
```
Start ex:`guile -e main -s /u/jimb/ex4 foo`  
Load the file /u/jimb/ex4, and then call the function main, passing it the list `("/u/jimb/ex4" "foo")`

还可以用load过程来直接调用Scheme语言源文件并执行它,格式为:`(load "filename")`

注释:
单行用`;`, 标准的Scheme语言定义中没有多行注释,不过在它的实现中几乎都有.
在Guile中就有多行注释,以符号组合"#!"开始,以相反的另一符号组合"!#"结束.

## Tips
Guile initialization file: `~/.guile`.  
To make it easier for you to repeat and vary previously entered expressions, or to edit the expression that you are typing in, Guile can use the GNU Readline library. 
This is not enabled by default because of licensing reasons
```scheme
(use-modules (ice-9 readline))
(activate-readline)
(use-modules (ice-9 history))
```

# [Quote, Quasiquote, and Metaprogramming](http://courses.cs.washington.edu/courses/cse341/04wi/lectures/14-scheme-quote.html)
```scheme
'3        ; => 3                 (a number)
'"hi"     ; => "hi"              (a string)
'a        ; => a                 (a symbol)
'(+ 3 4)  ; => (list '+ '3 '4)   (a list)
'(a b c)  ; => (list 'a 'b 'c)   (a list)

'(define x 25)                   (a list)
          ; => (list 'define 'x '25)
          ; => (list 'define 'x 25)

'(lambda (x) (+ x 3))            (a list)
          ; => (list 'lambda (list 'x) (list '+ 'x '3))
          ; => (list 'lambda (list 'x) (list '+ 'x 3))
```
"quoted" data remains unevaluated, and provides a convenient way of representing Scheme programs.
This makes it simple to write programs that manipulate other programs --- it is easy to construct and transform programs on the fly.

下面的有点不太理解
```scheme
scheme@(guile-user) [1]> '((1 2 3) (4 5 6))
$4 = ((1 2 3) (4 5 6))
scheme@(guile-user) [1]> '('(1 2 3) '(4 5 6))
$5 = ((quote (1 2 3)) (quote (4 5 6)))
scheme@(guile-user) [1]> (list '(1 2 3) '(4 5 6))
$6 = ((1 2 3) (4 5 6))
scheme@(guile-user) [1]> (list (list 1 2 3) (list 4 5 6))
$7 = ((1 2 3) (4 5 6))
```
Sometimes one doesn't want to escape evaluation of an entire list. 
```scheme
`(1 2 ,(+ 3 4))   ; => '(1 2 7)
(quasiquote (1 2 (unquote (+ 3 4))))  ; => '(1 2 7)
(quote (1 2 (+ 3 4)))   ; => '(1 2 (+ 3 4))
```

Since we have a method for representing programs as data, it is convenient to have a function that evaluates that data as if it were a part of the currently running program. 
Indeed, Scheme has such a function **eval**, but its behavior is not well-standardized across Scheme implementations.
```scheme
(define x 5)
(define y '(+ x 10))
(eval y)             ; => 15

(eval '((lambda (x y) (string-concat x y)) "foo" "bar")) ; => "foobar"
```

# Data Types
Scheme的结构就两种:原子和表达式.

- 原子是诸如数,字符串,布尔值,变量,空表这类简单数据. 对非变量的原子求值,得到原子自身. 对变量求值,得到变量绑定的值.
表达式的形式也只有一种:列表.
- 一对括号包含起来的就是列表.表里的元素用空格分开.列表可以嵌套.

**All names belong to a single unified namespace**, and the variables that these names identify can hold any kind of Scheme value, including procedure values.

The term **latent typing** is used to describe a computer language, such as Scheme, 
for which you cannot, in general, simply look at a program source code and determine what type of data will be associated with a particular variable, 
or with the result of a particular expression. 
Instead, **the types of variables and expressions are only known in general at run time**

**all values in Scheme – carry their type with them**.   
In other words, every value "knows," at runtime, what kind of value it is, a number, a string, a list, whatever.
Scheme语言的变量类型不是固定的,可以随时改变.

- (define 变量名 值): 定义变量
- (set! 变量名 值): 改变变量的值

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
```scheme
(define p (cons 4 5)) => (4 . 5)
(car p)	=> 4
(cdr p)	=> 5
```
还可以用set-car! 和 set-cdr! 来分别设定这两个值:
```scheme
(set-car! p "hello")
(set-cdr! p "good")
```
如此,以前定义的 p 又变成了 `("hello" . "good")` 这个样子了

### 列表(list)
列表是由多个相同或不同的数据连续组成的数据类型,它是编程中最常用的复合数据类型之一,很多过程操作都与它相关

- `(list)` 表示一个空的列表, 可以简写为 `'()`
- `(define liste (list 1 2 3 4 ))`
- `(length liste)`
- `(list-ref liste index)`
- `(list-set! liste index value)`
- `(define y (make-list 5 6)) => (6 6 6 6 6)`: 创建列表, make-list用来创建列表,第一个参数是列表的长度,第二个参数是列表中添充的内容
- `(append arg list)`: append arg to list, note the difference between cons and append from the example below
- `(list (list 1 2 3) (list 4 5 6))`, 相当于 `'((1 2 3) (4 5 6))`

`(car ''(1 2 3 4))`:这个表达式调用函数car.函数car接收一个列表参数,并返回这个参数的第一个值,也就是1.
注意例子里的参数`(1 2 3 4)`前有一单引号.这是因为Scheme总是把一个普通列表当作表达式计算.加上单引号相当于告诉Scheme,不要对`(1 2 3 4)`估值,把它当成数据对待.
如果不加这个单引号,Scheme会执行`(1 2 3 4)`.执行的规则是把该列表的第一个元素当成函数来调用.而第一个元素是1,不是函数, Scheme会抛出错误.

不要小看了列表.这个看似简单的数据类型的具有丰富的表达能力.
比如我们可以把下面2x3的矩阵
![matrix](http://p.blog.csdn.net/images/p_blog_csdn_net/g9yuayon/890834bc366e48e29ddb374a82b7554b.png)
表达为`((1 2 3) (4 5 6) (7 8 9))`

而下面的树也可以用列表直观表达:`(A B C (D (F H) G) E)`.也就是说,每个列表表示一个树或子树.列表的第一个元素是根.
![tree](http://p.blog.csdn.net/images/p_blog_csdn_net/g9yuayon/19562bff2b0c431cb0846843563a0a75.png)

在Scheme 里面, 程序即数据,数据即程序.
我们遍历列表修改数据.同理,我们也可以遍列类表修改程序.
正是这样的统一处理带给Scheme无与伦比的威力:无论是编译时还是运行时,我们都可以修改,注入,加载,或者生成新的程序

#### list 和 pair 的关系
```scheme
(define a (cons 1 (cons 2 (cons 3 (list)))))
a => (1 2 3)
(list? a) => #t
(pair? a) => #t
```
由上可见,a本来是我们上面定义的点对,最后形成的却是列表.事实上列表是在点对的基础上形成的一种特殊格式.
事实上列表是在点对的基础上形成的一种特殊格式.
list是pair的子类型,list一定是一个pair,而pair不是list

list
```scheme
(define liste (list 1 2 3))
(car liste) => 1
(caar liste) => fail
(cdr liste) => ''(2 3)
(cddr liste) => ''(3)
(cdddr liste) => ''()
(cddddr liste) => fail
```

pair
```scheme
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

The value of the procedure invocation expression is the value of the last evaluated expression in the procedure body.

Scheme语言中可以用lambda来定义过程,其格式如下:
```scheme
(define 过程名 (lambda (参数 ...) (操作过程 ...)))
(define (过程名 参数 ...) (操作过程 ...)))
```
省略lambda 的为简写形式

我们可以自定义一个简单的过程,如下: `(define add5 (lambda (x) (+ x 5)))`.  
`(lambda (x) (+ x 5))` 实际上就是一个匿名过程.

既然过程是一种数据类型,所以将过程作为过程的参数是完全可以的
```scheme
(define isp
	(lambda (x)
		(if (procedure? x) 'isaprocedure 'notaprocedure)))
```

过程的嵌套定义
在Scheme语言中,过程定义也可以嵌套,一般情况下,过程的内部过程定义只有在过程内部才有效,相当C语言中的局部变量.
如下面的代码的最终结果是50:
```scheme
(define fix 
	(lambda (x y z)
		(define add 
			(lambda (a b) (+ a b)))
		(- x (add y z))))
(display (fix 100 20 30))
```

此时过程add只在fix过程内部起做用,这事实上涉及了过程和变量的绑定,可以参考下面的关于过程绑定(let,let* 和letrec)的介绍.

### apply
[map vs apply](http://stackoverflow.com/questions/27488723/what-is-the-difference-between-map-and-apply-in-scheme)

apply的功能是为数据赋予某一操作过程,它的第一个参数必需是一个过程, **最后的一个参数必需是列表**.
`(apply procedure list)`

- `(apply + '(2 3 4))` 相当于 `(+ 2 3 4))`
- `(apply + 1 -2 3 '(10 20))` 相当于 `(+ 1 -2 3 10 20)`

### map
map的功能和apply有些相似,它的第一个参数也必需是一个过程,随后的参数必需是多个列表,返回的结果是此过程来操作列表后的值

- `(map + (list 1 2 3) (list 4 5 6)) => (5 7 9)`
- `(map car (list (a b) (c d) (e f))) => (a c e)`

```
(map - '(2 3 4))
 arguments     mapping "-"     result
     2       === (- 2) ===>     -2
     3       === (- 3) ===>     -3
     4       === (- 4) ===>     -4

(map + '( 1  2  3)
       '(10 20 30))
 arguments      mapping "+"      result
    1 10     === (+ 1 10) ===>     11
    2 20     === (+ 2 20) ===>     22
    3 30     === (+ 3 30) ===>     33
```

除了apply, map以外, Scheme 语言中还有很多,诸如:eval, delay, for-each, force, call-with-current-continuation等过程绑定的操作定义,它们都无一例外的提供了相当灵活的数据处理能力.

### ex
#### 定义一个函数sum_of_squares计算一列数的平方和
```scheme
(define (sum_of_squares numbers)
	(apply + (map (lambda (x) (/* x x)) numbers)))
```
[sum of squares test](http://p.blog.csdn.net/images/p_blog_csdn_net/g9yuayon/a6f5baf5fb64466aa0ef70fe34b5de12.png)

求出两个矢量的点乘
```scheme
(define (dot_product left_vector right_vector)
	(apply + (map * left_vector right_vector)))
```

#### 矩阵转置
```scheme
(define (transpose matrix)
	(apply map (cons list matrix)))
```
调用函数`(transpose '((1 2 3) (4 5 6)))`就应该得到`'((1 4) (2 5) (3 6))`

执行顺序如下:
```scheme
(apply map (cons list matrix))
=> (apply map (cons list '((1 2 3) (4 5 6))))
=> (apply map '(list (1 2 3) (4 5 6)))
=> (map list (1 2 3) (4 5 6))
=> ((list 1 4) (list 2 5) (list 3 6))
=> '((1 4) (2 5) (3 6))
```

#### 树的高度
[tree ex](http://p.blog.csdn.net/images/p_blog_csdn_net/g9yuayon/67adf90203e94115a36a9f0690abf6b9.png)
以`'(A B C (D (F H) G) E)`为例
```scheme
(define (tree_depth tree)
	(cond
		((list? tree) (+ 1 (apply max (map tree_depth tree))))
		(else 0)))
```
执行过程如下
```scheme
(tree_depth tree)
=> (tree_depth '(A B C (D (F H) G) E))
=> (+ 1 (apply max (map tree_depth '(A B C (D (F H) G) E))))
=> (+ 1 (apply max ((tree_depth A) (tree_depth B) (tree_depth C) (tree_depth '(D (F H) G)) (tree_depth E))))
=> (+ 1 (apply max (0 0 0 (tree_depth '(D (F H) G)) 0)))
=> (+ 1 (tree_depth '(D (F H) G)))
=> (+ 1 (+ 1 tree_depth '(F H)))
=> (+ 1 (+ 1 (+ 1 0)))
=> 3
```

# 控制结构
块(form)是Scheme语言中的最小程序单元,一个Scheme语言程序是由一个或多个form构成.
没有特殊说明的情况下 form 都由小括号括起来

Scheme语言中只有if结构是系统原始提供的, 其它的cond, case, and, or, 另外还有do,when,unless等都是可以用宏定义的方式来定义的.
这一点充分体现了Scheme的元语言特性, 关于do,when等结构的使用可以参考R5RS.

## 顺序结构
也可以说成由多个form组成的form, 用begin来将多个form放在一对小括号内, 最终形成一个form.

格式为:`(begin form1 form2 ...)`

如用Scheme语言写成的经典的helloworld程序是如下样子的:
```scheme
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
```scheme
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

## `let` `let*` `letrec`
在多数编程语言中都有关于变量的存在的时限问题, Scheme语言中用`let`,`let*`和`letrec`来确定变量的存在的时限问题,即局部变量和全局变量.
一般情况下,全局变量都用define来定义,并放在过程代码的外部,
而局部变量则用let等绑定到过程内部使用.

let的用法: `(let ((...)...) ...)`, 也就是`(let s-expression s-expression)`的结构.  
例如: `(let ((x 2) (y 4)) (+ x y)))`, 在 执行`(+ x y)`的时候, x 为2, y 为 4.

let 有一个局限, 以上面的例子来说, 虽然let y 为4 定义在 let x 为 2 之后, 但是let y 为 4的时候, x 为 2 是不可用的, 如果此时访问x, 访问的仍然是上一层的x(这里就是global x).  
这时可以使用`let*`, 在定义后面的时候, 前面的已经生效了.  

但是即时使用`let`* 还是有缺陷的, 例如
```scheme
(letrec ((even?
			(lambda(x)
				(if (= x 0) #t (odd? (- x 1)))))
		(odd?
			(lambda(x) 
				(if (= x 0) #f (even? (- x 1))))))
	(even? 88))
```
上面的操作过程中,内部定义了两个判断过程`even?`和`odd?`,这两个过程是互相递归引用的.
如果将`letrec`换成`let`或`let*`都会不正常,因为`letrec`是将内部定义的过程或变量间进行相互引用的.
这种情况下就需要使用 `letrec`

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

```scheme
(define input (open-input-file "input"))
(read input)
(close-input-port input)

(define output (open-output-file "output"))
(display var output)
(newline output)
(close-output-port output)
```

- delete-file

# 宏
一个简单的例子: Perl和Ruby里有一方便的语言后置修饰,即把条件判断放到执行语句的后面.
比如: `print "x > y" unless y >= x` 这相当于下面的语句: `if(! (y >= x) ){ print "x > y" }`  
Scheme里没有unless这个关键词,也不能后置修饰条件.不过用上宏就不一样了
```scheme
(define-syntax du
	(syntax-rules ()
		((_ experssion unless condition)
			(if (not condition) expression))))
```
ex: `(du (display "3>2") unless (> 2 3))`, 会把`"3>2"` 打出来

- 用函数define-syntax定义宏.
du是这个宏的名字(do-unless的缩写). 缺省情况下,宏扩展从表达式的第一个元素开始,所以我加上du作为关键字.
我们可以通过修改扩展宏的函数来去掉对起首关键字的依赖,不过这无关本质.
- 每个宏由一系列句法规则组成.这些句法规则由syntax-rules定义.
函数syntax-rules规定了一到多组模式匹配的语句:(模式 模板): (syntax-rules () (模式1 模板1) (模式2 模板2) ... (模式n 模板n))
Scheme会依次用列出的模板匹配定义的表达式.匹配成功的话,就扩展模板.
比如说当Scheme看到`(du (display "3 > 2") unless (> 2 3))`时,就开始试着用宏定义里的模式来匹配该表达式.  下划线`_`是一特殊字符,指代宏的名字.
匹配的结果是 `_` 与"du"匹配,expression与`(display "3 > 2")`匹配,而condition与`(> 2 3)`匹配.匹配成功,
所以这个模式对应的模板被展开为`(if (not (> 2 3)) (display "3 > 2"))`.执行该语句,便打印"3 > 2"

# Expressions and Evaluation
## Evaluating Special Syntactic Expressions
When a procedure invocation expression is evaluated, the procedure and all the argument expressions must be evaluated before the procedure can be invoked.  
Special syntactic expressions 不遵循这个规定.

Why is this needed? Consider a program fragment that asks the user whether or not to delete a file, and then deletes the file if the user answers yes.

	(if (string=? (read-answer "Should I delete this file?") "yes")
		(delete-file file))
这里, 如果最外面的if expression 是一个procedure invocation的话, 那么他的参数就必须在它之前执行, 也就是说`(delete-file file)`要执行, 
如果是这样的话, 就起不到向用户确认删除文件的作用.  
Therefore `if` must be special syntax, not a procedure. 

Other special syntaxes that we have already met are `define, set!` and `lambda`. 
define and set! are syntax because they need to know the variable name that is given as the first argument in a define or set! expression, not that variable value. 
lambda is syntax because it does not immediately evaluate the expressions that define the procedure body;

## Tail calls
获取list 中的最后一个元素

	(define my-last
	  (lambda (lst)
		(if (null? (cdr lst))
		  (car lst)
		  (my-last (cdr lst)))))

	(print (my-last (list 1 2 3)))

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


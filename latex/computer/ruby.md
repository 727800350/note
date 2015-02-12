```
dirname(__FILE__) 取到的是当前文件所在的绝对目录 
```

# 基本类型

ruby中的基本类型有Numeric(包括Fixnum, Integer和Float子类型), String, Array, Hash, Object, Symbol, Range和RegEx.

nil

在有的语言中,0,null,空字符串可能被当作false,但是在ruby中,他们被当作true,

除了保留关键字"false"和"nil"外,其他都是true,

Puts总是返回nil,这是Ruby中表示"绝对无值"(absolutely-positively-nothing value)的方式,看上去有些类似Java中的null

ruby中没有++和--运算符

还有一种特殊的字符串,用``括起来.键盘左上角

括起来的字符串会作为系统命令发送给操作系统.比如上面的'dir'在windows下会执行列目录的动作,并把结果返回给ruby程序.
puts `dir`

puts format("%.2f", x)

变量作用域

常量以大写字母开头.作用域指的是变量的生命空间或者说作用范围.不同类型的变量有不同的作用域.本章主要讲两种,全局变量(Global)和局部变量(Local).

全局作用域和全局变量
全局作用域指的是能够覆盖整个应用程序运行期(从应用程序开始运行到结束).全局变量的生命周期是全局作用域.

全局变量以一个美元符号($)开头,在整个应用程序运行期,任何地方都可以使用全局变量.但是稍微有经验的人都知道,应该尽量少的使用全局变量,以减少空间的占用.
内建的全局变量

ruby解释器在开始运行的时候初始化了一部分全局变量,在应用程序的任何地方都可以调用这些内建全局变量.
比如'$0'代表的是所运行应用程序的文件名称,'$:'代表的是默认的文件搜索路径,'$$'代表的是ruby程序的进程id.

常用的系统全局变量

- `$!`:  最近一次的错误信息
- `$@`:  错误产生的位置
- `$_`:  gets最近读的字符串 
- `$.`:  解释器最近读的行数(line number)
- `$&`:  最近一次与正则表达式匹配的字符串
- `$~`:  作为子表达式组的最近一次匹配 
- `$n`: 最近匹配的第n个子表达式(和$~[n]一样)
- `$=`:  是否区别大小写的标志 
- `$/`:  输入记录分隔符
- `$\`:  输出记录分隔符
- `$0`:  Ruby脚本的文件名
- `$*`:  命令行参数
- `$$`:  解释器进程ID
- `$?`:  最近一次执行的子进程退出状态

可以查看文档来了解更多的内建全局变量.

本地作用域

本地作用域存在于下面几种情况中:

1. 最顶级的程序.---我的理解是main程序.
2. 类和模块.
3. 方法.

变量命名规则:

ruby中的变量可以包含任何类型的数据.使用变量时不需要类型声明,变量名决定了它的作用域.
本地变量(在对象中定义)以小写字母或者下划线开头,后面跟合法字符.

实例变量以'@'开头,后面至少跟一个合法字符.类似于C#中的属性
在类中定义的变量,以'@@',两个'@'开头,后面至少跟一个合法字符.

全局变量以'$'开头,后面跟合法字符,也可以使用'$-'开头,后跟一个字母(是字母).
常量命名规则:

常量以大写字母开头,后跟合法字符.类和模块的命名规则和常量一样.
方法命名规则:

方法名以小写字母开头,后跟合法字符.但是在方法名的结尾,可以跟'?'或'!'(!或者bang代表这是一个比较危险的方法,说危险是相对和它重名,但是不带'!'的方法.后面会有专门的章节介绍'Bang Method').
 

ruby规范说明,如果方法或变量名由多个词组成,词和词之间用"_"隔开.如果是类,模块或者常量,不使用'_',只需要把单词的首字母大写就可以了.比如,my_variable, MyModule, MyClass, MyConstant..需要注意的是,一个变量可以是任何类型对象的引用,而且引用是可以动态变换的.常量也是对象的引用,可以在类和模块中定义常量,但是在方法中不可以.
Ruby允许修改常量的值,但是会给一个警告信息.

变量只是对象的引用,引用失效后,ruby会自动做垃圾收集.

以'!'结尾的方法总是返回对象本身,而不是一个拷贝

 
方法中并没有定义返回类型,一个方法的返回值就是该方法的最后一行.

多个方法定义之间最好用空行隔开,方便区分.
ruby规范说明,方法的参数需要用括号括起来.

但是看以前的例子,比如puts,p,gets等方法的参数都没有括起来.以后会讲到参数不需要括起来的情况.

'#{...}'

'#{...}'这样的代码,它的作用是,计算{}中表达式的结果,并把结果转换成字符串.运行这样的代码后,可以得到结果字符串,并不会看到'#{}'在输出结果中.

再比如下面的例子:

puts "100 * 5 = #{100 * 5}"

alias

在ruby中可以给一个方法指定别名.别名对应的是原方法的一个拷贝,如果原方法改变了,别名不会跟着变化,看例子:alias newmtd oldmtd    alias是别名的意思

 

method_missing

当调用一个对象的方法时,会根据方法名查找是否有这个方法存在.如果不存在这个方法,会抛出一个NoMethodError异常.

但是可以定义一个method_missing方法,来处理找不到方法这种情况,并且可以传递参数给这个方法.
method_missing提供的是一个友好的处理办法,它拦截这种发生了错误并且没有返回结果的信息
```
class Dummy
	def method_missing(m, *args)
		puts "There's no method called #{m} here -- please try again."
	end
end
```

## 字符串
- reverse方法得到一个反向排列的字符串(原来的字符串不会改变).
- length方法得到字符串的长度.
- upcase方法把字符串中的所有字母转换成大写.
- downcase方法把字符串中的所有字母转换成小写.
- swapcase方法也是转化字符串中字母的大小写,但是它会把大写字母换成小写,小写字母换成大写.
- capitalize方法把字符串的第一个字母换成大写,其他都换成小写.slice方法返回字符串的一个子字符串.

upcase, downcase, swapcase和capitalize方法返回的都是原字符串的一个拷贝.
但是upcase!, downcase!, swapcase!和capitalize!这些重磅方法,都是直接修改并返回原对象.如果你确认没有必要保留原字符串,可以使用它们.

- String.methods.sort列出类string的所有方法.
- String.instance_methods.sort列出string实例的所有实例方法.
- String.instance_methods(false).sort列出string实例的所有非继承的实例方法.

ruby中比较字符串的方法有好几种.其中最普通的就是"==",它比较两个字符串的内容是
否相同,还有String.eql?,它比较的也是两个字符串的内容.还有String.equal?,它比较两个字符串是否同一个对象

有时候定义一个字符串或字符数组,需要给每一个元素加上引号和逗号,比较麻烦.
ruby提供了一种简洁的方法,使用`%w`

```
64.times do |square|
	puts square
	puts "On square #{square + 1} are #{rice_on_square} grain(s)"
	rice_on_square *= 2
end
```

```
def foo(*my_string)
	my_string.each do |word|
		puts word
	end
end

foo('hello','world')
foo()
```

```
puts (5 % 3) # prints 2
puts (-5 % 3) # prints 1
puts (5 % -3) # prints -1
puts (-5 % -3) # prints -2
3**2 #3的平方
```

 
# Escape sequence

```
puts 'It\'s my Ruby'  #It's my Ruby
# New here, displays the string three times
puts 'Hello ' * 3
```

```
var1 = 5;var2 = '2'
puts var1 + var2.to_i
```
arity the number of arguments that a function can take

#对象-34的绝对值
```
-34.abs
```

#对一个浮点数进行四舍五入处理
```
10.8.round
```

#返回一个字符串对象的大写且逆转的副本
```
"This is Ruby".upcase.reverse
```

#返回数学sin方法的参数个数
```
Math.method(:sin).arity
```

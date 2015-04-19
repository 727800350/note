主要参考自:
1. [Shell脚本编程30分钟入门](https://github.com/qinjx/30min_guides/blob/master/shell.md)
1. [ShellProgramming](http://mprc.pku.edu.cn/mentors/training/TrainingCourses/material/ShellProgramming.HTM#\_Toc37518066)

# Introduction
## shell和shell脚本的概念
shell是指一种应用程序,这个应用程序提供了一个界面,用户通过这个界面访问操作系统内核的服务.Ken Thompson的sh是第一种Unix Shell,Windows Explorer是一个典型的图形界面Shell.

shell脚本(shell script),是一种为shell编写的脚本程序.业界所说的shell通常都是指shell脚本,但读者朋友要知道,shell和shell script是两个不同的概念.由于习惯的原因,简洁起见,本文出现的"shell编程"都是指shell脚本编程,不是指开发shell自身(如Windows Explorer扩展开发).

## 脚本解释器
### sh and bash
即Bourne shell,POSIX(Portable Operating System Interface)标准的shell解释器,它的二进制文件路径通常是/bin/sh,由Bell Labs开发.

Bash是Bourne shell的替代品,属GNU Project,二进制文件路径通常是/bin/bash.业界通常混用bash,sh,和shell,比如你会经常在招聘运维工程师的文案中见到:熟悉Linux Bash编程,精通Shell编程.

在CentOS里,/bin/sh是一个指向/bin/bash的符号链接, Mac OS上不是

### 高级编程语言
理论上讲,只要一门语言提供了解释器(而不仅是编译器),这门语言就可以胜任脚本编程,常见的解释型语言都是可以用作脚本编程的,如:Perl,Tcl,Python,PHP,Ruby.
Perl是最老牌的脚本编程语言了,Python这些年也成了一些linux发行版的预置解释器.

编译型语言,只要有解释器,也可以用作脚本编程,如C shell是内置的(/bin/csh),Java有第三方解释器Jshell,Ada有收费的解释器AdaScript.

### 简单 vs 高级
如果你觉得自己熟悉的语言(如Java,C)写shell脚本实在太啰嗦,你只是想做一些备份文件,安装软件,下载数据之类的事情,学着使用sh,bash会是一个好主意.

shell只定义了一个非常简单的编程语言,所以,如果你的脚本程序复杂度较高,或者要操作的数据结构比较复杂,那么还是应该使用Python,Perl这样的脚本语言,或者是你本来就已经很擅长的高级语言.
因为sh和bash在这方面很弱,比如说:

- 它的函数只能返回字串,无法返回数组
- 它不支持面向对象,你无法实现一些优雅的设计模式
- 它是解释型的,一边解释一边执行,连PHP那种预编译都不是,如果你的脚本包含错误(例如调用了不存在的函数),只要没执行到这一行,就不会报错

### 调试
最有用的调试脚本的工具是echo命令,可以随时打印有关变量或操作的信息,以帮助定位错误.也可使用打印最后状态($?) 命令来判断命令是否成功,这时要注意的是要在执行完要测试的命令后立即输出$?,否则$?将会改变.
Set命令也可以用来辅助脚本测试:
```
Set –n           读命令但是不执行
Set –v           显示读取的所有的行
Set –x           显示所有的命令及其参数
```
(要关闭set选项,只要把－换成+就可以了,这里有点特殊,要注意一下)

# IO
每个shell 过程一次最多可以有9 个文件描述符(其中 0(stdin),1(stdout),2(stderr)作为保留的文件描述符)
## 输入重定向
输入重定向符号: `<`.

内联输入重定向符号: `<<`, 在使用的时候, 除了这个符号, 你必须指定一个文本(任意的字符串)标记来划分要输入数据的开始和结尾. eg.
```
$ wc << EOF
> test string 1
> test string 2
> EOF
	2 6 28
```

## 输出重定向
文件描述符必须放在重定向符号前, 该值必须紧紧地放在重定向符号前, eg: `2> log.error`

- 重定向数据和错误: `command 1> log.out 2> log.error`
- 重定向到同一个文件: `command &> log`(注意: bash shell 会自动给错误消息更高的优先级, 使得可以在一处地方查看错误消息, 不用翻遍整个输出文件)
- 临时重定向: `echo "this is an error" >&2`: 将这条输出重定向到错误输出.(& 不能丢, 否则是将输出重定向到一个名为2的文件)
- 永久重定向: `exec 2> error`: 将错误消息全部重定向到error 文件. 相应的, 永久输入重定向为: `exec 0< input`([redirect input demo](../../demo/shell/input.sh))

## 创建文件描述符
除了0,1,2, 我们还可以自己定义文件描述符.

[重定向标准输出到一个文件, 然后再恢复](../../demo/shell/redirect.sh)  
同样的操作可以应用到输入上

关闭文件描述符: `exec 3>&-`(最后的4个字符之间不能有空格). [demo](../../demo/shell/close_file_descriptor.sh)

[创建临时文件](../../demo/shell/tmp_file.sh)

- in default, mktemp will create the tmp file in the current directory
- `mktemp -t`: 会强制mktemp 在系统的临时目录创建该文件, 在使用这个特性时, 命令会返回这个临时文件的全路径
- `mktemp -d`: 创建一个临时目录

# 变量
## 定义变量
定义变量时,变量名不加美元符号($),如:

	your_name="qinjx"

注意,变量名和等号之间不能有空格,这可能和你熟悉的所有编程语言都不一样.

除了显式地直接赋值,还可以用语句给变量赋值,如:

	for file in `ls /etc`

## 使用变量
使用一个定义过的变量,只要在变量名前面加美元符号即可,如:

	your_name="qinjx"
	echo $your_name
	echo ${your_name}

变量名外面的花括号是可选的,加不加都行,加花括号是为了帮助解释器识别变量的边界,比如下面这种情况:

	for skill in Ada Coffe Action Java do
		echo "I am good at ${skill}Script"
	done

如果不给skill变量加花括号,写成echo "I am good at $skillScript",解释器就会把$skillScript当成一个变量(其值为空),代码执行结果就不是我们期望的样子了.

推荐给所有变量加上花括号,这是个好的编程习惯.IntelliJ IDEA编写shell script时,IDE就会提示加花括号.

## 重定义变量
已定义的变量,可以被重新定义,如:

	your_name="qinjx"
	echo $your_name
	
	your_name="alibaba"
	echo $your_name
	
这样写是合法的,但注意,第二次赋值的时候不能写$your_name="alibaba",使用变量的时候才加美元符.

## 一些常用的shell变量
```
$#  传递到脚本的参数个数
$*  以一个单字符串显示所有向脚本传递的参数(可大于9个)
$$  脚本运行的当前进程的ID号
$!  后台运行的最后一个进程的ID号
$@  与$#相同,但使用时加引号,并在引号中返回每个参数
$-  显示shell使用的当前选项
$?  显示最后命令的退出状态,0表示无错误
```

## 注释
以"#"开头的行就是注释,会被解释器忽略.

如果在开发过程中,遇到大段的代码需要临时注释起来,过一会儿又取消注释,怎么办呢?每一行加个#符号太费力了,可以把这一段要注释的代码用一对花括号括起来,定义成一个函数,没有地方调用这个函数,这块代码就不会执行,达到了和注释一样的效果.

## 字符串
字符串是shell编程中最常用最有用的数据类型(除了数字和字符串,也没啥其它类型好用了,哈哈),字符串可以用单引号,也可以用双引号,也可以不用引号.单双引号的区别跟PHP类似.

### 单引号

	str='this is a string'

单引号字符串的限制:

- 单引号里的任何字符都会原样输出,单引号字符串中的变量是无效的
- 单引号字串中不能出现单引号(对单引号使用转义符后也不行)

注意: 反单引号 \` \`用于设置系统命令的输出到变量,shell会将\` \`中的内容作为一个系统命令并执行质
 
### 双引号

	your_name='qinjx'
	str="Hello, I know your are \"$your_name\"! \n"

- 双引号里可以有变量
- 双引号里可以出现转义字符

### 字符串操作
#### 拼接字符串
	
	your_name="qinjx"
	greeting="hello, "$your_name" !"
	greeting_1="hello, ${your_name} !"
	
	echo $greeting $greeting_1

#### 获取字符串长度:

	string="abcd"
	echo ${#string} #输出:4

#### 提取子字符串

	string="alibaba is a great company"
	echo ${string:1:4} #输出:liba

#### 查找子字符串

	string="alibaba is a great company"
	echo `expr index "$string" is`#输出:8,这个语句的意思是:找出单词is在这名话中的位置

#### 更多
参见本文档末尾的参考资料中[Advanced Bash-Scripting Guid Chapter 10.1](http://tldp.org/LDP/abs/html/string-manipulation.html)

## 数组

# 程序流程, 结构化命令
## 条件判断
`-o`: 逻辑或

### 字符串测试
五种格式: 
```
test  "string"
test  string_operator  "string"
test  "string"  string_operator  "string"
[ string_operator  "string" ]
[ "string"  string_operator  "string" ]
```
所以`test`和`[]`是等价的

其中string_operator可以为:

    =     两字符串相等
    !=    两字符串不等
    -z   空串
    -n   非空串

### 数值测试
**数值的运算最好通过awk 来进行, shell 本身只支持整数运算**

两种格式: 
```
"number"  number_operator  "number"
[ "number"  number_operator  "number" ]
```
其中:number_operator 可以为:`-eq  ,-ne,-gt,-lt,-ge`
例如:
```
NUMBER=130
[ "990"  –le  "995"  –a  "NUMBER"  -gt  "133" ]
(其中-a表示前后结果相"与")
```

###  文件状态测试(常用的)
```
-e  测试文件是否存在
-d  测试是否文件夹
-f  测试是否一般文件
-s  测试文件是否非空
-L  测试是否链接文件
-r  测试文件是否可读
-w  测试文件是否可写
-x  测试文件是否可执行
```

```
-a file exists. 
-b file exists and is a block special file. 
-c file exists and is a character special file. 
-g file exists and has its setgid(2) bit set. 
-G file exists and has the same group ID as this process. 
-k file exists and has its sticky bit set. 
-L file exists and is a symbolic link. 
-n string length is not zero. 
-o Named option is set on. 
-O file exists and is owned by the user ID of this process. 
-p file exists and is a first in, first out (FIFO) special file or named pipe. 
-r file exists and is readable by the current process. 
-s file exists and has a size greater than zero. 
-S file exists and is a socket. 
-t file descriptor number fildes is open and associated with a terminal device. 
-u file exists and has its setuid(2) bit set. 
```

## 流程控制

还要注意,sh里的`if [ $foo -eq 0 ]`,这个方括号跟Java/PHP里if后面的圆括号大不相同,它是一个可执行程序(和cd, ls, grep一样),相不到吧?在CentOS上,它在/usr/bin目录下:

	ll /usr/bin/[
	-rwxr-xr-x. 1 root root 33408 6月  22 2012 /usr/bin/[

正因为方括号在这里是一个可执行程序,方括号后面必须加空格,不能写成`if [$foo -eq 0]`

### if else
#### if

	if condition; then
		commands
	fi
或者then换行, 那么condition后面的分号就不要了

写成一行(适用于终端命令提示符):

	if `ps -ef | grep ssh`;  then echo hello; fi
	
末尾的fi就是if倒过来拼写,后面还会遇到类似的

#### if else

	if condition
	then
		command
	else
		command
	fi

#### if else-if else

	if condition1
	then
		command1
	elif condition2
		command2
	else
		commandN
	fi

### for while
#### for
在开篇的示例里演示过了:

	for var in item1 item2 ... itemN
	do
		command
	done

写成一行:

	for var in item1 item2 ... itemN; do command1; command2… done;

#### C风格的for

	for (( EXP1; EXP2; EXP3 ))
	do
		command
	done

#### while

	while condition
	do
		command
	done
	
#### 无限循环

	while :
	do
		command
	done

或者

	while true
	do
		command
	done

或者

	for (( ; ; ))

#### until

	until condition
	do
		command
	done

### case

	case "${opt}" in
		"Install-Puppet-Server" )
			install_master $1
			exit
		;;

		"Install-Puppet-Client" )
			install_client $1
			exit
		;;

		"Config-Puppet-Server" )
			config_puppet_master
			exit
		;;

		"Config-Puppet-Client" )
			config_puppet_client
			exit
		;;

		"Exit" )
			exit
		;;

		* ) echo "Bad option, please choose again"
	esac

case的语法和C family语言差别很大,它需要一个esac(就是case反过来)作为结束标记,每个case分支用右圆括号,用两个分号表示break


## 函数
### 定义
### 调用

## 文件包含
可以使用source和.关键字,如:

	source ./function.sh
	. ./function.sh

在bash里,source和.是等效的,他们都是读入function.sh的内容并执行其内容(类似PHP里的include),为了更好的可移植性,推荐使用第二种写法.

包含一个文件和在执行一个文件一样,也要写这个文件的路径,不能光写文件名,比如上述例子中:

	. ./function.sh

不可以写作:

	. function.sh

如果function.sh是用户传入的参数,如何获得它的绝对路径呢?方法是:

	## $1是用户输入的参数,如function.sh
	real_path=`readlink -f $1`
	. $real_path


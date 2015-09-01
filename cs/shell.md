主要参考自:
1. [Shell脚本编程30分钟入门](https://github.com/qinjx/30min_guides/blob/master/shell.md)
1. [ShellProgramming](http://mprc.pku.edu.cn/mentors/training/TrainingCourses/material/ShellProgramming.HTM#\_Toc37518066)

当运行一个需要很长时间才能结束的脚本时, 通过`nohup &` 让其在后台运行, 且在脚本的末尾加上 `mail`, 发送邮件通知

```
{
commands
}&
```
一组命令作为一个整体在后台运行

()运算符: ( 命令; 命令; 命令 )
 
{}运算符: { 命令; 命令; 命令 }

目录
```
$ dirname /dir/path.sh
/dir
$ basename /dir/path.sh
path.sh
```
在shell 脚本里一定要注意路径.

脚本里没有`ll`命令, 需要使用 `ls -l`

构造命令字符串, 然后执行(双引号不能丢)
```
$ bash -c "date"
Wed Apr 22 19:08:12 CST 2015
```

在CentOS里,/bin/sh是一个指向/bin/bash的符号链接, Mac OS上不是

# 调试
最有用的调试脚本的工具是echo命令,可以随时打印有关变量或操作的信息,以帮助定位错误.
也可使用打印最后状态($?) 命令来判断命令是否成功,这时要注意的是要在执行完要测试的命令后立即输出$?,否则$?将会改变.

## set 选项
在脚本里面添加  

**set -e**  
Exit immediately if a simple command exits with a non-zero status.

["set -e" 与 "set -o pipefail" ref](http://blog.sina.com.cn/s/blog_8bb0a3bd010171cp.html)

**set -o pipefail**  
对于set命令-o参数的pipefail选项,linux是这样解释的:  
"If set, the return value of a pipeline is the value of the last (rightmost) command to exit with a  non-zero  status,
or zero if all commands in the pipeline exit successfully.  This option is disabled by default."  
设置了这个选项以后,包含管道命令的语句的返回值,会变成最后一个返回非零的管道命令的返回值.听起来比较绕,其实也很简单:

```
# test.sh
set -o pipefail
ls ./a.txt |echo "hi" >/dev/null
echo $?
```

运行test.sh(当前不存在a.txt),输出:
```
ls: ./a.txt: No such file or directory
1  # 设置了set -o pipefail,返回从右往左第一个非零返回值,即ls的返回值1
```

注释掉set -o pipefail 这一行,再次运行,输出:
```
ls: ./a.txt: No such file or directory
0  # 没有set -o pipefail,默认返回最后一个管道命令的返回值
```

set -x

## 直接在运行的时候设置
```
bash –n script.sh          读命令但是不执行
bash –v script.sh          显示读取的所有的行
bash –x script.sh          显示所有的命令及其参数
```
(要关闭set选项,只要把－换成+就可以了,这里有点特殊,要注意一下)

## && and ||
`command1  && command2`  
&&左边的命令(命令1)返回真(即返回0,成功被执行)后,&&右边的命令(命令2)才能够被执行,换句话说,"如果这个命令执行成功&&那么执行这个命令". 

`command1 || command2`  
||则与&&相反.
如果||左边的命令(命令1)未执行成功,那么就执行||右边的命令(命令2),或者换句话说,"如果这个命令执行失败了||那么就执行这个命令."

## 注释
以"#"开头的行就是注释,会被解释器忽略.

多行注释
```
<<COM
code
COM
```

# IO
## 管道
数据处理类shell脚本中可能会多次用到 "cat xxx|awk yyy"类似的管道命令,判断返回值时不能仅仅用$?来判断返回值,最好使用PIPESTATUS获取管道中所有命令的返回码\\
好处:第一条命令失败后就能提前发现问题,避免问题被遗漏或置后发现.\\
用法:PIPESTATUS 是一个数组,第一条命令的返回码存储在${PIPESTATUS[0]},以此类推\\

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
- 永久重定向: `exec 2> error`: 将错误消息全部重定向到error 文件. 相应的, 永久输入重定向为: `exec 0< input`([redirect input demo](../demo/shell/input.sh))

## 创建文件描述符
除了0,1,2, 我们还可以自己定义文件描述符.

[重定向标准输出到一个文件, 然后再恢复](../demo/shell/redirect.sh)  
同样的操作可以应用到输入上

关闭文件描述符: `exec 3>&-`(最后的4个字符之间不能有空格). [demo](../demo/shell/close_file_descriptor.sh)

[创建临时文件](../demo/shell/tmp_file.sh)

- in default, mktemp will create the tmp file in the current directory
- `mktemp -t`: 会强制mktemp 在系统的临时目录创建该文件, 在使用这个特性时, 命令会返回这个临时文件的全路径
- `mktemp -d`: 创建一个临时目录

# 变量
[variable demo](../demo/shell/variable.sh)

**注意:
1. 变量名和等号之间不能有空格**
1. 只有在使用变量的时候, 才需要用$符号, 同时有时为了避免歧义, 还需要使用括号将变量名字括起来

## 一些常用的shell变量
```
$#  传递到脚本的参数个数, 不包括脚本名字本身
$0  shell 脚本本身的文件名, $1, ... $9 其他参数
$$  脚本运行的当前进程的ID号
$!  后台运行的最后一个进程的ID号
$*  以一个单字符串显示所有向脚本传递的参数(可大于9个), "arg1 arg2 arg3 ..."
$@  与$*相同,但使用时加引号,并在引号中返回每个参数, "arg1" "arg2" "arg3" ...
$-  显示shell使用的当前选项
$?  显示最后命令的退出状态,0表示无错误
```

位置参数可以用shift命令左移.  
比如shift 3表示原来的$4现在变成$1,原来的$5现在变成$2等等,原来的$1,$2,$3丢弃,$0不移动.
不带参数的shift命令相当于shift 1.  
Shift 命令还有另外一个重要用途, Bash 定义了9个位置变量,从 $1 到 $9,这并不意味着用户在命令行只能使用9个参数,借助 shift 命令可以访问多于9个的参数.
Shift 命令一次移动参数的个数由其所带的参数指定.例如当 shell 程序处理完前九个命令行参数后,可以使用 shift 9 命令把 $10 移到 $1.

## 数学运算
1. 使用 `$(())` 将表达式放在括号中,即可达到运算的功能, eg: `$a=1; b=2; echo $(($a+$b))`: 得到3
1. 使用expr, eg: `$expr 1 + 2`: 得到3, 对于乘法, 运算符需要使用 `\*`, 而不能直接使用`*`.

注意:将需要运算的表达式写入在expr 后面即可,保证**参数与运算符号中间有空格隔开**

上面的两种方式都不支持浮点运算, 如果需要, 可以借用bc 或 awk 来实现.
```
$ c=$(echo "5.01-4*2.0"|bc)
$ echo $c
-2.99

$ c=$(awk 'BEGIN{print 7.01*5-4.01 }')
$ echo $c
31.04
```
在shell 中单括号里面,可以是命令语句, 
也就是$() 与 ``等效, 中间包含命令语句执行,返回执行结果

## [字符串](http://tldp.org/LDP/abs/html/string-manipulation.html)
字符串是shell编程中最常用最有用的数据类型(除了数字和字符串,也没啥其它类型好用了),
字符串可以用单引号,也可以用双引号,也可以不用引号.单双引号的区别跟PHP类似.

- 单引号里的任何字符都会原样输出,单引号字符串中的变量是无效的, 转意符也不太好用
- 双引号里可以有变量,双引号里可以出现转义字符

### 获取字符串长度 String Length
```
str="abcABC123ABCabc"
echo ${#str}
echo `expr length ${str}`
```

### 提取子字符串 Substring Extraction
- `${string:position}`在$string中, 从位置$position开始提取子串
- `${string:position:length}`在$string中, 从位置$position开始提取长度为$length的子串
**字符串的第一个字符, position 为 0**

position 可以为负值
```
echo ${str: -0}
echo ${str: -1} ## c
echo ${str: -3:2} ## ab
```

substr: str 的第一个字符position 为 1: `echo `expr substr $str 1 2` ## ab`

- `expr match "$string" '\($substring\)'`: Extracts $substring at **beginning** of $string, where $substring is a regular expression.
- expr match "$string" '.*\($substring\)': Extracts $substring at **end** of $string. 这个实际上还是从beginning 开始匹配, 但是 `.*` 匹配开头所有部分, 只有 substring 是在 `\(\)` 里面
```
echo `expr match "$str" '\(.[b-c]\)'` ## ab
echo `expr match "$str" '.*\([A-C][A-C][A-C][a-c]*\)'`    # ABCabc
```

### Substring Removal
- ${string#substring} Deletes **shortest** match of $substring from **front** of $string.
- ${string##substring} Deletes **longest** match of $substring from **front** of $string.

```
echo ${str#a*C}      # 123ABCabc
echo ${str##a*C}     # abc
```
a*C 在 str 中有两个匹配, abcABC, abcABC123ABC, `#` 使用短的, `##` 使用长的

- ${string%substring} Deletes **shortest** match of $substring from **back** of $string.
- ${string%%substring} Deletes **longest** match of $substring from **back** of $string.

```
echo ${str%b*c}      # abcABC123ABCa
echo ${str%%b*c}     # a
```

### Substring Replacement
- ${string/substring/replacement} Replace first match of $substring with $replacement
- ${string//substring/replacement} Replace all matchs

```
echo ${str/abc/xyz}       # xyzABC123ABCabc
echo ${str//abc/xyz}      # xyzABC123ABCxyz
```

What happens if no $replacement string is supplied?
```
echo ${str/abc}           # ABC123ABCabc
echo ${str//abc}          # ABC123ABC
```
A simple deletion takes place.

- ${string/#substring/replacement} If $substring matches front end of $string, substitute $replacement for $substring.
- ${string/%substring/replacement} If $substring matches back end of $string, substitute $replacement for $substring.

使用了 `#` 或者 `%` 之后, 即时加上了 `//`, 也还是只对 `#, %` 部分生效

```
echo ${str/#abc/XYZ}          # XYZABC123ABCabc
echo ${str/%abc/XYZ}          # abcABC123ABCXYZ
```

## 数组
[ref](http://c.biancheng.net/cpp/view/7002.html)

bash支持一维数组(不支持多维数组),并且没有限定数组的大小.
类似与C语言,数组元素的下标由0开始编号.
获取数组中的元素要利用下标,下标可以是整数或算术表达式,其值应大于或等于0.

### 定义数组
在Shell中,用括号来表示数组,数组元素用"空格"符号分割开.定义数组的一般形式为:
```
array_name=(value1 ... valuen)
```
例如:
```
array_name=(value0 value1 value2 value3)
```
或者

    array_name=(
    value0
    value1
    value2
    value3
    )

还可以单独定义数组的各个分量:

    array_name[0]=value0
    array_name[1]=value1
    array_name[2]=value2

可以不使用连续的下标,而且下标的范围没有限制. 

### 读取数组
读取数组元素值的一般格式是:
```
${array_name[index]}
```
例如: `valuen=${array_name[2]}`

使用@ 或 * 可以获取数组中的所有元素,例如:

    ${array_name[*]}
    ${array_name[@]}

遍历数组
```
for item in ${array[*]}
do
	...
done
```

当数组的元素中含有空格时, 例如 `array=("bash -x ./test.sh")`, 本来我们预期结果是array 就一个元素, 
但是如果采用上面的遍历方式, 这个元素会被拆分为 3 个元素,`bash`, `-x`, `./test.sh`.
解决方法: "${array[*]}". [ref](http://unix.stackexchange.com/questions/108635/why-i-cant-escape-spaces-on-a-bash-script)

### 获取数组的长度
获取数组长度的方法与获取字符串长度的方法相同,例如:
```
# 取得数组元素的个数
num_items=${#array_name[@]}
num_items=${#array_name[*]}
```

# 程序流程, 结构化命令
## 条件判断
- `-o`: 逻辑或
- `-a`: 逻辑和

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
其中:number_operator 可以为:`-eq, -ne, -gt, -lt, -ge`
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

**wait**  
当脚本里需要执行多个命令, 但是需要并行执行.  
例如, 需要在一个机器列表上的每个机器上执行一个耗时很长的命令, 那么可以
```
ssh "bash run.sh" & < /dev/null
```
让命令在后台运行, 但是需要等待所有的命令执行完毕, 这时就需wait 命令.

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
	then
		command2
	else
		commandN
	fi

### for while
[Shell 循环控制break/continue](http://www.yiibai.com/shell/unix-loop-control.html)

- break: 退出循环
- continue: 直接跳到下次循环

两者都可以带一个整数参数, 表示跳出第n个封闭的循环

#### for
在开篇的示例里演示过了:

	for var in item1 item2 ... itemN
	do
		command
	done

写成一行:

	for var in item1 item2 ... itemN; do command1; command2… done;

#### C风格的for

	num=100
	for((i=0;i<=$num;i++))
	do
		command
	done

#### while

	while condition
	do
		command
	done

ex:
```
while(($i<100))	
do
	...
done
```

#### 无限循环

	while true
	do
		command
	done

#### until

	until condition
	do
		command
	done

### case

	case "${command}" in
		"add")
			...
		;;
		"check"|"clear")
			...
		;;
		*) 
			echo "${command} not supported."
			exit 1
	esac

## 函数
### 定义
[ref](http://www.cnblogs.com/chengmo/archive/2010/10/17/1853356.html)
```
[ function ] funname[()]
{
    action;
    [return int;]
}
```

[user defined func demo](../demo/shell/function.sh)

1. 必须在调用函数地方之前,声明函数,shell脚本是逐行运行.不会像其它语言一样先预编译,一次必须在使用函数前先声明函数
1. 可以带function fun()定义,也可以直接fun()定义,不带任何参数
2. 参数返回,可以显示加:return 返回, 也可以不加.
如果不加,将以最后一条命令运行结果作为返回值. 
return后跟数值n( n 的范围必须在0-255之间, 如果n>255, 返回的实际结果为 n % 256),

[变量作用域demo](../demo/shell/function_variable_scope.sh)

1. 定义函数可以与系统命令相同, 自定义的函数命令比系统自带的命令优先级高
2. 需要获得函数值,通过$?获得
3. 如果需要传出其它类型函数值,可以在函数调用之前,定义变量(这个就是全局变量),在函数内部就可以直接修改,然后在执行函数就可以修改
4. 如果需要定义local变量,可以在函数中定义:`local 变量=值`, 这时变量就是内部变量,它的修改,不会影响函数外部相同变量的值

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


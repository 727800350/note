# intro
pattern scanning and processing language
[awk ref](http://www.grymoire.com/Unix/Awk.html)

- AWK - the (very old) original from AT&T
- NAWK - A newer, improved version from AT&T
- GAWK - The FGNU Project implementation, linux 中默认的版本.

**特性**
It can be considered to be a **pseudo-C interpretor, as it understands the same arithmatic operators as C**. 
AWK also has string manipulation functions, so it can search for particular strings and modify the output. 
AWK also has associative arrays, which are incredible useful, and is a feature most computing languages lack. Associative arrays can make a complex problem a trivial exercise.

## awk脚本
`cat input | awk -F '\t' -v var="value" -f script.awk`:

- -F: 指定分隔符, 默认会同时使用 `\t` 和 `\space`
	- 当指定分隔符为`.`时, 需要使用 `-F.`
- -v: 传递一个变量给awk, 在awk里面可以直接使用 var 变量,它的值为 "value"
- -f: 指定awk脚本

参数的默认值
`${variable:-defaultvalue}` 注意那个减号
例如: `column=${1:-1}`

# Patterns
语法模式: `pattern {action}`

The pattern specifies where the action is performed.
A pattern or condition is simply an abbreviated test. 
If the condition is true, the action is performed. All relational tests can be used as a pattern.

可以使用逻辑运算符和括号: AND(`&&`), OR(`||`) and NOT(`!`)

AWK is line oriented. 

- 默认是所有行都匹配
- 两个特殊的pattern
	1. `BEGIN {action}`
	1. `END {action}`
- 使用 regex, eg: `{if($0 ~ /special/) {print}}`
- 限制起始与结束: `/start/,/stop/ {action}`: 在/start/ 与 /stop/ 两个pattern之间(包括边界), action 有效, 等同于下面的实现

		{
			if($0 ~ /start/){
				triggered=1;
			}
			if(triggered){
				print;
				if($0 ~ /stop/){
					triggered=0;
				}
			}
		}

`BEGIN { x=5; }{ print x, $x}`: 会输出5和第5个field的value.

# 运算符
Binary Operators

| Operator | Type       | Meaning        			 |
|----------|------------|----------------------------|
| +        | Arithmetic | Addition       			 |
| -        | Arithmetic | Subtraction    			 |
| *        | Arithmetic | Multiplication 			 |
| /        | Arithmetic | Division, 5/2=2.5       	 |
| %        | Arithmetic | Modulo         			 |
| `\space` | String     | Concatenation, 字符串拼接  |

Regular Expressions

- ~: Matches, eg: `if($0 ~ /:/)`
- !~: Does not match

A value of 0 is false, while anything else is true. Undefined variables has the value of 0.

# Built-in Variables
- $0 refers to the entire line that AWK reads in
- FS - The Input Field Separator Variable, 等同于命令行的`-F`
- OFS - The Output Field Separator Variable, 默认的OFS是一个空格
- NF - The Number of Fields Variable
- NR - The Number of Records Variable, 一行就是一个record
- FNR: contains the number of lines read, but is **reset for each file read**. The NR variable accumulates for all files read.
- RS - The Record Separator Variable, 默认是一行一条record
- ORS - The Output Record Separator Variable
- FILENAME - The Current Filename Variable

## prinf
The printf is very similar to the C function with the same name, 用法也一样.

同时可以使用输出重定向
```
printf("string\n") > "/tmp/file";
printf("string\n") >> "/tmp/file";
```

# String Functions
- index(string,search)	AWK, NAWK, GAWK
- length(string)	AWK, NAWK, GAWK
- split(string,array,separator)	AWK, NAWK, GAWK

	set time = 12:34:56
	set hms = `echo $time | awk '{split($0,a,":" ); print a[1], a[2], a[3]}'` # = 12 34 56

- substr(string,position)	AWK, NAWK, GAWK, 第一个字母的下标为1, 而不是0
- substr(string,position,max)	AWK, NAWK, GAWK
- sub(regex,replacement)	NAWK, GAWK
- sub(regex,replacement,string)	NAWK, GAWK
- gsub(regex,replacement)	NAWK, GAWK
- gsub(regex,replacement,string)	NAWK, GAWK
- match(string,regex)	NAWK, GAWK, eg:  `regex="[a-zA-Z0-9]+";`
- tolower(string)	GAWK
- toupper(string)	GAWK
- asort(string,[d])	GAWK
- asorti(string,[d])	GAWK
- gensub(r,s,h [,t])	GAWK
- strtonum(string)	GAWK

# 高级功能
## 高级命令
**system**
NAWK has a function system() that can execute any program. It returns the exit status of the program.
```
if (system("/bin/rm junk") != 0)
print "command did not work";
```
The command can be a string, so you can dynamically create commands based on input. 
Note that the output is not sent to the NAWK program. You could send it to a file, and open that file for reading. There is another solution, however.

**getline**
AWK has a command that allows you to force a new line. It does not take any arguments. It returns a 1, if successful, a 0 if end-of-file is reached, and a -1 if an error occurs.
度如下一行
As a side effect, the line containing the input changes.没有理解这个side effet 是什么意思?

**strftime**: GAWK has a special function for creating strings based on the current time.
The systime() function returns the current date in seconds. 
使用起来不是很方便,仅仅是在计算时间差的比较方便
```
strftime("%y_%m_%d_%H_%M_%S")
94_12_25_12_00_00
```

## 自定义函数
Finally, NAWK and GAWK support user defined functions.
```
function error(message){
    if(FILENAME != "-"){
        printf("%s: ", FILENAME) > "/dev/tty";
    }
    printf("line # %d, %s, line: %s\n", NR, message, $0) >> "/dev/tty";
}
```


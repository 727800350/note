pattern scanning and processing language
[awk ref](http://www.grymoire.com/Unix/Awk.html)

AWK - the (very old) original from AT&T
NAWK - A newer, improved version from AT&T
GAWK - The FGNU Project’s implementation, linux 中默认的版本.

**特性**
It can be considered to be a pseudo-C interpretor, as it understands the same arithmatic operators as C. 
AWK also has string manipulation functions, so it can search for particular strings and modify the output. 
AWK also has associative arrays, which are incredible useful, and is a feature most computing languages lack. Associative arrays can make a complex problem a trivial exercise.

If you find yourself needing a feature that is very difficult or impossible to do in AWK, I suggest you either use NAWK, or GAWK, or convert your AWK script into PERL using the "a2p" conversion program which comes with PERL.

# Basic Structure
```
pattern { action }
```
The pattern specifies when the action is performed.

AWK is line oriented. The default pattern is something that matches every line.

两个特殊的pattern
```
BEGIN { print "START" }
      { print         }
END   { print "STOP"  }
```
adds one line before and one line after the input file.
**BEGIN 后面的做大括号必须与BEGIN在同一行, END也一样.**


AWK does not evaluate variables within strings. To explain, the second line could not be written like this:
```
{print "$8\t$3" }
```
That example would print "$8 $3"
在引号里面, $符号变成了一般的字符, 就不是指示位置的了.

```
BEGIN { x=5 }
{ print x, $x}
```
会输出5和第5个field的value.

If you had four fields, and wanted to print out the second and fourth field, there are two ways. 
This is the first:
```
$1=""; $3=""; print;
```

and the second
```
print $2, $4;
```
两种都能够实现, 但是细节上还是不同的, 第一种方式只是把第1和第3个field 的value置为空的字符串,然后打印出整行, 但是output filed seperator还是3个.

## awk脚本
```
awk -f filename
```

或者在awk脚本的首行加上以下内容 `#!/bin/awk -f`, 然后加上x权限, 例如:
```
#!/bin/awk -f
BEGIN { print "File\tOwner" }
{ print $8, "\t", $3}
END { print " - DONE -" }
```

## Dynamic Variables
```
column 文件的内容
#!/bin/sh
column=$1
awk '{print $'$column'}'
```
执行上述脚本
```
ls -l | column 3
```
得到第3个field 的value

如果写成, `awk '{print $column}'`, 将不能工作, 单引号里面的变量$column的值将不会被awk看到, 需要把它分开,`awk '{print $'` 和 `$column` 和 `'}'` 三部分拼接而成

参数的默认值
```
${variable:-defaultvalue} 注意那个减号
```
例如: `column=${1:-1}`

# Syntax of AWK
## 运算符

Binary Operators

| Operator | Type       | Meaning        |
|----------|------------|----------------|
| +        | Arithmetic | Addition       |
| -        | Arithmetic | Subtraction    |
| *        | Arithmetic | Multiplication |
| /        | Arithmetic | Division, 5/2=2.5       |
| %        | Arithmetic | Modulo         |
| <space>  | String     | Concatenation, 字符串拼接  |

Regular Expressions

- ~	Matches
- !~	Does not match

A value of 0 is false, while anything else is true. Undefined variables has the value of 0.

And/Or/Not: "&&" / "||" / "!".

## commands 
There are only a few commands in AWK. The list and syntax follows:

```
if ( conditional ) statement [ else statement ]
while ( conditional ) statement
for ( expression ; conditional ; expression ) statement
for ( variable in array ) statement
break
continue
{ [ statement ] ...}
variable=expression
print [ expression-list ] [ > expression ]
printf format [ , expression-list ] [ > expression ]
next 
exit
```

```
#!/bin/awk -f
{
	if ( $0 ~ /:/ ) {
		FS=":";
		$0=$0;  ## 这句话是由于awk版本兼容性
	} else {
		FS=" ";
		$0=$0;
	}
	#print the third field, whatever format
	print $3
}
```

## Built-in Variables
- $0 refers to the entire line that AWK reads in

- FS - The Input Field Separator Variable
"-F": 在command line上指定awk的input field separator, 在脚本中是通过FS这个变量实现的.  
下面量段代码的功能是相同的, 前者是command line 的形式, 后者是script的形式

```
awk -F: '{if ($2 == "") print $1 ": no password!"}' </etc/passwd
```
与
```
#!/bin/awk -f
BEGIN {
	FS=":";
}
{
	if ( $2 == "" ) {
		print $1 ": no password!";
	}
}
```

如果FS中指定的是多个字符组成的字符串, 那么awk会严格匹配.  
If you change the field separator before you read the line, the change affects what you read. 
If you change it after you read the line, it will not redefine the variables. You would not want a variable to change on you as a side-effect of another action.
Note the field separator variable retains its value until it is explicitly changed.
```
#!/bin/awk -f
{
	print $2
	FS=":"
	print $2
}
```
输入: `One Two:Three:4 Five`  
print out "Two:Three:4" twice

- OFS - The Output Field Separator Variable
默认的OFS是一个空格
```
print $2 $3
```
$2和$3会拼接到一起成为一个field

- NF - The Number of Fields Variable

- NR - The Number of Records Variable, 一行就是一个record

- FNR: contains the number of lines read, but is **reset for each file read**. The NR variable accumulates for all files read.

- RS - The Record Separator Variable
Normally, AWK reads one line at a time, and breaks up the line into fields.
You can set the "RS" variable to change AWK definition of a "line". If you set it to an empty string, then AWK will read the entire file into memory. 
You can combine this with changing the "FS" variable.

- ORS - The Output Record Separator Variable

- FILENAME - The Current Filename Variable

## prinf
The printf is very similar to the C function with the same name, 用法也一样.

同时可以使用输出重定向
```
printf("string\n") > "/tmp/file";
printf("string\n") >> "/tmp/file";
```

### String Functions

- index(string,search)	AWK, NAWK, GAWK
- length(string)	AWK, NAWK, GAWK
- split(string,array,separator)	AWK, NAWK, GAWK
```
set time = 12:34:56
set hms = `echo $time | awk '{split($0,a,":" ); print a[1], a[2], a[3]}'` # = 12 34 56
```
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
function error ( message ) {
    if (FILENAME != "-") {
        printf("%s: ", FILENAME) > "/dev/tty";
    }
    printf("line # %d, %s, line: %s\n", NR, message, $0) >> "/dev/tty";
}
```

## AWK patterns
A pattern or condition is simply an abbreviated test. If the condition is true, the action is performed. All relational tests can be used as a pattern.

### if的省略
eg: The `head -10` command, which prints the first 10 lines and stops, can be duplicated with
```
{if (NR <= 10 ) {print}}
```
Changing the if statement to a condition shortens the code:
```
NR <= 10 {print}
```

### 含有regex的简化
`{if ($0 ~ /special/) {print}}` or more briefly `$0 ~ /special/ {print}` or `/special/ {print}`

### 逻辑运算法
可以使用逻辑运算符和括号
AND (&&), OR (||) and NOT (!) operator

### 开始与结束
```
/start/,/stop/ {print}
```
This form defines, in one line, the condition to turn the action on, and the condition to turn the action off. 
That is, when a line containing "start" is seen, it is printed. Every line afterwards is also printed, 
until a line containing "stop" is seen. This one is also printed, but the line after, and all following lines, are not printed.   
This triggering on and off can be repeated many times.
```
{
  if ($0 ~ /start/) {
    triggered=1;
  }
  if (triggered) {
     print;
     if ($0 ~ /stop/) {
	triggered=0;
     }
  }
}
```

最后附上所有的内置变量

| Variable    | Purpose                                                              | AWK | NAWK | GAWK |
|-------------|----------------------------------------------------------------------|-----|------|------|
| FS          | Field separator                                                      | Yes | Yes  | Yes  |
| NF          | Number of Fields                                                     | Yes | Yes  | Yes  |
| RS          | Record separator                                                     | Yes | Yes  | Yes  |
| NR          | Number of input records                                              | Yes | Yes  | Yes  |
| FILENAME    | Current filename                                                     | Yes | Yes  | Yes  |
| OFS         | Output field separator                                               | Yes | Yes  | Yes  |
| ORS         | Output record separator                                              | Yes | Yes  | Yes  |
| ARGC        | # of arguments                                                       |     | Yes  | Yes  |
| ARGV        | Array of arguments                                                   |     | Yes  | Yes  |
| ARGIND      | Index of ARGV of current file                                        |     |      | Yes  |
| FNR         | Input record number                                                  |     | Yes  | Yes  |
| OFMT        | Ouput format (default "%.6g")                                        |     | Yes  | Yes  |
| RSTART      | Index of first character after match()                               |     | Yes  | Yes  |
| RLENGTH     | Length of string after match()                                       |     | Yes  | Yes  |
| SUBSEP      | Default separator with multiple subscripts in array (default "\034") |     | Yes  | Yes  |
| ENVIRON     | Array of environment variables                                       |     |      | Yes  |
| IGNORECASE  | Ignore case of regular expression                                    |     |      | Yes  |
| CONVFMT     | conversion format (default: "%.6g")                                  |     |      | Yes  |
| ERRNO       | Current error after getline failure                                  |     |      | Yes  |
| FIELDWIDTHS | list of field widths (instead of using FS)                           |     |      | Yes  |
| BINMODE     | Binary Mode (Windows)                                                |     |      | Yes  |
| LINT        | Turns --lint mode on/off                                             |     |      | Yes  |
| PROCINFO    | Array of informaiton about current AWK program                       |     |      | Yes  |
| RT          | Record terminator                                                    |     |      | Yes  |
| TEXTDOMAIN  | Text domain (i.e. localization) of current AWK program               |     |      | Yes  |

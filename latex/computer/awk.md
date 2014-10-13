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
- !~	Doesn't match

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
If you change the field separator before you read the line, the change affects what you read. If you change it after you read the line, it will not redefine the variables. You wouldn't want a variable to change on you as a side-effect of another action.
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

- RS - The Record Separator Variable
Normally, AWK reads one line at a time, and breaks up the line into fields.
You can set the "RS" variable to change AWK's definition of a "line". If you set it to an empty string, then AWK will read the entire file into memory. 
You can combine this with changing the "FS" variable.

- ORS - The Output Record Separator Variable

- FILENAME - The Current Filename Variable


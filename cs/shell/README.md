主要参考自:

1. [Shell脚本编程30分钟入门](https://github.com/qinjx/30min_guides/blob/master/shell.md)
1. [ShellProgramming](http://mprc.pku.edu.cn/mentors/training/TrainingCourses/material/ShellProgramming.HTM#\_Toc37518066)

```bash
cmds_0
{
  cmds_1
} &
cmds_2
```
一组命令作为一个整体在后台运行.
但是需注意: **这种后台方式是新开一个shell环境来执行cmds_1, 所以在后台cmds_1中改变的变量, 新定义的变量在cmds_2 都无效**.

# time
计算时间差

- `date +"%Y%m%d"`
- `date +"%Y%m%d" --date="-1 day"`
- `date +"%Y%m%d" --date="-1 week"`

构造命令字符串, 然后执行(双引号不能丢)
```bash
$ bash -c "date"
Wed Apr 22 19:08:12 CST 2015
```

# tips
- `set -x`
- `set -o pipefail`: the return value of pipeline is the value of the last (rightmost) command to exit with a non-zero status, or zero if all are zero

- && and ||: 如果这个命令执行成功&&那么执行这个命令", || 则相反
- 脚本里没有`ll`命令, 需要使用 `ls -l`
- 在shell 脚本里一定要注意路径
  ```bash
  dirname /dir/path.sh  ## /dir
  basename /dir/path.sh  ## path.sh
  readlink -f ./file  ## 获得绝对路径
  ```
- 当运行一个需要很长时间才能结束的脚本时, 通过`nohup &` 让其在后台运行, 且在脚本的末尾加上 `mail`, 发送邮件通知
  ```bash
  echo hello | mail -s 'subject' target@xxx.com -- -f 12306@rails.com.cn  ## 任意指定发件人(这里要注意先是有两个短杠, 所以, 电子邮件是可以伪造的)
  ```

# IO
`printf %05d 123`: 输出是00123,且没有换行符, 显示5位数,不够5位的用0补充, 多余的不会进行截断

## 管道
每个shell 过程一次最多可以有9 个文件描述符(其中 0(stdin),1(stdout),2(stderr)作为保留的文件描述符)
管道会产生了子shell, 在管道命令里面修改的全局变量在管道之后是无效的.

## 输出重定向
文件描述符必须放在重定向符号前, 该值必须紧紧地放在重定向符号前, eg: `2>log.error`

- `cmd 1>out 2>err.log`: 重定向stdout 和stderr
- `cmd 1>out 2>&1`: 把 stderr 也重定向到 stdout

## [Here Document](https://juejin.im/post/6844903854023114760)
Here Document 就是标准输入的一种替代品.它使得脚本开发人员可以不必使用临时文件来构建输入信息,而是直接就地生产出一个文件并用作命令的标准输入.
一般来说其格式是这样的:
```bash
COMMAND <<IDENT
...
IDENT
```
在这里,<< 是引导标记,IDENT 是一个限定符,由开发人员自行选定,两个 IDENT 限定符之间的内容将被当做是一个文件并用作 COMMAND 的标准输入.
例如echo大段文本时, 我们可以使用 cat file 的语法:
```bash
cat <<EOF
SOME TEXT
HERE
!
EOF
```

# variables and types
## 一些常用的shell变量
```info
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
比如`shift 3` 表示原来的$4现在变成$1,原来的$5现在变成$2等等,原来的$1,$2,$3丢弃,$0不移动.
不带参数的shift命令相当于shift 1.  
Shift 命令还有另外一个重要用途, Bash 定义了9个位置变量,从 $1 到 $9,这并不意味着用户在命令行只能使用9个参数,借助 shift 命令可以访问多于9个的参数.
Shift 命令一次移动参数的个数由其所带的参数指定.例如当 shell 程序处理完前九个命令行参数后,可以使用 shift 9 命令把 $10 移到 $1.

## 数学运算
1. 使用 `$(())` 将表达式放在括号中,即可达到运算的功能, eg: `$a=1; b=2; echo $(($a+$b))`: 得到3
1. 使用expr, eg: `$expr 1 + 2`: 得到3, 对于乘法, 运算符需要使用 `\*`, 而不能直接使用`*`.

注意:将需要运算的表达式写入在expr 后面即可,保证**参数与运算符号中间有空格隔开**

上面的两种方式都不支持浮点运算, 如果需要, 可以借用bc 或 awk 来实现.
```bash
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

- 单引号里的任何字符都会原样输出,单引号字符串中的变量是无效的, 转义符也不太好用
- 双引号里可以有变量,双引号里可以出现转义字符

### 比较大小
类似于的 C 语言的strcmp

| operator | meaning                |
|----------|------------------------|
| >        | 大于                   |
| <        | 小于                   |
| =        | 相等                   |
| !=       | 不等                   |
| -z       | 空串                   |
| -n       | 非空串                 |

```bash
if [[ "x$var" < "xabc" ]]
then
  echo "xxx"
fi
```

### String Length
```bash
str="abcABC123ABCabc"
echo ${#str}  ## 15
echo `expr length ${str}`  ## 15
```

### Substring
- `${string:position}`: 在$string中, 从位置$position开始提取子串
- `${string:position:length}`: 在$string中, 从位置$position开始提取长度为$length的子串

字符串的第一个字符, position 为 0

position 可以为负值
```bash
echo ${str: -0}
echo ${str: -1} ## c
echo ${str: -3:2} ## ab
```

substr: 提取子字符串的另外一种方式, str 的第一个字符position 为 1
```bash
echo `expr substr $str 1 2` ## ab
```

- `expr match "$string" '\($substring\)'`: Extracts $substring at **beginning** of $string, where $substring is a regular expression.
- `expr match "$string" '.*\($substring\)'`: Extracts $substring at **end** of $string.
  这个实际上还是从beginning 开始匹配, 但是 `.*` 匹配开头所有部分, 只有 substring 是在 `\(\)` 里面
```bash
echo `expr match "$str" '\(.[b-c]\)'`  ## ab
echo `expr match "$str" '.*\([A-C][A-C][A-C][a-c]*\)'`  ## ABCabc
```

### Substring Removal
- ${string#substring} Deletes **shortest** match of $substring from **front** of $string.
- ${string##substring} Deletes **longest** match of $substring from **front** of $string.

```bash
echo ${str#a*C}      # 123ABCabc
echo ${str##a*C}     # abc
```
a*C 在 str 中有两个匹配, abcABC, abcABC123ABC, `#` 使用短的, `##` 使用长的

- ${string%substring} Deletes **shortest** match of $substring from **back** of $string.
- ${string%%substring} Deletes **longest** match of $substring from **back** of $string.

```bash
echo ${str%b*c}      # abcABC123ABCa
echo ${str%%b*c}     # a
```

### Substring Replacement
- ${string/substring/replacement} Replace first match of $substring with $replacement
- ${string//substring/replacement} Replace all matchs

```bash
echo ${str/abc/xyz}       # xyzABC123ABCabc
echo ${str//abc/xyz}      # xyzABC123ABCxyz
```

What happens if no $replacement string is supplied?
```bash
echo ${str/abc}           # ABC123ABCabc
echo ${str//abc}          # ABC123ABC
```
A simple deletion takes place.

- ${string/#substring/replacement} If $substring matches front end of $string, substitute $replacement for $substring.
- ${string/%substring/replacement} If $substring matches back end of $string, substitute $replacement for $substring.

使用了 `#` 或者 `%` 之后, 即时加上了 `//`, 也还是只对 `#, %` 部分生效

```bash
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
```bash
array_name=(value1 ... valuen)
```
例如:
```bash
array_name=(value0 value1 value2 value3)
```
还可以单独定义数组的各个分量:
```bash
array_name[0]=value0
array_name[1]=value1
array_name[2]=value2
```

```bash
list=()
list=(v0 v1 v2)
list+=(v3)  ## append
```

可以不使用连续的下标,而且下标的范围没有限制.

### 读取数组
读取数组元素值的一般格式是:
```bash
${array_name[index]}
```
例如: `valuen=${array_name[2]}`

使用@ 或 * 可以获取数组中的所有元素,例如:
```bash
${array_name[*]}
${array_name[@]}
```

遍历数组
```bash
for item in ${array[*]}
do
  ...
done
```

当数组的元素中含有空格时, 例如 `array=("bash -x ./test.sh")`, 本来我们预期结果是array 就一个元素,
但是如果采用上面的遍历方式, 这个元素会被拆分为 3 个元素,`bash`, `-x`, `./test.sh`.
解决方法: "${array[@]}". [ref](http://unix.stackexchange.com/questions/108635/why-i-cant-escape-spaces-on-a-bash-script)

- `${arr[*]}`: will suffer splitting on all spaces, leading to an unpredictable number of separate words,
- `"${arr[*]}"`: will yield all the array members in one string
- `"${arr[@]}"`: will very nicely offer each array member as an individual unbroken quoted string.

### 获取数组的长度
获取数组长度的方法与获取字符串长度的方法相同,例如:
```bash
# 取得数组元素的个数
num_items=${#array_name[@]}
num_items=${#array_name[*]}
```

# 程序流程, 结构化命令
## 条件判断
- `-o`: 逻辑或
- `-a`: 逻辑和

### 数值测试
数值的运算最好通过awk 来进行, shell 本身只支持整数运算

其中:number_operator 可以为:`-eq, -ne, -gt, -lt, -ge`

### 文件状态测试(常用的)
```info
-e  测试文件是否存在
-d  测试是否文件夹
-f  测试是否一般文件
-s  测试文件是否非空
-L  测试是否链接文件
-r  测试文件是否可读
-w  测试文件是否可写
-x  测试文件是否可执行
```

## 流程控制
### if
```bash
if condition; then
  commands
fi
```
或者then换行, 那么condition后面的分号就不要了

```bash
if condition1
then
  command1
elif condition2
then
  command2
else
  commandN
fi
```

### for while
在开篇的示例里演示过了:
```bash
for var in item1 item2 ... itemN
do
  command
done

num=100
for((i=0;i<=$num;i++))
do
  command
done

while condition
do
  command
done
```

无限循环
```bash
while true
do
  command
done
```

### case
```bash
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
```

```bash
while getopts p:m:d option; do
  case "$option" in
    p) path=$OPTARG;;
    m) module=$OPTARG;;
    d) debug=$OPTARG;;
    ?) echo "unsupported option" >&2
  esac
done
```

## 函数
### 定义
[ref](http://www.cnblogs.com/chengmo/archive/2010/10/17/1853356.html)
```bash
[ function ] funname[()]
{
    action;
    [return int;]
}
```

变量作用域

1. 定义函数可以与系统命令相同, 自定义的函数命令比系统自带的命令优先级高
1. 如果需要传出其它类型函数值,可以在函数调用之前,定义变量(这个就是全局变量),在函数内部就可以直接修改,然后在执行函数就可以修改
1. 如果需要定义local变量,可以在函数中定义:`local 变量=值`, 这时变量就是内部变量,它的修改,不会影响函数外部相同变量的值

cat a.sh
```bash
name="a"
echo $name
```

cat b.sh
```bash
name="b"
echo $name
operation ./a.sh
echo $name
```
- 当operation 为 bash 时, 最后一个 $name 仍为 "b", 也就是说bash ./a.sh 是在另外一个shell 环境中运行的.
- 当operation 为source时, 最后一个 $name 仍为 "a", 也就是说source ./a.sh 是将a.sh 引入到当前的shell 中运行.

# Util
[Shell脚本实现乱序排列文件内容的多种方法](http://www.cnblogs.com/clarke/p/5447389.html)

- 利用 Shell 的 `$RANDOM` 变量给原文件的每一行加上随机的行号然后根据这个随机行号进行排序,再把临时加上去的行号给过滤掉,这样操作之后得到的新文件就相当于被随机"洗"了一次:
`cat - | while read line; do echo "$line $RANDOM"; done | sort -k 2 -n | cut -d" " -f1`

- 利用AWK中散列的特性(详细请看:info gawk 中的7.x ),只要构造一个随机不重复的散列函数即可,因为一个文件每行的linenumber是独一无二的,所以用: `随机数＋每行linenumber --对应--> 那一行的内容`, 即为所构造的随机函数.
`cat - | awk 'BEGIN{srand()}{b[rand()NR] = $0}END{for(x in b) print b[x]}'`


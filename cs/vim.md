Markdown 高亮配置

1. 在[http://www.vim.org/](http://www.vim.org/scripts/script.php?script_id=2882)上下载高亮显示的配置文件
1. 用vim 打开下载到的文件(windows 下需要打开解压之后的文件)
1. `:source %`

# Move
| Movement                            | Commands   |
|-------------------------------------|------------|
| ←, ↓, ↑, →                      | h, j, k, l |
| To first character of next line     | +          |
| To first character of previous line | -          |
| To end of word                      | e or E     |
| Forward by word                     | w or W     |
| Backward by word                    | b or B     |
| To end of line                      | $          |
| To beginning of line                | 0          |
| Forward by window                   | ctrl + f   |
| Backward by window                  | ctrl + b   |
| %									  | 跳到配对的括号|
| gD 								  | 跳转到局部变量的定义处|
|nG or :n							  | 到第n行| 
|gg 								  | 到第一行|
|G 									  | 最后一行|
|H									  | high, 跳到屏幕上端|
|L 									  | low, 跳到屏幕下端|
|M 									  | middle, 跳到屏幕中间|

typing two backquotes \`\` returns you to the position where you issued the G command

`w` move the cursor forward one word, **counting symboles and punctuations as equivalent to a word**  
`W` not counting

`[[`  跳转到代码块的开头去(但要求代码块中'{'必须单独占一行) 

`''`  跳转到光标上次停靠的地方, 是两个, 而不是一个

**书签**  
`mx`  设置书签,x 只能是 a-z 的 26 个字母  
\`x  跳转到书签处(是 1 左边的键) 

**缩进**  
`>`  增加缩进, `x>` 表示增加以下 x 行的缩进   
`<`  减少缩进, `x<` 表示减少以下 x 行的缩进

# Edit
- Vim高亮显示的那个特殊的`^M`, 是Ctrl+V
- `Shift-v shift-g`  选择当前位置到文章结尾  
- `shfit-v $` 选择当前位置到当前行尾

transposing two letters `xp`, 也就是一个`x`, 然后再`p`

| fullname| abbreviation | example                                           |
|--------|--------|----------------------------------------------------------|
| delete | d      | :2,8d                                                    |
| move   | m      | :160,240m23 #moves line 160 through 240 to fllow line 23 |
| copy   | co     | :23,29co100                                              |

Editing action

| Editing action                               | Command |
|----------------------------------------------|---------|
| Insert text at current position              | i       |
| Insert text at beginning of line             | I       |
| Append text at current position              | a       |
| Append text at beginning of line             | A       |
| Open new line below cursor for new text      | o       |
| Open new line above cursor for new text      | O       |
| Delete line and substitute text              | S       |
| Overstrike existing characters with new text | R       |
| Join current and next line                   | J       |
| Toggle case                                  | ~       |
| Repeat last action                           | .       |
| Undo last change                             | u       |
| Restore line to original state               | U       |


Edit commands

| Text object                         | Change     | Delete     | Copy       |
|-------------------------------------|------------|------------|------------|
| One word                            | cw         | dw         | yw         |
| Two words, not counting punctuation | 2cW or c2W | 2dW or d2W | 2yW or y2W |
| Three words back                    | 3cb or c3b | 3db or d3b | 3yb or y3b |
| One line                            | cc         | dd         | yy or Y    |
| To end of line                      | c$ or C    | d$ or D    | y$         |
| To beginning of line                | c0         | d0         | y0         |
| Single character                    | r          | x or X     | yl or yh   |
| Five characters                     | 5s         | 5x         | 5yl        |


More editing commands

| Change     | Delete     | Copy       | From cursor to...         |
|------------|------------|------------|---------------------------|
| cH         | dH         | yH         | Top of screen             |
| cL         | dL         | yL         | Bottom of screen          |
| c+         | d+         | y+         | Next line                 |
| c5|        | d5|        | y5|        | Column 5 of current line  |
| 2c)        | 2d)        | 2y)        | Second sentence following |
| c{         | d{         | y{         | Previous paragraph        |
| c/ pattern | d/ pattern | y/ pattern | Pattern                   |
| cn         | dn         | yn         | Next pattern              |
| cG         | dG         | yG         | End of file               |
| c13G       | d13G       | y13G       | Line number 13            |

# 大小写
| Command | Explanation        |
|---------|--------------------|
| ?~       | 将光标下的字母改变大小写       |
| 3~      | 将光标位置开始的3个字母改变其大小写 |
| g~~     | 改变当前行字母的大小写        |
| U       | 将可视模式下选择的字母全改成大写字母 |
| u       | 将可视模式下选择的字母全改成小写   |
| gUU     | 将当前行的字母改成大写        |
| 3gUU    | 将从光标开始到下面3行字母改成大写  |
| guu     | 将当前行的字母全改成小写       |
| gUw     | 将光标下的单词改成大写        |
| guw     | 将光标下的单词改成小写        |

`\u or \l`  
Causes the next character in the replacement string to be changed to uppercase or lowercase, respectively.

`\U or \L and \e or \E`  
all following characters are converted to uppercase or lowercase until the end of the replacement string or until \e or \E is reached.   
`:%s/Fortran/\U&/`

# Match
删除包含特定字符的行(全局匹配): `g/pattern/d`

删除不包含指定字符的行: `v/pattern/d` 或者`g!/pattern/d`

## Replacement
`&` is replaced by the entire text matched by the search pattern when used in a replacement  
`:%s/Yazstremski/&,clar/`  #the result is `Yazstremski,clar`  
`:1,10s/.*/(&)/` #surrend each line from line 1 to line 10 with parentheses

**替换变量**: 
在正规表达式中使用 `\(` 和 `\)` 符号括起正规表达式,即可在后面使用`\1`,`\2`等变量来访问 `\(` 和 `\)` 之间的内容

[vim 正则表达式](http://blog.csdn.net/yyt8yyt8/article/details/7567455)

**表示内容的元字符**

| 模式     | 含义                                               |
|--------|--------------------------------------------------|
| .      | 匹配任意字符                                           |
| [abc]  | 匹配方括号中的任意一个字符.可以使用-表示字符范围,如[a-z0-9]匹配小写字母和阿拉伯数字. |
| [^abc] | 在方括号内开头使用^符号,表示匹配除方括号中字符之外的任意字符.                 |
| \| 	 |或,如:for\|bar 表示匹配for或者bar|
| \d     | 匹配阿拉伯数字,等同于[0-9].                                |
| \D     | 匹配阿拉伯数字之外的任意字符,等同于[^0-9].                        |
| \x     | 匹配十六进制数字,等同于[0-9A-Fa-f].                         |
| \X     | 匹配十六进制数字之外的任意字符,等同于[^0-9A-Fa-f].                 |
| \w     | 匹配单词字母,等同于[0-9A-Za-z_].                          |
| \W     | 匹配单词字母之外的任意字符,等同于[^0-9A-Za-z_].                  |
| \t     | 匹配<TAB>字符.                                       |
| \s     | 匹配空白字符,等同于[ \t].                                 |
| \S     | 匹配非空白字符,等同于[^ \t].                               |
| \a     | 所有的字母字符. 等同于[a-zA-Z]                             |
| \l     | 小写字母 [a-z]                                       |
| \L     | 非小写字母 [^a-z]                                     |
| \u     | 大写字母 [A-Z]                                       |
| \U     | 非大写字母 [^A-Z]                                     |

一般的正则表达式中表示或的方法:`(my name)|(your name)`

**表示数量的元字符**

| 模式     | 含义                                             |
|--------|------------------------------------------------|
| *      | 匹配0-任意个                                        |
| \+     | 匹配1-任意个                                        |
| \?     | 匹配0-1个                                         |
| \{n,m} | 匹配n-m个                                         |
| \{n}   | 匹配n个                                           |
| \{n,}  | 匹配n-任意个                                        |
| \{,m}  | 匹配0-m个                                         |
| \_.    | 匹配包含换行在内的所有字符                                  |
| \{-}   | 表示前一个字符可出现零次或多次,但在整个正则表达式可以匹配成功的前提下,匹配的字符数越少越好 |
| \=     | 匹配一个可有可无的项                                     |
| \_s    | 匹配空格或断行                                        |

**懒惰模式**  
`\{-n,m}` 与`\{n,m}`一样,尽可能少次数地重复  
`\{-}` 匹配它前面的项一次或0次, 尽可能地少

**表示转义和位置的元字符**

| 模式 | 含义      |
|----|------------|
| `\*` | 匹配 `*` 字符 |
| `\.` | 匹配 `.` 字符 |
| `\/` | 匹配 `/` 字符 |
| `\\` | 匹配 `\` 字符 |
| `\[` | 匹配 `[` 字符 |
| `$`  | 匹配行尾   |
| `^`  | 匹配行首   |
| `\<` | 匹配单词词首 |
| `\>` | 匹配单词词尾 |

# File
- `:w` write the buffer to the file
- `:230,$w newfile` #saves line 230 through the end of the file as a newfile
- `:230,$w >> newfile`
- `:q` quit
- `:w!` force write
- `:r` read
- `:$r filename` #place the content of `filename` at the end of the current session
- `:e filename` # editing another file, use `Ctrl+^` to swith to another file 
- `:! command` execute the command in the shell
- `vi + filename` open file at last line
- `vi +/pattern filename` open file at the first occurrence of pattern
- `vi -R filename` readmode
eg: `vi +/"you make"` filename or `vi +/you\ make filename`  
Using `+/pattern` is helpful if you have to leave an editing session before you're finished. You can mark your place by inerting a pattern such as `ZZZ` or `HERE`. Then when you return to the file,all you have to remember is `/ZZZ or /HERE`
- `vim -b file` edit binary file or we can use `:set binary`

**gain root permission without leaving vim**  
Often when I edit some files which require root permission (e.g. the files under /etc) I forget run vim with sudo.  
To force a save use the following command  
`:w !sudo tee %`  
It will prompt you for your password

## Buffer
**recovering from buffer**  
`$ex -r` or `$vi -r`  
you will get a list of any files that the system has saved  
`$vi -r file` #to recover the file

# Window
[ref](http://www.linuxidc.com/Linux/2012-08/69363.htm)
## 打开多个窗口
横向切割窗口

- :new+窗口名(保存后就是文件名) 
- :split+窗口名,也可以简写为:sp+窗口名

纵向切割窗口名  
:vsplit+窗口名,也可以简写为:vsp+窗口名

## 关闭多窗口
可以用:q!,也可以使用:close,最后一个窗口不能使用close关闭.使用close只是暂时关闭窗口,其内容还在缓存中,只有使用q!,w!或x才能真能退出.

- :tabc 关闭当前窗口
- :tabo 关闭所有窗口

## 窗口切换
快速双击ctrl+w依次切换窗口.

由于你可以用垂直分割和水平分割命令打开任意多的窗口,你就能够任意设置窗口的布
局.接着,你可以用下面的命令在窗口之间跳转:

- CTRL-W h 跳转到左边的窗口
- CTRL-W l 跳转到右边的窗口
- CTRL-W j 跳转到下面的窗口
- CTRL-W k 跳转到上面的窗口
- CTRL-W t 跳转到最顶上的窗口
- CTRL-W b 跳转到最底下的窗口

## 窗口大小调整
纵向调整

- :ctrl+w + 纵向扩大(行数增加)
- :ctrl+w - 纵向缩小 (行数减少)
- :res(ize) num  例如::res 5,显示行数调整为5行
- :res(ize)+num 把当前窗口高度增加num行
- :res(ize)-num 把当前窗口高度减少num行

横向调整

- :vertical res(ize) num 指定当前窗口为num列
- :vertical res(ize) +num 把当前窗口增加num列
- :vertical res(ize) -num 把当前窗口减少num列

## 给窗口重命名
:f file

## vi打开多文件  
`vi a b c`

- :n 跳至下一个文件,也可以直接指定要跳的文件,如:n c,可以直接跳到c文件
- :e# 回到刚才编辑的文件

##文件浏览
- :Ex 开启目录浏览器,可以浏览当前目录下的所有文件,并可以选择
- :Sex 水平分割当前窗口,并在一个窗口中开启目录浏览器
- :ls 显示当前buffer情况

## vi与shell切换
- :shell 可以在不关闭vi的情况下切换到shell命令行
- exit 再从shell回到vi

# Tools
## cscope
cscope 自身带一个基于文本的用户界面,不过 gvim 提供了cscope接口

	$cscope -Rbkq
	R 表示把所有子目录里的文件也建立索引
	b 表示cscope不启动自带的用户界面,而仅仅建立符号数据库
	q 生成cscope.in.out和cscope.po.out文件,加快cscope的索引速度
	k 在生成索引文件时,不搜索/usr/include目录

使用cs find 查找[3] (也可以简化写成 cs f)后面添加需要字符串类型

- c: Find functions **c**alling this function//查找调用本函数的函数
- d: Find functions calle**d** by this function //查找本函数调用的函数
- e: Find this **e**grep pattern//查找egrep模式,相当于egrep功能,但查找速度快多了
- f: Find this **f**ile //查找并打开文件,类似vim的find功能
- g: Find this **g**lobal definition//查找函数,宏,枚举等定义的位置,类似ctags的功能
- i: Find files #**i**ncluding this file //查找包含本文件的文件
- s: Find this C **s**ymbol //查找C语言符号,即查找函数名,宏,枚举值等出现的地方
- t: Find assignments **t**o //查找指定的字符串

例如:使用cscope查找do_fork函数的定义,在vim命令行下执行 `:cs f g do_fork`

## ctags
If you are currently sitting in the directory you want to index, just run:  
`ctags -R .`

press `Ctrl + ]`, jump to the source  
press `Ctrl + o` or `Ctrl + t` to climb back  
`C-W C-]` - Open the definition in a horizontal split, 这个动作可以进行多次, 会产生多个窗口(`ctrl+w+j/k`,上下切换;或者`ctrl+w`加上下左右键;还可以通过快速双击`ctrl+w`依次切换窗)

You can also go directly to a tag definition by entering one of the following in vim command mode:  

	:tag function_name
	:ta function_name

These commands will also accept regular expressions, so, for example,   
`:tag /^asserts_*` would find all tags that start with `asserts_`   
By default vim will jump to the first result, but a number of commands can be used to sort through the list of tags:

- :ts or :tselect shows the list
- :tn or :tnext goes to the next tag in that list
- :tp or :tprev goes to the previous tag in that list
- :tf or :tfirst goes to the first tag of the list
- :tl or :tlast goes to the last tag of the list
- To show the tags you have traversed since you opened vim, run :tags.

**windows**  
将 `E:\Program Files\ctags58` 添加到path中

## 代码格式化
### Vim
- 全部缩进 `gg=G`
- 缩进当前以下10行 `10==`
- 缩进第10行到第20行 `10G=20G`

Explanation:   
`gg` goes to the top of the file,   
`=` is a command to fix the indentation and  
`G` tells it to perform the operation to the end of the file.

### astyle
Artistic Style is a source code indenter, formatter, and beautifier for the `C, C++, C# and Java` programming languages

Open vim, gVim or MacVim, and after editing some C/C++ files, use the following command to reformat the source code:  
`:%!astyle`

[各个具体的设置](http://astyle.sourceforge.net/astyle.html)

## autocomplete
在使用autocompletion的功能时,`ctrl-n` 与`ctrl-p`在选择项中移动,`enter`确定选择,`ctrl-e` 取消自动完成

	(comm)(number)(text object)
	(number)(command)(text object)
`text object` is ofen a movement command
`ex` indicates text block with line address
`vi` indicates text block with text object,that is movement command

`sed` steam editor
`awk` data manipulation language


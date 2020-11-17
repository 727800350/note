- `*`可以查找当前光标下的word(完全符合),`g*`则部分符合,以`#`代替`*`表示向后(上)找.
- typing two backquotes \`\` returns you to the position where you issued the G command
- `[[`  跳转到代码块的开头去(但要求代码块中'{'必须单独占一行)
- `''`  跳转到光标上次停靠的地方, 是两个, 而不是一个

对于已保存的文件,可以使用下面的方法进行[空格和TAB的替换](http://blog.csdn.net/jiang1013nan/article/details/6298727):
TAB替换为空格:
```vim
:set ts=2
:set expandtab
:%retab!
```

空格替换为TAB:
```vim
:set ts=2
:set noexpandtab
:%retab!
```
加!是用于处理非空白字符之后的TAB,即所有的TAB,若不加!,则只处理行首的TAB.

# Move
| Movement                            | Commands               |
|-------------------------------------|------------------------|
| To first character of next line     | +                      |
| To first character of previous line | -                      |
| To end of word                      | e or E                 |
| Forward by word                     | w or W                 |
| Backward by word                    | b or B                 |
| Forward by window                   | ctrl + f               |
| Backward by window                  | ctrl + b               |
| %                                   | 跳到配对的括号         |
| gD                                  | 跳转到局部变量的定义处 |
| nG or :n                            | 到第n行                |
| H                                   | high, 跳到屏幕上端     |
| L                                   | low, 跳到屏幕下端      |
| M                                   | middle, 跳到屏幕中间   |

书签

- `mx`  设置书签,x 只能是 a-z 的 26 个字母
- \`x  跳转到书签处(是 1 左边的键)

# Edit
缩进

- `>`  增加缩进, `x>` 表示增加以下 x 行的缩进
- `<`  减少缩进, `x<` 表示减少以下 x 行的缩进

- Vim高亮显示的那个特殊的`^M`, 是Ctrl+V
- transposing two letters `xp`, 也就是一个`x`, 然后再`p`

## visual mode
在Vim里面有三种激活可视模式,并选择一块区域的方法,他们分别是:

| 模式类型 | 激活方式 | 选择效果             | status  |
| ---      | ---      | ---                  | ---     |
| 字符文本 | v(小写)  | 逐个字符选择文本     | VISUAL  |
| 行文本   | V(大写)  | 逐行选择文本         | V-LINE  |
| 块文本   | <C-v>    | 按照块的方式选择文本 | V-BLOCK |

visual mode 下

- o: 可视模式的时候, 默认情况下只可以控制右下角的端点,使用o按键来在左上角和右下角之间进行切换.
- G: 选择当前位置到文章结尾
- $: 选择当前位置到当前行尾

- 列删除
  1. 按下组合键"CTRL+v" 进入可视块模式,选取这一列操作多少行
  1. 按下d 即可删除被选中的整块

- 列插入
  1. 按下组合键"CTRL+v" 进入可视块模式,选取这一列操作多少行
  1. 按下shift+i
  1. 输入要插入的内容(这里要注意只会第一行会显示插入的内容, 其他行需要按下esc 之后才会显示, 新手应注意)
  1. 按ESC,之后就会看到插入的效果

Editing action

| Editing action                               | Command |
|----------------------------------------------|---------|
| Delete line and substitute text              | S       |
| Overstrike existing characters with new text | R       |
| Toggle case                                  | ~       |
| Repeat last action                           | .       |

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

## 大小写
| Command | Explanation                          |
|---------|--------------------------------------|
| ?~      | 将光标下的字母改变大小写             |
| 3~      | 将光标位置开始的3个字母改变其大小写  |
| g~~     | 改变当前行字母的大小写               |
| U       | 将可视模式下选择的字母全改成大写字母 |
| u       | 将可视模式下选择的字母全改成小写     |
| gUU     | 将当前行的字母改成大写               |
| 3gUU    | 将从光标开始到下面3行字母改成大写    |
| guu     | 将当前行的字母全改成小写             |
| gUw     | 将光标下的单词改成大写               |
| guw     | 将光标下的单词改成小写               |

- `\u or \l`: Causes the next character in the replacement string to be changed to uppercase or lowercase, respectively.
- `\U or \L and \e or \E`: all following characters are converted to uppercase or lowercase until the end of the replacement string or until \e or \E is reached.
- `:%s/Fortran/\U&/`

# Match
删除包含特定字符的行(全局匹配): `g/pattern/d`

删除不包含指定字符的行: `v/pattern/d` 或者`g!/pattern/d`

## Replacement
- `%s/a/b/gc`: 末尾的c代表确认的意思, 在进行每一次替换时会询问是否替换
- `&` is replaced by the entire text matched by the search pattern when used in a replacement
- `:%s/Yazstremski/&,clar/`  #the result is `Yazstremski,clar`
- `:1,10s/.*/(&)/` #surrend each line from line 1 to line 10 with parentheses
- 在正规表达式中使用 `\(` 和 `\)` 符号括起正规表达式,即可在后面使用`\1`,`\2`等变量来访问 `\(` 和 `\)` 之间的内容

[vim 正则表达式](http://blog.csdn.net/yyt8yyt8/article/details/7567455)

表示内容的元字符

| 模式   | 含义                                              |
| ---    | ---                                               |
| .      | 匹配任意字符                                      |
| [abc]  | 匹配方括号中的任意一个字符.可以使用-表示字符范围  |
| [^abc] | 表示匹配除方括号中字符之外的任意字符.             |
| \|     | 或, 如 for\ | bar 表示匹配for或者bar              |
| \d     | 匹配阿拉伯数字,等同于[0-9].                       |
| \D     | 匹配阿拉伯数字之外的任意字符,等同于[^0-9]         |
| \x     | 匹配十六进制数字,等同于[0-9A-Fa-f].               |
| \X     | 匹配十六进制数字之外的任意字符,等同于[^0-9A-Fa-f] |
| \w     | 匹配单词字母,等同于`[0-9A-Za-z_]`                 |
| \W     | 匹配单词字母之外的任意字符,等同于`[^0-9A-Za-z_]`  |
| \t     | 匹配<TAB>字符.                                    |
| \s     | 匹配空白字符,等同于[ \t].                         |
| \S     | 匹配非空白字符,等同于[^ \t].                      |
| \a     | 所有的字母字符. 等同于[a-zA-Z]                    |
| \l     | 小写字母 [a-z]                                    |
| \L     | 非小写字母 [^a-z]                                 |
| \u     | 大写字母 [A-Z]                                    |
| \U     | 非大写字母 [^A-Z]                                 |

一般的正则表达式中表示或的方法:`(my name)|(your name)`

表示数量的元字符

| 模式   | 含义                                                                                       |
| ---    | ---                                                                                        |
| *      | 匹配0-任意个                                                                               |
| \+     | 匹配1-任意个                                                                               |
| \?     | 匹配0-1个                                                                                  |
| \{n,m} | 匹配n-m个                                                                                  |
| \{n}   | 匹配n个                                                                                    |
| \{n,}  | 匹配n-任意个                                                                               |
| \{,m}  | 匹配0-m个                                                                                  |
| \_.    | 匹配包含换行在内的所有字符                                                                 |
| \{-}   | 表示前一个字符可出现零次或多次,但在整个正则表达式可以匹配成功的前提下,匹配的字符数越少越好 |
| \=     | 匹配一个可有可无的项                                                                       |
| \_s    | 匹配空格或断行                                                                             |

懒惰模式

- `\{-n,m}` 与`\{n,m}`一样,尽可能少次数地重复
- `\{-}` 匹配它前面的项一次或0次, 尽可能地少

**表示转义和位置的元字符**

| 模式 | 含义          |
| ---  | ---           |
| `\*` | 匹配 `*` 字符 |
| `\.` | 匹配 `.` 字符 |
| `\/` | 匹配 `/` 字符 |
| `\\` | 匹配 `\` 字符 |
| `\[` | 匹配 `[` 字符 |
| `$`  | 匹配行尾      |
| `^`  | 匹配行首      |
| `\<` | 匹配单词词首  |
| `\>` | 匹配单词词尾  |

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
- `vim -b file` edit binary file or we can use `:set binary`

**gain root permission without leaving vim**  
Often when I edit some files which require root permission (e.g. the files under /etc) I forget run vim with sudo.  
To force a save use the following command  
`:w !sudo tee %`  
It will prompt you for your password

# Buffer
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

## 文件浏览
- :Ex 开启目录浏览器,可以浏览当前目录下的所有文件,并可以选择
- :Sex 水平分割当前窗口,并在一个窗口中开启目录浏览器
- :ls 显示当前buffer情况

## vi与shell切换
- :shell 可以在不关闭vi的情况下切换到shell命令行
- exit 再从shell回到vi

## 代码格式化
- 全部缩进 `gg=G`
- 缩进当前以下10行 `10==`
- 缩进第10行到第20行 `10G=20G`

Explanation:

- `gg` goes to the top of the file
- `=` is a command to fix the indentation and
- `G` tells it to perform the operation to the end of the file.


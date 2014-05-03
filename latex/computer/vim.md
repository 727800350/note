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

`''`  跳转到光标上次停靠的地方, 是两个', 而不是一个"

**书签**  
`mx`  设置书签,x 只能是 a-z 的 26 个字母  
\`x  跳转到书签处(是 1 左边的键) 

**缩进**  
`>`  增加缩进, `x>` 表示增加以下 x 行的缩进   
`<`  减少缩进, `x<` 表示减少以下 x 行的缩进

# Edit
`Shift-v shift-g`  选择当前位置到文章结尾  
`shfit-v $` 选择当前位置到当前行尾

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
## Replacement
`&` is replaced by the entire text matched by the search pattern when used in a replacement  
`:%s/Yazstremski/&,clar/`  #the result is `Yazstremski,clar`  
`:1,10s/.*/(&)/` #surrend each line from line 1 to line 10 with parentheses

**替换变量**: 
在正规表达式中使用 `\(` 和 `\)` 符号括起正规表达式，即可在后面使用`\1`、`\2`等变量来访问 `\(` 和 `\)` 之间的内容

[vim 正则表达式](http://blog.csdn.net/yyt8yyt8/article/details/7567455)

**表示内容的元字符**

| 模式     | 含义                                               |
|--------|--------------------------------------------------|
| .      | 匹配任意字符                                           |
| [abc]  | 匹配方括号中的任意一个字符.可以使用-表示字符范围,如[a-z0-9]匹配小写字母和阿拉伯数字. |
| [^abc] | 在方括号内开头使用^符号,表示匹配除方括号中字符之外的任意字符.                 |
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
| \{-}   | 表示前一个字符可出现零次或多次，但在整个正则表达式可以匹配成功的前提下，匹配的字符数越少越好 |
| \=     | 匹配一个可有可无的项                                     |
| \_s    | 匹配空格或断行                                        |

**懒惰模式**  
`\{-n,m}` 与`\{n,m}`一样，尽可能少次数地重复  
`\{-}` 匹配它前面的项一次或0次, 尽可能地少

**表示转义和位置的元字符**

| 模式 | 含义      |
|----|------------|
| \* | 匹配 * 字符 |
| \. | 匹配 . 字符 |
| \/ | 匹配 / 字符 |
| \\ | 匹配 \ 字符 |
| \[ | 匹配 [ 字符 |
| $  | 匹配行尾   |
| ^  | 匹配行首   |
| \< | 匹配单词词首 |
| \> | 匹配单词词尾 |

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

# Tools
## ctags
If you're currently sitting in the directory you want to index, just run:  
`ctags -R .`

press `Ctrl + ]`, jump to the source  
press `Ctrl + o` or `Ctrl + t` to climb back

You can also go directly to a tag's definition by entering one of the following in vim's command mode:  

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
- To show the tags you've traversed since you opened vim, run :tags.

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


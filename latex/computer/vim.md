# Move
| Movement                            | Commands   |
|-------------------------------------|------------|
| ←, ↓, ↑, →                          | h, j, k, l |
| To first character of next line     | +          |
| To first character of previous line | -          |
| To end of word                      | e or E     |
| Forward by word                     | w or W     |
| Backward by word                    | b or B     |
| To end of line                      | $          |
| To beginning of line                | 0          |
| Forward by window                   | ctrl + f   |
| Backward by window                  | ctrl + b   |
| %									  |跳到配对的括号|
| gD 								  |跳转到局部变量的定义处|

[[  跳转到代码块的开头去(但要求代码块中'{'必须单独占一行) 

''  跳转到光标上次停靠的地方, 是两个', 而不是一个"

**书签**  
mx  设置书签,x 只能是 a-z 的 26 个字母  
`x  跳转到书签处(是 1 左边的键) 

**缩进**  
\>  增加缩进,"x>"表示增加以下 x 行的缩进   
<  减少缩进,"x<"表示减少以下 x 行的缩进

# Edit
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
| ﻿~       | 将光标下的字母改变大小写       |
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

# Tools
## ctags
If you’re currently sitting in the directory you want to index, just run:  
`ctags -R .`

press `Ctrl + ]`, jump to the source  
press `Ctrl + o` or `Ctrl + t` to climb back

You can also go directly to a tag’s definition by entering one of the following in vim’s command mode:  

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
- To show the tags you’ve traversed since you opened vim, run :tags.

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
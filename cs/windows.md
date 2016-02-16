# [Proto](http://miechu.pl/proto/)
Windows 下全键盘资源管理器

Basic

- F1: 可以调出热键说明,
- Alt \`: 默认呼出热键

- Return: ExecuteElm
- Alt Return: Open Folder In Explorer
- Shift Return: Open Folder in new tab
- Ctrl Backspace: go to ../

不知道什么意思

- Ctrl Return: Open with program
- Tab: open with program

复制剪切

- Ctrl C: copy
- Alt C: add path, 将路径加到clipboard中
- Ctrl Shift C: copy file name
- Alt Shift C: add file name
- Ctrl Alt C: copy files content
- Ctrl V: paste
- Alt V: copy without size check
- Ctrl X: move

重命名

- Ctrl R: rename
- Ctrl Shift R: regex rename

创建

- Ctrl N: create dir
- Ctrl Shift N: create empty file
- Alt N: create massive folders
- Ctrl Alt N: create massive files

删除

- Delete: delete to recylebin
- Shift Delete: delete forever
- Alt Delete: secure delete

查看

- Ctrl P: go to dir from clipboard
- Ctrl U: open command line here
- Ctrl SPACE: 文件信息
- Ctrl Shift SPACE: 修改文件属性
- Ctrl J: show thumbnails
- Ctrl G: calculate chart, 占用空间分布
- Ctrl Shift S: change sorting direction
- Ctrl S: chang sorting type
- F5: refresh view

选择

- Alt A: select files or folders

(解)压缩

- Ctrl E: extract
- Alt E: compress

查找

- Ctrl F: find
- Ctrl Shift F: find with regex
- Alt F: find files

标签

- Ctrl T: 新建标签, proto的初始界面
- Ctrl W: 关闭标签
- Ctrl Tab: Proto go to netxt tab
- Ctrl Shift Tab: go to the previous tab
- Ctrl right: go to next tab
- Ctrl left: go to previous tab
- Ctrl Alt Left Arrow: move the tab to left
- Ctrl 1: go to tab1
- Ctrl 2: go to tab2

快捷功能

- Alt 2: calculator
- Alt 3: network
- Alt 4: calendar
- Alt 6: regedit
- Alt 0: proto log

# Dos
**进入D盘, `cd D:\` 不管用**  
```
D: ENTER
```

Combine 2 text files into one:
```
type file1 file2 > file3
```
DOS:
```
copy file1+file2 file3
```
Create a text file from the keyboard: copy con: file1 {press Ctrl-Z to end}

Write a text file to the screen: 
```
type file
```
相当于linux的cat.

# bat script
REM [comment]  
If ECHO is ON, the comment is displayed. Otherwise, it is ignored

# Office

- 选中一段文字, 然后快速的双击格式刷,然后就可以**多次使用同一个格式刷**

## Excel
- `=MATCH(MAX(range), range, 0)`: 最大值所在的位置, 最后一个参数 0 表示range 里面的数据时无序的, 有序时使用1(asc, 默认) 或者 -1(desc)
- `concatenate(x, y)` 或者 `x&y`: 字符串拼接
- 行列转换, excel中在选择"编辑"菜单,点击"选择性粘贴", 选中"装置", 在google sheets 中为 "edit" --> "paste special" --> "paste transpose"
- transpose(目标区域),神奇的转置,把行变成列,把列变成行
- F4,对,你没看错,就是F4!重复上一步操作,比如,插入行,设置格式等等频繁的操作,F4简直逆天!
- ~ 比如输入网址的时候,一般输入完会自动变为超链接,在网址前输入"~"就解决咯

### Count
COUNTIF in Excel - count if not blank, greater than, duplicate or unique
[ref](https://www.ablebits.com/office-addins-blog/2014/07/02/excel-countif-examples/)

**COUNTIF not blank**  
- `=COUNTIF(range,"*")`: counts all cells that **contain any text**, meaning that cells with dates and numbers will be treated as blank cells!
- `=COUNTIF(range,"<>"&"")`:  universal COUNTIF formula for counting all non-blank cells(all value types - text, dates and numbers)

**COUNTIF blank**  
- `=COUNTIF(range,"<>"&"*")`:  Formula to count cells not containing any text
- `=COUNTIF(range,"")`: Universal COUNTIF formula for blanks (all value types)

### Google sheets
Within a single spreadsheet, you can replicate data and copy it from one sheet to another by 
entering the sheet name and an exclamation mark before the cell being copied. Here are some examples:
```
=Sheet1!A1
='Sheet number two'!B4
```

## Word
- Ctrl+A全选文章或者用鼠标拖动的方法选中部分文中, 然后,同时按下键盘上的Ctrl+Shift+F9,就可以取消所有的超链接

更新域

- 最简单的更新域的方法是右击域代码,从弹出的右击菜单中选择"更新域"项即可
- 还有一种方法就是利用"F9"功能键实现更新域操作.在选中域代码块的情况下,按"F9"即可实现更新域操作.
- 如果想更新文档中所有域代码,只需要全选文档,按"F9"即可.

## Visio
- visio 导出的pdf图片的周边的白色需要使用Adobe Acrobat 去掉, R生成的对比图, 上边距 0.3, 下边距 0.5

## Project
在选项的日程中 自动计划, 固定工期

勾选甘特图工具右上方的 大纲数字及下方的项目...

先填充内容(不管任务与子任务), 然后通过任务工具栏中的降级, 来划分子任务

里程碑任务: 工期为0 的任务

前置任务中填写任务所在行号, 逗号间隔

资源-> 工作组规划器->资源工作表
摘要任务不分配人, 如果要分配给多个人, 指定一个工作组
具体任务分配人

FS: finish start
SS: start start
SF: start finish
FF: finish finish

一般不超过七层

# VS(Visual Studio)
- ctrl+space  code hinting完成单词
- 在用到if while for,函数等需要大括号的时候,连续按下两次tab键可以快速输入

- ctrl+k,ctrl+c注释选中内容
- ctrl+k,ctrl+u取消注释选中内容

- ctrl+u 转换为小写
- ctrl+shift+u 转换为大写

- ctrl+] 在匹配的括号之间切换
- F12切换到定义处, 再按下Ctrl+- 回到按F12的地方


- ctrl+k,ctrl+d 设置文档的格式

- ctrl+k,ctrl+k 添加书签,删除书签
- F2下一个书签

- ctrl+j 查看成员
- F3 查找下一个

- 重定向在调试的命令行参数,对于C#文件要放到debug文件夹下


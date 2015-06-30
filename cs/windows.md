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
## Excel
`=MATCH(MAX(range), range, 0)`: 最大值所在的位置, 最后一个参数 0 表示range 里面的数据时无序的, 有序时使用1(asc, 默认) 或者 -1(desc)

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
更新域

- 最简单的更新域的方法是右击域代码,从弹出的右击菜单中选择"更新域"项即可
- 还有一种方法就是利用"F9"功能键实现更新域操作.在选中域代码块的情况下,按"F9"即可实现更新域操作.
- 如果想更新文档中所有域代码,只需要全选文档,按"F9"即可.

系统默认的样式

标题1
```
字体: 二号, 加粗, 字距调整二号
    行距: 多倍行距 2.41 字行, 段落间距
    段前: 17 磅
    段后: 16.5 磅, 与下段同页, 段中不分页, 1 级, 样式: 链接, 在样式库中显示, 优先级: 10
    基于: 正文
    后续样式: 正文
```

标题2
```
字体: (中文) +中文标题 (宋体), (默认) +西文标题 (Calibri Light), 三号, 加粗
    行距: 多倍行距 1.73 字行, 段落间距
    段前: 13 磅
    段后: 13 磅, 与下段同页, 段中不分页, 2 级, 样式: 链接, 使用前隐藏, 在样式库中显示, 优先级: 10
    基于: 正文
    后续样式: 正文
```

标题3
```
字体: 三号, 加粗
    行距: 多倍行距 1.73 字行, 段落间距
    段前: 13 磅
    段后: 13 磅, 与下段同页, 段中不分页, 3 级, 样式: 链接, 使用前隐藏, 在样式库中显示, 优先级: 10
    基于: 正文
    后续样式: 正文
```

标题
```
字体: (中文) 宋体, (默认) +西文标题 (Calibri Light), 三号, 加粗, 居中, 段落间距
    段前: 12 磅
    段后: 3 磅, 1 级, 样式: 链接, 在样式库中显示, 优先级: 11
    基于: 正文
    后续样式: 正文
```

副标题
```
字体: (中文) 宋体, (默认) +西文标题 (Calibri Light), 三号, 加粗, 字距调整四号, 居中
    行距: 多倍行距 1.3 字行, 段落间距
    段前: 12 磅
    段后: 3 磅, 2 级, 样式: 链接, 在样式库中显示, 优先级: 12
    基于: 正文
    后续样式: 正文
```


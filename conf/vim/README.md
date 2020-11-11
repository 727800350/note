# vimrc
vim 自带有很多快捷键,再加上各类插件的快捷键,大量快捷键出现在单层空间中难免引起冲突,为缓解该问题,引入了前缀键 <leader>,
键r 可以配置成r, <leader>r, <leader><leader>r 等等多个快捷键.前缀键是 vim 使用率较高的一个键(最高的当属 Esc),选一个最方
便输入的键作为前缀键,将有助于提高编辑效率.找个无须眼睛查找,无须移动手指的键 - 分号键,挺方便的,就在你右手小指处:
```vim
" 定义快捷键的前缀,即<Leader>
let mapleader=";"
```

使用下表中不同形式的map命令,可以针对特定的模式设置键盘映射:

| Command命令 | 常规模式 | 可视化模式 | 运算符模式 | 插入模式 | 命令行模式 |
| ---         | ---      | ---        | ---        | ---      | ---        |
| :map        | √        | √          | √          |          |            |
| :nmap       | √        |            |            |          |            |
| :vmap       |          | √          |            |          |            |
| :omap       |          |            | √          |          |            |
| :map!       |          |            |            | √        | √          |
| :imap       |          |            |            | √        |            |
| :cmap       |          |            |            |          | √          |


- [thrift](https://github.com/solarnz/thrift.vim)

# vim script
[vim scripting cheatsheet](https://devhints.io/vimscript)

# plugin
[将 VIM 打造成 go 语言的 ide](https://learnku.com/articles/24924)

[junegunn/vim-plug](https://github.com/junegunn/vim-plug)

## [junegunn/vim-easy-align](https://github.com/junegunn/vim-easy-align)

## [ianva/vim-youdao-translater](https://github.com/ianva/vim-youdao-translater)
在普通模式下,按 ctrl+t, 会翻译当前光标下的单词,

在 visual 模式下选中单词或语句,按 ctrl+t,会翻译选择的单词或语句,

点击引导键再点y,d,可以在命令行输入要翻译的单词或语句,

译文将会在编辑器底部的命令栏显示.

## go
### [jstemmer/gotags](https://github.com/jstemmer/gotags)

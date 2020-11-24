# conf dir
vim/after/ directory is a bit of a hack. Files in this directory will be loaded every time Vim starts, but after the
files in ~/. vim/plugin/ . This allows you to override Vim's internal files.

# vimrc
vim 自带有很多快捷键,再加上各类插件的快捷键,大量快捷键出现在单层空间中难免引起冲突,为缓解该问题,引入了前缀键 `<leader>`,
键r 可以配置成r, `<leader>r, <leader><leader>r` 等等多个快捷键.前缀键是 vim 使用率较高的一个键(最高的当属 Esc),选一个最方
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

nore: 表示非递归 not recursion

`:nmap`: 列出所有键盘映射

# vim script
[vim scripting cheatsheet](https://devhints.io/vimscript)

# plugin
[将 VIM 打造成 go 语言的 ide](https://learnku.com/articles/24924)

[junegunn/vim-plug](https://github.com/junegunn/vim-plug)

## [junegunn/fzf.vim](https://github.com/junegunn/fzf.vim)
需要fzf 版本至少0.24, 通过apt-get 安装的版本比较低, 所以需要通过vim-plug 安装最新版
```vim
Plug 'junegunn/fzf', { 'do': { -> fzf#install() } }
Plug 'junegunn/fzf.vim'
```

Most commands support CTRL-T / CTRL-X / CTRL-V key bindings to open in a new tab, a new split, or in a new vertical split

## [junegunn/vim-easy-align](https://github.com/junegunn/vim-easy-align)

## [preservim/nerdcommenter](https://github.com/preservim/nerdcommenter)
- `[count]<leader>cc`: 注释从当前行往下数的 count 行,count 可省略,默认值为 1 (cc 是 NERDCommenterComment 的缩写).
- `[count]<leader>cu`: 取消注释从当前行往下数的 count 行,count 可省略,默认值为 1 (cu 是 NERDCommenterUncomment 的缩写)
- `[count]<leader>ci`: 切换从当前行往下数的 count 行的注释状态 (ci 是 NERDCommenterInvert 的缩写).
- `<leader>c$`: 注释掉从当前光标所在位置到行尾的剩下内容,等价于在光标所在位置插入注释符(c$ 表示NERDCommenterToEOL 的缩写.
- `<leader>cA`: 在光标所在当前行行尾添加注释符号并进入Vim 的插入模式(cA 是NERDCommenterAppend 的缩写).

## [preservim/nerdtree](https://github.com/preservim/nerdtree)
Pressing m would open a menu below and you can select from a list of actions.

```info
NERDTree Menu. Use j/k/enter and the shortcuts indicated
==========================================================
> (a)dd a childnode
  (m)ove the current node
  (d)elete the current node
  (r)eveal in Finder the current node
  (o)pen the current node with system editor
  (q)uicklook the current node
  (c)opy the current node
Add a childnode
==========================================================
Enter the dir/file name to be created. Dirs end with a '/'
/Library/WebServer/Documents/new-teacher-center/app/Model/
```

## [ianva/vim-youdao-translater](https://github.com/ianva/vim-youdao-translater)
- 在普通模式下,按 ctrl+t, 会翻译当前光标下的单词;
- 在 visual 模式下选中单词或语句,按 ctrl+t, 会翻译选择的单词或语句;
- `<leader>yd` 可以在命令行输入要翻译的单词或语句.

译文将会在编辑器底部的命令栏显示.

## [neoclide/coc.nvim](https://github.com/neoclide/coc.nvim)
need node

- `:CocList services`
- `:CocList sources`

| command      | description |
| ---          | ---         |
| `<leader>rn` | rename      |

### [coc-marketplace](https://github.com/fannheyward/coc-marketplace)
`:CocList marketplace`: 进入marketplace

### go
coc-go 和 languageserver.golang 只能使用其中的一个, 否则自动补全的时候会重复提示.
[Duplicate Suggestions in autocompletion](https://github.com/neoclide/coc.nvim/issues/1824)

```json
{
  "languageserver": {
    "golang": {
      "command": "gopls",
      "args": ["-remote=auto", "-listen.timeout=1h", "-remote.listen.timeout=2h"],
      "rootPatterns": ["go.mod", ".git"],
      "trace.server": "verbose",
      "filetypes": ["go"],
      "initializationOptions": {
        "usePlaceholders": true,
        "completeUnimported": true
      }
    }
  }
}
```

[jstemmer/gotags](https://github.com/jstemmer/gotags)

### clangd
同理, coc-clangd 和 languageserver.clangd 也只能用一个.

```json
{
  "languageserver": {
    "clangd": {
      "command": "clangd",
      "rootPatterns": ["compile_flags.txt", "compile_commands.json", ".git"],
      "filetypes": ["c", "cc", "cpp", "c++", "objc", "objcpp"]
    }
  }
}
```

## [liuchengxu/vista.vim](https://github.com/liuchengxu/vista.vim)
| Command | Description                                       |
| ---     | ---                                               |
| Vista   | Open vista window for viewing tags or LSP symbols |
| Vista!  | Close vista view window if already opened         |
| Vista!! | Toggle vista view window                          |

- :Vista [EXECUTIVE]: open vista window powered by EXECUTIVE.
- :Vista finder [EXECUTIVE]: search tags/symbols generated from EXECUTIVE in fzf mode


"===============================复制===============================
"从系统剪切板中复制，剪切，粘贴
"Ctrl Y  复制
map <C-Y> "+y  
map <C-X> "+x
map <C-P> "+p

" 原样从剪贴板粘贴到vim中, 而不对粘贴的文字使用auto indent等设置
set pastetoggle=<F9>

"保存文件并留在插入模式 [插入模式]
imap jj <ESC>:w<CR>li
"返回Normal模式 [插入模式]
imap kk <ESC>l
"===============================自动补全===============================
imap <c-\> <c-p>
" c-j自动补全，当补全菜单打开时，c-j,k上下选择
" f:文件名补全，l:行补全，d:字典补全，]:tag补全,t thesaurus
imap <C-]>             <C-X><C-]> 
imap <C-F>             <C-X><C-F> 
imap <C-D>             <C-X><C-K>  
set dictionary+=~/.mydict "自定义单词库
set thesaurus+=~/.mthesaur  "自定义分类词库
imap <C-L>             <C-X><C-L> 
imap <C-T>             <C-X><C-T> 
set  complete-=k complete+=k
"Ctrl-X Ctrl-S completion for spelling correction
"Ctrl-X Ctrl-D completion by macro and definition name defined by #define
"Ctrl-X Ctrl-I searches forward through the current file and included files defined by #include <somefile.h>
"F3 switches between two windows after a split
map <F3> <c-w>w
set backspace=2 "启用退格键
let g:vimrc_author='Eric'

set nocompatible             " 关闭兼容模式

"===============================字体及编码===============================
set guifont=Lucida\ Console:h10  "字体 && 字号
set guifont=fangsong:h12  "字体 && 字号,中文竟然需要用拼音表示
let &termencoding=&encoding
set fileencodings=utf-8,ucs-2,ansi,gb18030,gbk,gb2312,big5,cp936,latin1

set nu "number 行号
colorscheme evening "颜色主题
set showmatch "显示匹配的括号
"set laststatus=2 "always show status line
set shortmess=atI "shortens messages"
set showcmd         " 显示命令``
set ruler                    " 右下角显示光标位置的状态行


set ic "ignore case
set hlsearch                 " 开启高亮显示结果
set incsearch                " 查找book时，当输入/b时会自动找到,in c search

set cursorline              " 突出显示当前行
syntax enable                " 打开语法高亮
syntax on                  "也是语法高亮,不同的系统可能用不一样的语法 
filetype indent on           " 针对不同的文件类型采用不同的缩进格式
filetype plugin on           " 针对不同的文件类型加载对应的插件
filetype plugin indent on    " 启用自动补全

set autoread     "当文件在外部被修改时，自动重新读取
set autoindent "自动缩进
set smartindent  "智能缩进
set tabstop=4  "tab 缩进4个空格，而不是默认的8个

"不要备份文件
set nobackup
set nowb
set noswapfile      " 不使用swp文件，注意，错误退出后无法恢复

"自动回到上次光标所在位置
if has("autocmd")
au BufReadPost * if line("'\"") > 1 && line("'\"") <= line("$") | exe "normal! g'\"" | endif
endif

" ======= 引号 && 括号自动匹配 ======= "
:inoremap ( ()<ESC>i
:inoremap ) <c-r>=ClosePair(')')<CR>
:inoremap { {}<ESC>i
:inoremap } <c-r>=ClosePair('}')<CR>
:inoremap [ []<ESC>i
:inoremap ] <c-r>=ClosePair(']')<CR>
":inoremap < <><ESC>i
":inoremap > <C-R>=ClosePair('>')<CR>
:inoremap " ""<ESC>i
" :inoremap ' ''<ESC>i
" :inoremap ` ``<ESC>i
:inoremap $ $$<ESC>i
function ClosePair(char)
    if getline('.')[col('.') - 1] == a:char
        return "\<Right>"
    else
        return a:char
    endif
endf
" ======= 引号 && 括号自动匹配 ======= "


" ======= 编译 && 运行 ======= "
" 编译源文件
func! CompileCode()
        exec "w"
        if &filetype == "c"
"             exec "!gcc -Wall $(mysql_config --cflags) $(mysql_config --libs) -lpcap -c %<.c"
            exec "!gcc -Wall $(mysql_config --cflags) $(mysql_config --libs) -lpcap %<.c -o %<"
        elseif &filetype == "cpp"
            exec "!g++ -Wall -std=c++98 %<.cpp -o %<"
        elseif &filetype == "java"
            exec "!javac %<.java"
        elseif &filetype == "haskell"
            exec "!ghc --make %<.hs -o %<"
        elseif &filetype == "lua"
            exec "!lua %<.lua"
        elseif &filetype == "perl"
            exec "!perl %<.pl"
        elseif &filetype == "gp" "gnuplot
            exec "!gnuplot %<.gp"
        elseif &filetype == "python"
            exec "!python %<.py"
        elseif &filetype == "ruby"
            exec "!ruby %<.rb"
        elseif &filetype == "mp"
            exec "!mpost -tex=latex  %<.mp"
        elseif &filetype == "dot"
            exec "!dot -Tps %<.dot -o %<.ps"
        elseif &filetype == "tex"
            exec "!xelatex  %<.tex"
        elseif &filetype == "sh"
            exec "!bash  %<.sh"
        endif
endfunc

" 运行可执行文件
func! RunCode()
        exec "w"
        if &filetype == "c" || &filetype == "cpp" || &filetype == "haskell"
            exec "! ./%<"
"             exec "!sudo ./%<"  "for libpcap program
        elseif &filetype == "java"
            exec "!java %<"
        elseif &filetype == "lua"
            exec "!lua %<.lua"
        elseif &filetype == "perl"
            exec "!perl %<.pl"
        elseif &filetype == "python"
            exec "!python %<.py"
        elseif &filetype == "ruby"
            exec "!ruby %<.rb"
        elseif &filetype == "mp"
            exec "!evince %<.mps"
        elseif &filetype == "tex"
            exec "!evince %<.pdf"
        elseif &filetype == "dot"
            exec "!evince %<.ps"
        endif
endfunc

func! Save()
        exec "w"
        if &filetype == "dot"
            exec "!epstopdf %<.ps"
        elseif &filetype == "mp"
            exec "!epstopdf %<.mps"
        endif
endfunc
" Ctrl + c 一键保存、编译
map <c-c> :call CompileCode()<CR>
imap <c-c> <ESC>:call CompileCode()<CR>
vmap <c-c> <ESC>:call CompileCode()<CR>

" Ctrl + R 一键保存、运行
map <c-r> :call RunCode()<CR>
imap <c-r> <ESC>:call RunCode()<CR>
vmap <c-r> <ESC>:call RunCode()<CR>

" Ctrl + s 一键保存
map <c-s> :call Save()<CR>
imap <c-s> <ESC>:call Save()<CR>
vmap <c-s> <ESC>:call Save()<CR>
" ======= 编译 && 运行 ======= "

"==============================comments===================================
"F11 Comment F12 uncomment
map <F11> :call Comment()<CR>
imap <F11> <ESC>:call Comment()<CR>
vmap <F11> <ESC>:call Comment()<CR>
map <F12> :call Uncomment()<CR>
imap <F12> <ESC>:call Uncomment()<CR>
vmap <F12> <ESC>:call Uncomment()<CR>

" Comments range (handles multiple file types)
function! Comment() range
  if &filetype == "css"
    execute ":" . a:firstline . "," . a:lastline . 's/^\(.*\)$/\/\* \1 \*\//'
  elseif &filetype == "html" || &filetype == "xml" || &filetype == "xslt" || &filetype == "xsd" || &filetype == "xhtml"
    execute ":" . a:firstline . "," . a:lastline . 's/^\(.*\)$/<!-- \1 -->/'
  else
    if &filetype == "c" || &filetype == "h" || &filetype == "java" || &filetype == "cs" || &filetype == "cpp" || &filetype == "hpp" || &filetype == "php"
      let commentString = "// "
    elseif &filetype == "vim"
      let commentString = '" '
    elseif &filetype == "scheme"
      let commentString = ';; '
    elseif &filetype == "tex" || &filetype == "bib" || &filetype == "mp"
      let commentString = "%% "
    elseif &filetype == "cmm"
      let commentString = ";; "
    elseif &filetype == "sql"
      let commentString = "-- "
    else
      let commentString = "## "
    endif
    execute ":" . a:firstline . "," . a:lastline . 's,^,' . commentString . ','
   "execute ":" . a:firstline . "," . a:lastline . 's,^' . commentString . ',,'
  endif
endfunction

" Uncomments range (handles multiple file types)
function! Uncomment() range
  if &filetype == "css" || &filetype == "html" || &filetype == "xml" || &filetype == "xslt" || &filetype == "xsd" || &filetype == "xhtml" 
    " http://www.vim.org/tips/tip.php?tip_id=271
    execute ":" . a:firstline . "," . a:lastline . 's/^\([/(]\*\|<!--\) \(.*\) \(\*[/)]\|-->\)$/\2/'
  else
    if &filetype == "c" || &filetype == "h" || &filetype == "java" || &filetype == "cs" || &filetype == "cpp" || &filetype == "hpp" || &filetype == "php"
      let commentString = "// "
    elseif &filetype == "vim"
      let commentString = '" '
    elseif &filetype == "scheme"
      let commentString = ';; '
    elseif &filetype == "tex" || &filetype == "bib" || &filetype == "mp"
      let commentString = "%% "
    elseif &filetype == "cmm"
      let commentString = ";; "
    elseif &filetype == "sql"
      let commentString = "-- "
    else
"python, bash shell
      let commentString = "## "
    endif
    execute ":" . a:firstline . "," . a:lastline . 's,^' . commentString . ',,'
  endif
endfunction


set pastetoggle=<F9> "raw copy, do not use auto indent etc.

set nocompatible "set vim not compatible with vi
set mouse-=a "disable visual mode

"=============================== font and encoding ===============================
set guifont=Lucida\ Console:h10
set guifont=fangsong:h12
set fileencodings=utf-8,ucs-bom,gb18030,gbk,gb2312,cp936
set termencoding=gb2312,utf-8
set encoding=utf-8
"=============================== font and encoding ===============================

set nu "show number
set showmatch "colorscheme evening
set laststatus=2 "always show status line
set shortmess=atI "shortens messages"
set showcmd
set ruler

set ic "ignore case
set hlsearch "hightlight search
set incsearch "when searching book, when type /b will automatically find in c search

" syntax
syntax enable
syntax on

filetype plugin on

" indent
set autoread
filetype indent on
filetype plugin indent on
set autoindent
set smartindent
set shiftwidth=4
set tabstop=4

" nobackup
set nobackup
set nowb
set noswapfile
if v:version >= 800
	set noundofile
endif

" go back the previvous cursor location
if has("autocmd")
	au BufReadPost * if line("'\"") > 1 && line("'\"") <= line("$") | exe "normal! g'\"" | endif
endif

" ======= auto match ===================
:inoremap ( ()<ESC>i
:inoremap ) <c-r>=ClosePair(')')<CR>
:inoremap { {}<ESC>i
:inoremap } <c-r>=ClosePair('}')<CR>
:inoremap [ []<ESC>i
:inoremap ] <c-r>=ClosePair(']')<CR>
":inoremap < <><ESC>i
":inoremap > <C-R>=ClosePair('>')<CR>
:inoremap " ""<ESC>i
:inoremap ' ''<ESC>i
:inoremap ` ``<ESC>i
:inoremap $ $$<ESC>i
function ClosePair(char)
	if getline('.')[col('.') - 1] == a:char
		return "\<Right>"
	else
		return a:char
	endif
endf
" ======= auto match ===================


"==============================comments===================================
" F11 Comment F12 uncomment
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
		if &filetype == "c" || &filetype == "h" || &filetype == "java" || &filetype == "cs" || &filetype == "cpp" || &filetype == "hpp" || &filetype == "php" || &filetype == "javascript"
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
		elseif &filetype == "fortran"
			let commentString = "!! "
		else
			let commentString = "## "
		endif
		execute ":" . a:firstline . "," . a:lastline . 's,^,' . commentString . ','
	endif
endfunction

" Uncomments range (handles multiple file types)
function! Uncomment() range
	if &filetype == "css" || &filetype == "html" || &filetype == "xml" || &filetype == "xslt" || &filetype == "xsd" || &filetype == "xhtml"
		execute ":" . a:firstline . "," . a:lastline . 's/^\([/(]\*\|<!--\) \(.*\) \(\*[/)]\|-->\)$/\2/'
	else
		if &filetype == "c" || &filetype == "h" || &filetype == "java" || &filetype == "cs" || &filetype == "cpp" || &filetype == "hpp" || &filetype == "php" || &filetype == "javascript"
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
		elseif &filetype == "fortran"
			let commentString = "!! "
		else
			let commentString = "## "
		endif
		execute ":" . a:firstline . "," . a:lastline . 's,^' . commentString . ',,'
	endif
endfunction
"==============================comments===================================

"=============================== New File Header ===================================================
autocmd BufNewFile *.cpp,*.h,*.sh exec ":call SetHeader()"
func SetHeader()
	if &filetype == 'sh'
		call setline(1,"\#########################################################################")
		call append(line("."), "\# File Name: ".expand("%"))
		call append(line(".")+1, "\# Author: Eric Wang")
		call append(line(".")+2, "\# Email: wangchaogo1990 at gmail.com")
		call append(line(".")+3, "\# Created Time: ".strftime("%Y-%m-%d %H:%M:%S"))
		call append(line(".")+4, "\#########################################################################")
		call append(line(".")+5, "\#!/bin/bash")
		call append(line(".")+6, "\set -x")
		call append(line(".")+7, "")
	else
		call setline(1, "/*************************************************************************")
		call append(line("."), "	> File Name: ".expand("%"))
		call append(line(".")+1, "	> Author: Eric Wang")
		call append(line(".")+2, "	> Email: wangchaogo1990 at gmail.com")
		call append(line(".")+3, "	> Created Time: ".strftime("%Y-%m-%d %H:%M:%S"))
		call append(line(".")+4, " ************************************************************************/")
		call append(line(".")+5, "")
	endif

	if &filetype == 'cpp'
		call append(line(".")+6, "#include <iostream>")
		call append(line(".")+7, "using namespace std;")
		call append(line(".")+8, "")
	endif

	"jump to the end of file
	autocmd BufNewFile * normal G
endfunc
"=============================== New File Header ===================================================

"===========================Chinese Punctuation: ctrl-s or :%call CheckChineseMark ==================
map <C-S> :call CheckChineseMark()<CR>
imap <C-S> <ESC>:call CheckChineseMark()<CR>
vmap <C-S> <ESC>:call CheckChineseMark()<CR>

function! CheckChineseMark()
	"check one by one
	if search('。')
		let s:line=search('。')
		execute s:line . "s/。/\./g"
	endif

	if search('，')
		let s:line=search('，')
		execute s:line . "s/，/,/g"
	endif

	if search('；')
		let s:line=search('；')
		execute s:line . "s/；/,/g"

	endif

	if search('？')
		let s:line=search('？')
		execute s:line . "s/？/?/g"
	endif

	if search('：')
		let s:line=search('：')
		execute s:line . "s/：/\:/g"
	endif

	if search('‘')
		let s:line=search('‘')
		execute s:line . "s/‘/\'/g"
	endif

	if search('’')
		let s:line=search('’')
		execute s:line . "s/’/\'/g"
	endif

	if search('”')
		let s:line=search('”')
		execute s:line . "s/”/\"/g"
	endif

	if search('“')
		let s:line=search('“')
		execute s:line . "s/“/\"/g"
	endif

	if search('《')
		let s:line=search('《')
		execute s:line . "s/《/\</g"
	endif

	if search('》')
		let s:linie=search('》')
		execute s:line . "s/》/\>/g"
	endif

	if search('——')
		let s:line=search('——')
		execute s:line . "s/——/-/g"
	endif

	if search('）')
		let s:line=search('）')
		execute s:line . "s/）/\)/g"
	endif

	if search('（')
		let s:line=search('（')
		execute s:line . "s/（/\(/g"
	endif

	if search('……')
		let s:line=search('……')
		execute s:line . "s/……/^/g"
	endif

	if search('￥')
		let s:line=search('￥')
		execute s:line . "s/￥/$/g"
	endif

	if search('！')
		let s:line=search('！')
		execute s:line . "s/！/!/g"
	endif

	if search('·')
		let s:line=search('·')
		execute s:line . "s/·/`/g"
	endif

	if search('、')
		let s:line=search('、')
		execute s:line . "s/、/,/g"
	endif

	if search('Θ')
		let s:line=search('Θ')
		execute s:line . "s/Θ/Theta/g"
	endif

	if search('ε')
		let s:line=search('ε')
		execute s:line . "s/ε/epsilon/g"
	endif

	if search('Ω')
		let s:line=search('Ω')
		execute s:line . "s/Ω/Omega/g"
	endif

	if search('∈')
		let s:line=search('∈')
		execute s:line . "s/∈/in/g"
	endif

	if search('≤')
		let s:line=search('≤')
		execute s:line . "s/≤/leq/g"
	endif

	if search('≥')
		let s:line=search('≥')
		execute s:line . "s/≥/geq/g"
	endif

	if search('≠')
		let s:line=search('≠')
		execute s:line . "s/≠/neq/g"
	endif

	if search('≈')
		let s:line=search('≈')
		execute s:line . "s/≈/approx/g"
	endif
endfunction
"===========================Chinese Punctuation: ctrl-s or :%call CheckChineseMark ==================


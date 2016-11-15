""""""""""""""""""""""""""""""
"Chinese Punctuation
"usage: ctrl-s or :%call CheckChineseMark, could use Tab for completion
""""""""""""""""""""""""""""""
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
map <C-S> :call CheckChineseMark()<CR>
imap <C-S> <ESC>:call CheckChineseMark()<CR>
vmap <C-S> <ESC>:call CheckChineseMark()<CR>

"copy and coller from system clipboard
"Ctrl Y copy
map <C-Y> "+y
map <C-X> "+x
map <C-P> "+p

" raw copy, do not use auto indent etc.
set pastetoggle=<F9>

"===============================auto completion===============================
imap <c-\> <c-p>
" c-j auto completion, c-j,k go up and down to choose
" f:filename; l:line; d:dic; ]: tag; t thesaurus
imap <C-]>			 <C-X><C-]>
imap <C-F>			 <C-X><C-F>
imap <C-D>			 <C-X><C-K>
set dictionary+=~/.mydict
set thesaurus+=~/.mthesaur
imap <C-L>			 <C-X><C-L>
imap <C-T>			 <C-X><C-T>
set complete-=k complete+=k
"Ctrl-X Ctrl-S completion for spelling correction
"Ctrl-X Ctrl-D completion by macro and definition name defined by #define
"Ctrl-X Ctrl-I searches forward through the current file and included files defined by #include <somefile.h>
"
"enable backspace
set backspace=2
set nocompatible
set mouse-=a "disable visual mode

"=============================== font and encoding ===============================
set guifont=Lucida\ Console:h10
set guifont=fangsong:h12
set fileencodings=utf-8,ucs-bom,gb18030,gbk,gb2312,cp936
set termencoding=gb2312,utf-8
set encoding=utf-8

set nu "show number
set showmatch "colorscheme evening
set laststatus=2 "always show status line
set shortmess=atI "shortens messages"
set showcmd
set ruler

set ic "ignore case
set hlsearch
set incsearch				" when searching book, when type /b will automatically find in c search

" set cursorline " hightlight the current line
syntax enable
syntax on
filetype indent on
filetype plugin on
filetype plugin indent on

set autoread
set autoindent
set smartindent
set tabstop=4

set nobackup
set nowb
set noswapfile
set noundofile

"go back the previvous cursor location
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
" :inoremap $ $$<ESC>i
function ClosePair(char)
	if getline('.')[col('.') - 1] == a:char
		return "\<Right>"
	else
		return a:char
	endif
endf
" ======= auto match ===================


" ======= compile and run ======= "
func! CompileCode()
		exec "w"
		if &filetype == "c"
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
		elseif &filetype == "sh"
			exec "!bash %<.sh"
		elseif &filetype == "mp"
			exec "!mpost -tex=latex %<.mp"
		elseif &filetype == "dot"
			exec "!dot -Tps %<.dot -o %<.ps"
		elseif &filetype == "tex"
			exec "!xelatex %<.tex"
		elseif &filetype == "r"
			exec "!R --no-save -f %<.r"
		elseif &filetype == "php"
			exec "!php -f %<.php"
		elseif &filetype == "fortran"
			exec "!gfortran -g -Wall %<.f90" | exec "!./a.out"
		endif
endfunc

func! RunCode()
		exec "w"
		if &filetype == "c" || &filetype == "cpp" || &filetype == "haskell"
			exec "! ./%<"
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
		elseif &filetype == "fortran"
			exec "!./a.out"
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

" Ctrl + c Compile
map <c-c> :call CompileCode()<CR>
imap <c-c> <ESC>:call CompileCode()<CR>
vmap <c-c> <ESC>:call CompileCode()<CR>

" Ctrl + R Run
map <c-r> :call RunCode()<CR>
imap <c-r> <ESC>:call RunCode()<CR>
vmap <c-r> <ESC>:call RunCode()<CR>

" Ctrl + s Save
map <c-s> :call Save()<CR>
imap <c-s> <ESC>:call Save()<CR>
vmap <c-s> <ESC>:call Save()<CR>
" ======= compile and run ======= "

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


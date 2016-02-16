# Tricks
`\def` and `\newcommand`  
`\def` is a TeX primitive, `\newcommand` is a LaTeX overlay on top of `\def`. The most obvious benefits of `\newcommand` over `\def` are:  
`\newcommand` checks whether or not the command already exists  
`\newcommand` allows you to define an optional argument

# Hyperlink
`\usepackage{hyperref}`

超链接中的% 必须用\%, 进行转义, _ 可以用也可以不用

- `\url{网址}`:生成网址链接,以等宽字体排版,
- `\href{网址}{描述}`:生成网址链接,以正常字体显示描述,隐藏网址.
- `\href{mailto:xxx@xxx.mail}{xxx@xxx.mail}`
- `\hyperref[label]{text}`, text指向文中的label, 而`\ref` 是编号形式的
- `\autoref{label}`, `\Autoref{label}`

**不同链接类型的颜色**

| Item       | Value         | Note                                                                              |
|------------|---------------|-----------------------------------------------------------------------------------|
| colorlinks | (=false,true) | surround the links by color frames (false) or colors the text of the links (true) |
| linkcolor  | =red          | color of internal links (sections, pages, etc.)                                   |
| citecolor  | =green        | color of citation links (bibliography)                                            |
| urlcolor   | =magenta      | color of URL links (mail, web), 描述文字的颜色                                      |

**anchor**  
It is also possible to create an anchor anywhere in the document (with or without caption) and to link to it. To create an anchor, use:  
`\hypertarget{label}{target caption}`  
and to link to it, use:  
`\hyperlink{label}{link caption}`  
where the target caption and link caption are the text that is displayed at the target location and link location respectively.

**Local file**  
Files can also be linked using the url or the href commands. You simply have to add the string run: at the beginning of the link string:

	\url{run:/path/to/my/file.ext}
	\href{run:/path/to/my/file.ext}{text displayed}


用renewcommand定义了命令后怎么恢复原来的命令啊?
利用原始命令`\let,renew` 之前保存一下,以后再恢复:
```
\let\oldfoo\foo
\renewcommand\foo{new def}
...
\let\foo\oldfoo
```
在使用book 类型的class 时, chapter 默认会在新的一页开始, 如果要让chapter连续而不分页, 那么我们可以把clearpage 这个命令给关闭掉, 
但是关闭掉之后, 又出现了另外一个问题, 参考文献部分会出现问题(文献会显示不全), 所以我们就需要在参考文献部分将clearpage的命令恢复

```
\let\old\clearpage
\renewcommand\clearpage{}
正文内容
\cleardoublepage
\phantomsection
\addcontentsline{toc}{chapter}{参考文献}
\nocite{*}
\bibliography{data/bibs}
\let\clearpage\old
\cleardoublepage
```

# Commands
**index**  
`\usepackage{makeidx}`  
在导言区使用`\makeindex`激活导言  
索引的内容通过`\index{key}`

`\cleardoublepage`: ends the current page and causes all figures and tables that have so far appeared in the input to be printed. In a two-sided printing style, it also makes the next page a right-hand (odd-numbered) page, producing a blank page if necessary.

`\todo[inline, color=green!40]{This is an inline comment.}`

根据条件进行不同的设置
```
\newif\ifdirector
\directorfalse %I set the conditional to false
\ifdirector
 I write something for the director.
\else
 I write something for common people.
\fi
```

```
\setcounter{page}{1}
\renewcommand{\thepage}{A\arabicpage}}
```
可改变页码的格式为A1,A2,...的型式,直到进行下一次改变为止.

Set noindent for entire file
`\setlength{\parindent}{0pt}`

\hrule  画一条水平标尺
\vrule 竖直标尺

# Error
**Dimension too large**

	[1] <use  "./figs/surface.png" > [2]
	! Dimension too large.
	<to be read again> 
As explained below this specific JPG seems to be incompatible with XeTeX.
Apparently XeTeX has issues reading the size from the meta-data from the JPG, which somehow leads to the "too-large" error.  
用一个可以编辑图形的软件打开图形, 然后重新保存图形

**`\dot`的使用**  
latex 中输入点 不能使用\dot, 而应该直接使用 "."

# Other
\usepackage{blindtext}
%% \blindtext %随机生成该语言的字符

**多文档组织**  
`\input{abc}`: 把abc.tex中内容包含在当前源文件中,如同在此处直接输入进来
一般把常用的导言设置放到一个单独文件中
也可以用`\input abc`

`\include{abc}` **新开始一页**,然后把文件如同调用`\input{abc}`一样包含进来
每个文件为一章

当文档很长时,编译一遍也会很花时间,我们可以用syntonly 宏包.
这样编译时就**只检查语法,而不生成结果文件**.  
`\usepackage{syntonly}`

**编码**

	\usepackage[utf8]{inputenc}
	% ...
	% In this area
	% The UTF-8 encoding is specified.
	% ...
	\inputencoding{latin1}
	% ...
	% Here the text encoding is specified as ISO Latin-1.
	% ...
	\inputencoding{utf8}
	% Back to the UTF-8 encoding.
	% ...

XETEX默认使用UTF-8 编码,但为了照顾兼容性,可以用参数来控制输入文件的编码,
而不用更改整个输入文件的编码.

例如本文的tex 源代码用的是GB2312 编码方式,这时只需要在文档的开头加上  
`\XeTeXinputencoding "GB2312"`,并在`\begin{document}`前面加上  
`\XeTeXdefaultencoding "UTF8"`(似乎不加也可以)把输入改回`UTF-8`,文档即可正常编译.  
这样主要是省去了用户调整源文码编码的麻烦,加上常用的WinEdt 编辑器对UTF-8 的支持并不好.

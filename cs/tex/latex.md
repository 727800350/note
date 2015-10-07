# General
\title{标题}
\author{作者}
\today
\maketitle

# Tricks
`\def` and `\newcommand`  
`\def` is a TeX primitive, `\newcommand` is a LaTeX overlay on top of `\def`. The most obvious benefits of `\newcommand` over `\def` are:  
`\newcommand` checks whether or not the command already exists  
`\newcommand` allows you to define an optional argument

# Space
## 行间距和段间距
若在文档中使用两倍行距,要在导言区中加入  
`\linespread{1.6}`  
或等价地  
`\renewcommand{\baselinestretch}{1.6}`

这时,除了使得正文中行距为缺省值的两倍外,脚注和浮动对象中标题的行距 也扩大为原来的两倍.
要想在正文中使用两倍行距,而在标题中使用单倍行距, 可由 `setspace` 宏包来完成这一任务.  
`\usepackage{setspace} \linestretch{1.6}`  
\linestrech 的值为 1 时为单倍行距, 1.2 时是一倍半行距, 而为 1.6 时是双倍行距.

无论 setspace 使用与否, `\captionfont` 命令都可以用来 调节标题文本的行距.例如:  
`\renewcommand{\captionfont}{\linespread{1.6}\normalsize}`  
使得无论正文中的行距是多少,标题标题文本为双倍行距.

用在`\begin{document}`之后, 且他们只影响它们之后的文本  
改变行间距  
`\setlength{\baselineskip}{1.5\baselineskip}`  
改变段间距,  
\renewcommand{\baselinestretch}{1.5}

## 长度单位
point 是个传统印刷业采用的单位,而big point 是Adobe 推出PS 时新定义的单位.em 是个相对单位,比如当前字体是11pt 时,1em 就是11pt.

绝对单位  
in 英寸  
pt point, 1/72.27 in 

cm 厘米  
bp big point, 1/72 in 

mm 毫米  
pc pica, 12 pt   
mu math unit,1/18 em 

相对单位  
em 当前字体中字母M的宽度  
ex 当前字体中字母X的高度

## 空白间隔
- \qquad  2em当前字体下2 个字母"M"的宽度
- \quad  1em当前字体下1 个字母"M"的宽度
- \ 当前字体下1/3 个字母"M"的宽度(斜杠后是一个空格)
- \; 5/18em
- \, 3/18em
- \: 4/18em
- \! -3/18em

**Phantom Spaces**  
You sometimes want to have a specific amount of white space inserted at a certain spot, but TeX normally ignores white space, so what to do? This is what \phantom{} is for, for example:  
`$$\Gamma^i_{\phantom{i}jk} \quad  \Gamma^i_{jk}$$`

数学公式中空格`\quad`  间距比`~` 大

## 经常见到的宽度
- `\hsize` is a TeX primitive that should not be usually used in LaTeX
- `\textwidth` is the (constant) width of the total text block
- `\columnwidth` is the (constant) width of a single column of text
- `\linewidth` is a variable that represents the current size of the line of text, whether inside a column or a minipage or a list

In general, then, it is **best to always use `\linewidth`** if you are specifying the relative size of an image or a box, since it will adapt to the current situation.

Note: `\linewidth` also appears to work in table columns, not just text columns. See this answer for an example where a fixed-width parbox is used within a table cell (actually a multirow cell).

通过下面的代码可以看出这几个宽度的不同

	\documentclass[twocolumn]{article}
	\parindent=0pt
	\begin{document}
	
	\leavevmode\rlap{text:}\rule{\textwidth}{2pt}\par
	\leavevmode\rlap{line:}\rule{\linewidth}{2pt}\par
	\leavevmode\rlap{hsize:}\rule{\hsize}{2pt}\par
	\leavevmode\rlap{column:}\rule{\columnwidth}{2pt}
	
	\begin{itemize}
	\item \rule{\textwidth}{5pt}
	\item \rule{\linewidth}{5pt}
	\item \rule{\hsize}{5pt}
	\item \rule{\columnwidth}{5pt}
	\end{itemize}
	\end{document}
![width](http://i.imgbox.com/8YWL2rG5.jpg)

You can insert a horizontal stretched space with `\hfill` in a line so that the rest gets "pushed" toward the right margin.  
For instance this may be useful in the header.

`Author Name \hfill \today`  
generate  
![\hfill](http://i.imgbox.com/b5yWFCBp.png)

Similarly you can insert vertical stretched space with `\vfill`. It may be useful for special pages.

	\maketitle
	\vfill
	\tableofcontents
	\clearpage
	\section{My first section}


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

`\textsuperscript`  
For example, let us say I want to write the `$n^{th}$` element, but without the math mode automatic italicization of the th. 
And what if I still want the n to be in math mode, but the th outside?  
`$n$\textsuperscript{th}`

```
\setcounter{page}{1}
\renewcommand{\thepage}{A\arabicpage}}
```
可改变页码的格式为A1,A2,...的型式,直到进行下一次改变为止.

Set noindent for entire file
`\setlength{\parindent}{0pt}`

\hrule  画一条水平标尺
\vrule 竖直标尺

**摘录**
```
\begin{quote}
引文两端都缩进
\end{quote}

\begin{quotation}
引文两端都缩进且引文的首行继续缩进
\end{quotation}

\begin{verse}
引文两端都缩进, 且引文的第二行继续缩进
\end{verse}
```

**对齐方式**
`flushleft, flushright, center` 三个环境

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
这样主要是省去了用户调整源文码编码的麻烦,加上常用的WinEdt 编辑器对UTF-8 的支持并不
好.

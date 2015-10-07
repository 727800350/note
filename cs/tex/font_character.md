# 字体大小
`\fontsize{字号}{行距}`  
这个命令对其后所有文本都起作用,在使用此命令后需要用 `\selectfont` 才能使字体大小设置起作用.

我们通常会遇到别人规定:"正文用小四,宋体",但是 LaTeX 并没有小四,只有 pt,因此下表为字号对应的转换表:
比如:`\fontsize{12pt}{18pt}\selectfont` 就设置了 小四,且 1.5 倍行距.

| ?字号                       | 初号   | 小初   | 一号   | 小一   | 二号   | 小二   | 三号   | 小三     | 四号   | 小四   | 五号      | 小五   | 六号    | 小六     | 七号     | 小七    |
|---------------------------|------|------|------|------|------|------|------|--------|------|------|---------|------|-------|--------|--------|-------|
| 大小                        | 42pt | 36pt | 26pt | 24pt | 22pt | 18pt | 16pt | 15pt   | 14pt | 12pt | 10.5pt  | 9pt  | 7.5pt | 6.5pt  | 5.5pt  | 5pt   |
| 1.5行距时的 \baselineskip 设置值 | 63pt | 54pt | 39pt | 36pt | 33pt | 27pt | 24pt | 22.5pt | 21pt | 18pt | 15.75pt | 13.5 | 11.25 | 9.75pt | 8.25pt | 7.5pt |

Latex 设置字体大小命令由小到大依次为:
```
\tiny, \scriptsize, \footnotesize, \small, \normalsize, \large, \Large, \LARGE, \huge, \Huge
```

# Special Characters
## [french characters](http://en.wikibooks.org/wiki/LaTeX/Special_Characters#Escaped codes)
LaTeX command	Sample	Description
```
\`{o}	grave accent
\''{o}	(single)acute accent
\^{o}	circumflex
\""{o}	(single)umlaut, trema or dieresis
\H{o}	long Hungarian umlaut (double acute)
\~{o}	tilde
\c{c}	cedilla
\k{a}	ogonek
\l		barred l (l with stroke)
\={o}	macron accent (a bar over the letter)
\b{o}	bar under the letter
\.{o}	dot over the letter
\d{u}	dot under the letter
\r{a}	ring over the letter (for ? there is also the special command \aa)
\u{o}	breve over the letter
\v{s}	caron/hacek ("v") over the letter
\t{oo}	"tie" (inverted u) over the two letters
\o		slashed o (o with stroke)
```
#French Ordinal Numbers
After having included `\usepackage[greek,francais]{babel}`, use
```
– 1\ier{}, 
– 1\iere{} or 
– 1\ieres{}
```
For other ordinal numbers, use
```
– \ieme{} or
– \iemes{}
```

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



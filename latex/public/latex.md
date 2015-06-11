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

# Special Characters
## [french characters](http://en.wikibooks.org/wiki/LaTeX/Special_Characters#Escaped codes)
LaTeX command	Sample	Description
```
\`{o}	grave accent
\'{o}	acute accent
\^{o}	circumflex
\"{o}	umlaut, trema or dieresis
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
After having included \usepackage[frenchb]{babel}, use
– 1\ier{}, 
– 1\iere{} or 
– 1\ieres{}

For other ordinal numbers, use
– \ieme{} or
– \iemes{}
# Math
`$\not\in$`

数学公式中输入中文 可以把中文用`\text{}`或是`\mbox{}`引起来  
在`\text{}` 仍然可以使用`$$` 符号进行数学输入

## 分数
- `\frac` 自动调整字号,比如在行间公式中小一点,在独立公式则大一点.
- `\dfrac` 命令把分数的字号显式设置为独立公式中的大小,
- `\tfrac` 命令则把字号设为行间公式中的大小.

`$$ \lim_{\| x\| \to +\infty \atop x \in P} f(x) = +\infty $$`  
generate:  
![\atop](http://i.imgbox.com/OFTF8dsw.png)

会产生下面的, 但是latex 对于\atop 的使用会给出警告, 提示说可以采用\genfrac

`\genfrac` won't produce a line if the third argument (which control the thickness of the rule) is set to 0pt; the syntax for `\genfrac` is  

`\genfrac{<left-delim>}{<right-delim>}{<thickness>}{<mathstyle>}{<numerator>}{<denominator>}`
	
Eg: 
`\newcommand\mycom[2]{\genfrac{}{}{0pt}{}{#1}{#2}}`  
`$A\mycom{a=x}{b=y}B$`  
generate  
![genfrac](http://i.imgbox.com/XqXECVbO.png)

`$$\xrightarrow[x<y]{a*b*c}$$`  
generate:  
![xrightarrow](http://i.imgbox.com/0zbtS5lV.png)

**多重积分**  
`\iint, \iiint, \iiiint, \idotsint`

## 多行公式
有多种条件的公式组用cases 次环境

	$$
	f(n) = 
	\begin{cases}
	 n/2 & \mbox{if } n \equiv 0 \\ 
	(3n +1)/2 & \mbox{if } n \equiv 1. 
	\end{cases}
	$$

	\begin{equation} 或者 $$
	\begin{aligned}
	\end{aligned}
	\end{equation}
	
	\begin{equation} 不能使用$$, 不知道为什么
	\begin{split}
	\end{split}
	\end{equation}
	
	\begin{align}
	\end{align}
	align 一行一个公式序号

	\begin{gather}
	\end{gather}
	gather 一行一个公式序号
	不需要对齐的公式组用 gather 环境,需要对齐的用align
	
	\begin{eqarray}
	\end{eqarray}

[Avoid eqnarray](https://tug.org/pracjourn/2006-4/madsen/madsen.pdf)

公式中对齐

```
\begin{equation}  fst_a(A)\triangleq 
\left\{
      \begin{aligned}
			 &\varnothing & if\ a \notin\Sigma(A)\\
             &hitter(A_m) & if\ m=min\{n \in \uppercase\expandafter{\romannumeral2^A|a \in \Sigma(A_n)}\land\Sigma(hitter(A_m))=a\}\\  
             &target(A_m) & if\ m=min\{n \in \uppercase\expandafter{\romannumeral2^A|a \in \Sigma(A_n)}\land\Sigma(target(A_m))=a\}\\  
           \end{aligned}
\right.  
\end{equation}  
```
\href{http://i.imgbox.com/JGSKOgTs.png}{result}


```
\begin{equation}  fst_a(A)\triangleq 
\left\{
      \begin{aligned}
             &\varnothing & if & \ a \notin\Sigma(A)\\
             &hitter(A_m) & if & \ m=min\{n \in \uppercase\expandafter{\romannumeral2^A|a \in \Sigma(A_n)}\land\Sigma(hitter(A_m))=a\}\\  
             &target(A_m) & if & \ m=min\{n \in \uppercase\expandafter{\romannumeral2^A|a \in \Sigma(A_n)}\land\Sigma(target(A_m))=a\}\\  
           \end{aligned}
\right.  
\end{equation}  
```
\href{http://i.imgbox.com/mcWpxQK9.png}{将if放在单独一格的result}

## Matrix
pmatrix,bmatrix,Bmatrix,vmatrix和Vmatrix

## 数学字体
- \mathrm 罗马直立
- \mathit 罗马斜体
- \mathbf 直立粗体
- \mathcal 花体
- \mathtt
- \mathsf

![数学注音符号](http://i.imgbox.com/I7RSbdFC.jpg)

`$$n+1 \choose 3$$` 生成$C_{n+1}^3$ 的括号形式

**括号**
`(), [], \{\}, \langle\rangle, \lvert\rvert, \lVert\rVert`
我们可以在上述分隔符前面加`\big \Big \bigg \Bigg` 等命令来调整

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

## 字体大小
```
\tiny
\scriptsize
\footnotesize
\small
\normalsize
\large
\Large
\LARGE
\huge
\Huge
```

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

In general, then, it's **best to always use `\linewidth`** if you are specifying the relative size of an image or a box, since it will adapt to the current situation.

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


# Figure
[Floats,_Figures_and_Captions](http://en.wikibooks.org/wiki/LaTeX/Floats,_Figures_and_Captions)

**图片的位置**
h: here, t: top, b: bottom, H: Here
!:  Override internal parameters LaTeX uses for determining "good" float positions.                                              |
[htp]搭配!可以使latex忽略美学排版
引入`\usepackage{float}`, 然后`[H]`,注意是大H,可以使图片在指定位置出现

## Special figure effect
### Side captions
have a caption appear on the side of a float, rather than above or below. 

`SCfigure` environment

	\usepackage{sidecap}
	\begin{SCfigure}
	  \includegraphics[width=0.3\textwidth]{Giraff_picture}
	\end{SCfigure}
![Latex_example_sidecap](http://upload.wikimedia.org/wikipedia/commons/5/59/Latex_example_sidecap.png)

### Lists of figures and tables
`\caption[short]{long}`, used for the List of Tables or List of Figures.
Typically the short description is for the caption listing(table of contents for figure or table), 
and the long description will be placed beside the figure or table.

	\listoffigures
	\begin{figure}[hb]
	  \includegraphics[width=4in]{gecko}
	  \caption[Close up of \textit{Hemidactylus} sp.]
	   {Close up of \textit{Hemidactylus} sp., which is part the genus of the gecko family. It is the second most speciose genus in the family.}
	\end{figure}
![list figure](http://upload.wikimedia.org/wikipedia/commons/thumb/f/f7/LaTeX_figure_caption_with_lof_entry.png/400px-LaTeX_figure_caption_with_lof_entry.png)

### Wrapping text around figures
```
\usepackage{wrapfig}
\begin{wrapfigure}[lineheight]{position}{width}
```

**possible positioning targets**:
r,R: right side of the text
l,L: left side of the text
i,I: inside edge–near the binding (in a twoside document)
o,O: outside edge–far from the binding

The uppercase-character allows the figure to float, while the lowercase version means "exactly here".

	\begin{wrapfigure}{r}{0.5\textwidth}
	  \vspace{-20pt}
	  \begin{center}
	    \includegraphics[width=0.48\textwidth]{gull}
	  \end{center}
	  \vspace{-20pt}
	  \caption{A gull}
	  \vspace{-10pt}
	\end{wrapfigure}
[result figure](http://upload.wikimedia.org/wikipedia/commons/d/dc/Latex_example_wrapfig_vspace.png)

## subfloat
```
\usepackage{subfig}
\subfloat[list entry][sub-caption]{\label{sub-label}body}
body 可以为figure, 也可以是table
```
Normally, if a caption is present, it is also included on the List-of-Floats page.
However, if a second optional argument is present, than the first one controls what is on the List-of-Floats page and the second is the caption text

# Table
```
\begin{table}[h]\small 
\begin{tabular}
...
\end{tabular}
\end{table}
```
`\small` 也可以用在其他地方, 尤其是beamer 中itemzie 等环境中

## Subtable
	\begin{table}[<placement specifier>]
	    \begin{subtable}[<placement specifier>]{<width>}
	        \centering
	        ... table 1 ...
	        \caption{<sub caption>}
	    \end{subtable}
	    ~
	    \begin{subtable}[<placement specifier>]{<width>}
	        \centering
	        ... table 2 ...
	        \caption{<sub caption>}
	    \end{subtable}
	\end{table}

## 图形格式
png 它是无损压缩格式,jpg是有损

LATEX 支持点阵图形格式JPEG 和PNG,也支持矢量格式EPS 和PDF.  
对于示意图,我们应该首选矢量格式,  
包含大量自然色彩的图像(比如照片)应该选JPEG,  
人工点阵图像应该选PNG

- dvips 喜欢PS,所以就爱屋及乌只支持嵌入EPS
- pdfLATEX 支持JPEG,PNG和PDF,不支持EPS.  
- LATEX 有两个宏包`epstopdf` 和`pst-pdf` 可以实时地(on the fly)把EPS 转换为PDF1.然而前者有安全漏洞,后者用法繁琐,用户最好还是用其它软件事先把EPS 转为PDF.
- dvipdfm 支持JPEG,PNG,PDF,不支持EPS,但是它可以实时 地调用Ghostscript 把EPS 转为PDF

`\usepackage{epstopdf}`  
但是在使用`\includegraphics`的时候仍然使用: `\includegraphics [width=5cm]{back1.eps}`  
而不需要换成相应的pdf文件, 在编译的时候, 会生成`back1-eps-converted-to.pdf`文件

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

# References
设置标签是`\label{}`  
引用标签是`\ref{}`  
引用标签所在页的页码是`\pageref{}` 命令  
引用equation `\eqref{}`, 相当于 (\ref{}), `\lasteq`(自定义的)

## 参考文献

	\bibliographystyle{plain}
	\bibliography{mybib}

其中`mybib.bib` 是参考文献 `database`

**使用** 

	\cite{item1, item2} %一个地方同时引用item1和item2
	\cite{item}

**编译步骤**

	xelatex paper.tex ## 只把条目的关键字写到中间文件`.aux` 中去.
	bibtex paper.aux ## 根据`.aux,.bib,.bst` 生成一个`.bbl` 文件,即参考文献列表
	xelatex paper.tex ## 把交叉引用写到`.aux` 中去.
	xelatex paper.tex ## 在正文中正确地显示引用.

**引用的目录问题**

	\clearpage
	\phantomsection
	\addcontentsline{toc}{section}{References}
	\bibliographystyle{plain}
	\bibliography{ref}
不能使用 `\section*{References}`, 否则会出现两遍标题性质的References,不明白为什么

**参考文献的行间距**  
`\setlength{\itemsep}{-0.5mm}`

**把参考文献的字体调小**  
And if you use the natbib package:  
`\def\bibfont{\footnotesize}`  
If you are having problems with getting itshapes, you can use 
`\renewcommand{\bibfont}{\normalfont\small}` 

Using the biblatex package you could also write  
`\renewcommand*{\bibfont}{\footnotesize}`  
Using beamer+biblatex you could also write  
`\frame[shrink=50] {\printbibliography}` 

**如何在章节标题中应用参考文献?**  
为实现章节标题中对参考文献的引用,采取保护脆弱命令的措施,如下所示:

	\subsection[the title of chapter(section et .)]
	{the title of chapter(section et .)\protect\upcite{keywords}}
即可是在章节标题中引用参考文献.

# Font
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

# Table of contents
`\tableofcontents`

`\setcounter{tocdepth}{2}`
table  of contents depth 也可以用\setcounter 命令来指定目录层次深度.

`\renewcommand{\thesection}{\Alph{section}}`  
上述命令将(section)的序号改成大写字母的型式, 如下图所示  
![Alph Table of Contents](http://i.imgbox.com/VPal8xXr.jpg)

如果不想让某个章节标题出现在目录中,可以使用以下带* 的命令来声明章节.
`\chapter*{...}, \section*{...}, \subsection*{...}`


更换掉默认的显示内容
`renewcommand{\contentsname}{目录}`

插图和表格的目录 `\listoffigures, \listoftables`

当章节或图表等结构发生变化时,我们需要执行两遍编译命令以获得正确结果.LATEX 之所以设计成这样可能是因为当时的电脑内存容量有限.

## \addcontentsline
`\addcontentsline{file}{sec_unit}{entry}`  
adds an entry to thespecified list or table where

`file` is the extension of the file on which information is to be written:

- toc - table of contents
- lof - list of figures
- lot - list of tables

`sec_unit` controls the formatting of the entry. It should be one of the following, depending upon the value of the file argument:

- toc - the name of the sectional unit, such as part or subsection
- lof - figure
- lot - table

`entry` is the text of the entry.

Eg:

	\chapter*{结论与展望}\addcontentsline{toc}{chapter}{结论与展望}
	\section*{结论}\addcontentsline{toc}{section}{结论}

**\addcontentsline{}{}{} 生成PDF 文件后链接不对**  
参看CTeX-faq 74 摘录如下:  
这是因为\addcontentsline 命令后面的命令换页造成的页码不一致.需要在\addcontentsline 命令之前加上
`\clearpage`.  
如果是book 类或者用了openright 参数的report 类文档,新的章总是从奇数页开始,则要把命令改成
`\cleardoublepage`.  
如果使用了hyperref 宏包来生成PDF 文件的书签链接,则还需要在上面的命令后再加上
`\phantomsection`

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

# 页眉页脚
页眉和页脚的样式是由命令 \pagestyle 和 \pagenumbering 来定义的. 
\pagestyle 命令定义了页眉和页脚的基本内容(如页码出现在哪里),而 \pagenumbering 则定义了页码的显示方式. 
LATEX 本身包含四种标准的页面样式.

1. empty 没有页眉也没有页脚
1. plain 没有页眉,页脚包含一个居中的页码
1. headings 没有页脚,页眉包含章 / 节或者子节的名字和页码
1. myheadings 没有页脚,页眉包含有页码和用户提供的其他信息

`\pagenumbering{num_style}`
Specifies the style of page numbers. Possible values of 'num_style' are:
arabic Arabic numerals roman Lowercase Roman numerals Roman Uppercase Roman numeral alph Lowercase letters Alph Uppercase letters
\pagestyle 命令便将 \thepage 放在适当的位置
另外, \pagenumbering 命令同时也将把页码重置为 1
应用:可在
\tableofcontents
\listoffigures
\listoftables
之前用 \pagenumbering{Roman}
在正文之前再用\pagenumbering{arabic}

`\setcounter{}{}`
重新开始页面编号
`\setcounter{page}{1}`,{1}可以是你需要的任意编号

第一页显然不需要这些页眉和页脚.在 `\begin{document}` 之后而在\maketitle 命令之前敲入下面的命令来
去掉页眉和页脚中其他内容而只保留页码: `\thispagestyle{plain}`
或者如果你什么也不想要的话. `\thispagestyle{empty}`

实际上`\maketitle` 中已经定义了命令 `\thispagestyle{plain}` .因此,如果你坚持要在 `\maketitle` 生成的页面上使用 fancy 的布局,
你必须在 `\maketitle` 之后马上发出 `\thispagestyle{fancy}` 的指示.

通常,对于 book 或者 report 来说,你可能会把章节的信息反映在页眉上(对于单面打印可能只需要章次的信息),
以及对于 article 文档类的节和子节(对于单面打印只需要节次的信息). LATEX 使用标记( mark )的机制来记录章节信息

命令 `\leftmark`(高层) 记录了页面上最近一次 `\markboth` 命令左边的参数,
而 `\rightmark`(底层) 则记录了页面上第一次 `\markboth` 命令右边的参数,或者是页面上第一次 `\markright` 命令的参数 值.如果当前页面没有标记,
则两者都维持前面的页面中的值保持不变

最后还要提醒你的是, * 形式的 `\chapter` 等命令不会调用标记相关的命令,因此如果你想要在前言中插入页眉或页脚,
那么你就得自己加入 `\markboth` 命令,因为前言并不会计算入章节号,也不会被列入目录中.  
```
\chapter*{Preface\markboth{Preface}{}}
```

## 双面打印
```
E 偶数页 even
O 奇数页 odd
L 居左内容
C 居中内容
R 居右内容
H 页眉
F 页脚
```

eg:
```
%% 在偶数页的左边和奇数页的右边放上底层信息, 例如 section 信息
\fancyhead[LE,RO]{\slshape \rightmark} 
%% 在奇数页的左边和偶数页的右边放上高层信息, 例如 chapter 信息
\fancyhead[LO,RE]{\slshape \leftmark}
\fancyfoot[C]{\thepage}
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

原样显示
\beigin{verbatim}...\end{verbatim}
\verb+...+

代码  
如果是beamer frame, 则必须用`\begin{frame}[fragile]`, verbatim  环境也一样
```
\usepackage{listings}
\lstinline[<options>]<char><code><char>
\begin{lstlisting}[language=...] Put your code here. \end{lstlisting}
\lstinputlisting[language=Python]{source_filename.py}
\lstinputlisting[language=Python, float, caption = Add Fake Source Dependencies, firstnumber=91, firstline=91, lastline=95]{source_filename.py}
```
Python 的注释只能使用一个#, 而不能使用两个##, 否则会报错.

代码中可以有_, 但是注释中不能有, 否则需要使用math mode

- language  
C, C++, bash, Python
- title={<text>}  
is used to set an unnumbered and unlabelled title. If <text> contains a comma or equal sign, make sure you enclose <text> in curly braces { and }.
- caption={[<short>]<text>}  
is used to set a numbered caption. The optional part <short> is an alternative short caption for the list of listings, which can be produced using  
`\lstlistoflistings`  
As above, if the caption contains a comma or equal sign, make sure you enclose it in curly braces { and }.
- mathescape  
This is a boolean key that can either be true (dollar $ character acts as the usual math mode shift) or false (deactivates the usual behaviour of $).
- escapechar=@  


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

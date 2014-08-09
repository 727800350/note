# General
\title{标题}
\author{作者}
\today
\maketitle

\hrule  画一条水平标尺

\vrule 竖直标尺

# Tricks
`\def` and `\newcommand`  
`\def` is a TeX primitive, `\newcommand` is a LaTeX overlay on top of `\def`. The most obvious benefits of `\newcommand` over `\def` are:  
`\newcommand` checks whether or not the command already exists  
`\newcommand` allows you to define an optional argument

# Special Characters
## [french characters](http://en.wikibooks.org/wiki/LaTeX/Special_Characters#Escaped codes)
LaTeX command	Sample	Description
```
\`{o}	ò	grave accent
\'{o}	ó	acute accent
\^{o}	ô	circumflex
\"{o}	ö	umlaut, trema or dieresis
\H{o}	ő	long Hungarian umlaut (double acute)
\~{o}	õ	tilde
\c{c}	ç	cedilla
\k{a}	ą	ogonek
\l	ł	barred l (l with stroke)
\={o}	ō	macron accent (a bar over the letter)
\b{o}	o	bar under the letter
\.{o}	ȯ	dot over the letter
\d{u}	ụ	dot under the letter
\r{a}	å	ring over the letter (for å there is also the special command \aa)
\u{o}	ŏ	breve over the letter
\v{s}	š	caron/hacek ("v") over the letter
\t{oo}	o͡o	"tie" (inverted u) over the two letters
\o	ø	slashed o (o with stroke)
```

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

## Matrix
pmatrix,bmatrix,Bmatrix,vmatrix和Vmatrix

## 数学字体
- \mathrm罗马直立
- \mathit   罗马斜体
- \mathbf 直立粗体
- \mathcal 花体
- \mathtt
- \mathsf

![math font](http://i.imgbox.com/3lpesF5s.jpg)

![数学注音符号](http://i.imgbox.com/I7RSbdFC.jpg)

`$$n+1 \choose 3$$`  
generate  
![C](http://i.imgbox.com/0GUfRPFH.png)

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

| Specifier  | Permission                                                                                                                   |
|------------|------------------------------------------------------------------------------------------------------------------------------|
| h          | Place the float here                                                                                                         |
| t          | Position at the top of the page.                                                                                             |
| b          | Position at the bottom of the page.                                                                                          |
| p          | Put on a special page for floats only.                                                                                       |
| !          | Override internal parameters LaTeX uses for determining "good" float positions.                                              |
| H          | Places the float at precisely the location in the LaTeX code. Requires the float package. This is somewhat equivalent to h!. |

## Special figure effect
### Side captions
have a caption appear on the side of a float, rather than above or below. The `sidecap` package can be used to place a caption **beside a figure or table**. 

The following example demonstrates this for a figure by using a `SCfigure` environment in place of the figure environment.

	\begin{SCfigure}
	  \centering
	  \caption{ ... caption text ... }
	  \includegraphics[width=0.3\textwidth]%
	    {Giraff_picture}% picture filename
	\end{SCfigure}
![Latex_example_sidecap](http://upload.wikimedia.org/wikipedia/commons/5/59/Latex_example_sidecap.png)

### Lists of figures and tables
`\caption[short]{long}`, used for the List of Tables or List of Figures.   Typically the short description is for the caption listing, and the long description will be placed beside the figure or table.

	\begin{document}
	\listoffigures
	\section{Introduction}
	\begin{figure}[hb]
	  \centering
	  \includegraphics[width=4in]{gecko}
	  \caption[Close up of \textit{Hemidactylus} sp.]
	   {Close up of \textit{Hemidactylus} sp., which is
	   part the genus of the gecko family. It is the
	   second most speciose genus in the family.}
	\end{figure}
	\end{document}
![list figure](http://upload.wikimedia.org/wikipedia/commons/thumb/f/f7/LaTeX_figure_caption_with_lof_entry.png/400px-LaTeX_figure_caption_with_lof_entry.png)

### Wrapping text around figures
Before you make the choice of including figures with text wrapping in your document, make sure you have considered all the options.  
For example, you could use a layout with two columns for your documents and have no text-wrapping at all.

\usepackage{wrapfig}

\begin{wrapfigure}[lineheight]{position}{width}

There are overall eight possible positioning targets:

| r | R | right side of the text                               |
|---|---|------------------------------------------------------|
| l | L | left side of the text                                |
| i | I | inside edge–near the binding (in a twoside document) |
| o | O | outside edge–far from the binding                    |

The uppercase-character allows the figure to float, while the lowercase version means "exactly here".

The width is, of course, the width of the figure.

	\begin{wrapfigure}{r}{0.5\textwidth}
	  \begin{center}
	    \includegraphics[width=0.48\textwidth]{gull}
	  \end{center}
	  \caption{A gull}
	\end{wrapfigure}

![Wrap figure](http://upload.wikimedia.org/wikipedia/commons/thumb/7/74/Latex_example_wrapfig.png/337px-Latex_example_wrapfig.png)


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

### Subfloats
`\setcounter{subfigure}{0}`
让每个figure的subfigure都重新编号, 而不是整个article 的subfigure的编号是连续的, 
[ref](http://tex.stackexchange.com/questions/4530/subfigures-in-beamer-documents)

	\setcounter{subfigure}{0}
	\begin{figure}
	        \centering
	        \begin{subfigure}[b]{0.3\textwidth}
	                \includegraphics[width=\textwidth]{gull}
	                \caption{A gull}
	                \label{fig:gull}
	        \end{subfigure}%
	        ~ %add desired spacing between images, e. g. ~, \quad, \qquad etc.
	          %(or a blank line to force the subfigure onto a new line)
	        \begin{subfigure}[b]{0.3\textwidth}
	                \includegraphics[width=\textwidth]{tiger}
	                \caption{A tiger}
	                \label{fig:tiger}
	        \end{subfigure}
	        ~ 
	        \begin{subfigure}[b]{0.3\textwidth}
	                \includegraphics[width=\textwidth]{mouse}
	                \caption{A mouse}
	                \label{fig:mouse}
	        \end{subfigure}
	        \caption{Pictures of animals}\label{fig:animals}
	\end{figure}
[result figure](http://upload.wikimedia.org/wikipedia/commons/thumb/e/e5/Latex_example_subfig.png/500px-Latex_example_subfig.png)

Subtable

You will notice that the figure environment is set up as usual. You may also use a table environment for subtables. For each subfloat, you need to use:

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

## 图形后缀, 目录
若想省略文件后缀,可在插入图形前使用两个命令.  
前者指定一个后缀列表,让LATEX 自行查找,  
后者告诉LATEX 未知后缀的都是EPS.

	\DeclareGraphicsExtensions{.eps,.mps,.pdf,.jpg,.png}
	\DeclareGraphicsRule{*}{eps}{*}{}
	图片搜索目录
	\graphicspath{{figure/}{image/}}

在选择图形驱动时, 已经有一个相应的预设的扩展名集. 举例来说, 如果选择divps作为图形驱动, 那么缺省的会使用下列图形文件扩展名  
`\DeclareGraphicsExtensions{.eps,.ps,.eps.gz, .eps.Z}`  
注意 `\includegraphics{file}`不会试图寻找`file`文件, 除非空的扩展名{}已被加入到扩展名列表中  
例如: `\DeclareGraphicsExtensions{.eps, .ps, {}}`

### 图形格式
png 它是无损压缩格式,jpg是有损

LATEX 支持点阵图形格式JPEG 和PNG,也支持矢量格式EPS 和PDF.  
对于示意图,我们应该首选矢量格式,  
包含大量自然色彩的图像(比如照片)应该选JPEG,  
人工点阵图像应该选PNG

- dvips 喜欢PS,所以就爱屋及乌只支持嵌入EPS
- pdfLATEX 支持JPEG,PNG和PDF,不支持EPS.  
- LATEX 有两个宏包`epstopdf` 和`pst-pdf` 可以实时地(on the fly)把
EPS 转换为PDF1.然而前者有安全漏洞,后者用法繁琐,用户最好还是
用其它软件事先把EPS 转为PDF.
- dvipdfm 支持JPEG,PNG,PDF,不支持EPS,但是它可以实时
地调用Ghostscript 把EPS 转为PDF

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

文档中新增交叉引用后,第一次执行`xelatex` 编译命令时会得到类似下面的警告信息.因为第一次编译只会扫描出有交叉引用的地方,第二次编译才能得到正确结果

	LaTeX Warning: There were undefined references.
	...
	LaTeX Warning: Label(s) may have changed. Rerun to get crossreferences
	right.

## 参考文献

	\bibliographystyle{plain}
	\bibliography{mybib}

其中`mybib.bib` 是参考文献 `database`

**使用** 

	\cite{item1, item2}
	%一个地方同时引用item1和item2
	\cite{item}

**编译步骤**

	xelatex paper.tex
	bibtex paper.aux
	xelatex paper.tex
	xelatex paper.tex

前文中我们提到含有交叉引用的文档需要编译两遍.含有参考文献的文档更麻烦,它需要依次执行`xelatex,bibtex,xelatex,xelatex` 等四次编译:

1. 第一遍`xelatex` 只把条目的关键字写到中间文件`.aux` 中去.
2. `bibtex` 根据`.aux,.bib,.bst` 生成一个`.bbl` 文件,即参考文献列表.它的内容就是`thebibliography` 环境和一些`\bibtem` 命令.
3. 第二遍`xelatex` 把交叉引用写到`.aux` 中去.
4. 第三遍`xelatex` 则在正文中正确地显示引用.

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

# Table of contents
`\tableofcontents`

`\setcounter{tocdepth}{2}`
table  of contents depth 也可以用\setcounter 命令来指定目录层次深度.

`\renewcommand{\thesection}{\Alph{section}}`  
上述命令将(section)的序号改成大写字母的型式, 如下图所示  
![Alph Table of Contents](http://i.imgbox.com/VPal8xXr.jpg)

如果不想让某个章节标题出现在目录中,可以使用以下带* 的命令来
声明章节.

	\chapter*{...}
	\section*{...}
	\subsection*{...}

插图和表格的目录

	\listoffigures
	\listoftables

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
	\section*{展望}\addcontentsline{toc}{section}{展望}

**\addcontentsline{}{}{} 生成PDF 文件后链接不对**  
参看CTeX-faq 74 摘录如下:  
这是因为\addcontentsline 命令后面的命令换页造成的页码不一致.需要在\addcontentsline 命令之前加上
`\clearpage`.  
如果是book 类或者用了openright 参数的report 类文档,新的章总是从奇数页开始,则要把命令改成
`\cleardoublepage`.  
如果使用了hyperref 宏包来生成PDF 文件的书签链接,则还需要在上面的命令后再加上
`\phantomsection`

# Index
`\usepackage{makeidx}`

在导言区使用`\makeindex`激活导言

索引的内容通过`\index{key}`

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

XETEX默认使用UTF-8 编码，但为了照顾兼容性，可以用参数来控制输入文件的编码，
而不用更改整个输入文件的编码。

例如本文的tex 源代码用的是GB2312 编码方式，这时只需要在文档的开头加上  
`\XeTeXinputencoding "GB2312"`，并在`\begin{document}`前面加上  
`\XeTeXdefaultencoding "UTF8"`（似乎不加也可以）把输入改回`UTF-8`，文档即可正常编译。  
这样主要是省去了用户调整源文码编码的麻烦，加上常用的WinEdt 编辑器对UTF-8 的支持并不
好。

# Figure
## 图形后缀, 目录
若想省略文件后缀，可在插入图形前使用两个命令。  
前者指定一个后缀列表，让LATEX 自行查找；  
后者告诉LATEX 未知后缀的都是EPS。

	\DeclareGraphicsExtensions{.eps,.mps,.pdf,.jpg,.png}
	\DeclareGraphicsRule{*}{eps}{*}{}
	图片搜索目录
	\graphicspath{{figure/}{image/}}

在选择图形驱动时, 已经有一个相应的预设的扩展名集. 举例来说, 如果选择divps作为图形驱动, 那么缺省的会使用下列图形文件扩展名  
`\DeclareGraphicsExtensions{.eps,.ps,.eps.gz, .eps.Z}`  
注意 `\includegraphics{file}`不会试图寻找`file`文件, 除非空的扩展名{}已被加入到扩展名列表中  
例如: `\DeclareGraphicsExtensions{.eps, .ps, {}}`

### 图形格式
png 它是无损压缩格式，jpg是有损

LATEX 支持点阵图形格式JPEG 和PNG，也支持矢量格式EPS 和PDF。  
对于示意图，我们应该首选矢量格式；  
包含大量自然色彩的图像（比如照片）应该选JPEG，  
人工点阵图像应该选PNG

- dvips 喜欢PS，所以就爱屋及乌只支持嵌入EPS
- pdfLATEX 支持JPEG、PNG和PDF，不支持EPS。  
- LATEX 有两个宏包`epstopdf` 和`pst-pdf` 可以实时地（on the fly）把
EPS 转换为PDF1。然而前者有安全漏洞，后者用法繁琐，用户最好还是
用其它软件事先把EPS 转为PDF。
- dvipdfm 支持JPEG、PNG、PDF，不支持EPS，但是它可以实时
地调用Ghostscript 把EPS 转为PDF

# Hyperlink
超链接中的% 必须用\%, 进行转义, _ 可以用也可以不用

- \url{网址}：生成网址链接，以等宽字体排版；
- \href{网址}{描述}：生成网址链接，以正常字体显示描述，隐藏网址。
- \href{mailto:i@huxuan.org}{i@huxuan.org}
- \hyperref[label]{text}, 指向文中的label

# References
设置标签是`\label{}`  
引用标签是`\ref{}`  
引用标签所在页的页码是`\pageref{}` 命令  
引用equation `\eqref{}`

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
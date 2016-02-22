单位

- 绝对单位  
	- in 英寸  
	- pt point, 1/72.27 in, point 是个传统印刷业采用的单位
	- cm 厘米  
	- bp big point, 1/72 in, big point 是Adobe 推出PS 时新定义的单位
	- mm 毫米  
	- pc pica, 12 pt   
	- mu math unit,1/18 em 
- 相对单位  
	- em 当前字体中字母M的宽度, em 是个相对单位,比如当前字体是11pt 时,1em 就是11pt.
	- ex 当前字体中字母X的高度

空白间隔

- \qquad  2em当前字体下2 个字母"M"的宽度
- \quad  1em当前字体下1 个字母"M"的宽度
- \ 当前字体下1/3 个字母"M"的宽度(斜杠后是一个空格)
- \; 5/18em
- \, 3/18em
- \: 4/18em
- \! -3/18em

You sometimes want to have a specific amount of white space inserted at a certain spot,
but TeX normally ignores white space, so what to do? This is what `\phantom{}` is for, for example:  
`$$\Gamma^i_{\phantom{i}jk} \quad  \Gamma^i_{jk}$$`

经常见到的宽度

- `\hsize` is a TeX primitive that should not be usually used in LaTeX
- `\textwidth` is the (constant) width of the total text block
- `\columnwidth` is the (constant) width of a single column of text
- `\linewidth` is a variable that represents the current size of the line of text, whether inside a column or a minipage or a list

In general, then, it is **best to always use `\linewidth`** if you are specifying the relative size of an image or a box, since it will adapt to the current situation.  
Note: `\linewidth` also appears to work in table columns, not just text columns. 

通过下面的代码可以看出这几个宽度的不同

```Latex
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
```
![width](http://i.imgbox.com/8YWL2rG5.jpg)

- `\hfill`:  insert a horizontal stretched space in a line so that the rest gets "pushed" toward the right margin.  
- `\vfill`: you can insert vertical stretched space with 


正文间距

- `\baselineskip` 正文中的行间距, 但是要改变行间距的时候, 一般不直接修改`\baselineskip`, 而是修改 `\baselinestretch` 的值
- `\baselinestretch` 是对 `\baselineskip` 进行缩放的, 默认是1. 因此, 设置2倍行间距的正确操作为: `\renewcommand{\baselinestretch}{2}`

- `parskip`: 段落前后间距, `\setlength{\parskip}{.5\baselineskip}`, 

段落首行缩进

- 局部取消: `\noindent`
- 全局取消: `\setlength{\parindent}{0pt}`

页边距
http://www.52souji.net/set-the-margin-header-footer-of-latex/
 
 
# floats
[ref](http://techshangrila.blogspot.com/2014/01/reduce-space-around-floats-algorithm.html)

- \captionsetup{belowskip=12pt,aboveskip=4pt}
	- aboveskip: 指caption 与 图标之间的距离.
	- belowskip: 指caption 与 正文之间的距离.
		注意, table 的caption 是在table的上面, 所以对于table来说, belowskip就是指上面的正文与caption的距离, figure 是caption 与下面的正文之间的距离.

- \floatsep: distance between two floats, 浮动体之间的距离
	- `\setlength{\floatsep}{1.25\baselineskip plus 3pt minus 2pt}`
- \intextsep:— for floats in the middle of text only - length between text above it, and text below it, 浮动体和正文间的距离.
	- `\setlength\{intextsep}{1.25\baselineskip plus 3pt minus 2 pt}`
- \textfloatsep: for floats on top and bottom of text only,
	For floats at top - length between float and text below it,
	For floats at bottom - length between float and text above it.
	- \setlength\{textfloatsep}{1.25\baselineskip plus 3pt minus 2pt}
 

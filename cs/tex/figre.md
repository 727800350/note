# Figure and Table
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


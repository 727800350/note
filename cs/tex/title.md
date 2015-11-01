[LaTeX Templates](http://www.latextemplates.com/)

[Sharelatex](https://www.sharelatex.com/templates)

[Beamer theme gallery](http://deic.uab.es/~iblanes/beamer_gallery/index.html)

# titlepage
```latex
\title[short title]{long title}
\subtitle[short subtitle]{long subtitle}
\author[short name]{long name}
\date[short date]{long date}
\institute[short name]{long name}
```
	
beamer
```
\begin{frame}[plain]
\titlepage
\end{frame}
```
其中的plain 选项表示不显示顶栏侧栏底栏等外部元素.

# author
如果需要在author 中使用tabular 以产生对齐效果, 那么需要把`\author` 命令放在`\begin{document}`之后, 否则 tabular 命令不认.  
如果使用`\quad` 等空白符, 还是需要加上`$$`符号.

# logo
Beamer provides `\logo{<logo code>}` to set the logo(s)
```latex
\logo{%
	\includegraphics[width=1cm,height=1cm,keepaspectratio]{logo1}~%
	\includegraphics[width=1cm,height=1cm,keepaspectratio]{logo2}~%
}
```

[logo in the first page only](http://tex.stackexchange.com/questions/61051/logo-in-the-first-page-only)
```
\titlegraphic{%
	\includegraphics[width=2cm]{logopolito}\hspace*{4.75cm}~%
	\includegraphics[width=2cm]{logopolito}
}
```

# 页眉页脚
`\pagestyle{option}`: command changes the style from the current page on throughout the remainder of your document

- plain - Just a plain page number.
- empty - Produces empty heads and feet - no page numbers.

`\thispagestyle{empty}` 只影响当前页, 只是不显示, 但是仍然占用页码数

`\pagenumbering{option}`, 页码的显示方式
- arabic, 阿拉伯数字
- roman, 小写罗马数字
- Roman,大写罗马数字
- alpha, 小写拉丁字母
- Alpha, 大写拉丁字母

`\setcounter{element}{number}`: 对element元素重新开始以number编号, eg: `\setcounter{page}{1}`

**实现titlepage 不用页码, 目录页用罗马数字页码, 正文用阿拉伯数字作为页码**
```
%% 设置标题
\thispagestyle{empty}
\maketitle
\newpage

%% 设置目录
\pagenumbering{Roman}
\setcounter{page}{1}
\tableofcontents
\newpage

%% 设置目录
\pagenumbering{arabic}
\setcounter{page}{1}
```

# Table of contents
- `\tableofcontents`: 生成目录.
	如果不想让某个章节标题出现在目录中,可以使用以下带* 的命令来声明章节, eg: `\chapter*{...}, \section*{...}, \subsection*{...}`
- `\listoffigures, \listoftables`: 插图和表格的目录 

- `\setcounter{tocdepth}{2}`: 目录只显示两级
- `\renewcommand{\thesection}{\Alph{section}}`: 将(section)的序号改成大写字母的型式, 如图 ![Alph Table of Contents](http://i.imgbox.com/VPal8xXr.jpg)
- `renewcommand{\contentsname}{目录}`: 更换掉默认的显示内容

`\addcontentsline{file}{sec_unit}{entry}`: 手动将entry 添加到特定的目录中

- file: is the extension of the file on which information is to be written:
	- toc - table of contents
	- lof - list of figures
	- lot - list of tables
- sec_unit: controls the formatting of the entry. It should be one of the following, depending upon the value of the file argument:
	- part, chapter, section, subsection, subsubsection
	- lof - figure
	- lot - table
- entry: the text of the entry.

eg:
```
\chapter*{结论与展望}\addcontentsline{toc}{chapter}{结论与展望}
\section*{结论}\addcontentsline{toc}{section}{结论}
```
**\addcontentsline{}{}{} 生成PDF 文件后链接不对**  
参看CTeX-faq 74 摘录如下:  
这是因为\addcontentsline 命令后面的命令换页造成的页码不一致.需要在\addcontentsline 命令之前加上 `\clearpage`.  
如果是book 类或者用了openright 参数的report 类文档,新的章总是从奇数页开始,则要把命令改成 `\cleardoublepage`.  
如果使用了hyperref 宏包来生成PDF 文件的书签链接,则还需要在上面的命令后再加上 `\phantomsection`


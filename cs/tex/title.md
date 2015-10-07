# Title Page
By default, the `\frametitle{title}` command creates a title page that includes:
Title, Author, His or Her Affiliation, The Date, Title Graphic

	\title[short title]{long title}
	\subtitle[short subtitle]{long subtitle}
	\author[short name]{long name}
	\date[short date]{long date}
	\institute[short name]{long name}
	
	\institute[某甲和某乙大学]
	{
	\inst{1}某甲大学数学系
	\inst{2}某乙大学计算机科学系
	}
	
	\begin{frame}[plain]
	\titlepage
	\end{frame}
	其中的plain 选项表示不显示顶栏侧栏底栏等外部元素.

也可以把信息都放在title中,在title中使用换行

	\title{From theory to engineering: \\
	 numerical simulation of
	turbulent flows in compressors}
	\author[L. Fang \textit{et al.}]{\underline{Le Fang}, Feng Gao, Liang Shao\\
	\scriptsize \textit{LMFA (Laboratoire de M茅caniques Fluides et d'Acoustique),} \\
	 \textit{Ecole Centrale de Lyon} \\
	~\\ %这个地方需要加一个空格,如果不加,就会报错说there is no line here to end
	\normalsize Yang-Wei Liu, Li-Peng Lu\\
	\scriptsize \textit{SJP (School of Jet Propulsion),} \\
	 \textit{Beihang University}}
	\date{Jan. 2012}
![\title](http://i.imgbox.com/AYtCSjXh.jpg)

## author
如果需要在author 中使用tabular 以产生对齐效果, 那么需要把`\author` 命令放在`\begin{document}`之后, 否则 tabular 命令不认.  
如果使用`\quad` 等空白符, 还是需要加上`$$`符号.

# logo
Beamer provides `\logo{<logo code>}` to set the logo(s)
```
\logo{%
    \includegraphics[width=1cm,height=1cm,keepaspectratio]{logo1}~%
    \includegraphics[width=1cm,height=1cm,keepaspectratio]{logo2}~%
    \includegraphics[width=1cm,height=1cm,keepaspectratio]{logo3}%
}
```

[logo in the first page only](http://tex.stackexchange.com/questions/61051/logo-in-the-first-page-only)
```
% logo of my university
\titlegraphic{\includegraphics[width=2cm]{logopolito}\hspace*{4.75cm}~%
   \includegraphics[width=2cm]{logopolito}
}
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

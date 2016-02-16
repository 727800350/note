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
```Latex
\chapter*{结论与展望}\addcontentsline{toc}{chapter}{结论与展望}
\section*{结论}\addcontentsline{toc}{section}{结论}
```
**\addcontentsline{}{}{} 生成PDF 文件后链接不对**  
参看CTeX-faq 74 摘录如下:  
这是因为\addcontentsline 命令后面的命令换页造成的页码不一致.需要在\addcontentsline 命令之前加上 `\clearpage`.  
如果是book 类或者用了openright 参数的report 类文档,新的章总是从奇数页开始,则要把命令改成 `\cleardoublepage`.  
如果使用了hyperref 宏包来生成PDF 文件的书签链接,则还需要在上面的命令后再加上 `\phantomsection`

## beamer
类似于标题页面，我们可以在幻灯片中用\tableofcontents 命令生成目录页。
```Latex
\begin{frame}
\tableofcontents[hideallsubsections]
\end{frame}
其中hideallsubsections 选项表示不显示小节标题。

\tableofcontents[pausesections,pausesubsections]
\tableofcontents[pausesections]
\tableofcontents[pausesubsections]

\begin{frame}
\frametitle{Outline}
\tableofcontents[part=1,pausesections]
\end{frame}
```


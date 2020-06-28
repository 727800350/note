标签相关的引用

- `\label{}`: 设置标签
- `\ref{}`: 引用标签
- `\pageref{}`: 引用标签所在页的页码
- `\eqref{}`: 引用equation , 相当于 (\ref{}), `\lasteq`(自定义的)

参考文献
```latex
\bibliographystyle{plain}
\bibliography{mybib}
```
其中`mybib.bib` 是参考文献的数据库

使用

- `\cite{item1, item2}` %一个地方同时引用item1和item2
- `\cite{item}`

编译步骤

1. xelatex paper.tex ## 只把条目的关键字写到中间文件`.aux` 中去.
1. bibtex paper.aux ## 根据`.aux,.bib,.bst` 生成一个`.bbl` 文件,即参考文献列表
1. xelatex paper.tex ## 把交叉引用写到`.aux` 中去.
1. xelatex paper.tex ## 在正文中正确地显示引用.

引用的目录问题
```latex
\clearpage
\phantomsection
\addcontentsline{toc}{section}{References}
\bibliographystyle{plain}
\bibliography{ref}
```
- \clearpage 是另起一页
- \phantomsection 是处理目录中的References链接不准确的问题

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
```latex
\subsection[the title of chapter(section et .)]
{the title of chapter(section et .)\protect\upcite{keywords}}
```
即可是在章节标题中引用参考文献.

index

1. `\usepackage{makeidx}`
1. 在导言区使用`\makeindex`激活导言
1. 索引的内容通过`\index{key}`

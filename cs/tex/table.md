[Floats,_Figures_and_Captions](http://en.wikibooks.org/wiki/LaTeX/Floats,_Figures_and_Captions)

如果想让标题放在表格的上方就把caption加在\begin{table}和\begin{tabular}之间,
如果想把标题放在表格下方,就把caption放在\end{table}和\end{tabular}之间.

```latex
\begin{table}[h]\small
\begin{tabular}
...
\end{tabular}
\end{table}
```
`\small` 也可以用在其他地方, 尤其是beamer 中itemzie 等环境中

Subtable
```latex
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
```

线条粗细

如果完美主义者可能觉得上面示例中的三条线一样粗不够美观,这时可以使用booktabs 宏包的几个命令.
```latex
\usepackage{booktabs}
\toprule
\midrule
\bottomrule
```

表格的宽度

表格总宽度为\linewidth,共3列,宽度均相同.每列宽度为\linewidth/3 是自动计算出来的.
X 参数表示某栏可以折行, 当这个cell 内容比较长时比较实用.
```latex
\begin{table}
\begin{tabularx}{\linewidth}{|X|X|X|}
\begin{tabularx}
\end{table}
```
则表格的总宽度是行宽,第1列列宽为3cm,其他两列的列宽自动计算.
```latex
\begin{table}
\begin{tabularx}{\linewidth}{|p{3cm}|X|X|}
\begin{tabularx}
\end{table}
```

跨行跨列

- \multirow{2}{*}{Multi-Row}: 第一个参数2表示跨兩列, 第二个参数*表示系统自动调整文字
- \multicolumn{2}{c}{显示内容}: 它的前两个参数指定横跨列数和对齐方式
- \cline{2-3}: 表示画出一条在第二栏位到第三栏位的横线短, 其他位置不画

```latex
\begin{tabular}{|c|c|c|c|c|}
\hline
\multirow{2}{*}{Multi-Row} & \multicolumn{2}{c|}{Multi-Column} & \multicolumn{2}{c|}{\multirow{2}{*}{Multi-Row and Col}} \\
\cline{2-3} & column-1 & column-2 & \multicolumn{2}{c|}{} \\ \hline
label-1 & label-2 & label-3 & label-4 & label-5 \\ \hline
\end{tabular}
```
[result]()

斜线

`\diagbox{类别}{数量}`: 还可以多于2个格

使用颜色

- ` \cellcolor[gray]{.8} Unix/Linux`: cell 级别
- `\rowcolor[gray]{.8} Unix/Linux & TeX Live & Emacs \\`: row 级别
- `\begin{tabular}{|l|>{\columncolor[gray]{0.8}[0pt]}c|r|}`: col 级别

颜色可以用color宏包定义一些颜色常量,例如
```latex
\definecolor{Gray}{gray}{0.9}
\definecolor{LightCyan}{rgb}{0.88,1,1}
```
为了方便使用columncolor,我们可以define a new colored column type
```latex
\newcolumntype{g}{>{\columncolor{Gray}}c}
\begin{tabular}{c|g|c|g|c|g|c|g}
```

longtable
```latex
%% \usepackage{longtable}
\begin{longtable}{|l|l|l|}
% 表格的首个表头
\caption{长表格示例\label{tab-long-example}} \\
\hline
\multicolumn{1}{|c|}{\textbf{Time (s)}} &
\multicolumn{1}{c|}{\textbf{Triple chosen}} &
\multicolumn{1}{c|}{\textbf{Other feasible triples}} \\ \hline
\endfirsthead
% 表格的其他表头
\multicolumn{3}{l}{{\bfseries\tablename\ \thetable{} --接\,上\,页}} \\
\hline
\multicolumn{1}{|c|}{\textbf{Time (s)}} &
\multicolumn{1}{c|}{\textbf{Triple chosen}} &
\multicolumn{1}{c|}{\textbf{Other feasible triples}} \\ \hline
\endhead
% 表格的其他表尾
\hline
\multicolumn{3}{|r|}{{接\,下\,页}} \\ \hline
\endfoot
% 表格的最后表尾
\hline \hline
\endlastfoot
0      & (1, 11, 13725) & (1, 12, 10980), (1, 13, 8235), (2, 2, 0) \\
2745   & (1, 12, 10980) & (1, 13,  8235), (2,  2,    0), (2, 3, 0) \\
5490   & (1, 12, 13725) & (2,  2,  2745), (2,  3,    0), (3, 1, 0) \\
8235   & (1, 12, 16470) & (1, 13, 13725), (2,  2, 2745), (2, 3, 0) \\
10980  & (1, 12, 16470) & (1, 13, 13725), (2,  2, 2745), (2, 3, 0) \\
13725  & (1, 12, 16470) & (1, 13, 13725), (2,  2, 2745), (2, 3, 0) \\
16470  & (1, 13, 16470) & (2,  2,  2745), (2,  3,    0), (3, 1, 0) \\
19215  & (1, 12, 16470) & (1, 13, 13725), (2,  2, 2745), (2, 3, 0) \\
21960  & (1, 12, 16470) & (1, 13, 13725), (2,  2, 2745), (2, 3, 0) \\
24705  & (1, 12, 16470) & (1, 13, 13725), (2,  2, 2745), (2, 3, 0) \\
27450  & (1, 12, 16470) & (1, 13, 13725), (2,  2, 2745), (2, 3, 0) \\
\end{longtable}
```

图表混排
```
\makeatletter\def\@captype{figure}\makeatother
\begin{minipage}{.45\textwidth}
\centering
\rule{3cm}{2cm}
\caption{The figure caption}
\end{minipage}
\makeatletter\def\@captype{table}\makeatother
\begin{minipage}{.45\textwidth}
\centering
\begin{tabular}{|c|c|c|c|c|}
  \hline
  % after \\: \hline or \cline{col1-col2} \cline{col3-col4} ...
  a & a & a & a & a \\
  a & a & a & a & a \\
  a & a & a & a & a \\
  a & a & a & a & a \\
  a & a & a & a & a \\
  \hline
\end{tabular}
\caption{The table title}
\end{minipage}
```
[达到的效果](http://photo.blog.sina.com.cn/list/blogpic.php?pid=5e16f1774ae896b1696df&bid=5e16f1770102dxz2&uid=1578561911)


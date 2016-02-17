[Floats,_Figures_and_Captions](http://en.wikibooks.org/wiki/LaTeX/Floats,_Figures_and_Captions)

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

## 图表混排
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


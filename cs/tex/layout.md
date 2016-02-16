手动调整间距

- `\vspace*{-0.7cm}`: 竖直距离
- `\hspace*{0.7cm}`: 水平距离

block
```Latex
\begin{block}{text}
\end{block}
```
text可以为空, 也可以使用`\small{text}` 等
定制区块环境例如,下面的代码修改了区块环境的样式: `\setbeamertemplate{blocks}[rounded]`

区块环境用圆角带阴影的矩形来表示`[shadow=true]`
```Latex
\setbeamercolor{blocktitle}{fg=yellow,bg=gray!50!black}
\setbeamercolor{blockbody}{bg=gray}
```

与区块环境类似地还有一个提醒环境(红色)和例子环境
```Latex
\begin{alertblock}{重要提醒}
2012年12月21日是世界末日.
\end{alertblock}

\begin{exampleblock}{重要例子}
2012年12月21日是世界末日.
\end{exampleblock}
```

columns 分栏
```Latex
\begin{columns}
\column{.4\textwidth}
\column{.3\textwidth}
\column{.3\textwidth}
\end{columns}
```

frame的布局
```Latex
\begin{frame}[<alignment>]
\end{frame}
```
The alignment options

- [c]: default
- [t]: top align
- [b]: bottom align
- [plain]: headlines, footlines, and sidebars to be suppressed, useful for showing large pictures.
- [fragile]: 可以对代码, list 自动分页


摘录

- `\begin{quote} \end{quote}`: 引文两端都缩进
- `\begin{quotation} \end{quotation}`: 引文两端都缩进且引文的首行继续缩进
- `\begin{verse} \end{verse}`: 引文两端都缩进, 且引文的第二行继续缩进

对齐方式

- flushleft
- flushright
- center


手动调整间距

- `\vspace*{-0.7cm}`: 竖直距离
- `\hspace*{0.7cm}`: 水平距离

block
```Latex
\begin{block}{text}
\end{block}
```
text可以为空, 也可以使用\small{text} 等

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


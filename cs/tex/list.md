# Beamer
If you want each item of a list to appear in order, use the `[<+->]` option. (i.e. `\begin{itemize}[<+->]` )
```Latex
\begin{enumerate}
\item<1-| alert@1> Suppose $p$ were the largest prime number.
\item<2-> Let $q$ be the product of the first $p$ numbers.
\item<3-> Then $q+1$ is not divisible by any of them.
\end{enumerate}
```
- `| alert@1`: 表示只有在第一页是alert
- `| alert@1,3`: 表示只有在第1和3两页是alert
- `| alert@1-3`: 表示从第1到第3页都是alert

setbeamertemplate
```Latex
\setbeamertemplate{itemize items}{\color{red}$\bullet$} 
\setbeamertemplate{itemize item}{$\infty$}
\setbeamertemplate{itemize subitem}{$\bullet$}
\setbeamertemplate{itemize subsubitem}{$\diamond$}
```

改变序号

```Latex
\begin{enumerate}[step 1]
\item good morning...
\item good morning....
\end{enumerate}
```
会生成 step 1..., step 2 etc, 类似的, 也可以采用 `[step i]`

## FAQ 溢出
如果一页溢出,考虑以下解决方法:

1. 减小字体,变成/small, /tiny字体 
2. 幻灯片写成plain格式: `\begin{frame}[plain]`
3.  由beamer 自动分页, `\begin{frame}[allowframebreaks]`\\
有时这种方法给出的自动分页不是很好, 好不如自己手动分页, 这样可以控制在那里断开

# article
## 自定义list style
`labelindent + labelwidth + labelsep  = itemindent`

![enumerate demo](http://imgbox.com/4YJKY9Gl)
"左边界"到"1"的距离为labelindent, "1." 占用的宽度为 labelwidth, "1.0" 与 "网络性能"之间的距离为 labelsep
"左边界"到"网络性能"的距离为 itemindent

下面的设置可以达到这样的效果
```Latex
\setlist[enumerate,1]{label=\arabic*.} %% label style
\setlist[enumerate]{
	topsep=0pt,
	itemsep=0pt,
	partopsep=0pt,
	parsep=\parskip,
	labelindent=\parindent, %% \parindent is usually 2em
	labelwidth=.7em,
	labelsep=.3em,
	itemindent=3em,
	leftmargin=!,
}
```
- `leftmargin=!`: automatically calculate what the leftmargin should be based on the provided parameters labelindent, labelwidth, labelsep etc


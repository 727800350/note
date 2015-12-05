# Beamer
If you want each item of a list to appear in order, use the `[<+->]` option. (i.e. `\begin{itemize}[<+->]` )
```Latex
\begin{enumerate}
\item<1-| alert@1> Suppose $p$ were the largest prime number.
\item<2-> Let $q$ be the product of the first $p$ numbers.
\item<3-> Then $q+1$ is not divisible by any of them.
\item<1-> But $q + 1$ is greater than $1$, thus divisible by some prime number not in the first $p$ numbers.\qedhere
\end{enumerate}
```

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

## FAQ 溢出
如果一页溢出,考虑以下解决方法:

1. 减小字体,变成/small, /tiny字体 
2. 幻灯片写成plain格式: `\begin{frame}[plain]`
3.  由beamer 自动分页, `\begin{frame}[allowframebreaks]`\\
有时这种方法给出的自动分页不是很好, 好不如自己手动分页, 这样可以控制在那里断开


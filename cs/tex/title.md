[LaTeX Templates](http://www.latextemplates.com/)

[Sharelatex](https://www.sharelatex.com/templates)

[Beamer theme gallery](http://deic.uab.es/~iblanes/beamer_gallery/index.html)

先是设置内容
```latex
\title[short title]{long title}
\subtitle[short subtitle]{long subtitle}
\author[short name]{long name}
\date[short date]{long date}
\institute[short name]{long name}
```
然后生成标题页

- \titlepage: 对于普通的article, book 等类型
- \begin{frame}[plain] \titlepage \end{frame}: 对于beamer类型
	
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

插入空白页
```Latex
\newpage
\thispagestyle{empty}
\mbox{}
\newpage
```


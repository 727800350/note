- `\baselineskip` 正文中的行间距, 但是要改变行间距的时候, 一般不直接修改`\baselineskip`, 而是修改 `\baselinestretch` 的值
- `\baselinestretch` 是对 `\baselineskip` 进行缩放的, 默认是1. 因此, 设置2倍行间距的正确操作为: `\renewcommand{\baselinestretch}{2}`

- `parskip`: 段落前后间距, `\setlength{\parskip}{.5\baselineskip}`, 

页边距
http://www.52souji.net/set-the-margin-header-footer-of-latex/
 
 
# floats
[ref](http://techshangrila.blogspot.com/2014/01/reduce-space-around-floats-algorithm.html)

- \captionsetup{belowskip=12pt,aboveskip=4pt}
	- aboveskip: 指caption 与 图标之间的距离.
	- belowskip: 指caption 与 正文之间的距离.
		注意, table 的caption 是在table的上面, 所以对于table来说, belowskip就是指上面的正文与caption的距离, figure 是caption 与下面的正文之间的距离.

- \floatsep: distance between two floats, 浮动体之间的距离
	- `\setlength{\floatsep}{1.25\baselineskip plus 3pt minus 2pt}`
- \intextsep:— for floats in the middle of text only - length between text above it, and text below it, 浮动体和正文间的距离.
	- `\setlength\{intextsep}{1.25\baselineskip plus 3pt minus 2 pt}`
- \textfloatsep: for floats on top and bottom of text only,
	For floats at top - length between float and text below it,
	For floats at bottom - length between float and text above it.
	- \setlength\{textfloatsep}{1.25\baselineskip plus 3pt minus 2pt}
 

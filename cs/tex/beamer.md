beamer 自己已经加载了 hyperref 这个package, 所以没有必要再次load 一遍

[beamer_gallery](http://deic.uab.es/~iblanes/beamer_gallery/)

# 对齐
A frame can be assigned a left, center, or right alignment with the
`flushleft`, `center`, and `flushright` environments.

	\begin{center}
	The center-aligned text goes here.
	\end{center}

# Block
	\begin{block}{重要内容}
	2012年12月21日是世界末日.
	\end{block}
![block](http://i.imgbox.com/e7k6k84Z.jpg)

定制区块环境例如,下面的代码修改了区块环境的样式:  
`\setbeamertemplate{blocks}[rounded]`

区块环境用圆角带阴影的矩形来表示`[shadow=true]`

`\setbeamercolor{blocktitle}{fg=yellow,bg=gray!50!black}\setbeamercolor{blockbody}{bg=gray}`

与区块环境类似地还有一个提醒环境(红色)和例子环境.先看看提醒环境.  
例如下面的代码

	\begin{alertblock}{重要提醒}
	2012年12月21日是世界末日.
	\end{alertblock}

	\begin{exampleblock}{重要例子}
	2012年12月21日是世界末日.
	\end{exampleblock}

# Note
note 的显示方式

	\setbeameroption{show notes}
	\setbeameroption{hide notes}
	\setbeameroption{show only notes}
	\setbeameroption{show notes on second screen = left} right, top, bottom

 插入note

	\note{text}
	
	\note[item]<2>{tell joke about plants}
	\note[item]<2>{make it short}

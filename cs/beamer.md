beamer 自己已经加载了 hyperref 这个package, 所以没有必要再次load 一遍

[beamer_gallery](http://deic.uab.es/~iblanes/beamer_gallery/)

Beamer载入了`xcolor`宏包  
预定义的颜色  
![预定义的颜色](http://i.imgbox.com/6zjHzTM3.jpg)
自定义颜色:  
`\xdefinecolor{darkgreen}{rgb}{0,0.35,0}`: 深绿色
使用颜色:  
`\color{...}`  
`\textcolor{...}{text}`

# Title Page
By default, the `\frametitle{title}` command creates a title page that includes:
Title, Author, His or Her Affiliation, The Date, Title Graphic

	\title[short title]{long title}
	\subtitle[short subtitle]{long subtitle}
	\author[short name]{long name}
	\date[short date]{long date}
	\institute[short name]{long name}
	
	\institute[某甲和某乙大学]
	{
	\inst{1}某甲大学数学系
	\inst{2}某乙大学计算机科学系
	}
	
	\begin{frame}[plain]
	\titlepage
	\end{frame}
	其中的plain 选项表示不显示顶栏侧栏底栏等外部元素.

也可以把信息都放在title中,在title中使用换行

	\title{From theory to engineering: \\
	 numerical simulation of
	turbulent flows in compressors}
	\author[L. Fang \textit{et al.}]{\underline{Le Fang}, Feng Gao, Liang Shao\\
	\scriptsize \textit{LMFA (Laboratoire de M茅caniques Fluides et d'Acoustique),} \\
	 \textit{Ecole Centrale de Lyon} \\
	~\\ %这个地方需要加一个空格,如果不加,就会报错说there is no line here to end
	\normalsize Yang-Wei Liu, Li-Peng Lu\\
	\scriptsize \textit{SJP (School of Jet Propulsion),} \\
	 \textit{Beihang University}}
	\date{Jan. 2012}
![\title](http://i.imgbox.com/AYtCSjXh.jpg)

## author
如果需要在author 中使用tabular 以产生对齐效果, 那么需要把`\author` 命令放在`\begin{document}`之后, 否则 tabular 命令不认.  
如果使用`\quad` 等空白符, 还是需要加上`$$`符号.

# logo
Beamer provides `\logo{<logo code>}` to set the logo(s)
```
\logo{%
    \includegraphics[width=1cm,height=1cm,keepaspectratio]{logo1}~%
    \includegraphics[width=1cm,height=1cm,keepaspectratio]{logo2}~%
    \includegraphics[width=1cm,height=1cm,keepaspectratio]{logo3}%
}
```

[logo in the first page only](http://tex.stackexchange.com/questions/61051/logo-in-the-first-page-only)
```
% logo of my university
\titlegraphic{\includegraphics[width=2cm]{logopolito}\hspace*{4.75cm}~%
   \includegraphics[width=2cm]{logopolito}
}
```

# List
If you want each item of a list to appear in order, use the `[<+->]` option. (i.e. `\begin{itemize}[<+->]` )

	\begin{enumerate}
	\item<1-| alert@1> Suppose $p$ were the largest prime number.
	\item<2-> Let $q$ be the product of the first $p$ numbers.
	\item<3-> Then $q+1$ is not divisible by any of them.
	\item<1-> But $q + 1$ is greater than $1$, thus divisible by some prime
	number not in the first $p$ numbers.\qedhere
	\end{enumerate}

## setbeamertemplate

	\setbeamertemplate{itemize items}[样式名]
	\setbeamertemplate{enumerate items}[样式名]

也可以更加详细的自定义

itemize

	\setbeamertemplate{itemize items}{\color{red}$\bullet$} 
	\setbeamertemplate{itemize item}{$\infty$}
	\setbeamertemplate{itemize subitem}{$\bullet$}
	\setbeamertemplate{itemize subsubitem}{$\diamond$}

enumerate

	\setbeamertemplate{enumerate item}{\insertenumlabel.}
	\setbeamertemplate{enumerate subitem}{\insertenumlabel.\insertsubenumlabel}
	\setbeamertemplate{enumerate subsubitem}{\insertenumlabel.\insertsubenumlabel.\insertsubsubenumlabel}
	\setbeamertemplate{enumerate mini template}{\insertenumlabel}

## 改变序号

	\begin{enumerate}[step 1]
	\item good morning...
	\item good morning....
	\end{enumerate}
生成

	step 1 good morning
	step 2 good morning

如果我们在中括号里输入`[step i]`, 那么生成的列表是

	step i good morning
	step ii good morning

## 溢出
如果一页溢出,考虑以下解决方法:

1. 减小字体,变成/small, /tiny字体 
2. 幻灯片写成plain格式: `\begin{frame}[plain]`
3.  由beamer 自动分页, `\begin{frame}[allowframebreaks]`\\
有时这种方法给出的自动分页不是很好, 好不如自己手动分页, 这样可以控制在那里断开

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

Beamer 中也定义了各种定理环境,而且默认是用区块环境的样式来显示的.  
例如下面的代码
	
	\begin{theorem}
	微积分基本公式:$\int_a^b f(x)\mathrm{d}x=F(b)-F(a)$.
	\end{theorem}

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

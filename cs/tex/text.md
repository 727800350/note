# 字体
大小

- `\fontsize{字号}{行距}`: 这个命令对其后所有文本都起作用, 在使用此命令后需要用 `\selectfont` 才能使字体大小设置起作用.
	`\fontsize{12pt}{18pt}\selectfont`: 就设置了小四 1.5 倍行距.
- Latex 设置字体大小命令由小到大依次为: `\tiny, \scriptsize, \footnotesize, \small, \normalsize, \large, \Large, \LARGE, \huge, \Huge`
- 对于beamer, 可以通过使用 package scrextend, 来设置任意大小的字
```latex
\usepackage{scrextend}
\changefontsizes{6pt}
```

颜色

使用`\definecolor{name}{system}{definition}`来定义颜色
```latex
\usepackage{color}
\definecolor{Gray}{gray}{0.9}
\definecolor{LightCyan}{rgb}{0.88,1,1}
\definecolor{dkgreen}{rgb}{0,0.6,0}
\definecolor{gray}{rgb}{0.5,0.5,0.5}
\definecolor{mauve}{rgb}{0.58,0,0.82}
```

## beamer text
字体

- \alert{text}: 红色突出
- \emph{text}: 强调, 但是区别度不是很大
- \textbf{text}: 黑体
- \textit{text}: 斜体
- \color{green} text: 加颜色

对于beamer, 还可以控制在哪些页码上显示特定的样式, 例如下面的.

- \alert{Alert on all slides}
- \alert<3>{Alert on slide 3}
- \alert<1,3>{Alert on slides 1 and 3}
- \alert<-2,4>{Alert on slides 1,2 and 4}

除了alert, emph, textbf等也有同样的功能

text border example(\usepackage{fancybox}):

- shadowbox{text}
- fbox{text}
- doublebox{text}
- ovalbox{text}
- Ovalbox{text}

## win字体
需要把字体安装到系统的字体库中(比如使用tex编辑文档时,通常会需要使用宋体或楷体),其实就是把字体拷贝到系统存放字体文件的地方,然后运行一下更新字体缓存之类的命令,通常如下:

1. $ sudo mv win_fonts /usr/share/fonts/truetype/
1. $ sudo mkfontscale
1. $ sudo mkfontdir
1. $ sudo fc-cache

这里假设在win_fonts目录中存放着从windows拷贝过来的ttf字体文件.
如果不出意外的话,因该就可以使用这些新安装的字体文件了,比如在主题中改改系统字体设置之类的.

# Special Characters
## [french characters](http://en.wikibooks.org/wiki/LaTeX/Special_Characters#Escaped codes)
LaTeX command	Sample	Description
```
\`{o}	grave accent
\''{o}	(single)acute accent
\^{o}	circumflex
\""{o}	(single)umlaut, trema or dieresis
\H{o}	long Hungarian umlaut (double acute)
\~{o}	tilde
\c{c}	cedilla
\k{a}	ogonek
\l		barred l (l with stroke)
\={o}	macron accent (a bar over the letter)
\b{o}	bar under the letter
\.{o}	dot over the letter
\d{u}	dot under the letter
\r{a}	ring over the letter (for ? there is also the special command \aa)
\u{o}	breve over the letter
\v{s}	caron/hacek ("v") over the letter
\t{oo}	"tie" (inverted u) over the two letters
\o		slashed o (o with stroke)
```
#French Ordinal Numbers
After having included `\usepackage[greek,francais]{babel}`, use
```
– 1\ier{}, 
– 1\iere{} or 
– 1\ieres{}
```
For other ordinal numbers, use
```
– \ieme{} or
– \iemes{}
```


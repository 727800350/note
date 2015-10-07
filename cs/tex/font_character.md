# 字体大小
`\fontsize{字号}{行距}`  
这个命令对其后所有文本都起作用,在使用此命令后需要用 `\selectfont` 才能使字体大小设置起作用.

我们通常会遇到别人规定:"正文用小四,宋体",但是 LaTeX 并没有小四,只有 pt,因此下表为字号对应的转换表:
比如:`\fontsize{12pt}{18pt}\selectfont` 就设置了 小四,且 1.5 倍行距.

| ?字号                       | 初号   | 小初   | 一号   | 小一   | 二号   | 小二   | 三号   | 小三     | 四号   | 小四   | 五号      | 小五   | 六号    | 小六     | 七号     | 小七    |
|---------------------------|------|------|------|------|------|------|------|--------|------|------|---------|------|-------|--------|--------|-------|
| 大小                        | 42pt | 36pt | 26pt | 24pt | 22pt | 18pt | 16pt | 15pt   | 14pt | 12pt | 10.5pt  | 9pt  | 7.5pt | 6.5pt  | 5.5pt  | 5pt   |
| 1.5行距时的 \baselineskip 设置值 | 63pt | 54pt | 39pt | 36pt | 33pt | 27pt | 24pt | 22.5pt | 21pt | 18pt | 15.75pt | 13.5 | 11.25 | 9.75pt | 8.25pt | 7.5pt |

Latex 设置字体大小命令由小到大依次为:
```
\tiny, \scriptsize, \footnotesize, \small, \normalsize, \large, \Large, \LARGE, \huge, \Huge
```

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


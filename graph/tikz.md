```
\usepackage{tikz} %pdf figure
\usetikzlibrary{positioning}
\def\pgfsysdriver{pgfsys-dvipdfmx.def}
\pgfsetxvec{\pgfpoint{10pt}{0}}
\pgfsetyvec{\pgfpoint{0}{10pt}}
```

一个简单的绘图实例
```
\tikz \draw (0,0)--(4,0)--(2,3)--(0,0); %绘图命令
```
或者
```
\begin{tikzpicture}
\draw (0,0)--(4,0)--(2,3)--(0,0);
\draw (5,0)--(9,0)--(7,3)--cycle; %绘图命令
\end{tikzpicture}
```

TikZ 中直线的语法和METAPOST 类似,加cycle 参数才能构成真正的封闭路径.

矩形命令如下,它的两个参数是矩形的两个对角顶点.
```
\draw (0,0) rectangle (4,2);
```

圆,椭圆,弧
```
\draw (1,1) circle (1);
\draw (5,1) ellipse (2 and 1);
```
圆弧和椭圆弧命令如下,
圆弧的参数是起始点,起始角度,终止角度,半径,
椭圆弧则把半径换成了长径和短径.
```
\draw (2,1) arc (0:270:1);
\draw (7,1) arc (0:270:2 and 1);
```

曲线命令如下,中间的参数是控制点.
```
\draw (0,0) .. controls (2,2) and (4,2) .. (4,0);
```

抛物线命令如下,除了起止点还可以指定顶点.
```
\draw (-1,1) parabola bend (0,0) (1.414,2);
```

线形和箭头
```
\draw[line width=2pt] (0,0)--(9,0);
\draw[->] (0,1)--(9,1);
\draw[<-] (0,2)--(9,2);
\draw[<->] (0,3)--(9,3);
```

颜色,填充,阴影
颜色参数的用法如下.PGF 可以使用xcolor 宏包中定义的所有颜色.
```
\draw[red] (0,4)--(9,4);
\draw[green] (0,2)--(9,2);
\draw[blue] (0,0)--(9,0);
```
封闭路径可以用颜色填充,\filldraw 命令可以分别指定边框色和填充色.
```
\fill[red] (1,1) circle (1);
\filldraw[fill=lightgray,draw=black] (4,1) circle (1);  %fill填充色, draw边框色
```

\shade 命令可以产生渐变和光影效果,缺省是从上到下,灰色渐变为白色.我们也可以使用其它方向和颜色的渐变.
```
\shade (0,0) rectangle (2,2);
\shade[left color=red,right color=orange] (3,0) rectangle (5,2);
\shade[inner color=red,outer color=orange] (6,0) rectangle (8,2);
\shade[ball color=blue] (10,1) circle (1);
```

图形变换
对图形对象可以进行平移和旋转操作,注意如果两种操作同时进行,
它们是有顺序的.注意预定义的长度单位在这里对平移参数失效.
```
\draw (0,0) rectangle (2,2);
\draw[xshift=30pt] (0,0) rectangle (2,2);
\draw[xshift=75pt,rotate=45] (0,0) rectangle (2,2);
```

样式
PGF比METAPOST 和PSTricks 多了一个有趣的概念:样式(style),
它的思路和HTML 的CSS 相近.我们可以先定义两种样式,
```
\tikzset{
myline/.style={line width=2pt},
myblueline/.style={myline,blue}
}
```
然后使用样式
```
\draw[myline] (0,2)--(9,2);
\draw[myblueline] (0,0)--(9,0);
```

除了用\tikzset 命令定义样式,我们也可以在tikzpicture 环境头部声明样式.前者是全局性的,后者则是局部性的.
```
\begin{tikzpicture}[
thickline/.style=2pt,
bluethickline/.style={thickline,color=blue}
]
...
\end{tikzpicture}
```
注意在样式中预定义长度单位有时会失效,所以最好使用绝对单位.

流程图
PGF 中的节点(node)可以是简单的标签,也可以有各种形状的边框,还可以有各种复杂的属性.
比如下例中的节点样式:box,它的边框是矩形,有圆角,它有最小宽度,高度,文字和边框的距离,边框和填充颜色等属性.
样式可以放到\begin{tikzpicture}环境的外面

```
\tikzset{
box/.style={rectangle, rounded corners=6pt,
minimum width=50pt, minimum height=20pt, inner sep=6pt,
draw=gray,thick, fill=lightgray}
}
\node[box] (tex) at(0,0) {.tex}; %文本框
\node[box] (dvi) at(10,0) {.dvi}; %文本框
\node[box] (pdf) at(20,0) {.pdf}; %文本框
\draw[->] (tex)--(dvi); %箭头
\draw[->] (dvi)--(pdf); %箭头
\node at (5,1) {latex}; %标签
\node at (15,1) {dvipdfmx}; %标签
```
![result figure](http://i.imgbox.com/Jyjfkpg4.jpg)

在上例中的节点都使用了绝对位置,PGF 中还可以使用更灵活一点的相对位置
```
\node[box] (tex) {.tex};
\node[box,right=5em of tex] (dvi) {.dvi};
\node[box,right=8em of dvi] (pdf) {.pdf};
\path
(tex) edge[->] node[above=.5] {latex} (dvi)
(dvi) edge[->] node[above=.5] {dvipdfmx} (pdf);
```

note:
```
\path
(tex) edge[->] node[above=.5] {latex} (dvi)
(dvi) edge[->] node[above=.5] {dvipdfmx} (pdf); 这是一个命令, 在最后需要一个分号结束
(tex) edge[->] (dvi) % 如果不需要在-> 上写字
```
有above, 还有below

树
下面是一棵简单的树.我们可以用一个参数控制相邻节点的距离,预定义长度单位对此参数也会失效.


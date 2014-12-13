# graphviz
装完graphviz之后, -Tps, eps 等都报错,
-c configure plugins

	[eric@human ~]$ dot sample.dot 
	There is no layout engine support for "dot"
	Perhaps "dot -c" needs to be run (with installer's privileges) to register the plugins?
	[eric@human ~]$ sudo dot -c
	[eric@human ~]$ dot -Teps -O sample.dot 

dot accepts input in the DOT language. 

This language describes three kinds of objects: graphs, nodes, and edges. 
The main (outermost) graph can be directed (digraph) or undirected graph. 
Because dot makes layouts of directed graphs, all the following examples use digraph.
(A separate layout utility, neato, draws undirected graphs.) 
Within a main graph, a subgraph defines a subset of nodes and edges.

	digraph G {
		label = "Dot demo"; //给这幅图设置名字
		labelloc = b; //图名字的位置在 bottom,也可以是 t
		labeljust = l; //图名字的位置在 left,也可以是 r
		main -> parse -> execute;
		main -> init;
		main -> cleanup;
		execute -> make_string;
		execute -> printf
		init -> make_string;
		main -> printf;
		execute -> compare;
	}

	$dot -Tps graph1.dot -o graph1.ps

Names of all these objects may be C identifiers, numbers, or quoted C
strings. Quotes protect punctuation and white space.

**Chinese demo**:

- 中文的不支持生成ps, can generate png, jpg, svg
- 文件需要使用utf-8 编码
- fc-list :lang=zh

	digraph G{
		edge [fontname = "FangSong"];
		node [shape=box, fontname="FangSong" size="20,20"];
		{
			start[label="开始"];
		}
		node[shape=plaintext, fontname="KaiTi"]
		{
			end[label="结束"];
		}
		start -> end[label="流程"];
	}

## Drawing Attributes
Nodes are drawn, by default, with `shape=ellipse, width=.75, height=.5`
and labeled by the node name.   
Other common shapes include box, circle, record and plaintext. 

	digraph G {
		size = "4,4";
    	fontsize=14;
		main[shape=box];
		## weight 的值越大, 越出现在中间位置
		main -> parse[weight=8];
		parse -> execute;
		main -> init[style=dotted];
		main -> cleanup;
		execute -> {make_string; printf};
		init -> make_string;
		## 定义edge的颜色, 影响之后所有edge的颜色
		edge[color=red];
		## edge的label 出现在edge 旁边
		main -> printf[style=bold, label="100 times"];
		make_string[label="make a\nstring"];
		## node 的label 显示在node 上面
		node[shape=box, style=filled, color=".7 .3 1.0"];
		execute -> compare;
	}
![graph above](http://i.imgbox.com/3rciGviM.png)

### Location
默认时图中的线都是从上到下的,我们可以将其改为从左到右,在文件的最上层打入
`rankdir=LR` 就是从左到右,默认是 `TB(top -> bottom)`,也可以是 RL,BT.

当图中时间表之类的东西时,我们会需要点能排在一行(列),这时要用到 rank,用花括
号把 rank=same,然后把需要并排的点一次输入

	digraph game{
		label = "Game history"; 
		labelloc = t;
		rankdir = LR;
		{
			node[shape = plaintext];
			1995 -> 1996 -> 1997 -> 1998 -> 1999 -> 2000 -> 2001;
		}
		{
			node[shape = box, style = filled];
			WAR3 -> Xhero -> Footman -> DOTA;
			WAR3 -> Battleship;
		}
		{rank = same; 1996; WAR3;}
		{rank = same; 1998; Xhero; Battleship;}
		{rank = same; 1999; Footman;}
		{rank = same; 2001; DOTA;}
	}
![Location](http://i.imgbox.com/JuOdzOH6.png)

### Node Shapes
The shape polygon exposes all the polygonal parameters, and is useful for
creating many shapes that are not predefined. In addition to the parameters regular,
peripheries and orientation, mentioned above, polygons are parameter-
ized by number of sides sides, skew and distortion. 

- Sides 用于设置它的边数
- peripheries 用于设置多边形的外框的层数
- regular = true 可以让你的多边形是一个规则的多边形
- orientation=*, 可以让你的多边形旋转一个角度
- skew is a floating point number (usually between ?1.0 and 1.0) that distorts the shape by slanting
it from top-to-bottom, with positive values moving the top of the polygon to the
right. Thus, skew can be used to turn a box into a parallelogram. 
- distortion shrinks the polygon from top-to-bottom, with negative values causing the bottom
to be larger than the top. distortion turns a box into a trapezoid.

record 和 Mrecord 的区别就是 Mrecord 的角是圆的,Record 就是由衡的和竖的矩形组成的图形

![node arrtibutes](http://i.imgbox.com/XnxCHvqn.png)
### Lables
the default node label is its name. Edges are unlabeled by
default. Node and edge labels can be set explicitly using the label attribute

在 label 后用< >而不是" "就能引入 html 语言

	digraph structs {
			node [shape=record];
			struct1 [shape=record,label="<f0> left|<f1> mid\ dle|<f2> right"];
			struct2 [shape=record,label="<f0> one|<f1> two"];
			struct3 [shape=record,label="hello\nworld |{ b |{c|<here> d|e}| f}| g | h"];
			struct1:f1 -> struct2:f0;
			struct1:f2 -> struct3:here;
	## 		struct1 -> struct2;
	## 		struct1 -> struct3;
	}
![label demo](http://i.imgbox.com/9Tp7qlt6.png)

### Edege
可以设置每条边箭头的方向,用 dir,有 forward(default),back,both,none 四种

	A -> B[dir = both];
	B -> C[dir = none];
	C -> D[dir = back];
	D -> A[dir = forward];

![edge attribute_1](http://i.imgbox.com/hFzMoMdf.png)  
![edge attribute_2](http://i.imgbox.com/TQpuXgTI.png)  

## Clusters
A cluster is a subgraph placed in its own distinct rectangle of the layout. A sub-
graph is recognized as a cluster when its name has the prefix cluster

Cluster labels appear above the graph by default.

	digraph G {
			subgraph cluster0 {
					node [style=filled,color=white];
					style=filled;
					color=lightgrey;
					a0 -> a1 -> a2 -> a3;
					label = "process #1";
			}
			subgraph cluster1 {
					node [style=filled];
					b0 -> b1 -> b2 -> b3;
					label = "process #2";
					color=blue
			}
			start -> a0;
			start -> b0;
			a1 -> b3;
			b2 -> a3;
			a3 -> a0;
			a3 -> end;
			b3 -> end;
			start [shape=Mdiamond];
			end [shape=Msquare];
	}
![cluser demo](http://i.imgbox.com/WqXrAzIf.png)

## FAQ
**Graphviz: how to have sub-graph nodes lined-up in a straight line?**

	digraph git{
	    subgraph master {
	        m1 -> m2 -> m3 -> m4 -> m5;
			m1 -> m5;
	    }
	    subgraph branch {
	        m2 -> b1; // branch from master
	        b1 -> b2 -> b3;
	        b3 -> m4; // merge into master
			b3 -> b1;
	    }
		{rankdir=LR; rank=same; m1;m2;m3;m4;m5;}
		{rankdir=LR; rank=same; b1;b2;b3;}
	}

## Command line options
**-Tformat sets the format of the output**

- mp MetaPost output.
- plain Simple, line-based ASCII format. Appendix B describes this output. An alternate format is plain-ext, which provides port names on the head and tail nodes of edges.
- png PNG (Portable Network Graphics) output.
- ps PostScript (EPSF) output.
- ps2 PostScript (EPSF) output with PDF annotations. It is assumed that this output will be distilled into PDF.


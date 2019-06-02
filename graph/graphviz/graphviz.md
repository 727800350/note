# manual

- [GraphvizOnline](https://dreampuf.github.io/GraphvizOnline)
- [Graphviz example](https://graphs.grevian.org/example)

graph attributes

- rankdir: direction, LR for left to right, TB for top to bottom, default is TB
- label: graph name
- labelloc: label location, b for bottom, t for top
- labeljust: label location, l for left, r for right
- size
- font size

node attributes:

- label: using node name as the default label
- shape: box, circle, record, plaintext, point, doublecircle
- style: filled
- color:
- width, heigth

edge attributes

- label: edge name
- dir: forward(default), back, both, none
- color: red
- style: dotted, bold
- weight: ֵԽ��, Խ�������м�λ��
- penwidth:

# Intro
This language describes three kinds of objects: graphs, nodes, and edges.
The main (outermost) graph can be directed (digraph) or undirected graph.
Because dot makes layouts of directed graphs, all the following examples use digraph.
(A separate layout utility, neato, draws undirected graphs.)
Within a main graph, a subgraph defines a subset of nodes and edges.

```
digraph G {
  label = "Dot demo";  // graph name
  labelloc = b;  // name location, b for bottom, t for top
  labeljust = l;  // name location, l for left, r for right
  main -> parse -> execute;
  main -> init;
  main -> cleanup;
  execute -> make_string;
  execute -> printf
  init -> make_string;
  main -> printf;
  execute -> compare;
}
```

Names of all these objects may be C identifiers, numbers, or quoted C strings.
Quotes protect punctuation and white space.

```
digraph G {
  node [shape = box, size="20,20"];  // ֮�����е�node �����������, ���Ǳ�������
  start[label="��ʼ"];
  node[shape = plaintext]
  end[label = "����"];
  start -> end[label="����"];
}
```

Nodes are drawn, by default, with `shape=ellipse, width=.75, height=.5` and labeled by the node name.

### Location
��ͼ��ʱ���֮��Ķ���ʱ,���ǻ���Ҫ��������һ��(��),��ʱҪ�õ� rank,�û���
�Ű� rank=same,Ȼ�����Ҫ���ŵĵ�һ������

```
digraph game {
  label = "Game history";
  labelloc = t;
  rankdir = LR;

  node[shape = plaintext];
  1995 -> 1996 -> 1997 -> 1998 -> 1999 -> 2000 -> 2001;

  node[shape = box, style = filled];
  WAR3 -> Xhero -> Footman -> DOTA;
  WAR3 -> Battleship;

  {rank = same; 1996; WAR3;}
  {rank = same; 1998; Xhero; Battleship;}
  {rank = same; 1999; Footman;}
  {rank = same; 2001; DOTA;}
}
```

### Node Shapes
The shape polygon exposes all the polygonal parameters, and is useful for
creating many shapes that are not predefined. In addition to the parameters regular,
peripheries and orientation, mentioned above, polygons are parameter-
ized by number of sides sides, skew and distortion.

- Sides �����������ı���
- peripheries �������ö���ε����Ĳ���
- regular = true ��������Ķ������һ������Ķ����
- `orientation=*`, ��������Ķ������תһ���Ƕ�
- skew is a floating point number (usually between ?1.0 and 1.0) that distorts the shape by slanting
it from top-to-bottom, with positive values moving the top of the polygon to the
right. Thus, skew can be used to turn a box into a parallelogram.
- distortion shrinks the polygon from top-to-bottom, with negative values causing the bottom
to be larger than the top. distortion turns a box into a trapezoid.

record �� Mrecord ��������� Mrecord �Ľ���Բ��,Record �����ɺ�ĺ����ľ�����ɵ�ͼ��

### Lables
�� label ����< >������" "�������� html ����

```
digraph structs {
  node [shape=record];
  struct1 [shape=record,label="<f0> left|<f1> mid\ dle|<f2> right"];
  struct2 [shape=record,label="<f0> one|<f1> two"];
  struct3 [shape=record,label="hello\nworld |{ b |{c|<here> d|e}| f}| g | h"];
  struct1:f1 -> struct2:f0;
  struct1:f2 -> struct3:here;
  ##struct1 -> struct2;
  ##struct1 -> struct3;
}
```

```
digraph clusters {
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
```

Graphviz: how to have sub-graph nodes lined-up in a straight line?
```
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
```

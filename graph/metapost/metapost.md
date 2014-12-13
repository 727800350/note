[metapost preview online](http://www.tlhiv.org/mppreview/)

# 数据类型和变量
METAPOST 中有10 种基本数据类型:numeric,pair,path,pen, color ,cmykcolor,transform,string,boolean,picture.

- 一个点的坐标是一个pair
- 几个点用直线或曲线连起来是一个path
- pencircle 是一种pen
- black 是一种color
- scaled,rotated,shifted 都是transform.
- numeric 类型变量的精度是1/65536,它的绝对值不能超过4096,在计算过程中数值可以达到32768.
- 这样的规定也应归功于当年的电脑硬件,不过对于科技文档插图而言,4096 一般还是够用的.

除了缺省的numeric,其它变量在使用之前都需要用数据类型来显式声明.  
相同类型的变量可以在一行语句中声明,但是带下标的变量不能放在同一行(这个规定很蹊跷).
```
numeric x,y,z; %正确
numeric x1,x2,x3; %错误
numeric x[]; %正确,然后可以使用x1,x2,x3
```

The numeric data type is used so often that it is the default type of any non-declared variable.  
This explains why `n := 10;` is the same as numeric n; `n := 10;`  
and why you cannot enter `p := (0,0);` nor `p = (0,0);` to define the point, but must use `pair p; p := (0,0);` or `pair p; p = (0,0);` .  
`z0=(1,2)` 意味着 `x0=1, y0=2`  
`pari p; p = (10,20);` 意味着 `xpart p =10, ypart p =20;`

## 长度单位
- a=.7in;
- b=.5in;
- 1 unit = 1/72 inch
- bp postscript points(big points)
- pt printer's points
- in inch
- pc picas
- cm 
- mm

## array
```
h=2in; w=2.7in;
path p[], q[], pp;
for i=2 upto 4: ii:=i**2;
p[i] = (w/ii,h){1,-ii}...(w/i,h/i)...(w,h/ii){ii,-1};
endfor
```
定义了三条路径 `p[2],p[3],p[4]`. % 也可以通过p2,p3,p4 访问  
还可以是小数, 但是需要预先定义, 例如 `p1.5 = ....` (或者 `p[1.5] = ...`)

## path
```
drawdot origin
```
definition: `origin:=(0,0)`

```
draw (0,0)--(1,1)--(2,4)--cycle;
```
--cycle
..cycle

```
1/2[point0, point1]
```

```
intersection = whatever[point0, point1] = whatever[point2, point3]
```

是一个实例中, 明明有交点, 但是用intersectionpoint 却告诉我没有, 而换用whatever 可以找到

```
pa intersectionpoint pb;
```

If there is more than one intersection point, it just chooses one; if there is no intersection, the macro generates an error message.

## color
颜色和填充
METAPOST 预定义的颜色有黑,白,红,绿,蓝,它们的RGB 值分别为(0,0,0),(1,1,1),(1,0,0),(0,1,0),(0,0,1),缺省色就是黑色.
绘图命令一般都可以通过withcolor 参数来使用各种颜色.
封闭路径可以用fill 命令填充.
```
draw (0,4u)--(9u,4u) withcolor red;
draw (0,2u)--(9u,2u) withcolor green;
draw (0,0)--(9u,0) withcolor blue;
```

除了为每个绘图命令单独指定颜色,我们也可以使用一个全局命令,使得其后的绘图命令都使用某种颜色.
```
drawoption(withcolor blue);
```

用RGB值效果是一样的.
```
color c[];
c1 := .9red + .6green + .3blue;
c2 := (.9,.6,.3);
```

## transformation
```
shifted
fill p scaled u;
fill p scaled u shifted (3u,0) withcolor red;
```

```
path p;
p := (0,0)--(2,0)--(1,1.732)--cycle;
draw p scaled u;
draw p xscaled 2u yscaled u shifted (3u,0);
draw p scaled u rotated 60 shifted (8u,0);
```

## pen
```
pickup pencircle scaled ipt;
draw p[i] withpen pencircle scaled 4;  % about eight times the default pen size
```

pickup 是一种全局操作,也就是说它会影响到之后所有的绘图命令,我们也可以用withpen 为单个绘图命令设置画笔

```
fill <path expression>
```
the argument should be a cyclic path

```
z0=center(pp); % 得到closed path pp的center, 也可以不加括号
```

## circle
- fullcircle: a circle of unit diameter centered on the origin.
- halfcircle: 第一第二象限, 也就是上半圆
- quartercircle: 第一象限

```
path a, b;
a = fullcircle scaled 2cm; %  直径为2cm
draw a;
b = a shifted (0, 1cm); %将circle 上移1cm, 称为b
```

buildcycle forms a closed cycle from the pieces of paths a and b
```
buildcycle(a,b)
```

## square
unitsquare: rectangle
```
draw unitsquare scaled 1cm;
```

## arrow
```
drawarrow (0,4u) -- (9u, 4u)
drawarrow reverse (0,4u) -- (9u, 4u)
drawdblarrow (0,4u) -- (9u, 4u) % double direction arrow
```

# label
![label的位置](http://i.imgbox.com/dtzokI47.png)
```
dotlabel.bot("(0,0)",z0);
```

```
draw p[i] withpen pencircle scaled 3;%效果同dotlabel, 但是没有标签
```

tex in windows, ctex can not compile it, in linux, it goes well
```
label.top(btex $\sqrt{x}$ etex,origin); % 含有tex 的标签
```
btex: begin tex; etex: end tex

```
label.lft(btex $\displaystyle y=\frac{2}{1+\cos x}$ etex,(120ux, 4uy));
```
**不能使用$$**

竖排
```
label.lft(btex $y$ axis etex rotated -90,(0,2uy));
dotlabel(string, position)
draw thelabel(string, position)
```

当要在fill 的picture 上放label 时, 可以使用thelabel
```
picture pa;
pa = thelabel(btex $A$ etex, (0,-.5cm));
unfill bbox pa; %去掉pa的颜色, 使之与默认的background(一般是white) 一致
draw pa;
```

# direction
dir, 定义方向 a simple command to define a point on the unit circle
```
dir(30) generates (1/2\sqrt{3}, 1/2)
```

```
pair A,B;
angle(A - B); %% 正X 方向到 BA 向量的夹角
```

![direction](http://i.imgbox.com/RzMcbkD2.png)
上面的图片中drawsquare与p0, p1的定义
```
pair p[];
p0:=(0,0);
p1:=(1cm,1cm);

def drawsquare = draw unitsquare
scaled 1cm withcolor 0.7white;
enddef;
```

![出发时切线的方向定义](http://i.imgbox.com/GtEMAH4U.png)

# Procedure
## for
```
draw origin
for i=1 upto 8: ..(i/2,sqrt(i/2))*u
endfor;
```
相当于
```
draw origin .. (1/2,sqrt(1/2)*u .. (2/2,sqrt(2/2)*u .. (3/2,sqrt(3/2)*u .. etc ..  (8/2,sqrt(8/2)*u ..
```

# def and vardef
```
def <symbolic token> = <replacement text> enddef
```

```
def fill = addto currentpicture contour enddef

def rotatedaround(expr z, d) = shifted -z rotated d shifted z enddef; 
```
The expr in this definition means that formal parameters z and d can be arbitrary expressions.
(They should be pair expressions but the MetaPost interpreter does not immediately check for that.)

## vardef
The following code defines a function to draw a circle, and returns the circle.

```
vardef drawCircle(expr c,r)= 
 path thecircle; 
 thecircle = fullcircle scaled (2*r) shifted c; 
 draw thecircle; 
 thecircle 
enddef; 
```
Note that the last statement has no semicolon; its value is returned by the function. 

**Differences btween vardef and def**  
One difference is that, vardef has begingroup and endgroup automatically inserted. 
Between begingroup and endgroup, you can use save to make variables local, but not indef:
```
x := 1; y := 2;
begingroup x:=3; y:=x+1; show(x,y); endgroup   %% (3,4)
show(x,y);  %% (1,2)

vardef f=
save a; numeric a; a:=2; show a; % 2
enddef;

def g=
save a; numeric a; a:=2; show a; % 2
enddef;

beginfig(1)
numeric a;
a:=1; f; show a; % 1
a:=1; g; show a; % 2, 'save' in def didn't proect a
endfig;
```

Another difference is that, vardef allows generic subsrcipts and multi-token in the name. For example, the following code is ok:

```
vardef a[]b= enddef; % do nothing 
beginfig(1) 
 numeric a[],b; 
 a1=1; b=0; a1b; 
endfig; 
```

but

```
def a[]b= enddef; % do nothing 
beginfig(1) 
 numeric a[],b; 
 a1=1; b=0; a1b; 
endfig; 
```

won't compile. See more details in the User's Manual.

in a macro definition you may want to use a so-called local variable, i.e., a variable that only has meaning inside that definition and 
does not interfere with existing variables. In general, variables are made local by  the statement 
save name sequence;

```
vardef unitvector primary z = z/abs z enddef;
```
As the example suggests, the keyword primary is enough to specify the macro as  a unary vardef operator. 
Other keywords are secondary and tertiary.
The  advantage of specifying an n-ary operator is that you do not need to place brackets aroundargumentsincompoundstatements; 
METAPOST will sort out which tokens are the arguments. For example 
`unitvector v rotated angle v;`
is understood to be equivalent to
`(unitvector(v)) rotated(angle(v));`

# 数学运算
```
prologues:=2;
numeric p,q,n;
n:=11;
p:=2**n;
q:=2**n + 1;
show p,q;
end
```

# tex
To use LaTeX formulas in labels, add the following lines at the beginning of your .mp file:
```
verbatimtex
%&latex
\documentclass{article}
\begin{document}
etex
```
Then you can do something like
```
label.bot(btex $\frac{x}{a+b}$ etex, z1);
```

# encoding
windows 下mp 文件使用utf-8 编码编译出错, 改为ansi 编码正常


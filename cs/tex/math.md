# Math
`$\not\in$`

数学公式中输入中文 可以把中文用`\text{}`或是`\mbox{}`引起来  
在`\text{}` 仍然可以使用`$$` 符号进行数学输入

`\textsuperscript`  
For example, let us say I want to write the `$n^{th}$` element, but without the math mode automatic italicization of the th. 
And what if I still want the n to be in math mode, but the th outside?  
`$n$\textsuperscript{th}`

## 分数
- `\frac` 自动调整字号,比如在行间公式中小一点,在独立公式则大一点.
- `\dfrac` 命令把分数的字号显式设置为独立公式中的大小,
- `\tfrac` 命令则把字号设为行间公式中的大小.

`$$ \lim_{\| x\| \to +\infty \atop x \in P} f(x) = +\infty $$`  
generate:  
![\atop](http://i.imgbox.com/OFTF8dsw.png)

会产生下面的, 但是latex 对于\atop 的使用会给出警告, 提示说可以采用\genfrac

`\genfrac` will not produce a line if the third argument (which control the thickness of the rule) is set to 0pt; the syntax for `\genfrac` is  

`\genfrac{<left-delim>}{<right-delim>}{<thickness>}{<mathstyle>}{<numerator>}{<denominator>}`
	
Eg: 
`\newcommand\mycom[2]{\genfrac{}{}{0pt}{}{#1}{#2}}`  
`$A\mycom{a=x}{b=y}B$`  
generate  
![genfrac](http://i.imgbox.com/XqXECVbO.png)

`$$\xrightarrow[x<y]{a*b*c}$$`  
generate:  
![xrightarrow](http://i.imgbox.com/0zbtS5lV.png)

**多重积分**  
`\iint, \iiint, \iiiint, \idotsint`

## 多行公式
有多种条件的公式组用cases 次环境

	$$
	f(n) = 
	\begin{cases}
	 n/2 & \mbox{if } n \equiv 0 \\ 
	(3n +1)/2 & \mbox{if } n \equiv 1. 
	\end{cases}
	$$

	\begin{equation} 或者 $$
	\begin{aligned}
	\end{aligned}
	\end{equation}
	
	\begin{equation} 不能使用$$, 不知道为什么
	\begin{split}
	\end{split}
	\end{equation}
	
	\begin{align}
	\end{align}
	align 一行一个公式序号

	\begin{gather}
	\end{gather}
	gather 一行一个公式序号
	不需要对齐的公式组用 gather 环境,需要对齐的用align
	
	\begin{eqarray}
	\end{eqarray}

[Avoid eqnarray](https://tug.org/pracjourn/2006-4/madsen/madsen.pdf)

公式中对齐

```
\begin{equation}  fst_a(A)\triangleq 
\left\{
      \begin{aligned}
			 &\varnothing & if\ a \notin\Sigma(A)\\
             &hitter(A_m) & if\ m=min\{n \in \uppercase\expandafter{\romannumeral2^A|a \in \Sigma(A_n)}\land\Sigma(hitter(A_m))=a\}\\  
             &target(A_m) & if\ m=min\{n \in \uppercase\expandafter{\romannumeral2^A|a \in \Sigma(A_n)}\land\Sigma(target(A_m))=a\}\\  
           \end{aligned}
\right.  
\end{equation}  
```
\href{http://i.imgbox.com/JGSKOgTs.png}{result}


```
\begin{equation}  fst_a(A)\triangleq 
\left\{
      \begin{aligned}
             &\varnothing & if & \ a \notin\Sigma(A)\\
             &hitter(A_m) & if & \ m=min\{n \in \uppercase\expandafter{\romannumeral2^A|a \in \Sigma(A_n)}\land\Sigma(hitter(A_m))=a\}\\  
             &target(A_m) & if & \ m=min\{n \in \uppercase\expandafter{\romannumeral2^A|a \in \Sigma(A_n)}\land\Sigma(target(A_m))=a\}\\  
           \end{aligned}
\right.  
\end{equation}  
```
\href{http://i.imgbox.com/mcWpxQK9.png}{将if放在单独一格的result}

## Matrix
pmatrix,bmatrix,Bmatrix,vmatrix和Vmatrix

## 数学字体
- \mathrm 罗马直立
- \mathit 罗马斜体
- \mathbf 直立粗体
- \mathcal 花体
- \mathtt
- \mathsf

![数学注音符号](http://i.imgbox.com/I7RSbdFC.jpg)

`$$n+1 \choose 3$$` 生成$C_{n+1}^3$ 的括号形式

**括号**
`(), [], \{\}, \langle\rangle, \lvert\rvert, \lVert\rVert`
我们可以在上述分隔符前面加`\big \Big \bigg \Bigg` 等命令来调整

Beamer 中也定义了各种定理环境,而且默认是用区块环境的样式来显示的.  
例如下面的代码
	
	\begin{theorem}
	微积分基本公式:$\int_a^b f(x)\mathrm{d}x=F(b)-F(a)$.
	\end{theorem}


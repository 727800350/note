- 否定: `\notin` or `\not \in`都可以
- 排列组合: `$$n+1 \choose 3$$` 生成$C_{n+1}^3$ 的括号形式
- 输入点不能使用`\dot`, 而应该直接使用 "."
- 公式中输入普通文字, 可以用`\text{}`或是`\mbox{}`, 或者自己定义的`\eqnote{}`
- 在`\text{}` 仍然可以使用`$$` 符号进行数学输入
- Matrix env: pmatrix, bmatrix, Bmatrix, vmatrix, Vmatrix
- 数学字体: \mathrm 罗马直立, \mathit 罗马斜体, \mathbf 直立粗体, \mathcal 花体, \mathtt, \mathsf
- 多重积分: `\iint, \iiint, \iiiint, \idotsint`
- 括号
	- `(), [], \{\}, \langle\rangle, \lvert\rvert, \lVert\rVert`
	- 同时可以在上述分隔符前面加`\big \Big \bigg \Bigg` 等命令来进一步调整大小
- 分数
	- `\frac` 自动调整字号,比如在行间公式中小一点,在独立公式则大一点
	- `\dfrac` 命令把分数的字号显式设置为独立公式中的大小
	- `\tfrac` 命令则把字号设为行间公式中的大小

theorem 的样式

There are three basic styles provided:

1. The 'plain' style produces bold headings and italic body text;
1. the 'definition' style produces bold headings and normal body text;
1. the 'remark' style produces italic headings and normal body text. eg: `\theoremstyle{remark}\newtheorem*{answer}{Answer}`

# 多行公式
cases环境产生一个大括号
```latex
$$
f(n) = 
\begin{cases}
n/2       & \si n \equiv 0 \\
(3n +1)/2 & \si n \equiv 1
\end{cases}
$$
```

```latex
\begin{equation} 或者 $$
\begin{aligned}
\end{aligned}
\end{equation}
```

```latex
\begin{equation} 不能使用$$, 不知道为什么
\begin{split}
\end{split}
\end{equation}
```

align 一行一个公式序号
```latex
\begin{align}
\end{align}
```

gather 一行一个公式序号, 不需要对齐的公式组用 gather 环境,需要对齐的用align
```latex
\begin{gather}
\end{gather}
```

```latex
\begin{eqarray}
\end{eqarray}
```
[Avoid eqnarray](https://tug.org/pracjourn/2006-4/madsen/madsen.pdf)

![数学注音符号](http://i.imgbox.com/I7RSbdFC.jpg)


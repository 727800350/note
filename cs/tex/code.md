原样显示
\beigin{verbatim}...\end{verbatim}
\verb+...+

代码  
如果是beamer frame, 则必须用`\begin{frame}[fragile]`, verbatim  环境也一样
```
\usepackage{listings}
\lstinline[<options>]<char><code><char>
\begin{lstlisting}[language=...] Put your code here. \end{lstlisting}
\lstinputlisting[language=Python]{source_filename.py}
\lstinputlisting[language=Python, float, caption = Add Fake Source Dependencies, firstnumber=91, firstline=91, lastline=95]{source_filename.py}
```
Python 的注释只能使用一个#, 而不能使用两个##, 否则会报错.

代码中可以有_, 但是注释中不能有, 否则需要使用math mode

- language  
C, C++, bash, Python
- title={<text>}  
is used to set an unnumbered and unlabelled title. If <text> contains a comma or equal sign, make sure you enclose <text> in curly braces { and }.
- caption={[<short>]<text>}  
is used to set a numbered caption. The optional part <short> is an alternative short caption for the list of listings, which can be produced using  
`\lstlistoflistings`  
As above, if the caption contains a comma or equal sign, make sure you enclose it in curly braces { and }.
- mathescape  
This is a boolean key that can either be true (dollar $ character acts as the usual math mode shift) or false (deactivates the usual behaviour of $).
- escapechar=@  



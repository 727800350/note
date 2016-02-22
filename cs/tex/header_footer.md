`\pagestyle{option}`: command changes the style from the current page on throughout the remainder of your document

- plain - Just a plain page number.
- empty - Produces empty heads and feet - no page numbers.

`\thispagestyle{empty}` 只影响当前页, 只是不显示, 但是仍然占用页码数

`\pagenumbering{option}`, 页码的显示方式
- arabic, 阿拉伯数字
- roman, 小写罗马数字
- Roman,大写罗马数字
- alpha, 小写拉丁字母
- Alpha, 大写拉丁字母

`\setcounter{element}{number}`: 对element元素重新开始以number编号, eg: `\setcounter{page}{1}`

**实现titlepage 不用页码, 目录页用罗马数字页码, 正文用阿拉伯数字作为页码**
```Latex
%% 设置标题
\thispagestyle{empty}
\maketitle
\newpage

%% 设置目录
\pagenumbering{Roman}
\setcounter{page}{1}
\tableofcontents
\newpage

%% 设置目录
\pagenumbering{arabic}
\setcounter{page}{1}
\renewcommand{\thepage}{A\arabicpage}} %% 可改变页码的格式为A1,A2,...的型式,直到进行下一次改变为止.
```

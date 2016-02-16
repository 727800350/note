beamer 自己已经加载了 hyperref 这个package, 所以没有必要再次load 一遍

[beamer_gallery](http://deic.uab.es/~iblanes/beamer_gallery/)

# Note
note 的显示方式
```Latex
\setbeameroption{show notes}
\setbeameroption{hide notes}
\setbeameroption{show only notes}
\setbeameroption{show notes on second screen = left} right, top, bottom
```

插入note
```Latex
\note{text}

\note[item]<2>{tell joke about plants}
\note[item]<2>{make it short}
```


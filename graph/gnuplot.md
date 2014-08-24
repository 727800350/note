# Style
in gnuplot prompt, Use the command `test` to see what is available.

[ref](http://gnuplot.sourceforge.net/docs_4.2/node231.html)

Default plotting styles are chosen with the set style data and set style function commands.
```
     set style function <style>
     set style data <style>
     show style function
     show style data
```

Default styles for specific plotting elements may also be set.
```
     set style arrow <n> <arrowstyle>
     set style fill <fillstyle>
     set style histogram <histogram style options>
     set style line <n> <linestyle>
```

- line types: solid, dash-dot, color, etc
- point types: circles, squares, crosses, etc

## Set style data

The set style data command changes the default plotting style for data plots.
```
     set style data <plotting-style>
     show style data
```

## Set style line
[demo](http://gnuplot.sourceforge.net/demo_canvas/dashcolor.html)
```
     set style line <index> default
     set style line <index> {{linetype  | lt} <line_type> | <colorspec>}
                            {{linecolor | lc} <colorspec>}
                            {{linewidth | lw} <line_width>}
                            {{pointtype | pt} <point_type>}
                            {{pointsize | ps} <point_size>}
                            {palette}
     unset style line
     show style line
```

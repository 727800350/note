Cascading Style Sheets 层叠样式表

引入一个定义好的css 文件: `<link href="StyleSheet1.css" rel="stylesheet" type="text/css" />`

计量单位:px像素;30%;em相对单位.width:20px

background-color背景颜色;color文本颜色

border

border-style;border-color;border-width(默认值是0)

`style="border-style:dotted;border-color:Red;border-width:2px"`

display控制显示与否none(不显示);block(显示为块级元素,元素前后带有换行符);inline(元素前后没有换行符)

CSS float 属性

float 属性定义元素在哪个方向浮动.以往这个属性总应用于图像,使文本围绕在图像周围,不过在CSS 中,任何元素都可以浮动.浮动元素会生成一个块级框,而不论它本身是何种元素.
如果浮动非替换元素,则要指定一个明确的宽度,否则,它们会尽可能地窄.
注释:假如在一行之上只有极少的空间可供浮动元素,那么这个元素会跳至下一行,这个过程会持续到某一行拥有足够的空间为止.
JavaScript 语法:object.style.cssFloat="left"

cursor:鼠标指针放到元素上面时显示的样子  
`td.style.cursor = "pointer";`//td是一个表table中的一个单元格,鼠标放到这个单元格上面时,鼠标变成手的形状

list-style-type:对于<ul></ul><li></li>列表时对那个圆点的控制
```
onfocus="this.style.textAlign='left';" onblur="this.style.textAlign='right';"
```

## 样式选择器
三种:标签选择器;class选择器;id选择器

标签
```
input{background-color:Green;border-color:Black;}
p{color:Blue;}
```

class
```
.Class_warning{font-style:italic;font-size:xx-large;}//前面的点不能丢
<p class=" Class_warning ">aaaaa</p>
```
当有多个样式一起作用相互冲突时,以用style直接指定的为准

 
标签+class
```
input.class1{background-color:Green;border-color:Black;}
p.class1{color:Blue;}
```

id
#id_username
```
{ background-color:Red; }
```

关联选择器

使用空格分隔
```
p strong{color:Yellow;}
```
同时具有p与strong标签


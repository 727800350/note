HyperText Markup Language超文本标记语言

特殊标记
```
&lt; 小于号 less than
&gt; 大于号greater than
&nbsp; 空格no-break space
&copy;
```

文字标签
```
<p>文字</p>创建段落
<br/>回车
<hr />
<center>文字</center>居中显示

<h1>文字</h1>
...
<h6>文字</h6>表示不同大小的字体

<b>文字</b>粗体
<u></u>下划线
<i></i>斜体
<del></del>删除线
<SUP>...</SUP> 上标字,类似于指数
<SUB>...</SUB> 下标字,字体下标字
 
<font size="30" color="red">文字</font>
```

超链接
```
<a href="www.baidu.com">百度</a>

<a href="http://www.rupeng.com">
	<img src="http://www.rupeng.com/forum.templates/uchome/images/logo.gif"/>
</a>
```

`<pre>` pre 元素可定义预格式化的文本.被包围在 pre 元素中的文本通常会保留空格和换行符.而文本也会呈现为等宽字体.
`<pre>` 标签的一个常见应用就是用来表示计算机的源代码.
```
<pre>
for i = 1 to 10
	 print i
next i
</pre>
```

列表,表格
```
<ul><li>灌水区</li><li>版务区</li><li>原创贴图</li></ul>
```
- ul unordered list
- ol ordered list

```
<table>
<CAPTION>...</CAPTION> 表格标题
<tr><th></th></tr>
<tr><td></td></tr>
<tr><td></td></tr>
</table>
```

Form表单
```
<form>
Input <input type="text"/>
Input <input type="file"/>//上传文件
submit <input type="submit"/>
</form>
```
Type submit/button/checkbox/file/hidden/image/reset/text/radio(单选按钮)

checkbox类型时,在javascript中有一个属性是checked,其值为布尔值

`<textarea>`

`<select>` comobox下拉框选择 listbox复选框(按下ctrl复选)
如果size大于1就是listbox(size的值为显示出来的列表数量)
或者Multiple=" Multiple" 就是listbox

Select中项是option `<option value="" selected="selected">北京</option>`
每一个项代表的值为value,后面编程的时候会用到,selected表示默认被选中

Optiongroup
```
<select>
	<optgroup label="湖北省">
		<option value="value">武汉</option>
		<option value="value">随州</option>
</optgroup>
</select>
```
当选中某一项时,这个项的value就会成为这个select的value属性

Radio的name相同的为一组,组内选择是互斥的

`<label>` 标签为 input 元素定义标注(标记).

label 元素不会向用户呈现任何特殊效果.不过,它为鼠标用户改进了可用性.如果您在label 元素内点击文本,就会触发此控件.就是说,当用户选择该标签时,浏览器就会自动将焦点转到和标签相关的表单控件上.
`<label>` 标签的 for 属性应当与相关元素的 id 属性相同.
```
<label for="name">姓名</label>
<input type="text" id="name" />

<label for="mariage">婚否</label>
<input type="checkbox" id="mariage" />
```

Fieldset 相当于winform的Groupbox
```
<fieldset>
	   <legend>常用</legend>
	   <input type="text" />
	   <input type="text" />
	   <input type="text" />	  
</fieldset>
```
 
Div/span  
Div是一个矩形块,影响布局与显示  
Span可以是任何形状,不影响布局与显示  
div标签是区块层级,span是内文层级  
一般使用css+div构造页面的布局,使用span控制局部的效果  
div与span都属于层标签


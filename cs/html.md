# HTML
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

<pre> pre 元素可定义预格式化的文本.被包围在 pre 元素中的文本通常会保留空格和换行符.而文本也会呈现为等宽字体.
<pre> 标签的一个常见应用就是用来表示计算机的源代码.
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

<textarea>

<select> comobox下拉框选择 listbox复选框(按下ctrl复选)
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

<label> 标签为 input 元素定义标注(标记).

label 元素不会向用户呈现任何特殊效果.不过,它为鼠标用户改进了可用性.如果您在label 元素内点击文本,就会触发此控件.就是说,当用户选择该标签时,浏览器就会自动将焦点转到和标签相关的表单控件上.
<label> 标签的 for 属性应当与相关元素的 id 属性相同.
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

# DOM
Document Object Model文档对象模型

CSS+JavaScript+DOM=DHTML

定时 setInterval  
每间隔一定时间执行
```
intervalId=setInterval("alert('hello')", 5000)//每隔5000毫秒,也就是5秒弹出一个hello对话框
```

setTimeout

clearTimeout

navigate

<input type="button" onclick="navigate('HTMLPageForm.htm')" value="navigate demo" />

navigate("http://www.baidu.com")
confirm()

```
if (confirm("是否要放弃输入的文本")) {
	alert("yes");
}
else{
	alert("no");
}
```
confirm()可以用来防止用户的误操作导致离开这个页面而致使输入的东西都丢失掉
 
## body,document对象的事件
- onload:网页加载完毕后触发
- onunload:网页关闭(或者离开)后触发
- onbeforeunload:在网页准备关闭(或者离开)时触发
- onbeforeunload="window.event.returnValue='文本会丢失'"
- onclick 单击
- ondblclick双击
- onkeydown按键放下
- onkeypress点击按键
- onkeyup按键释放
- onmousedown鼠标按下
- onmousemove鼠标移动
- onmouseout鼠标离开元素范围
- onmouseover鼠标移动到元素范围
- onmouseup鼠标按键释放
- onmousedown :当单机鼠标时触发的事件
- 地址location

window.location.href
既可以通过它取得当前地址,也可以给它赋值实现navigate的效果

window.location.reload()刷新

window.event 获得事件发生时的信息,时间不局限于window对象的事件.类似于winForm中的e(EventArg)

特殊按键的判断
altKey属性,bool类型,表示发生事件时alt键是按下.类似的还有ctrlKey,shiftKey属性
bool window.event.altKey
```
<input type="button" value="点击" onclick="if(window.event.ctrlKey){alert('点击了ctrl键');};else {alert('没有点击ctrl键');}" />
```

坐标

clientX ,clientY发生事件时鼠标在客户区的坐标

screen X, screen Y发生事件时鼠标在屏幕上的坐标
offsetX,offsetY发生事件时鼠标相对于事件源(比如点击按钮时触发onclick)的坐标

returnValue属性,如果将他设置为false,就会取消默认事件的处理
```
<a href="HTMLPageTable.htm" onclick="alert('禁止访问');window.event.returnValue=false;">hi</a>
```
例如用户在注册时,所填信息有误,这个时候就不用提交到服务器,就可以通过这个方式

 
srcElement 获得事件源对象

keyCode发生事件时的按键值
button发生事件时的鼠标按键,1为左键,2为右键,3为左右键同时按

 
screen对象

screen.width;screen.height

clipboardData对象

对粘贴板的操作
几乎任何元素都有oncopy与onpaste两个属性

clearData("Text")清空粘贴板,参数只能时Text(注:可能是为了以后的扩展而这样要求)
getData("Text")读取粘贴板的值,返回值为粘贴板中的内容

setData("Text",val)设置粘贴板中的值
 
```
<body oncopy="alert('禁止复制');window.event.returnValue=false;"></body>
<body oncopy="alert('禁止复制'); return false;"></body>
 
mobile:<input type="text" /><br />
mobile again:<input type="text" onpaste="alert('为保证你的资金充值到正确的手机号,请勿复制');return false;" />

function modifyClipboard(){
	clipboardData.setData('Text', clipboardData.getData('Text') +"文章转自:"+ window.location.href);
}
<body oncopy="setTimeout('modifyClipboard()', 100)"></body>
```

- window.history
- window.history.back();  window.history.go(-1);后退一个页面
- window.history.forward();  window.history.g0(1);
- window.history.go(2);
```
<a href="javascript:window.history.back()">back</a>

也可以放到input button的onclick事件中
<input type="button" onclick="window.history.back()" value="back"  />
```

window.document

write(text);

向文档中写入内容

只能在页面加载过程中创建

window.document.write("this is a document<br/>");
window.document.write("<a href='http://www.buaa.edu.cn'>buaa</a><br/>");

但是如果在onclick事件中,写入的内容会冲掉页面中的原有内容

<input type="button" value="document.write" onclick="window.document.write('buaa is mine');"/>
write经常在广告代码/整合资源代码中使用,例如引用百度的新闻http://news.baidu.com/newscode.html
 
getElementById();

alert(form1.textbox2.value);
alert(textbox1.value);

var txt = document.getElementById("textbox2");
alert(txt.value);

getElementsByName();

一个页面中name可以不唯一,所以返回的结果是一个数组
```
<head>
	<title></title>
	   <script type="text/javascript">
			  function btnclick() {
				  var radios = document.getElementsByName("gender");
				  for (var i = 0; i < radios.length; i++) {
					   alert(radios[i].value);
				  }
			  }
	   </script>
</head>

<body>
<input type="radio" name="gender"  value="男"/>男
<input type="radio" name="gender" value="女" />女
<input type="button" value="get the gender"  onclick="btnclick()"/>
</body>
```
 
parseInt(string,radix); 将string转换成数字,radix为进制

createElement 动态添加的内容通过查看网页源代码的方式是看不到的,可以通过debugbar查看 
```
var div = document.getElementById("divMain");
var input = document.createElement("input");
input.type = "button";
input.value = "je suis dynamique";
div.appendChild(input);

var input = document.createElement("<input type='button' value='hello'/>");
document.body.appendChild(input);
var label = document.createElement("label");
label.innerText = "user";
label.setAttribute("for", "username");//username是一个文本框,因为for在js中是一个关键字,所以直接使用label.for="username");是不行的
document.body.appendChild(label);
```

setAttribute

setAttribute还可以添加一些html本身没有的属性, 例如:
```
label.setAttribute("studentId", "39241715");//这样可以在以后使用
var id=label.getAttribute(("studentId");//这样就可以取到这个值
```
innerText innerHTML: 几乎所有的DOM元素都有innerText与innerHTML两个元素,这两个元素是大小写敏感的,且可以读和写

firefox不支持innerText,这就是以后我们要使用jquery的原因之一
The innerText property is valid for block elements only. By definition, elements that do not have both an opening and closing tag cannot have an innerText property.

对于innerHTML 属性,几乎所有的元素都有innerHTML属性,它是一个字符串,用来设置或获取位于对象起始和结束标签内的HTML.
firefox下innerText是target
 
this 获得事件对象

修改属性

css中background-color,在js中是backgroundcolor,因为在js中,-有特殊的含义,减号

onfocus onblur 焦点进入控件的事件是onfocus,焦点离开控件的事件是onblur
```
<head>
	<title></title>
	   <script type="text/javascript">
			  function initEvent() {
					 var inputs = document.getElementsByTagName("input");
					 for (var i = 0; i < inputs.length; i++) {
							var input = inputs[i];
							input.onblur = inputOnblur;
					 }
			  }

			  function inputOnblur() {
					 if (this.value.length <= 0) {
							this.style.background = "red"; //由于是将控件的onblur响应函数设置为inputOnblur,而不是该控件的onblur响应函数调用inputOnblur(),所以这里可以用this			   }
					 else {
							this.style.background = "white";
					 }
			  }
	   </script>
</head>

<body onload="initEvent()">
	   <input type="text" name="name" value="content1" onblur="alert('第一个失去了焦点')"  onfocus="alert('第一个得到了焦点')"/>
	   <input type="text" name="name" value="content2" onblur="alert('第二个失去了焦点')"  onfocus="alert('第二个得到了焦点')"/>
	   <br />
	   <input type="text" /><input type="text" /><input type="text" /><input type="text" /><input type="text" />
</body>
```

onchange

onchange只有在光标离开时才执行
这样是合理的,例如对于一个text文本框,输入内容的时候,里面的内容一直在改变,如果一改变就触发,那么onchange这个事件就会一直处于触发状态.

style

style.display属性

控制层显示与否

只有当值为none时,才不显示,其他值(包括空值)都显示

在IE中监听body的onclick,最后一个元素以下内容是不被监听的,如果一定要监听,需要监听document的onclick事件,在javascript中完成
例如下面的匿名函数
`document.onclick = function(){alert("OK");}`

postion
写某个对象的具体坐标之前,先要确定是哪种类型的position

absolute 生成绝对定位的元素,相对于 static 定位以外的第一个父元素进行定位.

fixed 生成绝对定位的元素,相对于浏览器窗口进行定位.

relative 生成相对定位的元素,相对于其正常位置进行定位.

static 默认值.没有定位,元素出现在正常的流中(忽略 top, bottom, left, right 或者 z-index 声明).

inherit 规定应该从父元素继承 position 属性的值.

style="position:absolute;top:200px;left:200px"

form submit
通过一个普通的按钮也能提交表单
```
<form action="default.aspx" method="post" id="form1">
	<input type="button" name="name" value="same as submit" onclick="form1.submit()" />
	<input type="submit" />
</form>
```
如果提交之前要检查数据,可以在form的`onsubmit`(如果是return false;就可以阻止提交)事件中写代码进行数据检查,
但是如果通过`form1.submit()`进行提交,他不会执行onsubmit里的代码.

另外,如果onsubmit事件要调用函数,要特别注意返回值的处理
```
function beforeSubmit(){
   var name = document.getElementById("name");
   if (name.value.length <= 0) {
   		alert('name can not be empty');
		return false;
   }
}
onsubmit="return beforeSubmit()"
```

通过一个按钮的点击触发另外一个按钮点击动作
```
<input type="button" id="btn1" value="btn" onclick="alert('btn')" />
<input type="button" name="name" value="clickonme"  onclick="btn1.click()" />
//不是btn1.onclick();
```

# Jquery
ready

`$(document).ready(function(){alert("加载完毕");});`
注册事件函数,和普通的dom函数不一样,不需要在元素的标签上添加on**等标记

ready()和dom中的onload类似,但是onload只能注册一次,而ready()可以注册多次

$(document).ready(function () {alert("加载完毕");});

$(document).ready(function () {alert("加载完毕2");});
onload是在所有的dom元素,图片,css等都加载完毕后才会被触发,而ready在所有的dom元素创建完毕后就会被触发,这样可以提高网页的响应速度

$(window).load()可以实现onload那种事件调用的时机
由于ready()这个函数很常用,所以jquery对其进行了简化, $(function () {alert("over");})

map

$.map(array,function)对array中的每个元素调用function函数进行处理,有返回值
each

$.each(array,function),没有返回值

$.each(array,function(key,value){})
当使用一个参数时,指key

当不使用参数时,可以实用this,这个时候this指value
jquery对象就是通过jquery封装的dom对象

jquery对象只能调用jquery封装的方法.不能调用dom对象的方法

可以讲一个dom对象转化为jquery对象

var div = document.getElementById("div1");
	   alert($(div).html());

请注意,array是javascript语言本身包含的,不是dom对象,所以不需要转换为jquery对象

同时,也可以将jquery对象转化为dom对象($(div)[0]或者($(div).get(0)

jquery对象都有内置的隐式调用each方法,如果是自己写的对象可以显示调用each方法

注册事件
```
$(function () {
	$("p").click(function(){
	  alert("i am is a p");
	});//隐式迭代,对取到的每个p都注册click事件
});
```

css
```
$(function () {$("#div1").css("background", "red");});
$(function () { alert($("#div1").css("backgroundColor")); });
```

class
```
attr("class")
attr("class","myclass")
```

addClass
removeClass
toggleClass 切换样式,如果没有,则添加,如果有,则删除
hasClass 判断是否有这个样式

filter:Gray黑白样式

选择器

id/tagName/cssClassName选择器
```
$("#id ")/$("TagName")/$(".cssClassName")
```

例如:
```
$(function(){
	$("p").click(function () { alert("i am is a p"); });//隐式迭代,对取到的每个p都注册click事件
});
```

多条件选择器
```
$("p,div,span.menuitem") 选择所有的p标签,div标签和带有menuitem样式的span标签
```

选择器表达式中的空格不能多也不能少
层次选择器

$("div li")获取div下的所有li元素
$("div > li")获取div下的直接li元素
$("body *")选取body下的所有元素
$("#table img").not(this)

过滤选择器
:first 选取第一个元素 $("div:first")
:last
:not  $("input:not(.myclass)")选取不是myclass的input
:event  :odd 选取奇偶数
:eq/:gt/:lt选取索引号等于,大于,小于的元素  $("input:lt(5)")
$(":header")
$("div:animated")选取正在执行动画的div

过滤选择器可以组合使用,但是请注意每一步过滤都是在前一步的过滤所得结果进一步过滤

属性过滤选择器

$("div[id]")选取有id属性的div
$("div[title=test]")

还可以选择开头/结束/包含等,可以组合使用

表单对象选择器

$("#form1 :enabled")选取id为form1的表单内所有启用的元素
$("#form1 :disabled")选取id为form1的表单内所有禁用的元素
$("input:checked")选取所有选中的元素(radio,checkbox)
$("select:selectd")
$(":input")选取所有<input>/<textarea>/<select>/<button>, 而$("input")只获得<input>
$(":text")选取所有单行文本框,等价于$("input:[type=text]")
$(":password")选取所有密码框.同理还有:radio,:checkbox,:submit,:image,:rest,:buttion,:file,:hidden

相对选择器

$("li",$(this));//this下的li元素

节点遍历

next()获取紧挨着下一个的元素
nextAll()/nextAll("div")//后一个为待选择性质的
siblings()/siblings("div") 所有的兄弟

链式编程
```
$(this).css("background", "red").siblings("div").css("background", "green");
```
等价于下面的两行:
```
$(this).css("background", "red");
$(this).siblings("div").css("background", "green");
```

attr

$(selector).attr(attribute)返回属性值
$(selector).attr(attribute,value)这只属性值
$("img").attr("width","180");
$(selector).attr(attribute,function(index,oldvalue))通过函数设置属性值
$("img").attr("width",function(index,oldvalue){return oldvalue -50;});
$("img").attr({width:"50",height:"80"});设置属性对

removeAttr()删除属性,之后这个属性就不存在了,不同于清空

创建元素

$(html语句)创建一个dom节点,并返回一个jquery对象

var link = $("<a href='http://www.baidu.com'>baidu</a>")
$("div:first").append(link);

append(newChild)
appendTo(newFather)

prepend在元素的开始添加元素(添加儿子)

after在元素之后添加元素(添加兄弟)

before在元素之前添加元素(添加兄弟)

var link = $('<a href="http://wwww.baidu.com" id="link1">百度</a>');//创建之后还位于内存中

$("#link1").text("新浪");//这个时候修改不成功,因为link还没有加载到页面上,所以找不到这个id的元素

//link.text("腾讯");//这个可以修改

$(document.body).append(link);

$("#link1").text("新浪");//这个可以修改

remove

remove方法的返回值是被删除的对象,还可以继续使用

替换节点

replace

wrap包裹

$("b").wrap("<font color='red'></font>");

radio checkbox

radio与checkbox设值的时候要通过数组的形式,虽然radio只有一个值,但是也必须这样做
```
<input type="radio" name="gender" value="homme" />男<br />
<input type="radio" name="gender" value="femme" />女<br />
<input type="radio" name="gender" value="secret" />未知<br />
$("#btnSet").click(function () {
	   $(":radio[name=gender]").val(["femme"]);
});

$("#btnGet").click(function () {
	   alert($(":radio[name=gender]:checked").val());
});
```

bind

bind注册函数,但是一般用简写形式
```
$("#btn").bind("click",function(){})
```

bind()移除所有注册事件

unbind("click")

hover(overfn,outfn)

光标放上去调用overfn,离开调用outfn

事件e

冒泡/stopPropagation

如果想获得事件的信息(就像dom中window.event中有是事件信息),只需在调用的匿名函数时加一个参数,一般我们用e

jquery中也有事件冒泡,我们可以用stopPropagation来阻止冒泡

preventDefault

e.preventDefault();//阻止默认行为的触发

target

target获得触发事件的元素(如果有冒泡,则为冒泡的起始元素)

pageX,pageY

pageX,pageY与Dom中clientX,clientY

按键

which

如果是鼠标事件,获得按键(1:左键,2:中键,3:右键)

altKey,shiftKey,ctrlKey

keyCode charCode

一次性事件

one()只执行一次的事件

show,hide,toggle

可以有参数,参数表示时间,以毫秒为单位

toggle(),当前为show,就hide;当前为hide,就show

click()模拟点击

当只写click()时,而不写具体的函数时,就相当于进行了一次模拟单击

cookie

cookie是与域名相关的,所以163.com不能读取baidu.com的cookie

同一个域名的另一个页面也能读取

一个域名能写入的cookie总尺寸是有限制的,这次写入的数据,下一次不一定能读到

$.cookie('name', 'value', {expires: 7, path: '/', domain: 'jquery.com', secure: true});

如果expires不设值,则浏览器关闭后,cookie就会被删除
```
$(function () {
	if ($.cookie("username")) {
		$("#username").val($.cookie("username"));
	}
	$("#signIn").click(function () {
		 $.cookie("username", $("#username").val());
		 alert("your name is " + $("#username").val());
	 });
});
<input type="text" name="name" value=" " id="username"/>
<input type="button" name="name" value="signIn" id="signIn" />
```
 
jquery addclass 中class 是指什么?

```
$( "#create-user" ).button().click(function()
$( "#create-user" ):代表底层JQUERY对象 
.button():将底层对象转换成button接口
.click:button接口的事件
```
# AJAX
异步javascript 和 xml
Asynchronous JavaScript and XML

```
WebClient wc = new WebClient();
string s = wc.DownloadString("getServerTime.ashx");
$("#textbox").val(s);
```
创建一个,向web页面发出一个请求,得到数据后,就更新

XMLHTTPRequest就是充当着webclient,这样页面不用刷新就可以获得服务器的数据

XMLHTTPRequest就是ajax的核心对象

IE浏览器版本的ajax 实例
```
<html>
<head>
	<title></title>
<script type="text/javascript">
	function btnclick(){
		var xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
		if(!xmlhttp){
			alert("xmlhttp 创建失败");
			return false;
		}
		xmlhttp.open("POST", "GetDate.ashx", false); // 准备向服务器的GetDate.ashx 页面发出post请求
		// xmlhttp 默认不是同步请求的, 也就是open 方法并不像web client 的downloadstring 那样把服务器返回的数据拿到才返回, 是异步的, 因此需要监听onreadystatechange事件
		xmlhttp.onreadystatechange = function(){
			if(xmlhttp.readystate == 4){
				if(xmlhttp.status == 200){ // 200 表示成功
					document.getElementById("Text").value = xmlhttp.responseText;
				}
				else{
					alert("xmlhttp return error");
				}
			}
		}
		xmlhttp.send(); // 这时才发出请求
	}
</head>
<body>
	<input id="Text1" type="text" /><input id="Button1" type="button" value="button" onclick="btnclick()" />
</body>
</html>
```
因为是异步的,所以无法通过send的返回值得到数据,需要通过事件来监听

readystate

- 0:请求未初始化(还没有调用 open()).
- 1:请求已经建立,但是还没有发送(还没有调用 send()).
- 2:请求已发送,正在处理中(通常现在可以从响应中获取内容头).
- 3:请求在处理中,通常响应中已有部分数据可用了,但是服务器还没有完成响应的生成.
- 4:响应已完成,您可以获取并使用服务器的响应了.

同时我们也可以加入参数
```
xmlhttp.open("POST", "GetDate.ashx?id=1715", false)
```
通过context.Request我们可以得到参数值
```
context.Response.Write( DateTime.Now.ToShortTimeString() +"---"+context.Request["id"]);
```

## Jquery 版的ajax
`$.post`: Load data from the server using a HTTP POST request.

```
functionButton1_onclick(){
	$.post("GetDate.ashx", function (data, Status) {
		if (Status == "success") {//data为返回的数据
			$("#Text1").val(data);
		}
		else {
			alert('fail');
		}
	})
}
```
也可以传入参数
```
jQuery.post = function( url, data, callback, type );
url: String A string containing the URL to which the request is sent.
data: String A map or string that is sent to the server with the request.//最好为字典
callback: Function A callback function that is executed if the request succeeds.
type: String The type of data expected from the server. Default: Intelligent Guess (xml, json, script, or html).
```

## 多进程或多线程的竞争问题
Ajax是以异步的方式向服务器提交需求.对服务器而言,其与传统的提交窗体需求并无不同,而且由于是以异步之方式提交,如果同时有多个Ajax需求及窗体提交需求,将无法保证哪一个需求先获得服务器的响应.
这会造成应用程序典型的多进程(process)或多线程(thread)的竞争(racing)问题.
程序员因此必须自行处理或在JavaScript里面动手脚以避免这类竞争问题的发生(如Ajax需求未响应之前,先disable送出按钮),这又不必要的增加了程序员的负担.
目前已知有自动处理此问题之开发框架似乎只有ZK.

# CSS
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


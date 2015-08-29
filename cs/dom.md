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

`<input type="button" onclick="navigate('HTMLPageForm.htm')" value="navigate demo" />`

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


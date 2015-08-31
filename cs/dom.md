# intro
Document Object Model文档对象模型

CSS+JavaScript+DOM=DHTML

# 定时任务
`var id = setTimeout(fn, delay);`
Initiates a single timer which will call the specified function after the delay(毫秒)
The function returns a unique ID with which the timer can be canceled at a later time.

`var id = setInterval(fn, delay);` 
Similar to setTimeout but continually calls the function (with a delay every time) until it is canceled.

`clearInterval(id);`, `clearTimeout(id);`
Accepts a timer ID (returned by either of the aforementioned functions) and stops the timer callback from occurring.

`setTimeout(myTimeoutFunction, 1000);` vs `setInterval(myTimeoutFunction, 1000);`

- setInterval code executes every 1000ms exactly, while...
- setTimeout waits 1000ms, runs the function (which takes few ms), then sets another timeout. So the wait period is actually a bit more than 1000ms.

# 事件
- srcElement: 获得事件源对象
- keyCode: 发生事件时的按键值
- button: 发生事件时的鼠标按键, 1为左键, 2为右键, 3为左右键同时按

## body, document对象的事件
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

- onchange只有在光标离开时才执行
这样是合理的,例如对于一个text文本框,输入内容的时候,里面的内容一直在改变,如果一改变就触发,那么onchange这个事件就会一直处于触发状态.

- onfocus: 焦点进入控件的事件是
- onblur: 焦点离开控件的事件是

navigate: `<input type="button" onclick="navigate('HTMLPageForm.htm')" value="navigate demo" />`

`window.location.href`
既可以通过它取得当前地址,也可以给它赋值实现navigate的效果

`window.location.reload()`刷新

`window.event` 获得事件发生时的信息,时间不局限于window对象的事件.类似于winForm中的e(EventArg)

`<a href="HTMLPageTable.htm" onclick="alert('禁止访问');window.event.returnValue=false;">hi</a>`
例如用户在注册时,所填信息有误,这个时候就不用提交到服务器,就可以通过这个方式
 
# 对象的操作
setAttribute还可以添加一些html本身没有的属性, 例如:
```
label.setAttribute("studentId", "39241715");//这样可以在以后使用
var id=label.getAttribute(("studentId");//这样就可以取到这个值
```

innerText, innerHTML: 
几乎所有的DOM元素都有innerText与innerHTML两个元素,这两个元素是大小写敏感的,且可以读和写

## 获取对象
- getElementById();
- getElementsByName();

一个页面中name可以不唯一,所以返回的结果是一个数组
```
function btnclick(){
	var radios = document.getElementsByName("gender");
	for(var i = 0; i < radios.length; i++){
		alert(radios[i].value);
	}
}
```

## createElement
动态添加的内容通过查看网页源代码的方式是看不到的,可以通过debugbar查看 

添加到 div 中
```
var div = document.getElementById("divMain");
var input = document.createElement("input");
input.type = "button";
input.value = "je suis dynamique";
div.appendChild(input);
```

添加到 document的body 中
```
var input = document.createElement("<input type='button' value='hello'/>");
document.body.appendChild(input);
```

创建label
```
var label = document.createElement("label");
label.innerText = "user";
label.setAttribute("for", "username");
// username是一个文本框,因为for在js中是一个关键字,所以直接使用label.for="username");是不行的
document.body.appendChild(label);
```

## 特殊对象
### clipboardData对象
对粘贴板的操作
几乎任何元素都有oncopy与onpaste两个属性

- `clearData("Text")`: 清空粘贴板,参数只能时Text(注:可能是为了以后的扩展而这样要求)
- `getData("Text")`: 读取粘贴板的值,返回值为粘贴板中的内容
- `setData("Text",val)`: 设置粘贴板中的值
 
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

### window.history
- window.history
- window.history.back();  window.history.go(-1);后退一个页面
- window.history.forward();  window.history.g0(1);
- window.history.go(2);
```
<a href="javascript:window.history.back()">back</a>

也可以放到input button的onclick事件中
<input type="button" onclick="window.history.back()" value="back"  />
```

### window.document
向文档中写入内容 
```
window.document.write("this is a document<br/>");
window.document.write("<a href='http://www.buaa.edu.cn'>buaa</a><br/>");
```

# style
style.display属性, 控制层显示与否.
只有当值为none时,才不显示,其他值(包括空值)都显示

css中background-color,在js中是backgroundcolor,因为在js中,-有特殊的含义,减号

# form
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


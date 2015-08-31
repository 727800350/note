# intro
jquery对象就是通过jquery封装的dom对象.
jquery对象只能调用jquery封装的方法.不能调用dom对象的方法

可以将一个dom对象转化为jquery对象
```
var div = document.getElementById("div1");
alert($(div).html());
```

同时,也可以将jquery对象转化为dom对象`$(div)[0]` 或者 `$(div).get(0)`

array是javascript语言本身包含的,不是dom对象,所以不需要转换为jquery对象

# 事件
## ready
ready()和dom中的onload类似,但是onload只能注册一次,而ready()可以注册多次
```
$(document).ready(function(){alert("加载完毕1");});
$(document).ready(function(){alert("加载完毕2");});
```

- onload是在所有的dom元素,图片,css等都加载完毕后才会被触发,
- ready 是在所有的dom元素创建完毕后就会被触发,这样可以提高网页的响应速度

`$(window).load()`可以实现onload那种事件调用的时机

由于ready()这个函数很常用,所以jquery对其进行了简化, `$(function(){alert("over");})`

注册事件
```
$(function () {
	$("p").click(function(){
		alert("i am is a p");
	});
});
```
隐式迭代,对取到的每个p都注册click事件

jquery对象都有内置的隐式调用each方法,如果是自己写的对象可以显示调用each方法

# 结构
- `$.map(array,function)`对array中的每个元素调用function函数进行处理,有返回值
- `$.each(array,function)`,没有返回值

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

- $(selector).attr(attribute)返回属性值
- $(selector).attr(attribute,value)这只属性值
- $("img").attr("width","180");
- $(selector).attr(attribute,function(index,oldvalue))通过函数设置属性值
- $("img").attr("width",function(index,oldvalue){return oldvalue -50;});
- $("img").attr({width:"50",height:"80"});设置属性对

`removeAttr()`删除属性, 之后这个属性就不存在了, 不同于清空

# css
```
$(function(){$("#div1").css("background", "red");});
$(function(){alert($("#div1").css("backgroundColor"));});
```

class
```
attr("class")
attr("class","myclass")
```

- addClass
- removeClass
- toggleClass 切换样式,如果没有,则添加,如果有,则删除
- hasClass 判断是否有这个样式

filter:Gray黑白样式

# 选择器
id/tagName/cssClassName选择器

用法: `$("#id ")/$("TagName")/$(".cssClassName")`

例如:
```
$(function(){
	$("p").click(function(){alert("i am is a p");});//隐式迭代,对取到的每个p都注册click事件
});
```

## 多条件选择器

`$("p,div,span.menuitem")`: 选择所有的p标签,div标签和带有menuitem样式的span标签

选择器表达式中的空格不能多也不能少

## 层次选择器

- $("div li")获取div下的所有li元素
- $("div > li")获取div下的直接li元素
- $("body *")选取body下的所有元素
- $("#table img").not(this)

## 过滤选择器

- :first 选取第一个元素 $("div:first")
- :last
- :not  $("input:not(.myclass)")选取不是myclass的input
- :event  :odd 选取奇偶数
- :eq/:gt/:lt选取索引号等于,大于,小于的元素  $("input:lt(5)")
- $(":header")
- $("div:animated")选取正在执行动画的div
 
过滤选择器可以组合使用,但是请注意每一步过滤都是在前一步的过滤所得结果进一步过滤

### 属性过滤选择器

- $("div[id]")选取有id属性的div
- $("div[title=test]")

还可以选择开头/结束/包含等,可以组合使用

## 表单对象选择器

- `$("#form1 :enabled")`选取id为form1的表单内所有启用的元素
- `$("#form1 :disabled")`选取id为form1的表单内所有禁用的元素
- `$("input:checked")`选取所有选中的元素(radio,checkbox)
- `$("select:selectd")`
- `$(":input")`选取所有`<input>/<textarea>/<select>/<button>`, 而$("input")只获得`<input>`
- `$(":text")`选取所有单行文本框,等价于`$("input:[type=text]")`
- `$(":password")`选取所有密码框.同理还有:radio, :checkbox, :submit, :image, :rest, :buttion, :file, :hidden

## 相对选择器

$("li",$(this));//this下的li元素

节点遍历

- next()获取紧挨着下一个的元素
- nextAll()/nextAll("div")//后一个为待选择性质的
- siblings()/siblings("div") 所有的兄弟

# 元素管理
## 创建
$(html语句)创建一个dom节点,并返回一个jquery对象
```
var link = $("<a href='http://www.baidu.com'>baidu</a>")
$("div:first").append(link);
```

- prepend在元素的开始添加元素(添加儿子)
- after在元素之后添加元素(添加兄弟)
- before在元素之前添加元素(添加兄弟)

`var link = $('<a href="http://wwww.baidu.com" id="link1">百度</a>');`
创建之后还位于内存中, 还不能通过 `$("#link1").text("新浪");` 来进行修改(因为link还没有加载到页面上,所以找不到这个id的元素)

`link.text("腾讯");`: 这个可以修改

```
$(document.body).append(link);
$("#link1").text("新浪");
```
这个可以修改, 因为已经将其添加到页面上了

## remove
remove方法的返回值是被删除的对象,还可以继续使用

## replace
replace 替换节点 

wrap包裹: `$("b").wrap("<font color='red'></font>");`

radio checkbox

radio与checkbox设值的时候要通过数组的形式,虽然radio只有一个值,但是也必须这样做
```
<input type="radio" name="gender" value="homme" />男<br />
<input type="radio" name="gender" value="femme" />女<br />
<input type="radio" name="gender" value="secret" />未知<br />
$("#btnSet").click(function(){
	$(":radio[name=gender]").val(["femme"]);
});

$("#btnGet").click(function(){
	alert($(":radio[name=gender]:checked").val());
});
```

# bind

bind注册函数,但是一般用简写形式
```
$("#btn").bind("click",function(){})
```
unbind()移除所有注册事件: `unbind("click")`

show,hide,toggle

可以有参数,参数表示时间,以毫秒为单位

toggle(), 在 show 和 hide 之间转换

click()模拟点击

当只写click()时,而不写具体的函数时,就相当于进行了一次模拟单击

# cookie
一个域名能写入的cookie总尺寸是有限制的,这次写入的数据,下一次不一定能读到

`$.cookie('name', 'value', {expires: 7, path: '/', domain: 'jquery.com', secure: true});`

如果expires不设值,则浏览器关闭后,cookie就会被删除
```
$(function(){
	if($.cookie("username")){
		$("#username").val($.cookie("username"));
	}
	$("#signIn").click(function(){
		$.cookie("username", $("#username").val());
		alert("your name is " + $("#username").val());
	});
});
<input type="text" name="name" value=" " id="username"/>
<input type="button" name="name" value="signIn" id="signIn" />
```

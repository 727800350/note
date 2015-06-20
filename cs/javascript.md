JavaScript(JS) 解释性语言

在浏览器上执行的程序代码,避免与服务器的交互,对服务器造成很大的压力.
只有在迫不得已的情况下才与后台服务器进行交互,列入从前台取数据

# Data Type
typeof六种类型number,string,boolean,object,undefined,function,date

javascript的简单类型,包括number,string,Boolean,null值和undefined值

如果不声明直接使用则这个变量将成为全局变量


类型判断
```
x == null
typeof(x) // object
```

其他所有的值都是对象

对象是属性的容器,其中每个属性都有名字和值,属性的名字可以是包含空字符串在内的所有字符串,值可以是除undefined之外的任何值

对象通过引用来传递,他们永远不会被拷贝

## boolean 真假
```
[1]==[1] //false

alert(/a/ == /a/);//false
alert(false==false);//true

alert(new Date() == new Date());//false
alert(undefined==undefined);//true

alert(null==false);//false
alert(''==false);//true

alert(NaN==false);//false
alert(true=='false');//false

alert(true=='true');//false
alert(null == undefined);//true

alert(null == null);//true
alert(0=='0');//true

alert(true == '1');//true
alert(false=='0');//true

alert('' == 0);//true
alert('55'==55);//true
```
- == 等于 
- === 严格等于, "==" 只要求值相等,如果类型不同,会自动进行转换; "===" 要求值和类型都相等

1. 如果两个值类型相同,进行 === 比较,比较规则同上 
1. 如果两个值类型不同,他们可能相等.根据下面规则进行类型转换再比较: 
	1. 如果一个是null,一个是undefined,那么[相等]. 
	1. 如果一个是字符串,一个是数值,把字符串转换成数值再进行比较. 
	1. 如果任一值是 true,把它转换成 1 再比较,如果任一值是 false,把它转换成 0 再比较. 
	1.  如果一个是对象,另一个是数值或字符串,把对象转换成基础类型的值再比较.对象转换成基础类型,利用它的toString或者valueOf方法.
	js核 心内置类,会尝试valueOf先于toString,例外的是Date,Date利用的是toString转换.非js核心的对象,令说(比较麻烦,我 也不大懂) 
	1. 任何其他组合(array数组等),都[不相等].

## 数字
javascript的数字只有一个类型,全都是double,所以1与1.0是完全一样的

NaN是一个数值,表示不能产生正常结果的运算结果,NaN不等于任何值
```
alert(NaN==NaN);//false
Infinity表示所有大于1.79769313486231570e+308的值
alert(Infinity==Infinity);//true
alert(Infinity==1e400);//true

document.write(1 / 0)//infinity
```

javascript有一个对象Math,里面包含常见的数学函数

产生随机数:`Math.random();`

## 字符串
javascript没有字符类型,要创建字符,只需创建包含一个字符的字符串就可以了

两个包含着完全相同的字符的字符串被认为是一样的字符串

字符串有一个length属性

`\`为转义符

## Function
函数不需要声明返回值类型与参数类型
```
function add(x, y) {
	return x + y;
}
```

匿名函数
```
var add=function (x, y) {
	return x + y;
}
```

匿名函数在JQuery中非常多
```
Alert(function(x,y){return x+y;}(10,20));
Alert(add(10,20));
```

用函数模拟类
```
function Person(name, age) {
	this.Name = name;//动态增加属性
	this.Age = age;
	this.Sayhello = function () {
		alert("Hi " + name + " you are " + this.Age.toString() + " years old");
	}
}
var p = new Person("eric", 22);
p.Sayhello();
```

## Date()
```
var now = new Date();
```

parseInt(string,radix)
value1 = parseInt(value1.value, 10);// 将字符串转换为数字,如果字符串中又非数字型字符,他会尽可能多的解析

 
## Array对象,字典
动态数组
```
var list=new Array();
list[0] = "tom";
list[1] = "jim";
list[2] = "clara";
list.length // 得到3
list[3] // undefined
list[3] == null // true
```

- pop()方法移除最后一个元素
- push()方法添加元素到末尾
```
a = []
a.push(1)
a.push('string')
a.pop()　// 'string'
```
Array也是一个Dictionary

也是一个stack(push()入栈,pop()出栈,同时出栈的这个元素被删除)
```
var list=new Array();
list["ren1"] = "tom";
list["ren2"] = "jim";
list["ren3"] = "clara";
for (var k in list) {
	alert(list[k]);
}

// 通过. 的方式也可以访问
alert(list.ren1);
```

```
var arr = [3, 4, 5, 7, 8];//普通的数组,使用中括号
alert(arr);
```

字典使用大括号
```
var dic = {"tom": 20, "jim": 40 };//key={tom,jim},value={20,40};
for (var key in dic) {
       alert(key);//e是key
       alert(dic[key]);//输出值
}
```

获得对象的所有成员,因为对象的所有成员都是以对象的key形式出现的
for (var e in document) {
	alert(e);//e是key
}

# 三种交互对话框

1. alert
1. confirm
```
function firm(){
	//利用对话框返回的值 (true 或者 false)
	if(confirm("你确信要转去 天轰穿的博客?")){
		//如果是true ,那么就把页面转向thcjp.cnblogs.com
		location.href="http://thcjp.cnblogs.com";
	}
	else{
		//否则说明下了,赫赫
		alert("你按了取消,那就是返回false");
	}
}
```
1. prompt
```
var name = prompt("please input your name", 'eric');//eric是默认值
document.writeln(name);
```

# bind

functionname.bind(thisArg[,arg1[,arg2[,argN]]])

example1:The following example illustrates the use of the bind method.
```
var checkNumericRange = function (value) {
	if (typeof value !== 'number')
		return false;
	else
		return value >= this.minimum && value <= this.maximum;
}
// 如果直接调用 checkNumericRange() 会返回false, 因为this.minimum与this.maximum 还没有指定
// The objForThis argument enables use of the this value within the callback function.
var objForThis = {minimum: 10, maximum: 20 };
var chk = checkNumericRange.bind(objForThis); // objForThis将成为chk的this

// Use the new function to check whether 12 is in the numeric range.
var result = chk(12);
document.write(result); // Output: true
```

出现bind的原因

以下面的例子说明:
```
window.name = "the window object"
function scopeTest() { return this.name}
scopeTest() // 得到 "the window object"
var foo = {name:"the foo object!", otherScopeTest: function(){ return this.name }}
foo.name // 得到 "the foo object"
foo.otherScopeTest()// -> "the foo object!"

// note that we are not calling the function, we are simply referencing it
window.test = foo.otherScopeTest
// now we are actually calling it:
test() // -> "the window object" 因为我们是通过window调用的, 所以this 就是window, 但是test 实际上是foo.otherScopeTest, 
// 我们希望得到"the foo object", 在这种情况下, 我们就可以通过bind 来强行改变this的指向
```

对比
```
var obj = {
  name: 'A nice demo',
  fx: function() {    alert(this.name);  }
};
window.name = 'I am such a beautiful window!';
function runFx(f) {f();}

runFx(obj.fx);// 输出I am such a beautiful window,说明this指向错误
runFx(obj.fx.bind(obj)); //A nice demo, 强行将obj绑定到obj.fx的this上
```

<a href="javascript:alert('hello')">Hello</a>
<a href="HTMLPageTable.htm" onclick="alert('888')">hi</a>

Alert() 相当于winform中的messagebox.show()
<script type="text/javascript" src="common.js"></script>


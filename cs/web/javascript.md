JavaScript(JS) 解释性语言

在浏览器上执行的程序代码,避免与服务器的交互,对服务器造成很大的压力.
只有在迫不得已的情况下才与后台服务器进行交互,列入从前台取数据

注释: `//` 或者 `/* ... */`

# Data Type
javascript的简单类型,包括 number, string, Boolean, null值和undefined值

typeof六种类型:
number, string, boolean, object, function, date, undefined

如果不声明直接使用则这个变量将成为全局变量


类型判断
```
x == null
typeof(x) // object
```

其他所有的值都是对象

对象是属性的容器,其中每个属性都有名字和值,属性的名字可以是包含空字符串在内的所有字符串,值可以是除undefined之外的任何值

<font color="red">对象通过引用来传递,他们永远不会被拷贝</font>

- == 等于, 只要求值相等,如果类型不同,会自动进行转换
- === 严格等于, "===" 要求值和类型都相等

## 数字
javascript的数字只有一个类型, 全都是double, 所以1与1.0是完全一样的

- `parseInt(string, radix)`

javascript有一个模块 Math, 里面包含常见的数学函数

- `Math.random();`: 产生随机数

## 字符串
javascript没有字符类型, 要创建字符, 只需创建包含一个字符的字符串就可以了.

两个包含着完全相同的字符的字符串被认为是一样的字符串

- str.length

## Function
函数不需要声明返回值类型与参数类型
```
function add(x, y){
	return x + y;
}
```

匿名函数
```
var add = function(x, y){
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
function Person(name, age){
	this.Name = name;//动态增加属性
	this.Age = age;
	this.Sayhello = function(){
		alert("Hi " + name + " you are " + this.Age.toString() + " years old");
	}
}
var p = new Person("eric", 22);
p.Sayhello();
```

## Date()
`var now = new Date();`

## Array对象,字典
动态数组
```
var list=new Array();
list[0] = "tom";
list[1] = "jim";
list[2] = "clara";
list.length // 得到3
```

- pop()方法移除最后一个元素
- push()方法添加元素到末尾
```
a = []
a.push(1)
a.push('string')
a.pop()　// 'string'
```

Array也是一个Dictionary, 也是一个stack(push()入栈,pop()出栈,同时出栈的这个元素被删除)
```
var list=new Array();
list["ren1"] = "tom";
list["ren2"] = "jim";
list["ren3"] = "clara";
for (var k in list){
	alert(list[k]);
}
```
// 通过. 的方式也可以访问: `alert(list.ren1);`

```
var arr = [3, 4, 5, 7, 8];//普通的数组,使用中括号
alert(arr);
```

字典使用大括号
```
var dic = {"tom": 20, "jim": 40 }; //key={tom,jim},value={20,40};
for(var key in dic){
	alert(key);//e是key
	alert(dic[key]);//输出值
}
```

获得对象的所有成员,因为对象的所有成员都是以对象的key形式出现的, 例如 document
```
for(var e in document){
	alert(e);
}
```

# 三种交互对话框

1. alert, 在文本里面加入`\n`就可以换行
1. confirm

	```
	function firm(){
		//利用对话框返回的值 (true 或者 false)
		if(confirm("你确信要转去 天轰穿的博客?")){
			location.href="http://thcjp.cnblogs.com";
		}
		else{
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
`functionname.bind(thisArg[,arg1[,arg2[,argN]]])`

example1
```
var checkNumericRange = function(value) {
	if(typeof value != 'number')
		return false;
	else
		return value >= this.minimum && value <= this.maximum;
}
```
如果直接调用 checkNumericRange() 会返回false, 因为this.minimum与this.maximum 还没有指定.  
但是可以为bind 函数指定一个参数, 并让这个参数成为 this 对象, 进而可以在函数里面访问到这个对象的属性信息.
```
var objForThis = {minimum: 10, maximum: 20 };
var chk = checkNumericRange.bind(objForThis);
```
The objForThis argument enables use of the this value within the callback function.

// Use the new function to check whether 12 is in the numeric range.
```
var result = chk(12);
document.write(result); // Output: true
```

出现bind的原因, 用下面的例子说明:
```
window.name = "the window object"
function scopeTest(){ return this.name}
scopeTest() // 得到 "the window object"
var foo = {name:"the foo object!", otherScopeTest: function(){ return this.name }}
foo.name // 得到 "the foo object"
foo.otherScopeTest()// -> "the foo object!"
```
note that we are not calling the function, we are simply referencing it
```
window.test = foo.otherScopeTest
```
now we are actually calling it:
```
test() // -> "the window object" 因为我们是通过window调用的, 所以this 就是window, 但是test 实际上是foo.otherScopeTest, 
```
我们希望得到"the foo object", 在这种情况下, 我们就可以通过bind 来强行改变this的指向

对比
```
var obj = {
	name: 'A nice demo',
	fx: function(){alert(this.name);  }
};
window.name = 'I am such a beautiful window!';
function runFx(f){f();}

runFx(obj.fx);// 输出I am such a beautiful window,说明this指向错误
runFx(obj.fx.bind(obj)); //A nice demo, 强行将obj绑定到obj.fx的this上
```


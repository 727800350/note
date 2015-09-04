JavaScript(JS) 解释性语言

在浏览器上执行的程序代码,避免与服务器的交互,对服务器造成很大的压力.
只有在迫不得已的情况下才与后台服务器进行交互,列入从前台取数据

注释: `//` 或者 `/* ... */`

**strict模式**  
JavaScript在设计之初,为了方便初学者学习,并不强制要求用var申明变量.
这个设计错误带来了严重的后果:如果一个变量没有通过var申明就被使用,那么该变量就自动被申明为全局变量:
启用strict模式的方法是在JavaScript代码的第一行写上: `'use strict';`, 未使用var申明变量就使用的,将导致运行错误.

# Data Type
javascript的简单类型,包括 number, string, boolean, null值和undefined值

typeof六种类型:
number, string, boolean, object, function, date, undefined

类型判断
```
x == null
typeof(x) // object
```
其他所有的值都是对象

对象是属性的容器,其中每个属性都有名字和值,属性的名字可以是包含空字符串在内的所有字符串,值可以是除undefined之外的任何值

**对象通过引用来传递,他们永远不会被拷贝**

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
- str.toUpperCase()
- str.toLowerCase()
- str.indexOf(), 没有找到, 返回 -1
- str.subString(), eg: `str.substring(0, 5);`, `str.substring(7);`

## Array
两种创建数组的方法:

1. `[1, 2, 3.14, 'Hello', null, true];`
1. `new Array(1, 2, 3);`

- array.length
- array.indexOf
- slice 就是对应String的substring()版本
- array.pop()方法移除最后一个元素
- array.push()方法添加元素到末尾
- array.unshift(): 往Array的头部添加若干元素
- array.shift(): shift()方法则把Array的第一个元素删掉
- array.sort: 直接**修改原来的array**
- array.reverse: 直接**修改原来的**
- concat()方法把当前的Array和另一个Array连接起来,并返回一个新的Array, 并**没有修改原来的array**
- join(): 把当前Array的每个元素都用指定的字符串连接起来,然后返回连接后的字符串:

## 对象
Array也是一个对象, 也是一个stack(push()入栈,pop()出栈,同时出栈的这个元素被删除)
```
var list=new Array();
list["ren1"] = "tom";
list["ren2"] = "jim";
for (var k in list){
	alert(list[k]);
}
```
通过.的方式也可以访问: `alert(list.ren1);`

字典使用大括号
```
var person = {
    name: 'Bob',
    age: 20,
    tags: ['js', 'web', 'mobile'],
    city: 'Beijing',
    zipcode: null
};
for(var key in person){
	alert(key);//e是key
	alert(person[key]);//输出值
}
```

如果我们要检测对象是否拥有某一属性,可以用in操作符, 
不过要小心,如果in判断一个属性存在,这个属性不一定是该对象的,它可能是继承得到的.
要判断一个属性是否是对象自身拥有的,而不是继承得到的,可以用hasOwnProperty()方法

## Map 和 Set
JavaScript的默认对象表示方式{}可以视为其他语言中的Map或Dictionary的数据结构,即一组键值对.
但是JavaScript的对象有个小问题,就是键必须是字符串.但实际上Number或者其他数据类型作为键也是非常合理的.
为了解决这个问题,最新的ES6规范引入了新的数据类型Map

初始化Map需要一个二维数组,或者直接初始化一个空Map
```
var m = new Map([['Michael', 95], ['Bob', 75], ['Tracy', 85]]);
m.get('Michael'); // 95
```

- map.get
- map.has
- map.set
- map.delete

## Set
Set和Map类似,也是一组key的集合,但不存储value
```
var s1 = new Set(); // 空Set
var s2 = new Set([1, 2, 3]); // 含1, 2, 3
```
- s.add
- s.delete

## [Iterable](http://www.liaoxuefeng.com/wiki/001434446689867b27157e896e74d51a89c25cc8b43bdb3000/00143450082788640f82a480be8481a8ce8272951a40970000)


# Function
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


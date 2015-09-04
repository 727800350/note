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


## IO
- Console 类: Read从标准输入流读取下一个字符.
- ReadKey ()获取用户按下的下一个字符或功能键.按下的键显示在控制台窗口中.ReadKey(Boolean)获取用户按下的下一个字符或功能键.按下的键可以选择显示在控制台窗口中.
- ReadLine从标准输入流读取下一行字符.
- Write()将指定值的文本表示形式写入标准输出流.


重定向
```
StreamWriter log_out = null;       
log_out = new StreamWriter("logfile.txt");
// Redirect standard out to logfile.txt. 
```

```
Console.SetOut(log_out);
//执行一次程序将内容输出到重定向的文件中,再执行一次程序,上次的内容会消失(好像是文件又重新建的)

Console.SetOut(Console.Out);
Console.in, console.out, console.error
```
文件流
```
FileStream(string path,FileMode mode)
FileMode.Append     //把输出附加到文件末端
FileMode.Create     //创建一个新的输出文件,已存在的文件会被销毁

FileMode.createNew  //
FileMode.open       //打开一个已经存在的文件

FileMode.openOrCreate       //
FileMode.Truncate   //打开一个已经存在的文件,文件的长度减小为0
```

## Data Type
对象之间的赋值是引用赋值,类似于指针的形式,而不是按值赋值

```
int i = new int(); // initialize i to zero
int j = new int();
j = i;//这样还是按值赋值的,i与j的地址是不一样的
```

- String.Trim: 返回一个新字符串,它相当于从当前String 对象中移除了一组指定字符的所有前导匹配项和尾随匹配项.
- Trim      从当前 String 对象移除所有前导空白字符和尾部空白字符.
- string.IsNullOrEmpty()//判断空值最好用这个方法

```
Int.tryparse(str,out num);//返回true or false
int.Parse()将数字的字符串表示形式转换为其等效的 32 位有符号整数.
int num = int.Parse(Console.ReadLine());
```
 
string之间的赋值是按值赋值的

Convert类的静态方法用于支持与 .NET Framework 的基础数据类型之间的转换. 
受支持的基类型是 Boolean, Char, SByte, Byte, Int16, Int32, Int64, UInt16, UInt32, UInt64, Single, Double, Decimal, DateTime 和 String.

# ASP.NET
请求request,处理process,响应response

用户访问的是.aspx/.ashx,而不是htm页面,htm页面仅仅是一个模板,用户是没有权限访问htm页面的

提交到服务器的表单元素一定要有name属性

id是给Dom与Jquery用的,提交到服务器的表单元素可以没有id,服务器只认name

所有的提交内容都是以name为key,value为value的dictionary.

放到form中的元素只要有name属性,都会被提交到服务器

通过表单得到的数据都是string类型的


只有input,select,textarea有了name属性后才能将value属性向服务器提交数据,其他(div等等)不能提交

要想得到div中的值,可以利用input(hidden)进行同步更新, 例如:
```
<input type="hidden" name="num" value="@value" /><div>@value</div>
```
但是在aspx页面中,label的text属性也能实现数据的修改,可是他使用了viewstate,其原理仍然是是使用隐藏的input,而TextBox就不用viewstate,因为TextBox本身就是input控件


method有两种get与post

- get(默认值),通过url传递表单值,get传递的数据量是有限的
- post,传递的表单值隐藏在http报文中,url中看不到

## cookie
浏览器向服务器提交数据的时候,会自动默认将这个域名下的cookie提交到服务器, 
在服务器端修改了cookie之后,服务器会自动将cookie返回到浏览器

设置
```
Context.Response.SetCookie( new HttpCookie( "color", TextBox1.Text ) );
```
读取
```
TextBox2.Text = Context.Request.Cookies["color"].Value;
```

同时,由于同一个域名下的cookie都会被提交到服务器,所以对于大的网站,比如yahoo中国http://cn.yahoo.com/ 及其图片 http://image.yahoo.cn/,
使用两个不同的域名(可以在同一个服务器上,只要域名不同就可以了),部分原因就是为了防止请求一张图片的时候,将这个域名下的cookie都发送给服务器,导致流量增加

## Page
我们创建的web窗体实现了,WebForm类:`public partial class WebForm1:System.Web.UI.Page`

而webform类是继承自page类,page类继承自IHttpHandler类

`public class Page:TemplateControl, IHttpHandler`

每当客户端浏览器,向服务器发出一个请求,服务器都将new一个IHttpHandler的对象实例进行处理,处理完之后返回结果,然后这个实例就被GC(Garbage Collection)掉了

## aspx/ashx
html内容用aspx处理,非html内容用ashx处理,例如生成动态图片等

因为aspx是专门为html内容而量身定做的,用aspx来处理图片,很可能会做无用功,导致效率降低

.aspx文件就是一个html模板,只不过他的控件是专用的,JavaScript与css代码也写到.aspx文件中,控制页面的长相

.aspx.cs是对应的aspx的后台代码,控制程序逻辑

这样前台aspx,后台cs代码的方式就是codebehind

cs文件中可以调用aspx中的控件,aspx中也可以访问cs文件的字段,aspx文件中实际上也可以写C#代码,但是一般不这样做,除非只是非常简单的代码
```
<%for ( int i = 0; i < 10; i++ )
 {Response.Write( "haha<br/>" ); }
%>
<%=name %>  //其中name为cs中定义的字段,可以使public或protected,但不能是private.
```
=的意识是输出,相当于response.write(name);也可以=接一个函数,这个时候输出函数返回值

## session会话
存储到服务器的与浏览器用户相关的数据

不要在session中放大量数据,session有超时的可能,服务器不可能知道浏览什么时候开着,什么关了

ASP.Net已经内置了session(注意:session不是Http内置的),且实现了超时等其他的问题

数据是object类型,注意类型转换

session是跨页面的,一个浏览器的不同页面访问同一个服务器,session是一样的

内置session的使用

但是我们通过浏览器的cookie查看不到,可能是由于asp.net用的什么方法将其隐藏起来了

设置:`Session["browser"] = "ie9";`  
取:`TxtSession.Text = Session["browser"].ToString( );`

## http
连接connection, 请求request, 响应response

返回代码

- 200代表处理成功
- 301永久转移
- 404页面找不到
- 500内部服务器错误

C#代码运行在服务器端,javascript代码运行在客户端,
对于服务器来说,生成的html代码仅仅是一长串字符串,没有其他的任何意义,只有到了客户端浏览器才能做进一步的解释

客户端验证不能代替服务端验证,因为客户端可能会禁用javascript,甚至客户端会伪造http请求,
客户端校验只是方便用户快速知道输入结果正确与否,免得页面一直刷新,而服务器端校验是为了最后把关,不可少

后面讲到的ASP.Net Validation就是为了方便进行数据校验的

服务端的visible是false时,是真的隐藏了,服务器没有将这个控件渲染到浏览器,即使在客户端查看源代码也是看不到了,
用`$("#控件id").show()`也无法将控件显示

XSS跨站脚本漏洞 Cross Site Scripting
不要轻信用户提交上来的数据  
解决方法:输出到浏览器的时候将用户提交的javascript中的<>/等替换成对应的html转义符,例如<换成&lt;  
在ASP.Net通过HttpUtility.HtmlEncode(string s)这个函数将s解释成对应的转义符, 
HttpUtility.HtmlDecode(string s)在将s转回来

**Literal控件**  
在网页上保留显示静态文本的位置,此控件可用来显示用户输入,而该输入可能包含恶意的客户端脚本.
在应用程序中显示从客户端发送来的任何信息之前,请检查它们是否包含可执行脚本,SQL 语句或其他代码

这个控件有一个mode属性,将其设置为encode就可以了

## request对象

AppRelativeCurrentExecutionFilePath:获取应用程序根的虚拟路径,并通过对应用程序根使用波形符 (~) 表示法(例如,以"~/page.aspx"的形式)使该路径成为相对路径

PhysicalApplicationPath:获取当前正在执行的服务器应用程序的根目录的物理文件系统路径.

MapPath 方法 :为当前请求将请求的 URL 中的虚拟路径映射到服务器上的物理路径.

string fullpath = context.Server.MapPath( "hello2.htm" );

UrlReferrer 属性:获取有关客户端上次请求的URL 的信息,该请求链接到当前的URL,可以用来防盗链

例如对于一个图片:如果UrlReferrer为null,则说明用户直接访问图片的地址

如果UrlReferrer.host属性值不为自己的主机地址(域名),则说明是别人盗用了这个图片

但是这个UrlReferrer.host是由客户端浏览器以http报文的形式提交到服务器的,因此是可以伪造的,而迅雷就是这么干的,他把大量的host地址保存下来,当用户在他的网站点击下载链接时,他就把对应的真实host地址添加到http报文中

HttpRequest.UserHostAddress 属性:获取远程客户端的 IP 主机地址.

生成一个图片(.ashx)
```
context.Response.ContentType = "image/Jpeg";
string fullpath = HttpContext.Current.Server.MapPath( "ASP.net.jpg" );
using ( System.Drawing.Bitmap map = new System.Drawing.Bitmap( fullpath ) ){
	using ( System.Drawing.Graphics g = System.Drawing.Graphics.FromImage( map ) ){
		g.Clear( System.Drawing.Color.White );
		g.DrawString( "yes", new System.Drawing.Font( "宋体", 12 ), System.Drawing.Brushes.Red, new System.Drawing.PointF( 0, 0 ) );
	}
	map.Save( context.Response.OutputStream, System.Drawing.Imaging.ImageFormat.Jpeg );
}
```

## response对象
asp.net向浏览器write的时候并不会每write一次,就立刻渲染输出到浏览器上,而是会缓存数据,到合适的时候在发送到客户端

- `public bool BufferOutput { get; set; }`如果缓冲发送给客户端的输出,则为 true,否则为 false. 默认值为 true
- `HttpResponse .Flush` 方法 :强制向客户端发送当前所有缓冲的输出.
- `HttpResponse .Clear`: 清除缓冲区流中的所有内容, 但不输出到浏览器.

Outputsteam: 处理非文本的二进制流, 例如图片, 文件等 (如果是文本就用output)

End 方法: 将当前所有缓冲的输出发送到客户端, 停止该页的执行, 并引发 EndRequest 事件.

HttpResponse .Redirect 方法:将客户端重定向到新的 URL,可以是图片.

WriteFile(string filename)方法: 将指定文件的内容作为文件块直接写入 HTTP 响应输出流,但是对于大的文件直接让用户下载,不要输出到浏览器,否则服务器的性能会受到影响

## Server
server是context的一个属性,是HttpServerUtility类的一个对象

Server.HtmlDecode(), Server.HtmlEncode()
Server.UrlEncode()//将中文转换成url地址
server.transfer 重定向,但是这个是服务器内部的接管,浏览器不知道已经重定向了,浏览器地址栏不变,但是实际上已经重定向了,
而response的redirect是让浏览器重新访问新的网址.因为是内部接管, 会将原来的cookie等信息提交到新的地址, 所以浏览器是可以访问到原来的cookie等信息的, 
而redirect后, 是访问不到这些原来的信息的, 因为是重新访问一个页面, 如果想要知道这些信息, 可以将信息主动传过去.

但是transfer是不能定向到外部的网站的,只能定向到内部的网站

transfer不能直接重定向到ashx页面,需要做一些处理

联想到电话呼叫中心,客户有一个问题打电话到呼叫中心请教一个问题,但是接电话的这个小姐人不知道答案,所以它有两种选择 

1. 告诉另外一个号码,让客户打过去,那么这个就是response的redirect,此时,客户需要将他的问题在重新告诉那个新的小姐,就相当于是没有带着参数的 ;
2. 小姐把电话直接转到知道答案的人,那么这就是server.transfer,小姐肯定会把客户的问题告诉她认为知道答案的人,就相当于是带着参数的

有的时候不能直接拿到HttpContext对象,比如在Global.asax中, 可以通过HttpContext.Current拿到当前的HttpContext, 进而拿到request,response,server等

## HttpHander
HttpHander是对请求的响应,可以输出html,也可以输出图片,视频,excel等文件,只要是http协议支持的东西都可以输出
能直接文件以流的形式向浏览器输出,就不要生成临时文件放到服务器上在将浏览器重定向到此临时文件
需要用户下载文件时,要在context.Response.WriteFile( "ASP.jpg" );之前加入AddHeader
```
context.Response.AddHeader( "Content-Disposition", "attachment;filename=ASP.jpg" );
```

## page
request,response,server,ispostback,session

request,response,server是page的属性,对context.request, context.response, context.server的简化调用

AppRelativeVirtualPath获得页面相对于应用根路径的路径,例如:~/login.aspx

FindCtrol

FindCtrol(ctrlId),返回类型为ctrol类似于getelementById
ResolveClientUrl/ ResolveUrl

ResolveClientUrl(url),传递一个虚拟路径,返回一个客户端访问时的路径,且会考虑当前页面的路径,尽量生成短路径

ResolveUrl(url) 传递一个虚拟路径,返回一个相对于网站根目录的路径

ResolveClientUrl("~/a/b.htm") 得到a/b.htm

ResolveUrl("~/a/b.htm")得到/website1/ a/b.htm

## 服务端控件
内网系统与互联网的后台部分用服务端控件是很好的,开发方便,对性能几乎没有影响互联网系统的前台用服务端控件,可能会生成大量的无用代码,致使性能下降,所以用的html控件比较多,
大部分asp.net控件都是从control或webcontrol类继承来的,几乎所有的成员都有asp.net控件是经过asp.net高度封装的控件,是运行在服务端的,最终会被渲染成html控件

ClientID:控件在服务端的ID与在客户浏览器端的ID可能不一样,所以为避免发生错误,在javascript中必须用ClientID来获取控件的客户端ID

javascript代码最好写在<script>块中,直接写在控件的OnClientClick里" 与'字符会被解析错误
```
OnClientClick="document.getElementById('<%=TextBox1.ClientID %>').value='yes';"
```

上面的这句话会被asp.net渲染成下面的,导致执行时发生错误

```
onclick="document.getElementById(&#39;&lt;%=TextBox1.ClientID %>&#39;).value=&#39;yes&#39;"
document.getElementById('<%=TextBox1.ClientID %>').value = 'yes';
<%=TextBox1.ClientID %>//这段代码是在服务器端运行的
```

visible:如果为false,则不仅在页面上看不到,在页面的html代码中也看不到,因为根本就没有渲染到html代码中,是真正的不可见

attributes:

Button1.Attributes["picName"] = "small.jpg";//存放有用信息

Button1.Attributes["onmouseover"] = "alert('hell0');";//可以这么干,但是不推荐,还是在aspx页面中使用jquery比较好,这里使用只是为了说明这么干原理上是可以的

label

单独使用的时候,会被asp.net引擎渲染成一个span, 
但是当他为关联label(AssociatedContrlID)时,他会被渲染成一个代for属性的html中的label,他会被关联到AssociatedContrlID所指向的控件的clientID

literal:不会被渲染成任何的标签,只是展示一段文本,而且会自动的进行html encode,从而避免XSS攻击

textbox:

- mode有singleline,multiline,password,会被分别渲染成text,textarea,password类型的input
- autopostback当这个属性为true时,当焦点离开文本框时,表单会自动提交

asp.net要提交表单时,最好调用_doPostBack()方法

radiobutton GroupName,会被渲染成html中的radio类型的input与name属性

button/linkbutton/imagebutton

imagebutton与button点击后会自动提交,而linkbutton是调用_doPostBack提交

linkbutton会被渲染成一个超链接,但是他的href为javascript代码,不要用它来实现普通的超链接功能

imagebutton被渲染成一个image类型的input

这三个button都有commandName,commandArgument,command这三个属性,通过command可以共享函数,但是在函数里面通过e.commandName和e.commandArgument来进行不同的处理

panel

panel控件用来盛放一些控件,便于一起移动位置,被渲染成一个div,如果设置groupingtext,他就被渲染成一个包含fieldset的div

hyperlink

hyperlink:超链接,但是当要引用内部资源时,使用hyperlink比较好,因为他会自动的进行路径的转换(绝对路径与虚拟路径),当使用imageurl属性时,会成为一个图片超链接,会被渲染成b包含<img>标签的<a>

fileupload

被渲染成file类型的input

hasfile属性判断文件是否存在,filebytes以二进制形式得到filecontent以流的形式,postedfile

savefile需要绝对路径(server.mappath/  virtualpathultility.toabsolute)

webshell

如果不做任何限制,上传文件后,会有漏洞,用户可能会上传aspx及其cs文件,然后用户访问这个aspx页面(加一些恶意代码),这个页面中的代码就会被执行,预防措施,限制上传文件的类型,上传的文件的存放路径下的文件不允许执行

网站注册

网站注册用户时,注册成功后,一定要重定向到另外一个地址,因为asp.net默认的是post提交,所以当用户注册后,如果不立刻强制重定向,这时一旦用户点了刷新,这时就会重复注册,这个情况用户肯定会很不理解为什么会这样.

html控件

对于html控件,asp.net不会对他进行任何的处理,仅仅把他们当成普通的字符串输出到客户端

但是将html控件加上runat="server"后,就变成了运行在服务器端的控件,只不过与真正的asp.net控件相比,被渲染的要少很多,很多属性设置还是要向html那样使用,但是如果要通过javascript使用这些控件,要向asp.net的控件那样使用,通过<%=txt1.ClientID%>这个来使用控件的id
```
<input type="text" id="txt1" runat="server" />
```

在cs代码中:txt1.Value = "hello";

注:所以的html控件都可以添加runat="server", 甚至非法的html标签(不是html标准的标签)也可以添加runat="server"

form-HtmlForm;a- HtmlAnchor;head- HtmlHead;input- HtmlInputButton, HtmlInputCheckBox, HtmlInputText; meta- HtmlMeta; table- HtmlTable; tr- HtmlTableRow; td- HtmlTableCell,title- HtmlTitle

未单独封装的标签(如div,label)与非法标签会是HtmlContainerControl类型的标签

用户控件

用户创建的一个控件组合页面,可以重复利用

asp.net控件
```
<asp:TextBox ID="TextBox1" runat="server"></asp:TextBox>
<asp:Button ID="Button1" runat="server" Text="Button" />
<script type="text/javascript">
    document.getElementById("<%=Button1.ClientID %>").onclick = function () {
    document.getElementById("<%=TextBox1.ClientID %>").value = "<%=Button1.ClientID %>" + "<%=TextBox1.ClientID %>";     }
</script>
```
如果按照下面的写

```
<asp:TextBox ID="TextBox1" runat="server"></asp:TextBox>
<asp:Button ID="Button1" runat="server" Text="Button"  OnClientClick="myclick()"/>
<script type="text/javascript">
    function myclick() {
    document.getElementById('<%=TextBox1.ClientID %>').value='yes';
    }
</script>
```
在一个页面中使用两个用户控件时,asp.net引擎会将这个myclick函数渲染成两个同名的函数,如下所示,就会产生歧义,所以在用户控件中要使用手动注册函数,如上面所示
```
<script type="text/javascript">
function myclick() {
   document.getElementById('WebUserControl11_TextBox1').value = "WebUserControl11_Button1" + "WebUserControl11_TextBox1";
}
</script>
function myclick() {
         document.getElementById('WebUserControl12_TextBox1').value = "WebUserControl12_Button1" + "WebUserControl12_TextBox1";
}
</script>
```
当在同一个页面使用多个由用户控件页面生成的控件时,在使用javascript操作控件时,如果是纯的html控件,则控件的id会冲突,
但是运行在服务器端的html控件与asp.net控件一样,控件id不会冲突,asp.net引擎会自动处理id的问题(在前面加上usercontrol的号),例如WebUserControl11$TextBox1这个就是WebUserControl下的TextBox1控件

html  server控件

像asp.net一样使用

html控件

用户控件中这么写:

```
<input type="button" id="btn1" value="click" />
<input type="text" id="txt1" />

<script type="text/javascript">
    document.getElementById("btn1").onclick= function () {
    document.getElementById("txt1").value = "hello";
    }
</script>
```

会被渲染成:

```
<input type="button" id="btn1" value="click" />

<input type="text" id="txt1" />
<script type="text/javascript">
     document.getElementById("btn1").onclick= function () {
     document.getElementById("txt1").value = "hello";
}
</script>
<br />
<input type="button" id="btn1" value="click" />
<input type="text" id="txt1" />
<script type="text/javascript">
     document.getElementById("btn1").onclick= function () {
     document.getElementById("txt1").value = "hello";
}
</script>两段代码是一样的,这样就导致了id出现了重复
```

验证控件

asp.net封装的一些验证控件,会同时进行客户端与服务器端的数据验证

page.isvalid

如果客户端禁用了javascript,那么即使使用了验证控件,并且服务器端的验证没有通过,button_click方法仍会执行,所以要在于服务器的数据库交互前要确保数据都正确,这是就需要使用page的isvalid属性(只有页面的所有验证都通过,才为真)

requiredfiledvalidator 字段必填

ErrorMessage="不能为空"    

ControlToValidate="TextBox1"   指向需要验证的控件

initialvalue当绑定的控件value与initialvalue相同时,仍被视为空值

rangevalidator 数据范围

可以使string,integer,double,date,currency等类型

空值是合法的,如果要求不为空,可以再加一个requiredfiledvalidator验证控件

comparevalidator

operator:datatypecheck,equal,greaterthan, greaterthanequl,lessthan, lessthanequl,notequal

还可以用来与另一个控件的值进行比较

controltocompare属性设置为operator右边的控件

regularexpressionvalidator正则表达式

在设计界面可以看到几个常用的正则表达式,在源页面不知道为什么不显示

http(s)?://([\w-]+\.)+[\w-]+(/[\w- ./?%&=]*)?  url

\w+([-+.']\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*       电子邮件地址

(\(\d{3}\)|\d{3}-)?\d{8}             中国电话号码

\d{17}[\d|X]|\d{15}    中国居民身份证号

\d{6}      中国邮政编码

customvalidator自定义

通过args.value获得需要校验的值,如果合法就将args. IsValid设为true

客户端校验函数ClientValidationFunction

function name(src,args){}  // src,args 这两个参数是必须的,因为是回调函数args.value,args.IsValid,和服务端校验一样

 

display

所有验证控件都有display属性,用来决定如果现实错误信息,有三个值static,dynamic,none

static:没有错误信息的时候控件的visibility样式为hidden来实现隐藏

dynamic:没有错误信息的时候控件的display样式为none来实现隐藏

这两者的区别是visibility:hidden隐藏控件仍会占据网页空间,而display:none隐藏则不会占据空间

validationgroup

如果在一个页面总同时实现注册和登陆的功能(都经过验证控件的绑定),注册与登陆会同时进行数据校验,这是就会出现一个问题,当登陆的时候,注册那么的数据校验通过不过,解决方法,将相应的控件及其验证控件的validationgroup都设为相同的值,就可以把他们分为一组,相同组的控件的数据校验才会被触发

causevalidation

如果将按钮控件的causevalidation设置为false,则这个按钮的点击不触发数据的校验(当这个按钮的作用不是要提交数据时)

validationsummary

当只填errmessage时,这条errormessage就会显示在验证控件的位置上,当页面中有一个validationsummary控件且验证控件text属性不为空时(一般我们会填上*),则验证控件上会显示text的值,而所有的errormessage(一般我们会解释错误原因)会集中显示在validationsummary控件上

masterpage母版

母版页挖坑,具体页面填坑

母版页中会出现如下所示的ContentPlaceHolder,在子页面中,我们只能在  
<asp:ContentPlaceHolder ID="head" runat="server">

</asp:ContentPlaceHolder>
<asp:ContentPlaceHolder ID="ContentPlaceHolder1" runat="server">

</asp:ContentPlaceHolder>
 

然后在子页面,会出现
<asp:Content ID="Content1" ContentPlaceHolderID="HeadContent" runat="server">

</asp:Content>
然后在content中填入内容就可以了,在子页面中,我们只能把内容填入content标签内,包括javascript脚本也要写到content里面

实际操作中,我们一般是通过html页面与css样式表文件来设计母版的布局(便于及时查看),在html页面中设计好了之后,将html页中body部分复制到masterpage中

注意要在母版页中添加form,将所有的内容添加到一个form中

在使用masterpage时一定要注意引用的资源的路径问题

在使用asp.net与html server控件时,使用相对路径,服务端控件的虚拟路径会被自动转换成绝对路径

在使用html控件时,使用绝对路径或使用ResolveClientUrl将虚拟路径路径转换成绝对路径

<img src='<% =ResolveClientUrl("~/images/bjxz.png") %>'  />

如果在子页面中要使用masterpage的控件,要使用Master.FindControl方法

Button btn=(Button)this.Master.FindControl("Button1");

数据绑定

联动机制,数据源的数据变了,数据绑定控件显示的数据就会变;反之亦然

bindingControl-bindingSource(dataSource)-dataset

数据绑定控件通过数据源(数据绑定源)来获得数据,通过数据源隔离数据提供者和数据使用者.

数据源有SqlDataSource, AccessDataSource, ObjectDataSource, LinqDataSource, EntityDataSource, XmlDataSource, SiteMapDataSource

SqlDataSource,AccessDataSource会直接连接数据库,破坏了网站的分层结构,dal,bal,ui三层,会变成一个ui层,所以这两个几乎就用不到,除非是是用于演示

objectdatasource是最通用的数据源

列表数据绑定控件:dropdownlist,radiobuttonlist,listbox,checkboxlist,bulletedlist

复杂控件:datagrid(2.0之后就被扔掉了), gridview, detailsview, formview, listview, repeater, datalist

listview继承了之前所有的控件的优点,但是如果简单到了极值,就用repeater

ObjectDataSource将一个类作为数据源, 大多时候这个类是强类型dataset, typename属性为数据源类的全名, deletemethod, insertmethod, selectmethod, updatemethod, 参数的值有deleteparameters, updateparameters, insertparameters

objectdatasource

手工写objectdatasource很麻烦,使用vs可视化工具就很方便

生成的代码如下所示

<asp:ObjectDataSource ID="ObjectDataSource1" runat="server"
             DeleteMethod="Delete" InsertMethod="Insert"

OldValuesParameterFormatString="original_{0}"   SelectMethod="GetData"
             TypeName="ASP.DAL.DataSet1TableAdapters.UsersTableAdapter"

             UpdateMethod="Update">
             <DeleteParameters>

                 <asp:Parameter Name="Original_Id" Type="Int32" />
             </DeleteParameters>

             <InsertParameters>
                 <asp:Parameter Name="Username" Type="String" />

                 <asp:Parameter Name="Password" Type="String" />
                 <asp:Parameter Name="Level" Type="Int32" />

             </InsertParameters>
             <UpdateParameters>

                 <asp:Parameter Name="Username" Type="String" />
                 <asp:Parameter Name="Password" Type="String" />

                 <asp:Parameter Name="Level" Type="Int32" />
                 <asp:Parameter Name="Original_Id" Type="Int32" />

             </UpdateParameters>
</asp:ObjectDataSource>

 
DropDownList

通过一个DropDownList来使用这个objectdatasource

<asp:DropDownList ID="DropDownList1" runat="server" AppendDataBoundItems="True"
DataSourceID="ObjectDataSource1" DataTextField="Username" DataValueField="Id">

<asp:ListItem>我</asp:ListItem>
</asp:DropDownList>

其中<asp:ListItem>我</asp:ListItem>是我手动添加的数据,手动添加之后,需要将AppendDataBoundItems 属性设置为true,意味着是添加数据. DataTextField是指给用户显示的数据, DataValueField是实际的值(给后台使用的)

被渲染成的html为

     <select name="DropDownList1" id="DropDownList1">
     <option value="我">我</option>

     <option value="1">admin</option>
     <option value="2">test</option>

     </select>
 

手工绑定

也可以手工绑定,数据只要实现了IEnumerable接口都可以绑定

protected void Page_Load ( object sender, EventArgs e )

         {

              if ( !IsPostBack )

              {

                   BulletedList1.DataSource = new int[] { 1, 23, 56, 78 };

                   BulletedList1.DataBind( );//刷新数据

              }

         }

但是推荐使用vs工具

repeater

repeater只是负责遍历数据库的每一行,然后itemtemplate决定行以什么方式展示数据

<asp:Repeater ID="Repeater1" runat="server" DataSourceID="ObjectDataSource1">

<ItemTemplate>姓名:<%#Eval("Username")<br /></ItemTemplate>

 </asp:Repeater>//其中Username为数据库中的字段名称

甚至可以显示到文本框中

<ItemTemplate>姓名:<input type="text" value='<%#Eval("Username") %>' /><br /></ItemTemplate>

 

隔行使用不同的格式:

<asp:Repeater ID="Repeater1" runat="server" DataSourceID="ObjectDataSource1">
    <ItemTemplate>Name:<%#Eval("Username") %><br /></ItemTemplate>

    <AlternatingItemTemplate>
<div style="background-color:red">Name:<%#Eval("Username") %></div><br /> </AlternatingItemTemplate>

</asp:Repeater>  
 

HeaderTemplate:头部显示

FooterTemplate:底部显示

 SeparatorTemplate:两行之间显示

用table显示数据

<asp:Repeater ID="Repeater1" runat="server" DataSourceID="ObjectDataSource1">       <ItemTemplate><tr><td>Username:</td><td><%#Eval("Username") %></td></tr></ItemTemplate>
<HeaderTemplate><table></HeaderTemplate>

<FooterTemplate></table></FooterTemplate>
</asp:Repeater>

 

对于图片,视频等文件,网站都是将图片,视频等文件的路径存放到数据库,而不是将这些文件直接存放到数据库中

对于服务端控件,不能写'~/images/*<%#Eval("PicPath")%>',这样在被asp.net引擎渲染的时候,会将"等符号渲染成&ldquo; ,要解决这个问题可以用拼接字符串的方法,但是更好的解决办法是在cs页面中增加一个FormatImgUrl方法,参数为object类型,在方法里面使用ResolveClientUrl将参数url转换成绝对路径,然后在aspx中使用的时候就是'<%#FormatImgUrl(Eval("PicPath"))%>'      //(因为aspx页面为相应的cs页面的子类,所以aspx页面中可以调用cs页面(父类)中的方法)

对于这个问题,用html控件就不会出现这个问题,而且还可以遵循我们的原则,能不用服务器端控件就不用服务端控件

ItemDataBound

对于每行数据(包括header与footer)显示的时候都会调用itemdatabound事件,在这个事件中可以对当前行进行处理,事件对象主要成员:

e.item.itemtype为当前行的类型,item为itemtemplate行,alternatingitem为alternatingitemtemplate行,还有header,footer等取值,为listitemtype的枚举值

e. item.itemindex为当前行的序号

e. item. DataItem为当前行绑定的对象 

-DataItem /{System.Data.DataRowView}/ object {System.Data.DataRowView}

Row/ {ASP.DAL.DataSet1.UsersRow} /System.Data.DataRow {ASP.DAL.DataSet1.UsersRow}如果要在itemdatabound事件中队itemtemplate模板中的控件进行处理,则必须使用asp.net控件或html server控件,为控件设置id,然后用findcontrol找到控件

 

例如:其中有一个表叫Users,Age为其中一个字段,实现的功能是将年龄大于30的文本框背景色设置为红色

protected void Repeater1_ItemDataBound ( object sender, RepeaterItemEventArgs e )
        {

            DataRowView rowview = (DataRowView)e.Item.DataItem;
            ASP.DAL.DataSet1.UsersRow userrow = (ASP.DAL.DataSet1.UsersRow)rowview.Row;

            if ( userrow.Age>30 )
            {

                TextBox txtAge = (TextBox)e.Item.FindControl( "textAge" );
                txtAge.BackColor = Color.Red;

            }
           

        }
itemcommand

commandname  命令的名字,通过commandname来判断进行不同的操作

系统有几个内置的commandname,Delete,Edit,
commandargument 传过去的参数

 
<ItemTemplate>

<%#Eval("Username") %>
<asp:TextBox runat="server"  ID="textAge" Text=' <%#Eval("Age") %>'/>

<asp:Button ID="Button1" runat="server" Text="长一岁" CommandName="IncAge" CommandArgument='<%#Eval("Id") %>' />
<asp:Button ID="Button2" runat="server" Text="减一岁" CommandName="Des" CommandArgument='<%#Eval("Id") %>' />

<br />
</ItemTemplate>

protected void Repeater1_ItemCommand ( object source, RepeaterCommandEventArgs e )
        {

            if ( e.CommandName=="IncAge" )
            {

                int id = Convert.ToInt32( e.CommandArgument );
                var adapter = new ASP.DAL.DataSet1TableAdapters.UsersTableAdapter( );

                adapter.IncAgeById( id );
                Repeater1.DataBind( );//强制刷新数据

            }
            else if ( e.CommandName=="Des" )

            {
                int id = Convert.ToInt32( e.CommandArgument );

                var adapter = new ASP.DAL.DataSet1TableAdapters.UsersTableAdapter( );
                adapter.DecAgeById( id );

                Repeater1.DataBind( );
            }

        }
 

上面的代码:通过一个函数来处理两个不同的过程,通过commandname来判断是哪个过程
listview

bind

bind是双向绑定,既可以返回又可以修改数据库的数据

而eval是单向的绑定,只能返回数据库中的数据

如果要使用删除,编辑等功能,数据库必须要有主键



上面图片上分别是处于不同状态时的相应的模板

itemPlaceholder

在<LayoutTemplate>中有一个<tr ID="itemPlaceholder" runat="server"></tr>,这是一个占位符, 在<LayoutTemplate>与itemPlaceholder之间的是header,在itemPlaceholder与</LayoutTemplate>之间的是footer,这个itemPlaceholder是必须的,但是不一定是tr
 

repeater与listview中都是可以设置validation的
ItemCreated

ItemCreated,新增行与编辑行时都会触发这个事件

可以通过这个事件为新增数据行设置默认值

DropDownList gender = (DropDownList)e.Item.FindControl( "ddgender" );

 

if ( e.Item.ItemType==ListViewItemType.InsertItem )

            {//findcontrol,设置默认值

               

            }

iteminserting

正在插入数据的时候.在浏览器上已修改,就要写到数据库中之前,

应用:例如数据库中某一个字段为guid,这个值不是有用户指定,而是系统自动生成的

通过e.values[key]取得相应的值

可以在插入的时候,如果发现数据有误,可以强制不插入

e.cancel=true;

itemupdating

找控件的时候 e.Item.FindControl( "ddgender" );,这个方法不行

DropDownList gender = (DropDownList)ListView1.Items[e.ItemIndex].FindControl( "ddgender" );

e.NewValues["gender"] = gender.SelectedValue;

dropdowmlist

由于listview对dropdownlist封装的不够好,很多代码updating与created需要我们自己写

ItemDataBound事件:显示的时候为dropdownlist绑定数据

(如果要显示为dropdownlist的时候,这个时候需要禁用dropdownlist的选择功能)

updating事件:当通过dropdownlist修改数据的时候,写进数据库的事件

created事件:当编辑的时候,显示的数据

对insert,item界面初始化放在itemcreated比在itemdatabound更好

inserting事件:插入的数据写到数据库

ItemUpdating

protected void ListView1_ItemUpdating ( object sender, ListViewUpdateEventArgs e )
        {

            DropDownList gender = (DropDownList)ListView1.Items[e.ItemIndex].FindControl( "ddgender" );
            e.NewValues["gender"] = gender.SelectedValue;

        }
 

ItemCreated

protected void ListView1_ItemCreated ( object sender, ListViewItemEventArgs e )
        {                      

                if ( e.Item.ItemType==ListViewItemType.DataItem )
            {

                DropDownList gender = (DropDownList)e.Item.FindControl( "ddgender" );
                if ( gender!=null )

                {
                    DataRowView rowview = (DataRowView)e.Item.DataItem;                

                    if ( rowview != null )
                    {

                        listview.DAL.DataSet1.T_UsersRow userrow = (listview.DAL.DataSet1.T_UsersRow)rowview.Row;
                        if ( userrow.gender=="Male" || userrow.gender=="Female" )

                        {
                            gender.SelectedValue = userrow.gender;

                        }                      
                    }                  

                }                  
            }

        }
 

行处理

在ListView1_ItemCommand中,

用intindex=e.Item.DisplayIndex;取得当前的行号

int id = (int)ListView1.DataKeys[index].Value;// ListView1.DataKeys取得所有keys,然后再去第index个的值
这个取得的字段是可以设置的DataKeyNames="id",也可以设置多个

例如DataKeyNames="name,id",这时就需要通过values来取得
排序

在layouttemplate中添加一个asp:button

将commandname="Sort" //必须为Sort

commandargument 为排序的字段值

但是这个内置的排序效率比较低(尤其当数据量比较大的时候,需要我们自己写排序的)

datapager分页

pagesize:每页显示的条目数

NextPreviousPagerField
buttontype=Button Link Image三种类型

ButtonCssClass

NumericPagerField

buttoncount:显示的数字个数
NumericButtonCssClass

TemplatePagerField用户自定义
 

先从数据源取得所有数据,然后在截取当前页面的部分,在数据量非常大的情况下效率非常低,因此默认分页基本不能用
row_number()

row_number() over(order by id) as rownum

 
SELECT   id, name,age,gender,row_number() OVER (ORDER BY age)

FROM      T_Users
 

row_number()是结果集的行号(从1开始),不是表的行号
 

SELECT   * FROM     
(SELECT   id, name, age, gender, row_number() OVER (ORDER BY id) AS rownum

FROM      T_Users)  AS t
WHERE   t .rownum > 2

高效率分页

startRowIndex   maximumRows
 

SelectMethod="GetPagedData"//这个查询是返回一个值,得到数据的总数目,这样才能根据pagesize知道一共要分多少页
SelectCountMethod="QueryCount"//这个查询是得到当前页的条目

EnablePaging="true"
 

GetPagedData 的代码如下
SELECT COUNT(*) FROM T_Users

 
QueryCount的代码如下

select * from
(SELECT   id, name, age, gender row_number() over(order by id) as rownum

FROM      T_Users) as t
where t.rownum>@startRowIndex and t.rownum<=@startRowIndex+@maximumRows

且要为这个查询手动添加参数
 

行的单独页码编辑

对于字段比较多的情况下,单击编辑后跳转到另个一个页面编辑
编辑ListViewEdit.aspx?id=22&action=edit//使用的时候用<%#Eval("Id")%>

新增ListViewEdit.aspx?action=addnew
查看(listviewedit.aspx?id=22&action=view)

 
对数据进行单条编辑的控件formview

对formview切换状态:formview1.changemode(Formmode.edit)
formmode有edit,insert,readonly三个值

由于formview默认的只是显示一条数据,所以需要在强类型dataset中增加一个查询GetDataById,只返回我们需要的一条数据
然后将formview绑定的objectdatasource的GetData方法设置为这个查询

参数源设置为QueryString
QueryStringField设置为id//因为上面我们用的是id

然后在page_load根据action更改formview的状态
 

完成以后,在重定向到原来的页面
updated/inserted事件里完成重定向到列表界面

 
模板中的控件不能通过ID直接引用,要通过findcontrol

在itemcreated中为控件添加javascript事件
通过Attributes

 
或者直接模板中添加一个literal,然后在itemcreated事件中将literal的text设置为我们需要操控的控件的id
```
<script type="text/javascript">
$('#<asp:Literal ID="litID" runat="server"></asp:Literal>').datapicker
</script>
```
 

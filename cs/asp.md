[安装与配置](http://www.jb51.net/article/29787.htm)
打开IIS
```
控制面板 --- 程序与功能  --- 打开或关闭Windows功能 --- Internet 信息服务"
```

IIS管理
```
控制面板 --- 系统和安全 --- 管理工具 --- Internet 信息服务(IIS)管理器
```

配置
1. 选中"Default Web Site",双击中间的"ASP" 把"启用父路径"改为"True"
1. 回到IIS,单击右侧的"绑定...",选中你要绑定的网站,点"编辑"
1. 网站绑定
```
类型 http 端口 80 ip地址 *
```
如果是一台电脑,只修改后面的端口号就行,可以随意修改数字.
如果是办公室局域网,点击下拉框,选择自己电脑上的局域网IP,例如:`192.168.**.**`,然后修改端口号

[ASP基础和应用环境配置](http://www.doc88.com/p-7804749230148.html)

`<% %>` 里面的脚本为server 端的脚本, 在server 上执行, 然后server将结果传给client
```
<%
	result = 5 - 6 * 8
	response.write "answer is:"&result
%>
```
或者用下面的
```
<Script Language="VBScript" RUNAT="Server">
	result = 5 - 6 * 8
	response.write "answer is:"&result
</Script>
```

`<Script></Script>` 标签默认是在client 端执行的, 也就是在浏览器中执行.

尝试那些按钮事件, 不知道为什么都失败了

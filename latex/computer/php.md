**php安装好之后**  
现在可以在浏览器中打开 http://localhost 或 http://127.0.0.1, 看到 Apache 2 Test Page 页面

新建`/var/www/html/test.php`文件, 然后输入如下内容:

	<?php
		phpinfo();
	?>

再用浏览器中打开 http://localhost/test.php 或 http://127.0.0.1/test.php  你会看到很多模块都可以在 PHP5 中使用了

**服务与目录**

- #service httpd restart      /重启apache服务
- /etc/httpd/conf/httpd.conf    /apache的配置文件
- /var/log/httpd               /日志文件目录,这里的文件很容易变的很大,需要提供足够的空间
- /var/www/html                /这里是 CentOS 默认的"首页"目录

# 习惯
为了避免关于编码的不必要的麻烦, 将所有文件设置为utf-8编码, 文件名不要含有中文.\\
[编码转换工具](../../bash/gbkTOutf8.sh)

	header('Content-type: text/html; charset=utf-8');

inc 通常指的是include的简写,表示这个文件被其他(多个)文件引用,当然最好写成 a.inc.php,
这样的好处一是含义依然明确,而是避免将代码直接被访客下载(这种情况有时很严重,比如密码或者密码加密算法)  
类似的,还有这样一些常用后缀名:  
.class.php   类文件  
.ini.php     配置文件

PHP文件最后空一行的原因是什么?  
PHP FIG中提及: All PHP files MUST end with a single blank line. -- 来源
主要有两个理由:

1. 某些工具(特别是比较古老的),如果文件的末尾没有\n或\r,就会忽略最后一行.最后有一个空行,便于判断这个文件传输完整(而不是只传了一半)
1. 文件的以空行结尾是Unix的惯例

# Form 
	<html>
	<body>
		<?php 
			if(!empty($_POST['name'])){
				echo "Greetings, {$_POST['name']}, and welcome.";
			}
		?>
		<form action="<?php $PHP_SELF; ?>" method="post">
			Enter your name: <input type="text" name="name" />
			<input type="submit" />
		</form>
	</body>
	</html>

## upload files
参数含义:

	$_FILES["file"]["name"] - the name of the uploaded file
	$_FILES["file"]["type"] - the type of the uploaded file
	$_FILES["file"]["size"] - the size in bytes of the uploaded file
	$_FILES["file"]["tmp_name"] - the name of the temporary copy of the file stored on the server
	$_FILES["file"]["error"] - the error code resulting from the file upload

# Database
php 使用mysql有两个方式, 一种是面向过程式的, 一种是面向对象式的, 官方推荐使用面向对象式的.

[面向对象式的demo](../../demo/php/db/mysql_object-oriented.php)

[面向过程式的demo](../../demo/php/db/mysql_procedure-oriented.php)

[微软的sqlserver, access, excel 连接](../../demo/php/db/ms.php)

**`$count = $$result->fetch_row()[0]` 错误:   
PHP Fatal error:  Call to a member function query() on a non-object in xxx.php. 

必须使用:

	$row = $result->fetch_row();
	$count = $row[0];
不知道为什么


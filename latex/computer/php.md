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

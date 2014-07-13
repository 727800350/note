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

# Database
php 使用mysql有两个方式, 一种是面向过程式的, 一种是面向对象式的, 官方推荐使用面向对象式的.

[面向对象式的demo](../../demo/php/db/mysql_object-oriented.php)

[面向过程式的demo](../../demo/php/db/mysql_procedure-oriented.php)

[微软的sqlserver, access, excel 连接](../../demo/php/db/ms.php)


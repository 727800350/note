# intro
Nginx ("engine x") 是一个高性能的 HTTP 和 反向代理 服务器,也是一个 IMAP/POP3/SMTP 服务器. 

因它的稳定性,丰富的功能集,示例配置文件和低系统资源的消耗而闻名.

# nginx 命令
重启 ngnix, 在 webserver/sbin/ 目录下 `./nginx -s reload`

# FAQ
**css 加载失败**

odp/webserver/conf/vhost/php.conf
```
server{
	location ~* .(jpg|gif|png|js|css)$ {
		root /home/img/bin/odp/webroot;
		if (-f $request_filename) {
			expires max;
			break;
		}
	}
}
```
<link href="./css/common.css" rel="stylesheet" type="text/css"/>
这个相对目录的root 是 配置文件中

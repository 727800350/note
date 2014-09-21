#encoding: utf-8
#!/usr/bin/python

import string

def main():

	# 这个是 str 的字符串
	s = '关关雎鸠'

	# 这个是 unicode 的字符串
	u = u'关关雎鸠'

	print isinstance(s, str)      # True
	print isinstance(u, unicode)  # True

	print s.__class__   # <type 'str'>
	print u.__class__   # <type 'unicode'>

	print u + "english"
	print u + u"中文"

	# 从 str 转换成 unicode
	print s.decode('utf-8')   # 关关雎鸠

## 	因为中文无法用 ascii 字符集编码, 下面的代码报错
## 	UnicodeEncodeError: 'ascii' codec can't encode characters in position 0-3: ordinal not in range(128) 
## 	print u.encode('ascii')   # 关关雎鸠

	# 从 unicode 转换成 str
## 	显示乱码
	print u.encode('utf-8')   # 关关雎鸠

	# 用 gbk 编码含中文的 unicode 字符串  
	print u.encode('gbk')  # 正确,因为 '关关雎鸠' 可以用中文 gbk 字符集表示  
    # '\xb9\xd8\xb9\xd8\xf6\xc2\xf0\xaf'  
    # 直接 print 上面的 str 会显示乱码,修改环境变量为 zh_CN.GBK 可以看到结果是对的


	# 用 ascii 解码 utf-8 字符串  
	s.decode('ascii')  
	# 错误,中文 utf-8 字符无法用 ascii 解码  
	## 	UnicodeDecodeError: 'ascii' codec can't decode byte 0xe5 in position 0: ordinal not in range(128)  
  
	# 用 gbk 解码 utf-8 字符串  
	s.decode('gbk')  
## 	不出错,但是用 gbk 解码 utf-8 字符流的结果,显然只是乱码  
##  u'\u934f\u51b2\u53e7\u95c6\u5ea8\u7b2d'  
if __name__ == "__main__":
	main()

陷阱一:在进行同时包含 str 与 unicode 的运算时,Python 一律都把 str 转换成 unicode 再运算,当然,运算结果也都是 unicode.
陷阱二:一旦你的 Python 代码是用管道 / 子进程方式运行,sys.stdout.encoding 就会失效,让你重新遇到 UnicodeEncodeError.

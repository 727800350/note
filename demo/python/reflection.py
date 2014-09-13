#encoding: utf-8
#!/usr/bin/python

import sys
import inspect

class Cat(object): # 类,Cat指向这个类对象
	def __init__(self, name='kitty'):
		self.name = name
	def sayHi(self): #  实例方法,sayHi指向这个方法对象,使用类或实例.sayHi访问
		print self.name, 'says Hi' # 访问名为name的字段,使用实例.name访问

cat = Cat("eric") # cat是Cat类的实例对象

print Cat.sayHi # 使用类名访问实例方法时,方法是未绑定的(unbound)
print cat.sayHi # 使用实例访问实例方法时,方法是绑定的(bound)

# 这个会报错, unbound method sayHi() must be called with Cat instance as first argument
## Cat.sayHi() 
Cat.sayHi(cat)
cat.sayHi()


## 访问对象的属性
cat = Cat('kitty')

print cat.name # 访问实例属性
cat.sayHi() # 调用实例方法

print dir(cat) # 获取实例的属性名,以列表形式返回
if hasattr(cat, 'name'): # 检查实例是否有这个属性
	setattr(cat, 'name', 'tiger') # same as: a.name = 'tiger'
print getattr(cat, 'name') # same as: print a.name

getattr(cat, 'sayHi')() # same as: cat.sayHi()

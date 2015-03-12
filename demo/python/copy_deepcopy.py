import copy

class Person:
	def __init__(self, name):
		self.name = name
		self.list = [1,2,3]
	def Print(self):
		print self.name
		print self.list

a = Person("a")
a.Print()
## a
## [1,2,3]

b = copy.copy(a)
b.Print()
## a
## [1,2,3]
b.name = "b"
b.list.append("b")
b.Print()
## b
## [1,2,3,'b']

a.Print()
## a
## [1,2,3,'b']

print
a = Person("a")
c = copy.deepcopy(a)
c.Print()
## a
## [1,2,3]
c.name = "c"
c.list.append("c")
c.Print()
## c
## [1,2,3,'c']

a.Print()
## a
## [1,2,3]


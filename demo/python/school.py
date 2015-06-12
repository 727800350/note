#!/usr/bin/python
## python class demo, inherience

"""
class SchoolMember(object) the string object must be needed
or the call super(Student, self).__init__(name, age) will throw an error

TypeError: super() argument 1 must be type, not classobj

Reason
class SchoolMember is not declared as a "new-style" class.

super() and all subclass/superclass stuff only works with new-style classes. 
I recommend you get in the habit of always typing that (object) on any class definition to make sure it is a new-style class.

Old-style classes (also known as "classic" classes) are always of type classobj; new-style classes are of type type. 
"""

class SchoolMember(object):
	'''Represents any school member.'''
	def __init__(self, name, age):
		self.name = name
		self.age = age
		print'(Initialized SchoolMember: %s)'% self.name

	def tell(self):
		'''Tell my details.'''
		print'Name:"%s" Age:"%s"'% (self.name, self.age),

class Teacher(SchoolMember):
	'''Represents a teacher.'''
	def __init__(self, name, age, salary):
## 		super(Teacher, self).__init__(name, age)
		SchoolMember.__init__(self, name, age)
		self.salary = salary
		print'(Initialized Teacher: %s)'% self.name

	def tell(self):
		SchoolMember.tell(self)
		print'Salary: "%d"'% self.salary

class Student(SchoolMember):
	'''Represents a student.'''
	def __init__(self, name, age, marks):
		super(Student, self).__init__(name, age)
## 		SchoolMember.__init__(self, name, age)
		self.marks = marks
		print'(Initialized Student: %s)'% self.name

	def tell(self):
		SchoolMember.tell(self)
		print'Marks: "%d"'% self.marks

t = Teacher('Mrs. Shrividya',40,30000)
s = Student('Swaroop',22,75)
members = [t, s]
for member in members:
	member.tell()# works for both Teachers and Students

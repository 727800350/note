#encoding: utf-8
#!/usr/bin/python

s = set([ 'a' , 'b' , 'c'])   # 集合对象 s 
print len(s)  # 集合s 的长度 
## 3 
print 'a' in s  # 元素'a' 在集合 s 中, 返回布尔类型 
## True 
print 'd' not in s  # 元素'd' 不在集合 s 中, 返回布尔类型 
## True 
t = set(['a' , 'b' , 'c' , 'd'])   # 集合对象t 
print s.issubset(t)  # s 是否是t 的子集, 等价于 s <=t 
## True 
print s.issuperset(t)  # s 是否是t 的超集, 等价于 s >=t 
## False 
print s.union(t)  # 集合的并, 等价于 s |t 
## set(['a' , 'c' , 'b' , 'd']) 
print s |t 
## set(['a' , 'c' , 'b' , 'd']) 
print s. intersection(t)  # 集合的交, 等价于 s &t 
## set(['a' , 'c' , 'b']) 
print s &t 
## set(['a' , 'c' , 'b']) 
print s.difference(t)  # 集合的差, 等价于 s -t 
## set([]) 
print s - t 
## set([]) 
print t.difference(s) 
## set(['d']) 
print t - s 
## set(['d']) 
print s.symmetric_difference(t)  # 集合的异或, 等价于 s ^t  
## set(['d']) 
print s^t 
## set([ 'd']) 

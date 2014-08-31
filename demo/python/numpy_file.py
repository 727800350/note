#encoding: utf-8
#!/usr/bin/python

import numpy as np

y = np.array([2.,4.,6.,8.])

y.tofile("myfile.dat") # binary format
y.tofile("myfile.txt", sep='\n', format = "%e") # ascii format, one column, exponential notation

print np.fromfile('myfile.dat', dtype=float)
# array([ 2., 4., 6., 8.])

print np.fromfile('myfile.txt', dtype=float, sep='\n')
# array([ 2., 4., 6., 8.])

## format = "%d", 结果如下
## 2
## 4
## 6
## 8
## format = "%e"
## 2.000000e+00
## 4.000000e+00
## 6.000000e+00
## 8.000000e+00

#encoding: utf-8
#!/usr/bin/python

import numpy as np

a0 = np.array([2,3,4])

a = np.arange(15).reshape(3, 5)
print a
## array([[ 0,  1,  2,  3,  4],
##        [ 5,  6,  7,  8,  9],
##        [10, 11, 12, 13, 14]])

b0 = np.array( [ (1.5,2,3), (4,5,6) ] )

b = np.array( [ [1.5,2,3], [4,5,6] ] )

c = np.array( [ [1,2], [3,4] ], dtype=complex )
print c
## array([[ 1.+0.j,  2.+0.j],
##        [ 3.+0.j,  4.+0.j]])

alist = [1, 2, 3] 
arr = np.array(alist)

a = np.arange(12)
a.shape = 3,4   # changes the shape of a

np.zeros((3,4) )
np.ones((2,3,4), dtype = np.int16 ) 
np.empty( (2,3) )  ## random value
np.arange( 10, 30, 5 )  # 由10 到30, 间隔5
np.array([10, 15, 20, 25]) 
np.array([ 0. ,  0.3,  0.6,  0.9,  1.2,  1.5,  1.8])
np.linspace( 0, 2, 9 )                 # 9 numbers from 0 to 2
np.array([ 0.  ,  0.25,  0.5 ,  0.75,  1.  ,  1.25,  1.5 ,  1.75,  2.  ])
x = np.linspace( 0, 2 * np.pi, 100 )        # useful to evaluate function at lots of points
f = np.sin(x)
## print f

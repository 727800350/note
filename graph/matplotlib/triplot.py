#!/usr/bin/python
#encoding: utf-8

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.tri as tri

data = np.random.rand(100, 2)
## import the matplotlib.tri module, which provides helper functions to compute triangulations from points.
triangles = tri.Triangulation(data[:,0], data[:,1])

plt.triplot(triangles)
plt.show()


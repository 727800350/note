#!/usr/bin/python
#encoding: utf-8

import numpy as np
import matplotlib.pyplot as plt

def basic():
	data = np.random.rand(1024, 2)

	plt.scatter(data[:,0], data[:,1])
	plt.show()

def common_color():
	A = np.random.standard_normal((100, 2))
	A += np.array((-1, -1)) # Center the distrib. at <-1, -1>
	B = np.random.standard_normal((100, 2))
	B += np.array((1, 1)) # Center the distrib. at <1, 1>

	plt.scatter(A[:,0], A[:,1], color = '.25')
	plt.scatter(B[:,0], B[:,1], color = '.75')
	plt.show()

def marker():
	A = np.random.standard_normal((100, 2))
	A += np.array((-1, -1))
	B = np.random.standard_normal((100, 2))
	B += np.array((1, 1))

	plt.scatter(A[:,0], A[:,1], color = 'k', marker = 'x')
	plt.scatter(B[:,0], B[:,1], color = 'k', marker = '^')
	plt.show()

basic()
common_color()
marker()


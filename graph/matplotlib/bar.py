#!/usr/bin/python
#encoding: utf-8

import numpy as np
import matplotlib.pyplot as plt
data = [[5., 25., 50., 20.],
	[4., 23., 51., 17.],
	[6., 22., 52., 19.]]

# Plotting multiple bar charts
## hard code of position
def plot_static():
	X = np.arange(4)

	width = 0.2
	plt.bar(X + width * 0, data[0], color = 'b', width = width)
	plt.bar(X + width * 1, data[1], color = 'g', width = width)
	plt.bar(X + width * 2, data[2], color = 'r', width = width)

	plt.show()

def plot_dynamic():
	color_list = ['b', 'g', 'r']
	gap = .8 / len(data) ## number of rows
	for i, row in enumerate(data):
		X = np.arange(len(row))
		color = color_list[i % len(color_list)]
		plt.bar(X + i * gap, row, width = gap, color = color)
	plt.show()

# Plotting stacked bar charts
## plot B upon A
def plot_upside():
	A = [5., 30., 45., 22.]
	B = [5., 25., 50., 20.]
	X = range(4)
	plt.bar(X, A, color = 'b')
	plt.bar(X, B, color = 'r', bottom = A)
	plt.show()

def plot_upside_dynamic():
	data = np.array([[5., 30., 45., 22.],
			[5., 25., 50., 20.],
			[1., 2., 1., 1.]])
	color_list = ['b', 'g', 'r']
	X = np.arange(data.shape[1])
	for i in range(data.shape[0]):
		plt.bar(X, data[i], bottom = np.sum(data[:i], axis = 0), color = color_list[i % len(color_list)])
	plt.show()

# Plotting back-to-back bar charts
def plot_back2back():
	women_pop = np.array([5., 30., 45., 22.])
	men_pop = np.array( [5., 25., 50., 20.])
	X = np.arange(4)
	plt.barh(X, women_pop, color = 'r')
	plt.barh(X, -men_pop, color = 'b')
	plt.show()

## plot_static()
## plot_dynamic()

## plot_upside()
## plot_upside_dynamic()

plot_back2back()


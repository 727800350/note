#!/usr/bin/python
#encoding: utf-8

import numpy as np
import matplotlib.pyplot as plt
data = [[5., 25., 50., 20.],
        [4., 23., 51., 17.],
        [6., 22., 52., 19.]]

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

## plot_static()
plot_dynamic()


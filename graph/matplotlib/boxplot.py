#!/usr/bin/python
#encoding: utf-8

import numpy as np
import matplotlib.pyplot as plt

def plot_one():
	data = np.random.randn(100)
	plt.boxplot(data)
	plt.show()

def plot_five():
	data = np.random.randn(100, 5)
	plt.boxplot(data)
	plt.show()

## plot_one()
plot_five()


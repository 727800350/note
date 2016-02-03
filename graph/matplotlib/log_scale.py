#!/usr/bin/python
#encoding: utf-8

import numpy as np
import matplotlib.pyplot as plt

def plot_normal():
	X = np.linspace(1, 10, 1024)
	plt.plot(X, X, c = 'k', lw = 2., label = 'f(x)=x')
	plt.plot(X, 10 ** X, c = '.75', ls = '--', lw = 2., label = 'f(x)=e^x')
	plt.plot(X, np.log(X), c = '.75', lw = 2., label = 'f(x)=log(x)')
	plt.legend()
	plt.show()

def plot_ylog():
	X = np.linspace(1, 10, 1024)
	plt.yscale('log')
	plt.plot(X, X, c = 'k', lw = 2., label = 'f(x)=x')
	plt.plot(X, 10 ** X, c = '.75', ls = '--', lw = 2., label = 'f(x)=e^x')
	plt.plot(X, np.log(X), c = '.75', lw = 2., label = 'f(x)=log(x)')
	plt.legend()
	plt.show()

plot_normal()
plot_ylog()


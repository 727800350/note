#!/usr/bin/python
#encoding: utf-8

import matplotlib.pyplot as plt

X = range(10)
Y = [value ** 2 for value in X]
plt.plot(X, Y)
plt.show()


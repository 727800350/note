#!/usr/bin/python
#encoding: utf-8

import numpy as np
import matplotlib.pyplot as plt

X = np.random.randn(1000)
plt.hist(X, bins = 20)
plt.show()


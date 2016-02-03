#!/usr/bin/python
#encoding: utf-8

import numpy as np
import matplotlib.pyplot as plt

def plot_axe_default():
    plt.plot(2 * np.cos(T), np.sin(T), c = 'k', lw = 3)
    plt.title("ellipse with default axe scale")
    plt.show()

def plot_axe_equal():
    plt.plot(2 * np.cos(T), np.sin(T), c = 'k', lw = 3)
    plt.axes().set_aspect('equal')
    plt.title("ellipse with equal axe scale")
    plt.show()

T = np.linspace(0, 2 * np.pi, 1024)
plot_axe_default()
plot_axe_equal()


#!/usr/bin/python
#encoding: utf-8

"""A very simple MNIST classifier.
See extensive documentation at http://tensorflow.org/tutorials/mnist/beginners/index.md
"""
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import argparse
import sys
from tensorflow.examples.tutorials.mnist import input_data
import tensorflow as tf

parser = argparse.ArgumentParser()
parser.add_argument('--data_dir', action = 'store', dest = 'data', type = str, default = './data/mnist', help = 'input data')
arg = parser.parse_args()

## data set handler
mnist = input_data.read_data_sets(arg.data, one_hot = True)

## input and label
x = tf.placeholder(tf.float32, [None, 784])
y_ = tf.placeholder(tf.float32, [None, 10])

## model
W = tf.Variable(tf.zeros([784, 10]))
b = tf.Variable(tf.zeros([10]))
y = tf.matmul(x, W) + b
'''
here use matmul(x, W), because x is batched, that is there are many instances in a single x
W * x is not appropriate
'''

## loss function using cross_entropy
'''
The raw formulation of cross-entropy,
    tf.reduce_mean(-tf.reduce_sum(y_ * tf.log(tf.nn.softmax(y)), reduction_indices=[1]))
can be numerically unstable.
So here we use tf.nn.softmax_cross_entropy_with_logits on the raw outputs of 'y', and then average across the batch.
'''
cross_entropy = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(y, y_))

## train
train_step = tf.train.GradientDescentOptimizer(0.5).minimize(cross_entropy)

# run the model training
sess = tf.Session()
sess.run(tf.global_variables_initializer())
for _ in range(100):
	batch_xs, batch_ys = mnist.train.next_batch(100)
	sess.run(train_step, feed_dict={x: batch_xs, y_: batch_ys})
## print(sess.run([W, b]))

# Test trained model
correct_prediction = tf.equal(tf.argmax(y, 1), tf.argmax(y_, 1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
print(sess.run(accuracy, feed_dict={x: mnist.test.images, y_: mnist.test.labels}))

sess.close()


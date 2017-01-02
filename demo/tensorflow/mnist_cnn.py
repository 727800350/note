#!/usr/bin/python
#encoding: utf-8

import argparse
import sys
from tensorflow.examples.tutorials.mnist import input_data
import tensorflow as tf

def weight_variable(shape):
	intial = tf.truncated_normal(shape, stddev = 0.1)
	return tf.Variable(intial)

def bias_variable(shape):
	intial = tf.constant(0.1, shape = shape)
	return tf.Variable(intial)

def conv2d(x, W):
	return tf.nn.conv2d(x, W, strides = [1, 1, 1, 1], padding = "SAME")

def max_pool_2x2(x):
	return tf.nn.max_pool(x, ksize = [1, 2, 2, 1], strides = [1, 2, 2, 1], padding = "SAME")

def main():
	parser = argparse.ArgumentParser()
	parser.add_argument('--data_dir', action = 'store', dest = 'data', type = str, default = './data/mnist', help = 'input data')
	arg = parser.parse_args()

	## input data
	mnist = input_data.read_data_sets(arg.data, one_hot = True)
	x = tf.placeholder(tf.float32, [None, 784])
	y_ = tf.placeholder(tf.float32, [None, 10])

	## first convolutional layer
	w_conv1 = weight_variable([5, 5, 1, 32])
	b_conv1 = bias_variable([32])
	x_image = tf.reshape(x, [-1, 28, 28, 1])
	h_conv1 = tf.nn.relu(conv2d(x_image, w_conv1) + b_conv1)
	h_pool1 = max_pool_2x2(h_conv1)

	## second convolutional layer
	w_conv2 = weight_variable([5, 5, 32, 64])
	b_conv2 = bias_variable([64])
	h_conv2 = tf.nn.relu(conv2d(h_pool1, w_conv2) + b_conv2)
	h_pool2 = max_pool_2x2(h_conv2)

	## densely connected layer
	n_fc1 = 1024
	w_fc1 = weight_variable([64 * 7 * 7, n_fc1])
	b_fc1 = bias_variable([n_fc1])
	h_pool2_flat = tf.reshape(h_pool2, [-1, 64 * 7 * 7])
	h_fc1 = tf.nn.relu(tf.matmul(h_pool2_flat, w_fc1) + b_fc1)

	## dropout
	keep_prob = tf.placeholder(tf.float32)
	h_fc1_drop = tf.nn.dropout(h_fc1, keep_prob)

	## readout layer
	n_fc2 = 10
	w_fc2 = weight_variable([n_fc1, n_fc2])
	b_fc2 = bias_variable([n_fc2])
	y_conv = tf.matmul(h_fc1_drop, w_fc2) + b_fc2

	## train
	cross_entropy = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(y_conv, y_))
	train_step = tf.train.AdamOptimizer(1e-4).minimize(cross_entropy)
	correct_prediction = tf.equal(tf.argmax(y_conv, 1), tf.argmax(y_, 1))
	accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))

	# run
	sess = tf.Session()
	sess.run(tf.global_variables_initializer())
	for i in range(20000):
		batch_xs, batch_ys = mnist.train.next_batch(50)
		if i % 100 == 0:
			train_accuracy = sess.run(accuracy, feed_dict = {x: batch_xs, y_: batch_ys, keep_prob: 1.0})
			print("step %d training_accuracy: %g" % (i, train_accuracy))
			sess.run(train_step, feed_dict={x: batch_xs, y_: batch_ys, keep_prob: 0.5})

	test_accuracy = sess.run(accuracy, feed_dict = {x: batch_xs, y_: batch_ys, keep_prob: 1.0})
	print("test accuracy: %g" % test_accuracy)
	sess.close()

if __name__ == "__main__":
	main()

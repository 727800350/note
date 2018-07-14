Tensor就是张量,张量有多种

- 0阶张量: 标量, 比如[1]
- 1阶张量: 向量, 比如[1,2]
- 2阶张量: 矩阵, 比如[[1,2,3],[3,4,5],[5,6,7]]

- `global_step` refers to the number of batches seen by the graph. `tf.train.get_global_step()`: The global step variable, or None if none was found.
- `dataset.shuffle(buffer_size=1000)`: Randomizes input using a window of 1000 elements(read into memory)

## layers
`tf.nn.conv2d` 和 `tf.layers.conv2d`

- 对于卷积来说, 作用是一样的. `tf.layers.conv2d` 使用`tf.nn.convolution`作为后端.
- `tf.layers.conv2d` 参数丰富, 一般用于从头训练一个模型.
- `tf.nn.conv2d`, 一般在下载预训练好的模型时使用. filter是一个卷积核Tensor,由tf.Variable生成,在加载已经训练好的权值时很有用

## loss function
### softmax
softmax模型可以用来给不同的对象分配概率
[How to choose cross-entropy loss in tensorflow?](https://stackoverflow.com/questions/47034888/how-to-choose-cross-entropy-loss-in-tensorflow)

A logit(also called a score) is a raw unscaled value associated with a class, before computing the probability.
In terms of neural network architecture, this means that a logit is an output of a dense(fully-connected) layer.

Tensorflow naming is a bit strange: all of the functions below accept logits, not probabilities, and apply the transformation themselves (which is simply more efficient).

Softmax functions family

- `tf.nn.softmax_cross_entropy_with_logits_v2`
- `tf.losses.softmax_cross_entropy`

These loss functions should be used for multinomial mutually exclusive classification, i.e. pick one out of N classes. Also applicable when N = 2.

The labels must be one-hot encoded or can contain soft class probabilities: a particular example can belong to class A with 50% probability and class B with 50% probability.
Note that strictly speaking it doesn't mean that it belongs to both classes, but one can interpret the probabilities this way.

Sparse functions family

- `tf.nn.sparse_softmax_cross_entropy_with_logits`
- `tf.losses.sparse_softmax_cross_entropy`

Like ordinary softmax above, these loss functions should be used for multinomial mutually exclusive classification, i.e. pick one out of N classes.
The difference is in labels encoding: the classes are specified as integers(class index), not one-hot vectors.
Obviously, this doesn't allow soft classes, but it can save some memory when there are thousands or millions of classes.
However, note that logits argument must still contain logits per each class, thus it consumes at least `[batch_size, classes]` memory.

# utils
`tensorboard --logdir=PATH` (PATH 为 传给的estimator 的 model dir):
正常情况会输出很多信息，并在最后显示类似下面的提示, eg: TensorBoard 1.6.0 at http://xxx-xxx-xxx.local:6006 (Press CTRL+C to quit).
for remote: `ssh -L 6006:127.0.0.1:6006 dl2017@140.113.xxx.xxx -p xxx`


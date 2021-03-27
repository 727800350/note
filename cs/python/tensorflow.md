# intro
TensorFlow is an open source software library for **numerical computation using data flow graphs**.
Nodes in the graph represent mathematical operations, while the graph edges represent the multidimensional data arrays (tensors) communicated between them. 
The flexible architecture allows you to deploy computation to one or more CPUs or GPUs in a desktop, server, or mobile device with a single API.

TensorFlow isn't a rigid neural networks library. If you can express your computation as a data flow graph, you can use TensorFlow.

TF 的编程模型是符号编程(symbolic computation).
大致的想法就是使用符号API去描述计算通过计算流图的方式, 以及更新逻辑,然后通过像服务器去发送计算流图的方式来进行计算.

和目前的符号语言比起来,TF最大的特点是强化了数据流图,引入了mutation的概念.这一点是TF和包括Theano在内的符号编程框架最大的不同.
所谓mutation,就是可以在计算的过程更改一个变量的值,而这个变量在计算的过程中(也就是会话中)会被带入到下一轮迭代里面去(也就是可以维护状态).

TensorFlow hierarchy
```
TensorFlow Estimators: high level, object oriented API
tf.layers, tf.losses, tf.metrics: resuable libraries for common model components
Python TensorFlow: provides Ops, which warp C++ kernel, low level API
C++ tensorflow
CPU | GPU | TPU
```

```C++
import tensorflow as tf
```

- `tf.Variable.initialized_value()`: Returns the value of the initialized variable, could use this instead of the variable itself to initialize another variable
- sess.run(): run 一个op 的时候, 可以写 sess.run(op), 也可以用 sess.run([op]), 但若要得到多个ops 的结果时, 要用list: `sess.run([op1, op2])`

# Dataset Input Pipeline
`tf.data.Dataset` allows you to build complex input pipelines.


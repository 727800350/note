`lr_mult`: 学习率的系数, 最终的学习率是这个数乘以solver.prototxt配置文件中的`base_lr`.
如果有两个`lr_mult`, 则第一个表示权值的学习率,第二个表示偏置项的学习率.一般偏置项的学习率是权值学习率的两倍.

数据层(Data Layers)
数据通过数据层进入Caffe,数据层在整个网络的底部.数据可以来自高效的数据库(LevelDB 或者 LMDB),直接来自内存.如果不追求高效性,可以以HDF5或者一般图像的格式从硬盘读取数据.

类型: DATA
必须参数:

- source:包含数据的目录名称
- `batch_size`:一次处理的输入的数量
 
可选参数:
- `rand_skip`:在开始的时候从输入中跳过这个数值,这在异步随机梯度下降(SGD)的时候非常有用
- backend [default LEVELDB]: 选择使用 LEVELDB 或者 LMDB

Convolution层:
就是卷积层,是卷积神经网络(CNN)的核心层.
层类型:Convolution
在后面的`convolution_param`中, 我们可以设定卷积层的特有参数.

必须设置的参数:

- `num_output`: 卷积核(filter)的个数
- `kernel_size`: 卷积核的大小.如果卷积核的长和宽不等,需要用`kernel_h`和`kernel_w`分别设定

其它参数:

- stride: 卷积核的步长,默认为1.也可以用`stride_h`和`stride_w`来设置.
- pad: 扩充边缘, 默认为0, 不扩充. 扩充的时候是左右,上下对称的. 也可以通过`pad_h`和`pad_w`来分别设定.
	- 比如卷积核的大小为`5*5`, 对于输入`28 * 28`, 输出就是`24 * 24`, 如果pad设置为2, 则四个边缘都扩充2个像素, 输出就为`28 * 28`, 这样卷积运算之后的特征图就不会变小. 
- `weight_filler`: 权值初始化. 默认为"constant", 值全为0. 很多时候我们用"xavier"算法来进行初始化, 也可以设置为"gaussian"
- `bias_filler`: 偏置项的初始化.一般设置为"constant", 值全为0.
- `bias_term`: 是否开启偏置项, 默认为true, 开启
- group: 分组,默认为1组.如果大于1,我们限制卷积的连接操作在一个子集内.如果我们根据图像的通道来分组,那么第i个输出分组只能与第i个输入分组进行连接.

Pooling层
也叫池化层,为了减少运算量和数据维度而设置的一种层.
层类型:Pooling

必须设置的参数:

- kernel_size: 池化的核大小.也可以用kernel_h和kernel_w分别设定.

其它参数:
- pool: 池化方法,默认为MAX.目前可用的方法有MAX, AVE, 或STOCHASTIC
- pad: 和卷积层的pad的一样,进行边缘扩充.默认为0
- stride: 池化的步长,默认为1.一般我们设置为2,即不重叠.也可以用stride_h和stride_w来设置.


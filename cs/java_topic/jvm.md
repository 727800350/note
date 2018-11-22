# memory
[JVM内存区域详解(Eden Space,Survivor Space,Old Gen,Code Cache和Perm Gen)](https://blog.csdn.net/shiyong1949/article/details/52585256/)

JVM区域总体分两类, heap区和非heap区.

- heap区又分为:
	- Eden Space(伊甸园): Eden Space字面意思是伊甸园,对象被创建的时候首先放到这个区域,进行垃圾回收后,不能被回收的对象被放入到空的survivor区域.
	- Survivor Space(幸存者区): 用于保存在eden space内存区域中经过垃圾回收后没有被回收的对象. Survivor有两个, To Survivor 和 From Survivor,这个两个区域的空间大小是一样的.
		执行垃圾回收的时候Eden区域不能被回收的对象被放入到空的survivor(也就是To Survivor,同时Eden区域的内存会在垃圾回收的过程中全部释放),另一个survivor(即From Survivor)里不能被回收的对象也会被放入这个survivor(即To Survivor),然后To Survivor 和 From Survivor的标记会互换,始终保证一个survivor是空的.
	- Old Gen(老年代): Eden Space和Survivor Space都属于新生代,新生代中执行的垃圾回收被称之为Minor GC(因为是对新生代进行垃圾回收,所以又被称为Young GC),每一次Young GC后留下来的对象age加1.
		Old Gen老年代,用于存放新生代中经过多次垃圾回收仍然存活的对象,也有可能是新生代分配不了内存的大对象会直接进入老年代.经过多次垃圾回收都没有被回收的对象,这些对象的年代已经足够old了,就会放入到老年代.
		当老年代被放满之后,虚拟机会进行垃圾回收,称之为Major GC.由于Major GC除并发GC外均需对整个堆进行扫描和回收,因此又称为Full GC.
- 非heap区又分:
	- Code Cache(代码缓存区): 主要用于存放JIT所编译的代码, CodeCache代码缓冲区的大小在client模式下默认最大是32m,在server模式下默认是48m
	- Perm Gen(永久代): Permanent Generation space,是指内存的永久保存区域,因而称之为永久代. 这个内存区域用于存放Class和Meta的信息, Class在被 Load的时候被放入这个区域.因为Perm里存储的东西永远不会被JVM垃圾回收的,所以如果你的应用程序LOAD很多CLASS的话,就很可能出现PermGen space错误.默认大小为物理内存的1/64.
	- Jvm Stack(java虚拟机栈)
	- Local Method Statck(本地方法栈)


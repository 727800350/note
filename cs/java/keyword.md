final, finally, finalize的区别

- final: 用于声明属性, 方法和类, 分别表示**属性不可变, 方法不可覆盖, 类不可继承**.
- finally: 是异常处理语句结构的一部分, 表示总是执行.
- finalize: 是Object类的一个方法, 在GC的时候会被调用. 但是GC 的时机我们是不能控制的, [这个方法已经被废弃掉了](https://hackernoon.com/java-lang-object-finalize-is-finally-deprecated-f99df40fa71)


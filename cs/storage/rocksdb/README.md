相比于leveldb

- RocksDB支持一次获取多个K-V, 还支持Key范围查找. LevelDB只能获取单个Key
- 压缩方面RocksDB可采用多种压缩算法, 除了LevelDB用的snappy,还有zlib,bzip2. LevelDB里面按数据的压缩率(压缩后低于75%)判断
  是否对数据进行压缩存储, 而RocksDB典型的做法是Level 0-2不压缩,最后一层使用zlib,而其它各层采用snappy.
- RocksDB支持管道式的Memtable,也就说允许根据需要开辟多个Memtable, 以解决Put与Compact速度差异的性能瓶颈问题. 在LevelDB里
  面因为只有一个Memtable,如果Memtable满了却还来不及持久化,这个时候LevelDB将会减缓Put操作,导致整体性能下降.


- [io](#io)
- [thread](#thread)

定义了一系列接口
```cpp
// include/leveldb/env.h
class Env {
 public:
  static Env* Default();

  virtual Status NewSequentialFile(const std::string& fname, SequentialFile** result) = 0;

  virtual Status NewRandomAccessFile(const std::string& fname, RandomAccessFile** result) = 0;

  virtual Status NewWritableFile(const std::string& fname, WritableFile** result) = 0;

  ...

  // Arrange to run "(*function)(arg)" once in a background thread.
  //
  // "function" may run in an unspecified thread.  Multiple functions
  // added to the same Env may run concurrently in different threads.
  // I.e., the caller may not assume that background work items are
  // serialized.
  virtual void Schedule(void (*function)(void* arg), void* arg) = 0;
}
```
还有一些常见的文件及目录操作, 比如删除文件, 创建删除目录, 获取文件大小.

# io
文件IO 接口
```cpp
// A file abstraction for reading sequentially through a file
class SequentialFile {
 public:
  virtual Status Read(size_t n, Slice* result, char* scratch) = 0;
  virtual Status Skip(uint64_t n) = 0;
}

class RandomAccessFile {
 public:
  virtual Status Read(uint64_t offset, size_t n, Slice* result, char* scratch) const = 0;
}

class WritableFile {
 public:
  virtual Status Append(const Slice& data) = 0;
  virtual Status Close() = 0;
  virtual Status Flush() = 0;
  virtual Status Sync() = 0;
}
```

附带提供了一个默认实现PosixEnv, 在启动的时候全局初始化, 并通过下面的接口暴漏出去
```cpp
// util/env_posix.cc
Env* Env::Default()
```

- SequentialFile 的实现
  - PosixSequentialFile: read 使用了 `fread_unlocked`, 相比于 fread, fread_unlocked 不加锁, 因此是 thread unsafe 的,
    追求更高的性能
- RandomAccessFile 的实现
  - PosixRandomAccessFile: read 使用了 `pread`, 而不是 fseek + fread, 保证了原子性
  - PosixMmapReadableFile: 使用mmap, 直接内存偏移
- WritableFile
  - PosixMmapFile: 每次map 一个小的region 64KB, 写的时候直接 memcpy

其中 `PosixEnv::NewRandomAccessFile` 会根据情况选择创建 PosixMmapReadableFile 还是 PosixRandomAccessFile.

64位操作系统是, 会优先创建一定数量的 PosixMmapReadableFile.

# thread
posix env 对 Schedule 的实现是只有一个后台thread, 所以所有通过Schedule 调度的任务都是串行的.


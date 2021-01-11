- [table cache](#table-cache)
- [block cache](#block-cache)

# table cache
cache of sstable

db/table_cache.h

- cache 的key 是 file number
- cache 的value 是 TableAndFile

```cpp
struct TableAndFile {
  RandomAccessFile* file;
  Table* table;
};
```

需要注意的是暴漏出去的两个接口, 都有一个file_size 参数
```cpp
Iterator* TableCache::NewIterator(const ReadOptions& options,
                                  uint64_t file_number,
                                  uint64_t file_size,
                                  Table** tableptr)

Status TableCache::Get(const ReadOptions& options,
                       uint64_t file_number,
                       uint64_t file_size,
                       const Slice& k,
                       void* arg,
                       void (*saver)(void*, const Slice&, const Slice&))
```
因为根据file number 从 env 拿到的file 是 RandomAccessFile 类型, 这个类型只有一个read 方法, 没法从file 拿到文件大小, 而要
解析sstable 需要从文件末尾的 footer 开始, 因此外面需要传进一个 file_size 参数.

TableCache::Get 最终会走到 Table::InternalGet

# block cache
当打开db 没有在options 中指定的时候, 会自动创建一个.

每个sstable 在打开的时候, 都会生成一个 uint64_t 类型的自增 cache_id.

- cache key: cache_id(8 bytes) + block offset(8 bytes)
- cache value: `Block* block`

读取到block 后放到block cache的地方

```cpp
// table/table.cc
Iterator* Table::BlockReader(void* arg, const ReadOptions& options, const Slice& index_value)
```

index_value: BlockHandle 结构, 里面有 offset 和 size 信息

`options.fill_cache` 可以设置为 true, 应对当某些情况下比如遍历整个db 时不需要填充cache


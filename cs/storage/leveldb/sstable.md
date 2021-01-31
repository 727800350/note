- [footer](#footer)
- [read](#read)
  - [get](#get)
  - [iterator](#iterator)
- [table builder](#table-builder)

<img src="./pics/sstable_logic.jpg" alt="sstable logic" width="60%"/>

```plain
<beginning_of_file>
[data block 1]
[data block 2]
...
[data block N]
[meta block 1]
...
[meta block K]
[meta index block]
[index block]
[Footer]        (fixed size; starts at file_size - sizeof(Footer))
<end_of_file>
```

在逻辑上,根据功能不同,leveldb在逻辑上又将sstable分为:

1. data block 用来存储key value数据对,
1. meta block: 目前只有filter block, 用来存储一些过滤器相关的数据(布隆过滤器),但是若用户不指定leveldb使用过滤器,leveldb
  在该block中不会存储任何内容,
1. meta Index block: It contains one entry for every other meta block where the key is the name of the meta block and
  the value is a BlockHandle pointing to that meta block.
1. index block: It contains one entry per data block, where the key is a string >= last key in that data block and
  before the first key in the successive data block. The value is the BlockHandle for the data block.
1. footer: At the very end of the file is a fixed length footer that contains the BlockHandle of the meta index and
  index blocks as well as a magic number.

# footer
```plain
metaindex_handle: char[p];       // Block handle for metaindex
index_handle:     char[q];       // Block handle for index
padding:          char[40-p-q];  // zeroed bytes to make fixed length
                                 // (40==2*BlockHandle::kMaxEncodedLength)
magic:            fixed64;       // == 0xdb4775248b80fb57 (little-endian)
```

```cpp
// table/format.h

class Footer {
 public:
  enum {
    kEncodedLength = 2*BlockHandle::kMaxEncodedLength + 8  // 48 bytes
  };

 private:
  BlockHandle metaindex_handle_;
  BlockHandle index_handle_;
}

class BlockHandle {
 public:
  enum { kMaxEncodedLength = 10 + 10 };

 private:
  uint64_t offset_;
  uint64_t size_;
}
```

# read
```cpp
Status TableCache::FindTable(uint64_t file_number, uint64_t file_size,
                             Cache::Handle** handle) {
  // ...
      s = Table::Open(options_, file, file_size, &table);
  // ...
}

struct Table::Rep {
  Options options;
};
```
Table::Rep 的 options 就是TableCache 的 options_, 也就是DBImpl 的 options_.

## get
```cpp
// table/table.cc
Status Table::InternalGet(const ReadOptions& options, const Slice& k, void* arg,
                          void (*saver)(void*, const Slice&, const Slice&)) {
  Iterator* iiter = rep_->index_block->NewIterator(rep_->options.comparator);
  if (iiter->Valid()) {
    Slice handle_value = iiter->value();
    FilterBlockReader* filter = rep_->filter;
    BlockHandle handle;
    if (filter != NULL && handle.DecodeFrom(&handle_value).ok() &&
      !filter->KeyMayMatch(handle.offset(), k)) {
       // Not found
    }
    ...
  }
  ...
}
```
如果有 bloom filter 的话, 先判断一下

最终如果找到, 就会调用 `(*saver)(arg, block_iter->key(), block_iter->value())`

## iterator
```cpp
// table/table.cc
Iterator* Table::NewIterator(const ReadOptions& options) const {
  return NewTwoLevelIterator(rep_->index_block->NewIterator(rep_->options.comparator),
                             &Table::BlockReader, const_cast<Table*>(this), options);
}
```

返回的是一个 `TwoLevelIterator` , 实际上就是一个 index block iterator + data block iterator
```cpp
class TwoLevelIterator: public Iterator {
 private:
  BlockFunction block_function_;  // read block
  void* arg_; // Table*, block function 会用到
  const ReadOptions options_;
  Status status_;
  IteratorWrapper index_iter_;
  IteratorWrapper data_iter_; // May be NULL
  // If data_iter_ is non-NULL, then "data_block_handle_" holds the
  // "index_value" passed to block_function_ to create the data_iter_.
  std::string data_block_handle_;
}
```
- arg_ 就是传入的 `const_cast<Table*>(this)`
- block_function_ 也就是 Table::BlockReader, 因为 BlockReader 是一个 static function, 需要 arg_ 指示block function 从哪
  个sstable 读取 block
- `data_block_handle_` 起到cache 的作用, 当index_iter_ seek 得到的data block handle 和 cache 住的 data_block_handle_, 就避
  免了再次调用block function

# table builder
```cpp
// include/leveldb/table_builder.h, table/table_builder.cc

class TableBuilder {
}

struct TableBuilder::Rep {
  WritableFile* file;
  uint64_t offset;  // offset in file
  BlockBuilder data_block;
  BlockBuilder index_block;
  std::string last_key;
  int64_t num_entries;
  FilterBlockBuilder* filter_block;  // filter policy 有效时

  // We do not emit the index entry for a block until we have seen the
  // first key for the next data block.  This allows us to use shorter
  // keys in the index block.  For example, consider a block boundary
  // between the keys "the quick brown fox" and "the who".  We can use
  // "the r" as the key for the index block entry since it is >= all
  // entries in the first block and < all entries in subsequent
  // blocks.
  //
  // Invariant: r->pending_index_entry is true only if data_block is empty.
  bool pending_index_entry;
  BlockHandle pending_handle;  // Handle to add to index block
}
```
- last_key: 确保顺序和用于计算分界点, 也就是index block 的key

```cpp
void TableBuilder::Add(const Slice& key, const Slice& value)
```
1. 如果是一个新的 block(也就是上一个block 刚flush 到磁盘, 通过 `pending_index_entry` 为 true 来标记), 则需要为上一个data
  block 写一个记录到 index block 中, 没有在flush 之后立刻写index block, 是因为写到index block 的记录的key 没有直接取data
  block 的最后一个key, 虽然理论上也是可行的, 但是为了尽可能的节省空间和加快之后的比较速度, 挑选一个短的 index key 更好,
  正如 上面的那段注释中举例的那样: 上一个data block 的最后一个key 是"the quick brown fox" 紧接着的data block 的第一个key
  是 "the who", 我们可以直接使用 "the quick brown fox" 作为index key. 但是我们有更好的选择(通过
  `Comparator::FindShortestSeparator` 计算得到), 比如 "the r", 够短, 只要满足>= all entries in the first block and < all
  entries in subsequent blocks 就行
1. 如果filter_block 不为 nullptr, 则添加一个一个记录到filter block 中
1. 将key, value加到 data block 中, `BlockBuilder::Add`
1. 当当前block 的大小超过阈值时, 默认4k, 由 `options.block_size` 配置, flush 磁盘
	1. 写磁盘, 当compression type 不为 kNoCompression 时, 会对数据进行压缩, 但是压缩后也会检查, 如果压缩比太低, 会强制改为
		不压缩
	1. 置 `pending_index_entry` 为true
	1. 当filter_block 不为nullptr 时, 开启一个新的filter

```cpp
Status TableBuilder::Finish()
```
1. 写filter block, 不压缩
1. 写 meta index block
1. 写 index block, 如果 `pending_index_entry` 为true, 要先为最后一个data block 记录一条记录到 index block 中
1. 写 footer


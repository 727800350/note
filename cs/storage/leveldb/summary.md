- [iterator](#iterator)
  - [db iterator](#db-iterator)
  - [MergingIterator](#mergingiterator)
  - [MemTableIterator](#memtableiterator)
  - [SkipList::Iterator](#skiplistiterator)
  - [Table Iterator](#table-iterator)
  - [TwoLevelIterator](#twoleveliterator)
  - [level 0 TwoLevelIterator](#level-0-twoleveliterator)
  - [level n(n >= 1) TwoLevelIterator](#level-nn--1-twoleveliterator)
  - [Block::Iter](#blockiter)

# iterator
leveldb 中 key 的查找遍历,存储层面之上统一通过 Iterator 的方式处理.
存储结构(memtable/sstable/block) 都提供对应的 Iterator, 另外还有为操作方便封装的特殊Iterator.

从 DBImpl::NewIterator 层层往下

## db iterator
Memtables and sstables that make the DB representation contain (userkey,seq,type) => uservalue entries.
DBIter combines multiple entries for the same userkey found in the DB representation into a single entry while
accounting for sequence numbers, deletion markers, overwrites, etc.

也就是说DBIter 作为存储层和用户层的桥梁.
存储层的Iterator(iter_) 不关心实际的数据, 只需要做遍历;
DBIter 是提供给用户的最外层Iterator, 返回对应的kv 数据, 需要做逻辑上的解析, 比如, 遍历到相同或者删除的key 要跳过, 如果指
定了Snapshot, 要跳过不属于Snapshot 的数据等.

```cpp
// db/db_iter.cc
class DBIter : public Iterator {
 public:
  void Seek(const Slice& target) override;  // seek user key
};

// db/db_impl.cc
Iterator* DBImpl::NewIterator(const ReadOptions& options) {
  SequenceNumber latest_snapshot;
  uint32_t seed;
  Iterator* iter = NewInternalIterator(options, &latest_snapshot, &seed);
  return NewDBIterator(this, user_comparator(), iter,
                       (options.snapshot != nullptr
                            ? static_cast<const SnapshotImpl*>(options.snapshot)->sequence_number()
                            : latest_snapshot),
                       seed);
}
```
- `user_comparator()`: 获取user key comparator
- 如果用户没有在options 指定 snapshot( 也即没有指定 sequence number), 那么就用当前最新的sequence number 来创建DBIter
- DBIter 是直接面向用户层的, 所以seek 的参数是user key, 而不是 internal key

## MergingIterator
NewIterator 中调用的 `NewInternalIterator(options, &latest_snapshot, &seed);` 生成的实际上是 MergingIterator.
```cpp
// table/merger.[h, cc]
Iterator* NewMergingIterator(const Comparator* comparator, Iterator** children, int n) {
```
- comparator: 是internal comparator
- children: `std::vector<Iterator*> list`, 包含了memtable, immutable memtable 和当前版本所有的sstable 生成的iterator,
  children 实际上是 `&list[0]`
- n: list 中iterator 的个数

```cpp
class MergingIterator : public Iterator {
 public:
  // seek internal key
  void Seek(const Slice& target) override {
    for (int i = 0; i < n_; i++) {
      children_[i].Seek(target);
    }
    FindSmallest();
    direction_ = kForward;
  }
};
```
因为MergeingIterator 是把memtable, immutable memtable 和当前版本所有的sstable 生成的iterator 汇总到了一起, 而这些底层的
数据源是很大概率会overlap, 所以每次seek 后, 需要把所有的底层iterator 都seek 一遍, 然后找到最小值.

## MemTableIterator
memtable 和 immutable memtable 生成的iterator
```cpp
// db/memtable.[h, cc]
class MemTableIterator : public Iterator {
 public:
  // k is internal key format
  // but entry in skiplist is in format: varint32(internal key size), internal key, varint32(value size), value
  // so before seeking, we need call EncodeKey first to add varint32(internal key size) component
  void Seek(const Slice& k) override { iter_.Seek(EncodeKey(&tmp_, k)); }

 private:
  MemTable::Table::Iterator iter_;  // skiplist iterator
};

Iterator* MemTable::NewIterator() { return new MemTableIterator(&table_); }
```

## SkipList::Iterator
entry format: varint32(internal key size) + internal key + varint32(value size) + value

```cpp
template <typename Key, class Comparator>
class SkipList {
  class Iterator {
   public:
    explicit Iterator(const SkipList* list);
    void Seek(const Key& target);  // target format: varint32(internal key size) + internal key
  };
};
```

请注意 SkipList::Iterator 没有继承自Iterator.

## Table Iterator
```cpp
void Version::AddIterators(const ReadOptions& options,
                           std::vector<Iterator*>* iters) {
  // Merge all level zero files together since they may overlap
  for (size_t i = 0; i < files_[0].size(); i++) {
    iters->push_back(vset_->table_cache_->NewIterator(
        options, files_[0][i]->number, files_[0][i]->file_size));
  }

  // For levels > 0, we can use a concatenating iterator that sequentially
  // walks through the non-overlapping files in the level, opening them
  // lazily.
  for (int level = 1; level < config::kNumLevels; level++) {
    if (!files_[level].empty()) {
      iters->push_back(NewConcatenatingIterator(options, level));
    }
  }
}
```

level 0 的sstable 的key range 可能会overlap, 所以需要把每个sstable 的iterator都加进去.

其他level 的就可以一层生成一个复合的 iterator, 也就是 ConcatenatingIterator

## TwoLevelIterator
对于类似index ==> data 这种需要定位index, 然后根据index 定位到具体data 的使用方式, leveldb 封装成TwoLevelIterator 使用.
TwoLevelIterator 封装了index Iterator(index_iter), 和根据index 中的信息可以返回data Iterator(data_iter)的 hook 函数.
index_iter 以及 data_iter 需要支持同一个 key 的 seek.

```cpp
// table/two_level_iterator.cc
Iterator* NewTwoLevelIterator(Iterator* index_iter,
                              BlockFunction block_function, void* arg,
                              const ReadOptions& options) {
  return new TwoLevelIterator(index_iter, block_function, arg, options);
}

class TwoLevelIterator : public Iterator {
 public:
  void Seek(const Slice& target) override;  // seek internal key

 private:
  BlockFunction block_function_;
  IteratorWrapper index_iter_;
};
```
内部由 index block iterator 和 data block reader function 组成.

## level 0 TwoLevelIterator
```cpp
// db/table_cache.[h, cc]
Iterator* TableCache::NewIterator(const ReadOptions& options,
                                  uint64_t file_number, uint64_t file_size,
                                  Table** tableptr) {
  if (tableptr != nullptr) {
    *tableptr = nullptr;
  }

  Cache::Handle* handle = nullptr;
  Status s = FindTable(file_number, file_size, &handle);
  if (!s.ok()) {
    return NewErrorIterator(s);
  }

  Table* table = reinterpret_cast<TableAndFile*>(cache_->Value(handle))->table;
  Iterator* result = table->NewIterator(options);
  result->RegisterCleanup(&UnrefEntry, cache_, handle);
  if (tableptr != nullptr) {
    *tableptr = table;
  }
  return result;
}
```

```cpp
// table/table.cc
Iterator* Table::NewIterator(const ReadOptions& options) const {
  return NewTwoLevelIterator(
      rep_->index_block->NewIterator(rep_->options.comparator),
      &Table::BlockReader, const_cast<Table*>(this), options);
}
```
Table 的 rep_ 的 options 是打开table 时传入的, 也就是和DBImpl 的options_ 一样(有一个小差别, 这里不会产生影响, 可以认为
一样的), 所以这里用的comparator 是 internal key comparator.

seek 的时候, 先通过index iterator 找到data block 的block handle, 然后再通过data block reader function(也就是
Table::BlockReader) 读到data block 并将其转换为data block iterator

## level n(n >= 1) TwoLevelIterator
NewConcatenatingIterator 生成的还是 TwoLevelIterator.
```cpp
// db/version_set.cc
Iterator* Version::NewConcatenatingIterator(const ReadOptions& options,
                                            int level) const {
  return NewTwoLevelIterator(
      new LevelFileNumIterator(vset_->icmp_, &files_[level]), &GetFileIterator,
      vset_->table_cache_, options);
}

// db/version_set.cc
// An internal iterator.  For a given version/level pair, yields
// information about the files in the level.  For a given entry, key()
// is the largest key that occurs in the file, and value() is an
// 16-byte value containing the file number and file size, both
// encoded using EncodeFixed64.
class Version::LevelFileNumIterator : public Iterator {
 public:
  LevelFileNumIterator(const InternalKeyComparator& icmp,
                       const std::vector<FileMetaData*>* flist)
      : icmp_(icmp), flist_(flist), index_(flist->size()) {  // Marks as invalid
  }
  // seek internl key
  void Seek(const Slice& target) override {
    index_ = FindFile(icmp_, *flist_, target);
  }
};
```
FileMetaData 中含有这个sstable 文件的最小和最大 internal key.
所以可以通过二分查找找到target 位于哪个sstable 中, 得到这个sstable 的file number 和 file size.

然后通过 GetFileIterator 和 vset_->table_cache_ 可以拿到 table iterator.

## Block::Iter
```cpp
// table/table.cc
// Convert an index iterator value (i.e., an encoded BlockHandle)
// into an iterator over the contents of the corresponding block.
Iterator* Table::BlockReader(void* arg, const ReadOptions& options,
                             const Slice& index_value) {
  // ...
  block->NewIterator(table->rep_->options.comparator)
  // ...
}

// table/block.cc
Iterator* Block::NewIterator(const Comparator* comparator) {
  const uint32_t num_restarts = NumRestarts();
  return new Iter(comparator, data_, restart_offset_, num_restarts);
}
```
从前面分析我们已经知道了 table->rep_->options.comparator 是 internal key comparator.

```cpp
// table/block.cc
class Block::Iter : public Iterator {
 private:
 public:
  void Seek(const Slice& target) override;
};
```

seek

先在restart points 中二分查找, 找到target 的所以在区间, 这个在这个区间内在顺序遍历查找.

- 对于index block 与 data block, target 是 internal key
- 对于meta index block, 在打开sstable 的时候, comparator 用的 BytewiseComparator(), target 是 bloom filter 的名字


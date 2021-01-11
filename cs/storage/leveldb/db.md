- [write and minor compaction](#write-and-minor-compaction)
- [version](#version)
- [read](#read)
  - [get](#get)
  - [iterator](#iterator)
- [major compaction](#major-compaction)

# write and minor compaction
```cpp
// include/leveldb/db.h
class DB {
 public:
  virtual Status Put(const WriteOptions& options,
                     const Slice& key,
                     const Slice& value) = 0;
  virtual Status Delete(const WriteOptions& options, const Slice& key) = 0;
  virtual Status Write(const WriteOptions& options, WriteBatch* updates) = 0;
}
```
DB 的 Put 和 Delete 都是转换为Write 的, Write 都是通过WriteBatch 来操作的.

```cpp
// include/leveldb/write_batch.h
class WriteBatch {
 public:
  // Store the mapping "key->value" in the database.
  void Put(const Slice& key, const Slice& value);

  // If the database contains a mapping for "key", erase it.  Else do nothing.
  void Delete(const Slice& key);
 private:
  std::string rep_;
}

// db/write_batch.cc
// WriteBatch header has an 8-byte sequence number followed by a 4-byte count.
static const size_t kHeader = 12;
```
WriteBatch 初始化的时候, 会在rep_ 头部添加12 bytes 的header

Put 把kv 连同 kTypeValue 编码进 rep_, 同时要修改header 中的count

Delete 与Put 差不多的操作, 只是type 为 kTypeDeletion, 没有value 部分

```cpp
class DBImpl {
 private:
  port::Mutex mutex_;
  std::deque<Writer*> writers_;
  port::CondVar bg_cv_;  // Signalled when background work finishes
  MemTable* mem_;
  MemTable* imm_;  // Memtable being compacted
}

// db/db_impl.cc
struct DBImpl::Writer {
  Status status;
  WriteBatch* batch;
  bool sync;
  bool done;
  port::CondVar cv;  // 由 DBImpl 的mutex_ 初始化
};

Status DBImpl::Write(const WriteOptions& options, WriteBatch* my_batch)
```
1. 将my_batch 封装成一个新的Writer 插入到writers_ 尾;
1. 从 `writers_` 中挑出一个writer 作为这批writers 的真正写入着. `writers_` 相当于一个任务队列, 生产者线程不断向任务队列中添
  加待处理的任务. 一般计算模型是将消费者和生产者分开,也就是这里需要另开线程处理任务. 但是leveldb 采用了另外一种路线, 选
  择从生产者线程中找一个线程来处理任务, 问题在于选择哪个生产者作为消费者线程. 每个生产者在向Writers_ 队列中添加任务之后,
  都会进入一个while循环, 然后在里面睡眠, 只有当下面两种情况时, 才会被唤醒:
    - 该线程加入的任务已经被处理, 即 writer.done == true, 也就是自己没有被选中. 这种情况被唤醒后直接返回, 因为写入任务已
      经完成了.
    - 这个生产者所加入的任务位于队列的头部&w == writers_.front(). 这种情况被唤醒后, leveldb将这个生产者选为消费者, 然后
      让它负责这一批任务的写入处理.
1. MakeRoomForWrite
1. 真正的writer 将`writers_` 中的所有WriteBatch 合并为一个大的batch. 当然也会有大小控制, 大小阈值是根据被选中的writer 的
  batch 大小来确定的. 同时记录下这批的最后一个writer 为 last_writer
1. 获取db 的last sequence 也就是最大的sequence, 然后设置合并后的batch 的sequence 为last + sequence + 1
1. 释放mutex_, 写 WAL
1. 把合并后的write batch 写到 mem_, 重新加上 mutex_
1. 更新当前db 的最大sequence 为 last sequence + 这一批kv 的个数
1. 遍历 writers_, 将这一批写入完成的writer 的 done 置为true, 并通过 condition variable 通知, 注意遍历到last_writer 为止
1. 如果writers_ 队列不为空, 则唤醒队首位置的writer.

需要注意的是 6 和 7 操作前把 `mutex_` 释放了, 之后又加上了, 也就是执行6 和 7 的过程中, 是不阻塞写入的, 所以可以继续往
writers_ 对尾添加新的writer

```cpp
// db/db_impl.cc
// force: true when compaction
Status DBImpl::MakeRoomForWrite(bool force) {
  while (true) {
    if (versions_->NumLevelFiles(0) >= config::kL0_SlowdownWritesTrigger /* default 8 */) {
      // We are getting close to hitting a hard limit on the number of
      // L0 files.  Rather than delaying a single write by several
      // seconds when we hit the hard limit, start delaying each
      // individual write by 1ms to reduce latency variance.  Also,
      // this delay hands over some CPU to the compaction thread in
      // case it is sharing the same core as the writer.
      mutex_.Unlock();
      env_->SleepForMicroseconds(1000);
      allow_delay = false;  // Do not delay a single write more than once
      mutex_.Lock();
    } else if (mem_->ApproximateMemoryUsage() <= options_.write_buffer_size) {
      // There is room in current memtable
      break
    } else if (imm_ != nullptr) {
      // We have filled up the current memtable, but the previous
      // one is still being compacted, so we wait.
      bg_cv_.Wait();
    } else if (versions_->NumLevelFiles(0) >= config::kL0_StopWritesTrigger /* default 12 */) {
      // There are too many level-0 files.
      Log(options_.info_log, "waiting...\n");
      bg_cv_.Wait();
    } else {
      // Attempt to switch to a new memtable and trigger compaction of old
      // 创建新的memtable 和 log 文件, 更新 logfile_number_, 并把之前的 mem_ 赋值给 imm_
      MaybeScheduleCompaction()
  }
}
```
上面的核心代码只考虑了普通写入场景.

```cpp
void DBImpl::MaybeScheduleCompaction() {
  // ... check ....
  bg_compaction_scheduled_ = true;
  env_->Schedule(&DBImpl::BGWork, this);
}

class DBImpl : public DB {
 private:
  static void BGWork(void* db);
}
```

BGWork 最终会调用 BackgroundCompaction

```cpp
// db/db_impl.cc
Status DBImpl::BackgroundCompaction() {
  if (imm_ != NULL) {
    return CompactMemTable();
  }
  ...
}

Status DBImpl::CompactMemTable() {
  // Save the content of the immutable memtable as a new Table
  VersionEdit edit;
  Version* base = versions_->current();
  Status s = WriteLevel0Table(imm_, &edit, base);

  // Replace immutable memtable with the generated Table
  edit.SetPrevLogNumber(0);
  edit.SetLogNumber(logfile_number_);  // Earlier logs no longer needed
  s = versions_->LogAndApply(&edit, &mutex_);

  // Commit to the new state
  imm_->Unref();
  imm_ = NULL;
  DeleteObsoleteFiles();
}
```

CompactMemTable 中有三个重要的函数调用

1. WriteLevel0Table
2. LogAndApply
3. DeleteObsoleteFiles

先来看 WriteLevel0Table
```cpp
// db/version_edit.h
struct FileMetaData {
  int refs;
  int allowed_seeks;          // Seeks allowed until compaction
  uint64_t number;
  uint64_t file_size;         // File size in bytes
  InternalKey smallest;       // Smallest internal key served by table
  InternalKey largest;        // Largest internal key served by table

  FileMetaData() : refs(0), allowed_seeks(1 << 30), file_size(0) { }
};

class VersionEdit {
 private:
  uint64_t prev_log_number_;
  uint64_t log_number_;  // Earlier logs no longer needed  

  typedef std::set< std::pair<int, uint64_t> > DeletedFileSet;
  DeletedFileSet deleted_files_;
  std::vector< std::pair<int, FileMetaData> > new_files_;
}

Status DBImpl::WriteLevel0Table(MemTable* mem, VersionEdit* edit, Version* base) {
  FileMetaData meta;
  // 同时会将新生成的 sstable 加到 table cache 中, 附带验证生成的sstable 是完好的
  s = BuildTable(dbname_, env_, options_, table_cache_, iter, &meta);

  // 就是meta 的smallest 和 largest
  level = base->PickLevelForMemTableOutput(min_user_key, max_user_key);
  // 加到 new_files_ 中
  edit->AddFile(level, meta.number, meta.file_size, meta.smallest, meta.largest);
}
```
其中调用的 `base->PickLevelForMemTableOutput(min_user_key, max_user_key)` 会挑出level (默认从0 - 2 `kMaxMemCompactLevel`
中挑), 不是0 的条件比较苛刻, 需要

1. 新生成的sstable 需要和 level 0 已有的文件没有overlap
2. 新生成的sstable 需要和 level x + 1已有的文件没有overlap
3. level x + 2 中和新 sstable overlap 的文件总大小不能超过一定阈值 `kMaxGrandParentOverlapBytes`

所以 immutable memtable 生成的sstable 不都是 level 0 的.

```cpp
Status VersionSet::LogAndApply(VersionEdit* edit, port::Mutex* mu)
```
1. 通过 `VersionSet::Builder` 在当前版本 version_ 的基础上 Apply edit, 生成一个新的 version v
2. 遍历v 各层, 计算各层的打分, 记录下最高分值的level 和具体的score 到 v->compaction_level_ 和 v->compaction_score_, 作为
  下一次compaction 的依据
    - level 0 的计算规则, file number / config::kL0_CompactionTrigger
    - 其他层: total file size / MaxBytesForLevel(level)
3. 创建一个新的manifest 文件(log 文件的格式), 并把当前版本 version_ 的数据以VersionEdit 的形式
  (`void VersionEdit::EncodeTo(std::string* dst) const`)写到manifest 中, 包括:
    1. comparator 的名字
    2. compaction pointer 也就是 Per-level key at which the next compaction at that level should start. Either an empty
      string, or a valid InternalKey.
    3. 每层的文件信息, 包括 level, number, fize, smallest key, largest key
4. 把传入的 edit 追加到 manifest 文件末尾
5. 将 CURRENT 指向新生成的 manifest 文件
6. 把current_ 指向新生成的Version v, 并将新生成的 version v 追加到链表末尾

各层的所有文件加起来的最大大小计算规则:
```cpp
static double MaxBytesForLevel(int level) {
  // Note: the result for level zero is not really used since we set
  // the level-0 compaction threshold based on number of files.
  double result = 10 * 1048576.0;  // Result for both level-0 and level-1
  while (level > 1) {
    result *= 10;
    level--;
  }
  return result;
}
```

DeleteObsoleteFiles
```cpp
// db/db_impl.cc
void DBImpl::DeleteObsoleteFiles()
```

1. 正在进行的 compaction 过程中生成的文件, 肯定不能删除掉
2. version 链表中所有引用的文件不能删, 有些虽然不在 current_ 中, 但还在被外部读, 所以也不能删除
3. 通过 `env_->GetChildren` 把db 下的所有文件名字都拿到
4. 遍历文件列表, 从名字解析出文件类型并进行判断是否可以删除
    1. 日志文件: 保留大于 VersionSet::log_number_以及辅助 log 文件 (VersionSet::prev_log_number_)
    2. Manifest 文件只保留当前的
    3. sstable 文件以及临时文件只保留 live 的
    4. CURRENT/LOG/LOCK 文件均保留

# version
VersionSet 所有Version 构成的双向链表, 这些Version按时间顺序先后产生,记录了当时的元信息,链表头指向当前最新的Version, 同
时维护了每个Version的引用计数, 被引用中的Version不会被删除, 其对应的sst文件也因此得以保留,通过这种方式,使得LevelDB可以在
一个稳定的快照视图上访问文件.
VersionSet中除了Version的双向链表外还会记录一些如LogNumber, Sequence, 下一个sst文件编号的状态信息.

<img src="./pics/version_set.png" alt="version set" width="100%"/>

```cpp
// Apply all of the edits in *edit to the current state.
void VersionSet::Builder::Apply(VersionEdit* edit)
```
1. 将 edit 中的 deleted_files_ 添加到 对应LevelState 的deleted_files 中
2. 将 edit 中的 new_files_ 添加到对应 LevelState 的 added_files 中, 同时从 deleted_files 中erase 掉, 也就是 add 的优先级
  高于 delete. 同时会为每个文件计算一个 allowd_seeks

allow_seeks 的设定规则
```cpp
// We arrange to automatically compact this file after
// a certain number of seeks.  Let's assume:
//   (1) One seek costs 10ms
//   (2) Writing or reading 1MB costs 10ms (100MB/s)
//   (3) A compaction of 1MB does 25MB of IO:
//         1MB read from this level
//         10-12MB read from next level (boundaries may be misaligned)
//         10-12MB written to next level
// This implies that 25 seeks cost the same as the compaction
// of 1MB of data.  I.e., one seek costs approximately the
// same as the compaction of 40KB of data.  We are a little
// conservative and allow approximately one seek for every 16KB
// of data before triggering a compaction.
f->allowed_seeks = (f->file_size / 16384);
if (f->allowed_seeks < 100) f->allowed_seeks = 100;
```

# read
## get
```cpp
// db/db_impl.cc
Status DBImpl::Get(const ReadOptions& options, const Slice& key, std::string* value)
```
1. 读取顺序 memtable > immutable memtable > `versions_->current()`, 如果是从 sstable 中读取的, 设置 have_stat_update 标
  记位为 true
2. 如果从sstable 中读取的, 将命中的sstable 的allow_seeks 减一, 如果减到0, 就尝试compact 命中文件的所在层
  `MaybeScheduleCompaction`, 这就是 seek compaction.

```cpp
Status Version::Get(const ReadOptions& options, const LookupKey& k, std::string* value, GetStats* stats)
```
遍历处理每层

1. 把与LookupKey 有交集的本层文件找出来, 加到待处理数组 `std::vector<FileMetaData*> tmp` 中
    - 当是 level 0 时, 每个文件都加入到tmp 中, 并且要按照新文件在前的顺序排
    - 其他level 时, 先二分查找定位到可能在的文件中, 找到的话, 加入到 tmp 中
2. 遍历处理tmp 中的每个文件, 调用 `table_cache_->Get(options, f->number, f->file_size, ikey, &saver, SaveValue);`

## iterator
```cpp
// db/db_impl.cc
Iterator* DBImpl::NewIterator(const ReadOptions& options) {
  SequenceNumber latest_snapshot;
  Iterator* internal_iter = NewInternalIterator(options, &latest_snapshot);
  return NewDBIterator(
      &dbname_, env_, user_comparator(), internal_iter,
      (options.snapshot != NULL
       ? reinterpret_cast<const SnapshotImpl*>(options.snapshot)->number_
       : latest_snapshot));
}
```

```cpp
Iterator* DBImpl::NewInternalIterator(const ReadOptions& options, SequenceNumber* latest_snapshot)
```
1. 定义一个iterator 的vecotr std::vector<Iterator*> list;
2. 把 memtable 生成的 Iterator 实现 MemTableIterator 加到 list 中
3. 把immutable memtable(如果存在的话) 生成的 Iterator 实现 MemTableIterator 加到 list 中
4. 把当前版本中的所有文件生成的 Iterator 加入到 list 中, `versions_->current()->AddIterators(options, &list);`
    1. level 0 的每个文件独立生成一个Table Iterator, 实际上是 TwoLevelIterator (index iterator + block read function +
      block iterator)
    2. 其他每个 level 生成一个 ConcatenatingIterator, 实际是另外一种TwoLevelIterator(LevelFileNumIterator +
      GetFileIterator + Table Iterator)
5. 把list 转成 MergingIterator

```cpp
// table/merger.cc
class MergingIterator : public Iterator {
 public:
  virtual void Seek(const Slice& target) {
    for (int i = 0; i < n_; i++) {
      children_[i].Seek(target);
    }
    FindSmallest();
    direction_ = kForward;
  }

  virtual void Next() {
    assert(Valid());

    // Ensure that all children are positioned after key().
    // If we are moving in the forward direction, it is already
    // true for all of the non-current_ children since current_ is
    // the smallest child and key() == current_->key().  Otherwise,
    // we explicitly position the non-current_ children.
    if (direction_ != kForward) {
      for (int i = 0; i < n_; i++) {
        IteratorWrapper* child = &children_[i];
        if (child != current_) {
          child->Seek(key());
          if (child->Valid() &&
              comparator_->Compare(key(), child->key()) == 0) {
            child->Next();
          }
        }
      }
      direction_ = kForward;
    }

    current_->Next();
    FindSmallest();
  }
}

void MergingIterator::FindSmallest() {
  IteratorWrapper* smallest = NULL;
  for (int i = 0; i < n_; i++) {
    IteratorWrapper* child = &children_[i];
    if (child->Valid()) {
      if (smallest == NULL) {
        smallest = child;
      } else if (comparator_->Compare(child->key(), smallest->key()) < 0) {
        smallest = child;
      }
    }
  }
  current_ = smallest;
}
```

# major compaction
[leveldb笔记之17:major compaction之筛选文件](https://izualzhy.cn/leveldb-PickCompaction)

```cpp
// db/db_impl.cc
Status DBImpl::BackgroundCompaction() {
  Compaction* c = versions_->PickCompaction();
  if (c->IsTrivialMove()) {
  }
}
```
1. 按照一定的规则挑选出这次需要major compaction 的level, `versions_->PickCompaction()`
2. 如果是平凡的 compaction, 就把需要执行的增减文件记录到 compaction 的 edit_ 中. 平凡的判断规则
  `Compaction::IsTrivialMove`, 即要compact 的level 只有一个文件, 且level + 1 层与要compact 的文件没有overlap
3. 如果是非平凡的 compaction, 执行compact, `DoCompactionWork`

- seek_compaction
  一个直观的想法是如果一个文件多次 seek 但是没有查找到数据,那么就应该被 compact 了,否则会浪费更多的 seek.用一次 compact
  来解决长久空 seek 的问题,本质上,还是如何平衡读写的思想.
  具体的,当一个新文件更新进入版本管理,计算该文件允许 seek 但是没有查到数据的最大次数,当超过该次数后,就应该 compact 该文
  件了.
- size_compaction
  compaction 另外一个直观的想法就是,当某一层文件变得很大,往往意味着冗余数据过多,应该 compact 以避免占用磁盘以及读取过慢
  的问题.
  level 越大,我们可以认为数据越"冷",读取的几率越小,因此大的 level,能"容忍"的程度就越高,给的文件大小阈值越大.
  具体的,当产生新版本时,遍历所有的层,比较该层文件总大小与基准大小,得到一个最应当 compact 的层.

```cpp
// db/version_set.h
class Compaction {
 private:
  // Each compaction reads inputs from "level_" and "level_+
  std::vector<FileMetaData*> inputs_[2];  // The two sets of inputs  
}
```

```cpp
// db/version_set.cc
Compaction* VersionSet::PickCompaction()
```
size compaction 的优先级高于seek compaction


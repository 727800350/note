# status
```cpp
// include/leveldb/status.h, util/status.cc
class Status {
 private:
  // OK status has a null state_.  Otherwise, state_ is a new[] array
  // of the following form:
  //    state_[0..3] == length of message
  //    state_[4]    == code
  //    state_[5..]  == message
  const char* state_;

  enum Code {
    kOk = 0,
    kNotFound = 1,
    kCorruption = 2,
    kNotSupported = 3,
    kInvalidArgument = 4,
    kIOError = 5
  };
}
```
message 可能是包含两部分的, msg1 + separator + msg2.
用msg 和 msg2 可以方便的区分导言部分和数据部分, 例如下面的用法
```cpp
Status::Corruption("corrupted key for ", user_key)
```

我们自己写代码, 很少会这样写, 大概率会写成
```cpp
class Status {
 private:
  Code code_;
  std::string msg_;
}
```
理解和实现都会更容易

# options
通过options 而不是直接在函数中写死, 可以方便的增加参数
```cpp
// include/leveldb/options.h, util/options.cc

// Options to control the behavior of a database (passed to DB::Open)
struct Options {
  const Comparator* comparator;  // 默认是 lexicographic byte-wise ordering comparator
  Env* env;  // default: Env::Default()
  size_t block_size;  // default 4k
  int block_restart_interval;  // default 16
  CompressionType compression;  // default kSnappyCompression
  const FilterPolicy* filter_policy;  // default nullptr, 可以设置为使用bloom filter
}

// Options that control read operations
struct ReadOptions {
  bool verify_checksums;  // default false
  bool fill_cache;  // default true
  const Snapshot* snapshot;  // default nullptr
}

// Options that control write operations
struct WriteOptions {
  bool sync;  // default false
}
```

# slice
```cpp
// include/leveldb/slice.h
class Slice {
 public:
  // Create a slice that refers to the contents of "s"
  Slice(const std::string& s) : data_(s.data()), size_(s.size()) { }

  // Create a slice that refers to s[0,strlen(s)-1]
  Slice(const char* s) : data_(s), size_(strlen(s)) { }

 private:
  const char* data_;
  size_t size_;
}
```
这几个构造函数都没有加explicit, 也即意味着可以做隐式转换, 在需要slice 的函数参数, 可以直接传 std::string 和 c string, 使
用起来也比较方便.

相比于 std::string

1. 值传递时, 相当于只传一个header, 不需要拷贝
2. 支持 std::string 没有的方法, 比如 starts_with (C++ 20 已经支持了), remove_prefix

不过需要特别注意的一点就是必须保证slice 使用期间, 它底层的数据还是有效的.

整个leveldb 中所使用的comparator, 最终都是用的 Slice 的 compare 方法.

# coding
定长和变长的uint32_t, uint64_t 编码

[https://developers.google.com/protocol-buffers/docs/encoding](
https://developers.google.com/protocol-buffers/docs/encoding)

Each byte in a varint, except the last byte, has the most significant bit (msb) set – this indicates that there are
further bytes to come.

varint 编码时

- uint32_t 最多占用 5 个bytes, 32 / 7 = 4.xxx
- uint64_t 最多占用 10 个bytes, 64 / 7 = 9.xxx

```cpp
// util/coding.h, util/coding.cc
char* EncodeVarint64(char* dst, uint64_t v) {
  static const int B = 128;
  unsigned char* ptr = reinterpret_cast<unsigned char*>(dst);
  while (v >= B) {
    *(ptr++) = (v & (B-1)) | B;
    v >>= 7;
  }
  *(ptr++) = static_cast<unsigned char>(v);
  return reinterpret_cast<char*>(ptr);
}

int VarintLength(uint64_t v) {
  int len = 1;
  while (v >= 128) {
    v >>= 7;
    len++;
  }
  return len;
}
```
因为B 是2 的幂, 所以 v & (B - 1) 等价于 v % B.
最后的 | B 就是给大于128 的数字最高位补1, 表示还有bytes

EncodeVarint32 的实现采用了硬编码的方式
```cpp
char* EncodeVarint32(char* dst, uint32_t v) {
  // Operate on characters as unsigneds
  unsigned char* ptr = reinterpret_cast<unsigned char*>(dst);
  static const int B = 128;
  if (v < (1<<7)) {
    *(ptr++) = v;
  } else if (v < (1<<14)) {
    *(ptr++) = v | B;
    *(ptr++) = v>>7;
  } else if (v < (1<<21)) {
    *(ptr++) = v | B;
    *(ptr++) = (v>>7) | B;
    *(ptr++) = v>>14;
  } else if (v < (1<<28)) {
    *(ptr++) = v | B;
    *(ptr++) = (v>>7) | B;
    *(ptr++) = (v>>14) | B;
    *(ptr++) = v>>21;
  } else {
    // use at most 5 bytes
    *(ptr++) = v | B;
    *(ptr++) = (v>>7) | B;
    *(ptr++) = (v>>14) | B;
    *(ptr++) = (v>>21) | B;
    *(ptr++) = v>>28;
  }
  // now ptr points to the next byte of real data
  return reinterpret_cast<char*>(ptr);
}
```
完全可以代码复用, 直接调用 EncodeVarint64. uint32_t 和 uint64_t 的varint 编码结构是完全一样的, 不需要明确区分.

# arena
内存分配器, skiplist 中使用

```cpp
// util/arena.h, util/arena.cc

static const int kBlockSize = 4096;

class Arena {
 public:
  char* Allocate(size_t bytes);
  char* AllocateAligned(size_t bytes);

 private:
  // Allocation state
  char* alloc_ptr_;
  size_t alloc_bytes_remaining_;

  // Array of new[] allocated memory blocks
  std::vector<char*> blocks_;

  // Bytes of memory in blocks allocated so far
  size_t blocks_memory_;
}
```

这里会涉及到两方面:

1. skiplist/memtable 是没有删除接口的, 所以里面的元素总是不断地添加进来.
2. skiplist/memtable 会在生成L0 的文件之后, 统一销毁掉, 所以内存块可以直接由`Arena`来统一销毁(每个memtable 都有一个自己
  的arena).

基于这两点原因, 所以`Arena`是没有归还内存的接口的.

Allocate: 当当前block 的剩余空间 alloc_bytes_remaining_ > 所需空间时, 直接使用当前block

一个block 4k, 但是 blocks_ 里面的block 不全是4k 大小的

```cpp
char* Arena::AllocateFallback(size_t bytes) {
  if (bytes > kBlockSize / 4) {
    // Object is more than a quarter of our block size.  Allocate it separately
    // to avoid wasting too much space in leftover bytes.
    char* result = AllocateNewBlock(bytes);
    return result;
  }

  // We waste the remaining space in the current block.
  alloc_ptr_ = AllocateNewBlock(kBlockSize);
  alloc_bytes_remaining_ = kBlockSize;

  char* result = alloc_ptr_;
  alloc_ptr_ += bytes;
  alloc_bytes_remaining_ -= bytes;
  return result;
}
```

当请求的bytes > kBlockSize / 4 时, 直接使用malloc 分配.

AllocateAligned 保证分配的内存是按照指针大小对齐的, 对于32位系统, 就是4 bytes.

# filter policy
class FilterPolicy 定义了接口
```cpp
// include/leveldb/filter_policy.h, util/bloom.cc
class FilterPolicy {
 public:
  virtual const char* Name() const = 0;
  virtual void CreateFilter(const Slice* keys, int n, std::string* dst) const = 0;
  virtual bool KeyMayMatch(const Slice& key, const Slice& filter) const = 0;
}
```

只有 bloom filter 一种实现
```cpp
class BloomFilterPolicy : public FilterPolicy {
 private:
  size_t bits_per_key_;
  size_t k_;  // hash 函数的个数
}
```

真正的hash 函数只有一个 BloomHash, 然后 k_ 个hash 都是在BloomHash 的结果上进行计算得来的.
```cpp
uint32_t h = BloomHash(keys[i]);
const uint32_t delta = (h >> 17) | (h << 15);  // Rotate right 17 bits
for (size_t j = 0; j < k_; j++) {
  const uint32_t bitpos = h % bits;  // bits 是这个bloom filter 总的bit 个数
  array[bitpos/8] |= (1 << (bitpos % 8));
  h += delta;
}
```

依据是一篇paper, [Less Hashing, Same Performance: Building a Better Bloom Filter](
https://www.eecs.harvard.edu/~michaelm/postscripts/rsa2008.pdf)

看了下abstract 和 conclusion: "Specifically, only two hash functions are necessary to effectively implement a Bloom
filter without any loss in the asymptotic false positive probability. This leads to less computation and potentially
less need for randomness in practice."

leveldb 的实现更进一步, 只用了一个 hash 函数, 至于为什么能够缩减到一个hash 函数, 没有去深究.

# lru cache
LevelDB 完全解析(5):Cache [https://juejin.cn/post/6844904147427262477](https://juejin.cn/post/6844904147427262477)

## HandleTable
LRUHandle 是 hash 表的基础元素, 包含了key, value 等数据.

HandleTable 类似于 std::unordered_map, Google 的人觉得标准库的实现因为要考虑跨平台舍弃了各种hack, 也比较慢.

```cpp
// include/leveldb/cache.h, util/cache.cc

struct LRUHandle {
  void* value;
  void (*deleter)(const Slice&, void* value);
  LRUHandle* next_hash;  // hash冲突时指向下一个LRUHandle
  size_t key_length;
  uint32_t hash; // Hash of key(); used for fast sharding and comparisons
  char key_data[1]; // Beginning of key, 柔性数组
}

class HandleTable {
 public:
  LRUHandle* Insert(LRUHandle* h);
  LRUHandle* Remove(const Slice& key, uint32_t hash);

 private:
  // The table consists of an array of buckets where each bucket is
  // a linked list of cache entries that hash into the bucket.
  uint32_t length_; // the length of buckets, each bucket is a linked list of LRUHandle with the same hash
  uint32_t elems_; // the number of elements
  LRUHandle** list_; // LRUHandle*[length_]
}

LRUHandle** HandleTable::FindPointer(const Slice& key, uint32_t hash) {
  LRUHandle** ptr = &list_[hash & (length_ - 1)];
  while (*ptr != NULL &&
         ((*ptr)->hash != hash || key != (*ptr)->key())) {
    ptr = &(*ptr)->next_hash;
  }
  return ptr;
}
```
需要特别注意的是, FindPointer 返回的数据类型是 `LRUHandle**`, 而不是`LRUHandle*`, 这样做的好处是在 insert 和 remove 的时
候比较方便, 相当于同时拿到了prev 节点和要查找的节点, 之后再做修改就方便许多. 例如:

假设 a, b, c (都是`LRUHandle*` 类型)三个元素的hash 一样, LRUHandle 链表结构如下
```plain
a -> b -> c -> nullptr
```

```cpp
a->next_hash = b
b->next_hash = c
c->next_hash = nullptr
```

那么当

- key = a 时, ret = &a, *ret = a
- key = b 时, ret = &(a→next_hash), *ret = b
- key = c 时, ret = &(b→next_hash), *ret = c
- key not found 时, ret = &(c→next_hash), *ret = nullptr

假设h 和 b 具有同样的key, 我们想用h 替换掉b, 只需要执行下面的代码就行了.
```cpp
LRUHandle** ptr = FindPointer(h->key(), h->hash);  // 返回的ptr 就会是 &(a->next_hash)
h->next_hash = (*ptr)->next_hash // 此时, a -> b -> c -> nullptr and h -> c
*ptr = h // 此时, a -> h ->c -> nullptr and b -> c
```
删除同理.

如果FindPointer 返回的是 `LRUHandle*` 类型, 也就是

- key = a 时, ret = a
- key = b 时, ret = b
- key not found 时, ret = nullptr

对于FindPointer 理解就会简单许多, 但是因为是单链表, 就没法在 FindPointer 的基础上实现insert 和 remove 两个操作了.

同时 FindPointer 的第一行代码 定位slot 用了 hash & (length_ -1) 而不是 hash % length_

```cpp
LRUHandle** ptr = &list_[hash & (length_ - 1)];
```

当 elems_ > length_ 时就会触发 resize, 新的length 选择规则
```cpp
uint32_t new_length = 4;
while (new_length < elems_) {
  new_length *= 2;
}
```
之所以让new_length 是2 的幂, 这主要是为了加速根据 hash 值选择 slot 的计算, 因为
hash % new_length = hash & (new_length - 1)

## LRUCache
```cpp
class LRUCache {
 public:
  Cache::Handle* Insert(const Slice& key, uint32_t hash, void* value, size_t charge,
                        void (*deleter)(const Slice& key, void* value));
  Cache::Handle* Lookup(const Slice& key, uint32_t hash);

 private:
  // Dummy head of LRU list.
  // lru.prev is newest entry, lru.next is oldest entry.
  LRUHandle lru_;

  HandleTable table_;
}

// LRUHandle 增加几个字段
struct LRUHandle {
  LRUHandle* next;
  LRUHandle* prev;
  uint32_t refs;
}
```

柔性数组分配空间

insert 的时候
```cpp
Cache::Handle* LRUCache::Insert(
    const Slice& key, uint32_t hash, void* value, size_t charge,
    void (*deleter)(const Slice& key, void* value)) {
  LRUHandle* e = reinterpret_cast<LRUHandle*>(malloc(sizeof(LRUHandle)-1 + key.size()));
  e->value = value;
  e->deleter = deleter;
  e->charge = charge;
  e->key_length = key.size();
  e->hash = hash;
  e->refs = 2;  // One from LRUCache, one for the returned handle
  // as malloc size has taken key size into account, so it is safe to copy key into key_data
  memcpy(e->key_data, key.data(), key.size());

  ...
}
```

[https://gcc.gnu.org/onlinedocs/gcc/Zero-Length.html](https://gcc.gnu.org/onlinedocs/gcc/Zero-Length.html)

ISO C99 flexible array
```cpp
struct line {
  int length;
  char contents[];
};

struct line *thisline = (struct line *)malloc (sizeof (struct line) + this_length);
thisline->length = this_length;
```
ISO C90, contents 通常被声明为 char contents[1], 也就是leveldb 中的用法.

redis 中的 sds 也是采用了柔性数组.
```cpp
typedef char *sds;
struct sdshdr {
  int len;
  int free;
  char buf[];
};
```

## ShardedLRUCache
只是在 LRUCache 基础上进行了分片.

table cache 和 block cache 用的都是 SharedLRUCache.

```cpp
static const int kNumShardBits = 4;
static const int kNumShards = 1 << kNumShardBits;  // 16 个

class ShardedLRUCache : public Cache {
 public:
  virtual Handle* Insert(const Slice& key, void* value, size_t charge, void (*deleter)(const Slice& key, void* value));
  virtual Handle* Lookup(const Slice& key);

 private:
  LRUCache shard_[kNumShards];

  // use the most significant 4 bits as shard index
  static uint32_t Shard(uint32_t hash) {
    return hash >> (32 - kNumShardBits);
  }
}
```

hash 的基础上取最高四位, 而不是我们习惯上通常采用的 取余的方式

# skiplist
db/skiplist.h

skiplist 只用于memtable, 而memtable 不会出现真正的删数据情况, 所以skiplist 也不会删除数据, 而只会在compact 到 level 0 的
sstable 文件后整体把skiplist 统一删除.

memtable 中的定义

```cpp
typedef SkipList<const char*, KeyComparator> Table;
```

```cpp
template<typename Key, class Comparator>
class SkipList {
 public:
	void Insert(const Key& key);

 private:
  enum { kMaxHeight = 12 };

  // Immutable after construction
  Comparator const compare_;
  Arena* const arena_;    // Arena used for allocations of nodes

  Node* const head_;

  // Modified only by Insert().  Read racily by readers, but stale
  // values are ok.
  port::AtomicPointer max_height_;   // Height of the entire list, initial 1

	// Read/written only by Insert().
	// 产生随机高度的
  Random rnd_;
}

template<typename Key, class Comparator>
struct SkipList<Key,Comparator>::Node {
  Key const key;

 private:
  // Array of length equal to the node height.  next_[0] is lowest level link.
  // Flexible array agagin
	// next_ 实际上是 Node* 类型的
  port::AtomicPointer next_[1];
}
```

NewNode
```cpp
template<typename Key, class Comparator>
typename SkipList<Key,Comparator>::Node*
SkipList<Key,Comparator>::NewNode(const Key& key, int height) {
  char* mem = arena_->AllocateAligned(sizeof(Node) + sizeof(port::AtomicPointer) * (height - 1));
  return new (mem) Node(key);
}
```

最后一句syntax 有点儿奇怪, 这是采用了不常用的 placement new

[https://stackoverflow.com/questions/222557/what-uses-are-there-for-placement-new](
https://stackoverflow.com/questions/222557/what-uses-are-there-for-placement-new)

Placement new allows you to construct an object in memory that's already allocated.

也就是说 `new (mem) Node(key)` 不会去重新申请内存, 而会直接使用已经申请好的 mem, 会去调用
`typename SkipList<Key, Comparator>::Node` 的构造函数.

同时使用了placement new 之后, 不能直接delete Node, 而只能delete mem[], 但是在skiplist 场景下, 不需要考虑删除节点, 也就无
关紧要了.

在mem 上执行了Node 的构造函数之后, 并没有把 `next_` 的height 个指针全部置为nullptr, 因为NewNode 函数只有在Insert 中才会
调用, 拿到新节点之后, 马上把 `next_` 都赋值了.

skiplist 中最重要的操作
```cpp
// Return the earliest node that comes at or after key.
// Return NULL if there is no such node.
//
// If prev is non-NULL, fills prev[level] with pointer to previous
// node at "level" for every level in [0..max_height_-1].
template<typename Key, class Comparator>
typename SkipList<Key,Comparator>::Node* SkipList<Key,Comparator>::FindGreaterOrEqual(const Key& key, Node** prev)
    const {
  Node* x = head_;
  int level = GetMaxHeight() - 1;
  while (true) {
    Node* next = x->Next(level);
    if (KeyIsAfterNode(key, next)) {
      // Keep searching in this list
      x = next;
    } else {
      if (prev != NULL) prev[level] = x;
      if (level == 0) {
        return next;
      } else {
        // Switch to next list
        level--;
      }
    }
  }
}
```
```plain
example from https://zhuanlan.zhihu.com/p/33674267
kMaxHeight = 4
level
3    |    | --------> | | -------------------------------------------------------> |   |
2    |    | --------> | | ---------------------------------------> |  | ---------> |   |
1    |    | --------> | | --------> | | -------------------------> |  | ---------> |   |
0    |head| -> |3| -> |6| -> |7| -> |9| -> |12| -> |19| -> |21| -> |25| -> |26| -> |nil|

call FindGreaterOrEqual(17, prev)

初始化, x = head_, level = 3
第一次循环: next = head_->Next(3) = 6 < 17, 所以 x = next = 6
第二次循环: next = 6->Next(3) = nullptr > 17, 所以进到else, 同时在level = 3 的链表上, 17 肯定是在6 和 nullptr 之间, 所以
  需要把17 的上一个节点6 记录到prev[3] 中, 之后在链表插入的时候会用到; 然后再level 减一到 2
第三次循环: next = 6->Next(2) = 25 > 17, 所以在level = 2 的链表上, 17 肯定是 6 和 25 之间, 把6 记录到prev[2], level 减一
  到 1
第四次循环: next = 6->Next(1) = 9 < 17, x = next = 9
第五次循环: next = 9->Next(1) = 25 > 17, 所以在level = 1 的链表上, 17 肯定在 9 和 25 之间, 把9 记录到prev[1], level 减一
  到 0
第六次循环: next = 9->Next(0) = 12 < 17, x = next = 12
第七次循环: next = 12->Next(0) = 19 > 17, 所以在level = 0 的链表上, 17 肯定在 12 和 19 之间, 把12 记录到 prev[0]; 同时已
  经到最低一层链表了, 可以返回节点 19 了.

最后 19 is returned, and
prev[3] = 6
prev[2] = 6
prev[1] = 9
prev[0] = 12
```

skiplist 的构建过程

构造函数初始化:
```cpp
max_height_ = 1
head_(NewNode(0 /* any key will do */, kMaxHeight))
for (int i = 0; i < kMaxHeight; i++) {
    head_->SetNext(i, NULL);
}
```

初始化后 skiplist 结构如下
```plain
kMaxHeight = 4
max_height_ = 1
level
3    |    | ---------------> |   |
2    |    | ---------------> |   |
1    |    | ---------------> |   |
0    |head| ---------------> |nil|
```


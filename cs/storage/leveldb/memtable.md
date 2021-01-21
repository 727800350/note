- [comparator](#comparator)
- [add](#add)
- [get](#get)
- [iterator](#iterator)

```cpp
// db/memtable.h
class MemTable {
 private:
	// user key asc and sequence number | type desc
  typedef SkipList<const char*, KeyComparator> Table;

	struct KeyComparator {
    const InternalKeyComparator comparator;
    explicit KeyComparator(const InternalKeyComparator& c) : comparator(c) { }
    int operator()(const char* a, const char* b) const;
  };

  KeyComparator comparator_;
  int refs_;
  Arena arena_;
  Table table_;
}
```

# comparator
默认情况下, 使用 BytewiseComparator 来初始化 InternalKeyComparator

internal key size = user key size + 8

internal key := user key + sequence number(7 bytes) + value type(1 byte).

sequence number 的最后一个byte 被drop 掉了

```cpp
typedef uint64_t SequenceNumber;
// We leave eight bits empty at the bottom so a type and sequence#
// can be packed together into 64-bits.
static const SequenceNumber kMaxSequenceNumber = ((0x1ull << 56) - 1);

enum ValueType {
  kTypeDeletion = 0x0,
  kTypeValue = 0x1
};
```

按照user key 升序, sequence number + type 降序.
```cpp
// db/dbformat.cc
int InternalKeyComparator::Compare(const Slice& akey, const Slice& bkey) const {
  // Order by:
  //    increasing user key (according to user-supplied comparator)
  //    decreasing sequence number
  //    decreasing type (though sequence# should be enough to disambiguate)
  int r = user_comparator_->Compare(ExtractUserKey(akey), ExtractUserKey(bkey));
  if (r == 0) {
    const uint64_t anum = DecodeFixed64(akey.data() + akey.size() - 8);
    const uint64_t bnum = DecodeFixed64(bkey.data() + bkey.size() - 8);
    if (anum > bnum) {
      r = -1;
    } else if (anum < bnum) {
      r = +1;
    }
  }
  return r;
}

// Returns the user key portion of an internal key.
inline Slice ExtractUserKey(const Slice& internal_key) {
  assert(internal_key.size() >= 8);
  return Slice(internal_key.data(), internal_key.size() - 8);
}
```

由于每一次数据更新将递增sequence number, 所以同一个user key 的不同操作sequence number 肯定是不一样的, 实际上只需要比较前
7 个bytes(也即真正的 sequence number 部分) 就行了, 不过这里为了方便把 sequence number + type 一起考虑了.

然后使用InternalKeyComparator 来初始化MemTable 的 KeyComparator

KeyComparator 只是从 internal key size, internal key 的结构中抽取出internal key, 然后使用 InternalKeyComparator 来比较
internal key

# add
```cpp
void MemTable::Add(SequenceNumber s, ValueType type, const Slice& key, const Slice& value);
```

Format of an entry is concatenation of:
```plain
key_size:    varint32 of internal_key.size()
key bytes :  char[internal_key.size()]
value_size:  varint32 of value.size()
value bytes: char[value.size()]
```
最后把 entry 整体加到skiplist 中.

# get
```cpp
bool MemTable::Get(const LookupKey& key, std::string* value, Status* s)

class LookupKey {
 public:
	LookupKey(const Slice& user_key, SequenceNumber sequence);

 private:
	// We construct a char array of the form:
  //    klength  varint32               <-- start_
  //    userkey  char[klength]          <-- kstart_
  //    tag      uint64
  //                                    <-- end_
  // The array is a suitable MemTable key.
  // The suffix starting with "userkey" can be used as an InternalKey.
  const char* start_;  // start of buf
  const char* kstart_; // start of user key
  const char* end_;  // end of buf
  char space_[200];      // Avoid allocation for short keys
}
```
因为 internal key 的最后 8 bytes 是 sequence number + value type, 而lookup 的构造函数中没有指定value type, 为了能够找到
具有相同的user key 和 sequence number 的所有数据, value type 必须是"最小"的, 而 value type 是和sequence number 一起降序
排的, 所以value type 的值必须是最大的, 这里就是 kTypeValue.

# iterator
```cpp
class MemTableIterator: public Iterator {
 public:
	virtual void Seek(const Slice& k) {
    // TODO: after EncodeKey, there is only user key part, no sequence number and type.
    // in InternalKeyComparator::Compare of dbformat.cc, ExtractUserKey could have a parameter whose size < 8, it will
    // have a problem.
    iter_.Seek(EncodeKey(&tmp_, k));
  }
 private:
  MemTable::Table::Iterator iter_;
  std::string tmp_;       // For passing to EncodeKey
}

static const char* EncodeKey(std::string* scratch, const Slice& target) {
  scratch->clear();
  PutVarint32(scratch, target.size());
  scratch->append(target.data(), target.size());
  return scratch->data();
}
```
MemTableIterator 的seek 最后实际上 skiplist 的seek.

<img src="./pics/leveldb.png" alt="leveldb" width="90%"/>

basic

* [status](basic.md#status)
* [options](basic.md#options)
* [slice](basic.md#slice)
* [coding](basic.md#coding)
* [arena](basic.md#arena)
* [filter policy](basic.md#filter-policy)
* [lru cache](basic.md#lru-cache)
  * [HandleTable](basic.md#handletable)
  * [LRUCache](basic.md#lrucache)
  * [ShardedLRUCache](basic.md#shardedlrucache)
* [skiplist](basic.md#skiplist)

cache

* [table cache](cache.md#table-cache)
* [block cache](cache.md#block-cache)

log

* [log 的结构](log.md#log-的结构)
* [log writer](log.md#log-writer)
* [log reader](log.md#log-reader)
* [ref](log.md#ref)

env

* [io](env.md#io)
* [thread](env.md#thread)

memtable

* [comparator](memtable.md#comparator)
* [add](memtable.md#add)
* [get](memtable.md#get)
* [iterator](memtable.md#iterator)
  * [bug?](memtable.md#bug)

sstable

* [footer](sstable.md#footer)
* [block](sstable.md#block)
  * [物理结构](sstable.md#物理结构)
  * [block 数据部分](sstable.md#block-数据部分)
  * [block iterator](sstable.md#block-iterator)
* [block builder](sstable.md#block-builder)
* [filter block](sstable.md#filter-block)
* [read](sstable.md#read)
  * [get](sstable.md#get)
  * [iterator](sstable.md#iterator)
* [table builder](sstable.md#table-builder)

db

* [write and minor compaction](db.md#write-and-minor-compaction)
* [version](db.md#version)
* [read](db.md#read)
  * [get](db.md#get)
  * [iterator](db.md#iterator)
* [major compaction](db.md#major-compaction)

Created by [gh-md-toc](https://github.com/ekalinin/github-markdown-toc.go)

源码阅读是基于 [leveldb-1.9.0](https://github.com/chen3feng/thirdparty/tree/master/leveldb-1.9.0)

源代码目录结构
```plain
|_leveldb
    |_doc: 相关文档, 有 log 和 sstable 的格式介绍
    |_include/leveldb: 头文件, 对外暴漏的接口
    |_db: 主要逻辑的实现
    |_table: sstable 相关
    |_port: 编译系统环境相关
    |_util: 通用的功能实现
    |_helper/memenv: 一个简单的完全内存的文件系统实现
```

main reference

- [doc](https://github.com/google/leveldb/tree/master/doc)
- [leveldb笔记](https://izualzhy.cn/archive.html?tag=leveldb)
- [leveldb实现解析.pdf](https://yuerblog.cc/wp-content/uploads/leveldb%E5%AE%9E%E7%8E%B0%E8%A7%A3%E6%9E%90.pdf)


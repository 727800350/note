# hbase shell
[HBase shell commands](https://learnhbase.net/2013/03/02/hbase-shell-commands)

shell 启动时默认选择 `$HBASE_HOME` 中的配置目录.
如果要访问其他集群, 新建包含其他集群的 hbase-site.xml 文件的单独目录, 配置hbase.zookeeper.quorum 属性指定另外一个集群, 然后用 `$ HBASE_CONF_DIR="<your-other-config-dir>/" bin/hbase shell` 来启动.
注意: 必须指定完整的目录, 而不仅仅是 hbase-site.xml 文件

- 可以用tab 键提示自动补全
- debug 来切换调试模式
- 命令行支持 二进制, 八进制, 十六进制的输入输出.
- 文本在单引号内被当做文本对待, 双引号内将被替换, 比如将八进制或者十六进制转换成字节.
- 键值对属性使用Ruby 散列的形式: {'key1' => 'value1', 'key2' => 'value2'}, 使用键值模式的 NAME, VERSIONS, COMPRESSION 等这些关键词不需要使用引号.
- `echo "status" | ./bin/hbase shell` 一旦命令完成, shell 会自动退出, 也可以 `cat ./sample_commands.txt ./bin/hbase shell`
- `./bin/hbase shell ./sample_commands.txt`: 如果文件最后没有exit, 整个命令执行完之后还是处于hbase shell 的状态

命令历史纪录, 在$HOME 目录创建`.irbrc`
```ruby
requre 'irb/ext/save-history'
IRB.conf[:SAVE_HISTORY] = 100
IRB.conf[:HISTORY_FILE] = "#{ENV['HOME']}/.irb-save-history"
```

## General  HBase shell commands
- list: 列出hbase中存在的所有表, 除-ROOT表和.META表(被过滤掉了)
- status: 返回hbase集群的状态信息

## Tables Management commands
- disable/enable/drop/exists/describe
- truncate: 重新创建指定表, 会丢掉之前配置的presplit region, column family, coprocess 等信息
- `truncate_preserve`: 只清空表里面的数据, 其他信息都会被保留
- flush: 手工把memstore写到Hfile中

- `create 'table', {NAME => 'f1', VERSIONS => 2}, {NAME => 'f2', VERSIONS => 2}`
- delete the 'f1' column family in table 't1': `alter 't1', 'delete' => 'f1'` or  `alter 't1', NAME => 'f1', METHOD => 'delete'`

- `alter 'performance', {NAME => 'f', BLOCKCACHE => 'false', BLOOMFILTER => 'NONE'}`
	- BLOCKCACHE: 改为false, 也只是禁用了 data block cache, 而index block 和 bloom block 的cache 依然有效
	- BLOOMFILTER: Valid values are NONE, ROW (default), or ROWCOL

## Data Manipulation commands
- `put 'table', 'row', 'family:qualifier','value'`
- `get 'table', 'row'`
- `get 'table', 'row', 'family'`
- `get 'table', 'row', 'family:qualifier'`
- MAXLENGTH 限制返回的数据的长度 `get 'testtable', 'rowlong', MAXLENGTH => 60`

- `delete 'table', 'row', 'family:qualifer'`
- `deleteall 'table', 'row'`

- `scan 'hbase:meta', LIMIT => 5` 查看所有表的 region 信息
- `scan 'hbase:meta', {FILTER => "PrefixFilter('table_name')"}`: 特定表单region 信息

## HBase surgery tools
- `move 'ENCODED_REGIONNAME', 'SERVER_NAME'`: 将一个region move 到指定的region server 上, ENCODED REGIONNAME 不带最后的`.`
- major compact
	- Compact all regions in a table: `hbase> major_compact 't1'`
	- Compact an entire region: `hbase> major_compact 'r1'`
	- Compact a single column family within a region: `hbase> major_compact 'r1', 'c1'`
	- Compact a single column family within a table: `hbase> major_compact 't1', 'c1'`
- minor compact 的命令'compact' 也有和 major compact 的一样的使用方式, 可以指定table, region, column family
- `balancer`: Trigger the cluster balancer
- ` balance_switch true/false`: Enable/Disable balancer, 即使把集群重启, 这个配置会被保留

### 特殊的命令
```shell
hbase(main):001:0> Bytes.toLong("\x00\x00\x01/s@3\xD8".to_java_bytes)
=> 1303308678104
hbase(main):002:0> Time.at(Bytes.toLong("\x00\x00\x01/s@3\xD8".to_java_bytes) / 1000)
=> 2011-04-20 14:11:18 +0000
hbase(main):002:0> org.apache.hadoop.hbase.util.Bytes.toInt("\x00\x01\x06[".to_java_bytes)
=> 67163
```

```shell
hbase(main):004:0> java.text.SimpleDateFormat.new("yyy/MM/dd HH:mm:ss").parse("2011/05/30 20:56:29")
=> #<Java::JavaUtil::Date:0x28fa541>
hbase(main):005:0> java.text.SimpleDateFormat.new("yyy/MM/dd HH:mm:ss").parse("2011/05/30 20:56:29").getTime()
=> 1306788989000
hbase(main):006:0> java.util.Date.new(1306788989000).toString()
=> "Mon May 30 20:56:29 GMT 2011"

hbase(main):019:0> for i in 'a'..'z' do for j in 'a'..'z' do put 'testtable', "row-#{i}#{j}", "colfam1:#{j}","#{j}" end end
hbase(main):019:0> count 'testtable'
676 row(s)
Took 0.4914 seconds
=> 676 ## 26 * 26 = 676
```

# Tool
- `./bin/hbase org.apache.hadoop.hbase.io.hfile.HFile`: hfile 查看工具
- `./bin/hbase hbck`: 功能由class HBaseFsck 来实现, hbck 工具会扫描 `.META` 表来收集其持有的所有相关信息, 还会扫描HBase 使用的 HDFS 的root 目录.

[bulk load](https://blog.cloudera.com/blog/2013/09/how-to-use-hbase-bulk-loading-and-why)
`./bin/hbase org.apache.hadoop.hbase.mapreduce.LoadIncrementalHFiles hfile_dir table_name`

	- 表需要在HBase 中提前建好
	- hfile dir 需要按照 column family 布局好, 但是不要求不同region 的同一个column family 也提前分割到不同的文件, [导入的过程中hbase 会自动进行split](http://photo.weibo.com/5652735758/photos/large/photo_id/4314927866205193/album_id/4314927492922430)

# hbase-site.xml
[HBase hbase-site.xml中各参数意义](https://blog.csdn.net/u014782458/article/details/56679136)

- `hbase.rootdir`: 这个目录是region server的共享目录,用来持久化HBase, 默认 file:///tmp/hbase-${user.name}/hbase.
- `hbase.cluster.distributed`: HBase的运行模式, 默认false.false是单机模式,true是分布式模式.若为false,HBase和Zookeeper会运行在同一个JVM里面
- `hbase.master.port`: HBase的Master的端口, 默认: 60000

block

- `hbase.mapreduce.hfileoutputformat.blocksize`: 默认65536, 也就是64KB
- `hfile.block.cache.size`: 分配给HFile/StoreFile的block cache占最大堆(-Xmx setting)的比例.默认是20%,设置为0就是不分配

zookeeper

- `hbase.zookeeper.quorum`: Zookeeper集群的地址列表,用逗号分割.例如:"host1.mydomain.com,host2.mydomain.com,host3.mydomain.com". 默认是localhost,是给伪分布式用的.
- `hbase.zookeeper.property.dataDir`: ZooKeeper的zoo.conf中的配置. 快照的存储位置, 默认: `${hbase.tmp.dir}/zookeeper`
- `hbase.zookeeper.property.clientPort`: ZooKeeper的zoo.conf中的配置. 客户端连接的端口, 默认: 2181

# JRuby
`./bin/hbase org.jruby.Main ./hbase-shell-status-2.rb`

```ruby
include Java
import org.apache.hadoop.hbase.HBaseConfiguration
import org.apache.hadoop.hbase.client.HBaseAdmin

conf = HBaseConfiguration.new
admin = HBaseAdmin.new(conf)
tables = admin.listTables
tables.each{ |table| puts table.getNameAsString() }
```


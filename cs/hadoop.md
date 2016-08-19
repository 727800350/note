单机测试: `cat input | ./mapper.sh | sort | reducer.sh > output`

# streaming and bistreaming
- streaming 处理明文的数据, 如果要处理带seq 头的文件, 可以用文件列表作为输入(注意, mapper 读到的key 是字节偏移, value 才是文件路径, 所以需要使用awk取$NF), 然后get 到本地, 手动去seq 头(若要输出seq头的文件, 则相反处理)
- bistreaming 处理二进制的数据, 输入与输出都是带seq头的kv 数据, 一个kv 是一个record, 对于用户的程序, 输入与输出均为kv 格式, 去seq 与加seq 头都是bistreaming 框架自动完成.
	如果要输出明文数据, 需要put 到hdfs 上.
	必须加上`-inputformat "org.apache.hadoop.mapred.SequenceFileAsBinaryInputFormat" -outputformat "org.apache.hadoop.mapred.SequenceFileAsBinaryOutputFormat"`

# hadoop cmd
## 文件命令
- hadoop dfs -mkdir path: 相当于linux shell 中的 mkdir -p
- hadoop dfs -get path: 可以get 到多个文件, 参数可以是一个目录
- hadoop dfs -lsr 递归遍历
- To set replication of an individual file to 4: `hadoop dfs -setrep -w 4 /path/to/file`.
	do this recursively. To change replication of entire HDFS to 1: `hadoop dfs -setrep -R -w 1 path`
- 统计出目录数,文件数及指定路径下文件的大小, `hadoop fs -count [-q] <paths>` 输出列为: `DIR_COUNT, FILE_COUNT, CONTENT_SIZE FILE_NAME`.
	带上-q选项后的输出列为: QUOTA, REMAINING_QUOTA, SPACE_QUOTA, REMAINING_SPACE_QUOTA, DIR_COUNT, FILE_COUNT, CONTENT_SIZE, FILE_NAME.
	但是这个命令的效率很低

- DistCp(分布式拷贝)是用于大规模集群内部和集群之间拷贝的工具.
	它使用方便,传输高效,并且能错误处理和断点续传.
	它使用Map/Reduce实现文件分发,错误处理和恢复,以及报告生成.
	它把文件和目录的列表作为map任务的输入,每个任务会完成源列表中部分文件的拷贝.

		hadoop distcp \
			-D mapred.max.map.failures.percent=5 \
			-D mapred.job.map.capacity=20 \
			-su user,passwd \
			-du user,passwd \
			hdfs://host:port/src_path \
			hdfs://host:port/dst_path

	- -overwrite 将强行覆盖dst目录中和src重名的文件
	- -update 将比较同名文件大小,若不一致则覆盖
	- -f 可以将待传输的文件或目录写到平台上存放的一个文件中, 作为输入,多目录可以采用这种方式
	- -p参数是保留dst文件的权限位是否与src集群的文件权限位一致,若不加此选项则在dst集群上生成的文件的权限信息将于distcp的ugi信息一致.
	- -i 若在拷贝的过程中出现错误,则忽略此错误,否则就会进行4次重试直到整个任务失败.

- `hadoop fs -dus /path/to/directory`: 显示的文件大小表示的是文件占用的逻辑空间, 这个空间是没有考虑备份数的.

## 任务管理
- 调整map并发:hadoop job -set-map-capacity JOB_ID number(Reduce同样)
- 调整reduce并发:hadoop job -set-reduce-capacity JOB_ID number
- 调整优先级:hadoop job -set-priority <job-id> <LOW/NORMAL/HIGH/VERY_HIGH>
- 设置是否运行超过并发数:hadoop job -set-map-over-capacity <job-id> <true/false>
- 杀掉任务:hadoop job -kill <job-id>
- 因特殊情况,需要挂起任务:hadoop job -suspend <job-id> <hours>
- 恢复挂起的任务:hadoop job -recover <job-id>
- 手动让一个任务失败, 之后hadoop 会再次尝试运行这个任务: hadoop job -fail-task attempt_20150507164539_1386984_m_000236_0
- 获取状态 hadoop job -status jobid

# 环境变量
- mapred_task_partition: 当前任务是全局map或reduce中的第几个,例如0号reduce,则该变量为0, 3号map,则该变量为3, 一般用这个数字来标记put 的文件名字
- map_input_file: 当前map读入的文件绝对路径
- mapred_work_output_dir: 计算临时输出路径, 多路数据输出时, 先将文件上传到临时目录`$mapred_work_output_dir`中(文件名字需要和标准输出的part 区分开来), 任务结束后hadoop平台自动将其移动到最终输出目录$mapred_output_dir中, 
	这样能保证在预测执行打开的情况下, 一个task 的多个attempt不会相互冲突
- mapred_output_dir: 就是启动MapReduce 任务时设置的 output 目录
- mapred_job_id: 当前作业ID, mapred_job_id="job_200902192042_0075"
- mapred_job_name: 当前作业名, mapred_job_name="ps_spider_css_mapreduce_job_setp1"
- mapred_tip_id: 当前任务ID, mapred_tip_id="task_200902192042_0075_m_000000"
- mapred_task_id: 当前任务是TIP的第几次重试, mapred_task_id="attempt_200902192042_0075_m_000000_0"
- mapred_task_is_map: 当前任务是map还是reduce, mapred_task_is_map="true"
- mapred_map_tasks, 计算的map任务数, mapred_map_tasks="2"
- mapred_reduce_tasks, 计算的reduce任务数, mapred_reduce_tasks="1"

# job config
## IO
- -inputformat org.apache.hadoop.mapred.lib.NLineInputFormat 一行一行地处理, `while read line; do echo $line; done`

## map 与 reduce 数目
- -D mapred.map.tasks: 这个参数决定了任务个数,但不绝对等于任务个数,还取决于输入文件的大小,分片大小等因素
- -D mapred.reduce.tasks: reduce任务数, 这个是绝对相等的
- -D mapred.job.map.capacity: map并行度, 最多同时运行map任务数
- -D mapred.job.reduce.capacity: reduce并行度, 最多同时运行reduce任务数
- -D mapred.map.over.capacity.allowed (true): 是否允许当有空余资源时超过并发限制
- -D mapred.reduce.over.capacity.allowed (true) 

- -D mapred.min.split.size: 最小分片大小(单位B),通过增大此值,可以减少Map数量

## 内存资源
- -D stream.memory.limit="12345" #设置map/reduce中slaver启动实例占用内存, 默认都为800MB
- -D mapred.map.memory.limit="12345"
- -D mapred.reduce.memory.limit="12345"

- -D mapred.blacklist.percent (0.25, 不是百分比!)
- -D mapred.max.tracker.failures (4): 如果task在一个tasktracker上失败超过mapred.max.tracker.failures次, 则不再分配新任务,
	但如果这样的tasktracker总数超过集群大小乘以mapred.blacklist.percent时不再有此限制.
- -D mapred.max.map.failures.percent (0)
- -D mapred.max.reduce.failures.percent (0) 失败的task数超过总task数的此百分比后,认为job失败,杀死该job.  默认为0即只要有一个, task失败整个job就失败

- -D mapred.map.tasks.speculative.execution (true)
- -D mapred.reduce.tasks.speculative.execution (true) 会对运行慢的任务起一个备份任务, 待某个完成之后, kill 掉备份任务.

- -D mapred.map.max.attempts="10": 单个任务重试次数
- -D mapred.dynamic.input (false): 是否启用了dynamic input

## partitioner
- -partitioner org.apache.hadoop.mapred.lib.KeyFieldBasedPartitioner 一般使用的partitioner, **即使 reduce 的数目大于 partition key的种类, 也有可能不同的 key 会被分到同一个 reduce 上**

- -partitioner com.baidu.sos.mapred.lib.IntHashPartitioner 严格按照int[0, num_reducer - 1] 值进行partition
	- map输出每一行为:`N \space key \t value`, 这个输出经过IntHashPartitioner后被传给第N个reducer处理, N是大于等于0的int
	- reducer的输入格式为:`N \space key \t value`. 到达reduce 的值是按照key 排好序的.

### 分隔符
默认情况下Streaming框架将map输出的每一行第一个"\t"之前的部分作为key,之后的部分作为value,key\tvalue又作为 reduce的输入.

- -D stream.map.output.field.separator=":": 指定使用冒号":"将map输出的一行分隔为key/value
- -D stream.num.map.output.key.fields=2: 指定在第二个冒号处进行分隔,也就是第二个冒号之前的作为key,之后的作为value.如果没有冒号或冒号少于两个,则key为整行,value为空
- -D stream.reduce.output.field.separator
- -D stream.num.reduce.output.key.fields定制key/value分隔方式

```
hadoop streaming
	-partitioner org.apache.hadoop.mapred.lib.KeyFieldBasedPartitioner
	-D stream.map.output.field.separator= . ## map输出分隔符为., 而不是默认的\t
	-D stream.num.map.output.key.fields=4 ## map的输出行第4个英文句号"."之前为key,后面为value
	-D map.output.key.field.separator=.  ## 指定key的内部用英文句号"."分隔
	-D num.key.fields.for.partition=2  ## 指定将key分隔出来的前两个部分而不是整个key用于Partitioner做partition
```
例如Map的输出如下,第4个句号前的数字部分是key,后面是value:
```
raw input ==> <key, value> ==> <partition key, the rest of key, value>
11.12.1.2.value3 ==> <11.12.1.2, value3> ==> <11.12, 1.2, value3>
11.14.2.3.value5 ==> <11.14.2.3, value5> ==> <11.14, 2.3, value5>
11.10.4.1.value1 ==> <11.10.4.1, value1> ==> <11.10, 4.1, value1>
11.12.1.1.value2 ==> <11.12.1.1, value2> ==> <11.12, 1.1, value2>
11.14.2.2.value4 ==> <11.14.2.2, value4> ==> <11.14, 2.2, value4>
```
partition key有 11.10, 11.12, 11.14三种, 所以会被分到三个reducer上(如果不同的partition key分到不同的reducer上)

## 分发文件
- -file <fileNameURI> 一般的脚本文件, 可执行文件, 配置文件等, 上传后都在当前目录, 且没有x 权限, 需手动chmod
- -cacheArchive <fileNameURI> 如果要分发的文件有目录结构,可以先将整个目录打包,然后上传到HDFS,再用`-cacheArchive hdfs://host:port/path/to/archivefile.tar.gz#linkname`分发压缩包.

# 任务优化
- -mapred.reduce.slowstart.completed.maps 如果你设置为0.6, 那么reduce将在map完成60%后进入运行态.如果设置的map和reduce数量都很大,势必造成map和reduce争抢资源,造成有些进程饥饿,超时出错,最大的可能就是socket.timeout的出错,网络过于繁忙.
	所以说,这些需要根据集群的性能,适当调试添加和减少,以达到最好的效果. 如果你发现reduce在33%时,map正好提早一点点到100%,那么这将是最佳的配比,因为reduce是在33%的时候完成了数据copy阶段,
	也就是说,map需要再reduce到达33%之前完成所有的map任务,准备好数据.千万不能让reduce在等待,但是可以让map先完成.

- -io.sort.mb 设置一个map sort的可用buffer大小是多少,如果map在内存中sort的结果达到一个特定的值,就会被spill进入硬盘.具体这个值是等于mb*io.sort.spill.percent.
	按照通常的设置方式,为了让jvm发挥最佳性能,一般设置JVM的最大可用内存量为mb设置的内存量的两倍.
	如果一个map的结果数据量为600M,那么如果你设置的mb*io.sort.spill.percent=200M,那么将进行3次spill进入硬盘,然后map完成后再将数据从硬盘上取出进行copy.
	mb设置如果是600M的话,那么就不需要进行这次硬盘访问了,节省了很多时间. 但是最大的问题是内存耗费很大.如果mb是600M,那么jvm.opts将需要设置为1G以上

打开预测执行

- -mapred.map.tasks.speculative.execution=true
- -mapred.reduce.tasks.speculative.execution=true

配置慢节点I/O检测参数,在启动任务的命令中添加,不支持任务运行后动态调整.
由于集群机器性能配置不一, 不同计算结点的处理能力不一样,设置该参数是为了跳过慢节点,选择更好节点处理task.
这里对map和reduce设置了至少5M的读写速度限制, 小于该值就会切换节点. 可根据自己业务的具体情况设置,切忌设置过大值,可能会引起节点频繁切换导致任务失败.

- -jobconf dfs.client.slow.write.limit=5242880
- -jobconf dfs.client.slow.read.limit=5242880


合并小文件

- -inputformat org.apache.hadoop.mapred.CombineTextInputFormat \
- -jobconf mapred.max.split.size=5368709120 \ ## (5GB)

使用压缩  
经过测试在时间性能上:lzo>gzip>lzma,即lzo压缩运行时间最短,gzip次之,运行最长的是lzma.
在压缩比上:刚好相反,lzma压缩比最大,试验中压缩比为3.83/1,gzip的压缩比为2.75/1,lzo压缩比为1.6/1
Lzo一般用于内置轻量级压缩,用在中间结果压缩,不建议外部使用,即适合map的输出进行压缩,不建议作为作业reduce最终输出的压缩.

- 适合中间结果
	- org.apache.hadoop.io.compress.LzoCodec
	- org.apache.hadoop.io.compress.QuickLzCodec
	- 使用
		- `-jobconf mapred.compress.map.output="true"`
		- `-jobconf mapred.map.output.compression.codec="org.apache.hadoop.io.compress.QuickLzCodec"`
- 适合输入输出
	- org.apache.hadoop.io.compress.GzipCodec
	- org.apache.hadoop.io.compress.LzmaCodec
	- 使用
		- `-jobconf mapred.output.compress="true"`
		- `-jobconf mapred.output.compression.codec="org.apache.hadoop.io.compress.GzipCodec"`


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
- mapred_task_partition: 当前任务是全局map或reduce中的第几个,例如0号reduce,则该变量为0, 3号map,则该变量为3
- map_input_file: 当前map读入的文件, map_input_file="hdfs://yx-mapred-a001.yx01.baidu.com:64310/user/spider/index-analysis/data/css/csspacktest"
- mapred_work_output_dir: 就是启动MapReduce 任务时设置的 output 目录
- mapred_job_id: 当前作业ID, mapred_job_id="job_200902192042_0075"
- mapred_job_name: 当前作业名, mapred_job_name="ps_spider_css_mapreduce_job_setp1"
- mapred_tip_id: 当前任务ID, mapred_tip_id="task_200902192042_0075_m_000000"
- mapred_task_id: 当前任务是TIP的第几次重试, mapred_task_id="attempt_200902192042_0075_m_000000_0"
- mapred_task_is_map: 当前任务是map还是reduce, mapred_task_is_map="true"
- mapred_output_dir: 计算输出路径, mapred_output_dir="hdfs://yx-mapred-a001.yx01.baidu.com:64310/user/spider/index-analysis/data/outputTest"
- mapred_map_tasks, 计算的map任务数, mapred_map_tasks="2"
- mapred_reduce_tasks, 计算的reduce任务数, mapred_reduce_tasks="1"

# job config
## map 与 reduce 数目
- -D mapred.map.tasks: 这个参数决定了任务个数,但不绝对等于任务个数,还取决于输入文件的大小,分片大小等因素
- -D mapred.min.split.size: 最小分片大小(单位B),通过增大此值,可以减少Map数量
- -D mapred.map.capacity.per.tasktracker=1
- -D mapred.job.map.capacity: map并行度, 最多同时运行map任务数

- -D mapred.reduce.tasks: reduce任务数, 这个是绝对相等的
- -D mapred.job.reduce.capacity: reduce并行度, 最多同时运行reduce任务数

- -D mapred.map.over.capacity.allowed (true): 是否允许当有空余资源时超过并发限制
- -D mapred.reduce.over.capacity.allowed (true) 

## 内存资源
- -D stream.memory.limit="12345" #设置map/reduce中slaver启动实例占用内存, 默认情况下mapper与reducer都为 800MB
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
默认的 partitioner 为KeyFieldBasedPartitioner(`org.apache.hadoop.mapred.lib.KeyFieldBasedPartitioner`),
即使 reduce 的数目多余 partition, 也有可能不同的 key 会被分到同一个 reduce 上.
使用百度提供的`com.baidu.sos.mapred.lib.IntHashPartitioner` 则可以避免这种情况

map输出每一行为:`N \space key \t value`,这个输出经过IntHashPartitioner后被传给第N个reducer处理, N是大于等于0的int
reducer的输入格式为:`N \space key \t value`. 到达reduce 的值是按照key 排好序的.

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
### -file <fileNameURI>
一般的脚本文件, 可执行文件, 配置文件等
如果要分发的文件在本地且没有目录结构,可以使用-file /path/to/FILENAME选项分发文件,将本地文件/path/to/FILENAME分发到每个计算节点.
在Streaming程序中通过./FILENAME就可以访问该文件.

对于本地可执行的文件,除了指定的mapper或reducer程序外,可能分发后没有可执行权限,
所以需要在包装程序如mapper.sh中运行chmod +x ./FILENAME设置可执行权限

### -cacheFile <fileNameURI> 
如果文件(如字典文件)存放在HDFS中,希望计算时在每个计算节点上将文件当作本地文件处理,
可以使用`-cacheFile hdfs://host:port/path/to/file#linkname`选项在计算节点缓存文件,
Streaming程序通过./linkname访问文件.
一般不便于分发的大文件可以采用这种方式让计算节点访问到.

### -cacheArchive <fileNameURI>
如果要分发的文件有目录结构,可以先将整个目录打包,然后上传到HDFS,再用`-cacheArchive hdfs://host:port/path/to/archivefile#linkname`分发压缩包.
本地打包时要进入目录app而不是在app的上层目录打包,否则要通过app/app/mapper.pl才能访问到mapper.pl文件.
hadoop支持zip, jar, tar.gz格式的压缩包,由于Java解压zip压缩包时会丢失文件权限信息而且遇到中文文件名会出错,所见建议采用tar.gz压缩包.

## streaming and bistreaming
input 一行一个数字
```
1
2
3
```
- 如果是 streaming, 那么 mapper 获取到的是 key value, 其中key 是偏移量, value 是输入文件中的数字
- 如果是 bistreamling, 那么mapper 获取到的是其他编码的数据

# IO
**多路数据输出**:
先将文件上传到临时目录`$mapred_work_output_dir`中, 任务结束后hadoop自动将其移动到最终输出目录, 这样能保证不同的mapper或 reducer上传数据不会冲突


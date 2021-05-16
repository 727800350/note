[TiDB 简介](https://docs.pingcap.com/zh/tidb/stable/overview)

TiDB 是PingCAP 公司自主设计, 研发的开源分布式关系型数据库, 是一款同时支持在线事务处理与在线分析处理(Hybrid Transactional
and Analytical Processing, HTAP) 的融合型分布式数据库产品, 具备水平扩容或者缩容, 金融级高可用, 实时HTAP, 云原生的分布式
数据库, 兼容MySQL 5.7 协议和MySQL 生态等重要特性.
目标是为用户提供一站式OLTP (Online Transactional Processing), OLAP(Online Analytical Processing),HTAP 解决方案.
TiDB 适合高可用,强一致要求较高,数据规模较大等各种应用场景.

# architecture
[TiDB 整体架构](https://docs.pingcap.com/zh/tidb/stable/tidb-architecture)

与传统的单机数据库相比,TiDB 具有以下优势:

- 纯分布式架构,拥有良好的扩展性,支持弹性的扩缩容
- 支持SQL, 对外暴露 MySQL 的网络协议,并兼容大多数MySQL 的语法,在大多数场景下可以直接替换 MySQL
- 默认支持高可用,在少数副本失效的情况下,数据库本身能够自动进行数据修复和故障转移,对业务透明
- 支持 ACID 事务,对于一些有强一致需求的场景友好,例如:银行转账
- 具有丰富的工具链生态,覆盖数据迁移,同步,备份等多种场景
- 在内核设计上,TiDB 分布式数据库将整体架构拆分成了多个模块,各模块之间互相通信,组成完整的 TiDB 系统.对应的架构图如下:

<img src="./pics/tidb/tidb-architecture-v3.1.png" alt="tidb architecture" width="50%"/>

## TiDB Server
SQL 层, 对外暴露MySQL 协议的连接endpoint, 负责接受客户端的连接, 执行SQL 解析和优化, 最终生成分布式执行计划.
TiDB 层本身是无状态的, 实践中可以启动多个TiDB 实例,通过负载均衡组件(如 LVS,HAProxy 或 F5)对外提供统一的接入地址,客户端的
连接可以均匀地分摊在多个 TiDB 实例上以达到负载均衡的效果.
TiDB Server 本身并不存储数据,只是解析 SQL,将实际的数据读取请求转发给底层的存储节点 TiKV(或 TiFlash).

## PD (Placement Driver) Server
整个 TiDB 集群的元信息管理模块,负责存储每个 TiKV 节点实时的数据分布情况和集群的整体拓扑结构,提供 TiDB Dashboard 管控界面,
并为分布式事务分配事务ID.
PD不仅存储元信息,同时还会根据TiKV 节点实时上报的数据分布状态,下发数据调度命令给具体的TiKV 节点,可以说是整个集群的"大脑".
此外,PD 本身也是由至少 3 个节点构成,拥有高可用的能力.建议部署奇数个 PD 节点.

TSO时钟分配

通过raft实现多副本数据同步,保证高可用.

## 存储节点
### TiKV Server
负责存储数据,从外部看 TiKV 是一个分布式的提供事务的 Key-Value 存储引擎.存储数据的基本单位是Region,每个Region 负责存储一
个 Key Range(从 StartKey 到 EndKey 的左闭右开区间)的数据,每个 TiKV 节点会负责多个 Region.
TiKV 的 API 在 KV 键值对层面提供对分布式事务的原生支持,默认提供了SI (Snapshot Isolation) 的隔离级别,这也是 TiDB 在SQL 层
面支持分布式事务的核心.
TiDB 的 SQL 层做完 SQL 解析后,会将 SQL 的执行计划转换为对 TiKV API 的实际调用.所以,数据都存储在 TiKV 中.
另外,TiKV 中的数据都会自动维护多副本(默认为三副本),天然支持高可用和自动故障转移.

TiKV Server 底层采用RocksDB作为本地KV存储引擎, 在之上构建了业务MVCC.

### TiFlash
一类特殊的存储节点.和普通 TiKV 节点不一样的是,在TiFlash 内部,数据是以列式的形式进行存储,主要的功能是为分析型的场景加速.

TiFlash 提供列式存储,且拥有借助 ClickHouse 高效实现的协处理器层.除此以外,它与 TiKV 非常类似,依赖同样的 Multi-Raft 体系,
以 Region 为单位进行数据复制和分散.

TiFlash 暂时无法直接接受数据写入,任何数据必须先写入 TiKV 再同步到 TiFlash.TiFlash 以 learner 角色接入 TiDB 集群.

# storage
[TiDB 数据库的存储](https://docs.pingcap.com/zh/tidb/stable/tidb-storage)

# computing
[TiDB 数据库的计算](https://docs.pingcap.com/zh/tidb/stable/tidb-computing)

# schedule
[TiDB 数据库的调度](https://docs.pingcap.com/zh/tidb/stable/tidb-scheduling)


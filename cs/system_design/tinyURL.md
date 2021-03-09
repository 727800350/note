- [短网址的长度](#短网址的长度)
- [一对一还是一对多映射?](#一对一还是一对多映射)
- [如何计算短网址](#如何计算短网址)
  - [UUID](#uuid)
  - [多台MySQL服务器](#多台mysql服务器)
  - [分布式ID生成器(Distributed Id Generator)](#分布式id生成器distributed-id-generator)
- [如何存储](#如何存储)
- [301还是302重定向](#301还是302重定向)
- [预防攻击](#预防攻击)

ref

- [如何设计短网址系统(TinyURL)](http://cn.soulmachine.me/2017-04-10-how-to-design-tinyurl/)
- [短URL 系统是怎么设计的?](https://www.zhihu.com/question/29270034)

# 短网址的长度
短网址的长度该设计为多少呢? 当前互联网上的网页总数大概是 45亿(参考 http://www.worldwidewebsize.com), 超过了
2^32=4294967296, 那么用一个64位整数足够了.

一个64位整数如何转化为字符串呢? 假设我们只是用大小写字母加数字, 那么可以看做是62进制数
(26个小写字母+26个大写字母+10个数字=62), log62(264−1)=10.7, 即字符串最长11就足够了.

实际生产中,还可以再短一点,比如新浪微博采用的长度就是7,因为62^7=3521614606208, 这个量级远远超过互联网上的URL总数了,绝对够
用了.

现代的web服务器(例如Apache, Nginx)大部分都区分URL里的大小写了,所以用大小写字母来区分不同的URL是没问题的.

因此,正确答案: 长度不超过7的字符串,由大小写字母加数字共62个字母组成

# 一对一还是一对多映射?
一个长网址,对应一个短网址,还是可以对应多个短网址? 这也是个重大选择问题

一般而言,一个长网址,在不同的地点,不同的用户等情况下,生成的短网址应该不一样,这样,在后端数据库中,可以更好的进行数据分析.
如果一个长网址与一个短网址一一对应,那么在数据库中,仅有一行数据,无法区分不同的来源,就无法做数据分析了.

以这个7位长度的短网址作为唯一ID,这个ID下可以挂各种信息,比如生成该网址的用户名,所在网站,HTTP头部的 User Agent等信息,收集
了这些信息,才有可能在后面做大数据分析,挖掘数据的价值.短网址服务商的一大盈利来源就是这些数据.

正确答案:一对多

# 如何计算短网址
现在我们设定了短网址是一个长度为7的字符串,如何计算得到这个短网址呢?

最容易想到的办法是哈希,先hash得到一个64位整数,将它转化为62进制整,截取低7位即可.但是哈希算法会有冲突,如何处理冲突呢,又是
一个麻烦.这个方法只是转移了矛盾,没有解决矛盾,抛弃.

MySQL数据库有一个自增ID,能不能借鉴这个呢?每来一个长网址,就给它发一个号码,这个号码不断的自增.这个方法跟哈希相比,好处是没
有冲突,不用考虑处理冲突的问题.如何实现单台的发号服务器呢?可以用一台MySQL服务器来做(一定要用REPLACE INTO,不要存储所有ID),
也可用一台Redis服务器(用INCR),一行代码也不用写,也可以自己写一个RESTful API,代码也很简单,就不赘述了.

单台发号器有什么缺点呢?它是一个单点故障(SPOF, Single Point Of Failure),也会成为性能瓶颈(其实,如果你的QPS能大到压垮这台
MySQL,那说明你的短网址服务很成功,可以考虑上市了),所以它适合中小型企业,对于超大型企业(以及在面试显得追求高大上),我们还是
要继续思考更好的方案,请接着往下看.

下面开始讲如何打造多台机器组成的分布式发号器.

## UUID
使用UUID算法或者MongoDB产生的ObjectID.其实MongoDB的ObjectID也算是一种UUID,这类算法,每台机器可以独立工作,天然是分布式的,
但是这类算法产生的ID通常都很长,那短网址服务还有什么意义呢?所以这个方法不行.

## 多台MySQL服务器
前面讲了单台MySQL作为发号服务器,那么自然可以扩展一下,比如用8台MySQL服务器协同工作,第一台MySQL初始值是1,每次自增8,第二台
MySQL初始值是2,每次自增8,依此类推.前面用一个 round-robin load balancer 挡着,每来一个长网址请求,由round-robin balancer 随
机地将请求发给8台MySQL中的任意一个,然后返回一个ID.

Flickr用的就是这个方案,仅仅使用了两台MySQL服务器.
这个方法仅有的一个缺点是,ID是连续的,容易被爬虫抓数据,爬虫基本不用写代码,顺着ID一个一个发请求就是了,太方便了.

## 分布式ID生成器(Distributed Id Generator)
分布式的产生唯一的ID,比如Twitter 有个成熟的开源项目,就是专门做这个的, Twitter Snowflake. Snowflake的核心算法如下:

<img src="./pics/tinyurl/snowflake-64bit.jpg" alt="snowflake" width="50%"/>

最高位不用,永远为0,其余三组bit占位均可浮动,看具体的业务需求而定.默认情况下41bit的时间戳可以支持该算法使用到2082年,10bit
的工作机器id可以支持1023台机器,序列号支持1毫秒产生4095个自增序列id.

Instagram用了类似的方案,41位表示时间戳,13位表示shard Id(一个shard Id对应一台PostgreSQL机器),最低10位表示自增ID,怎么样,
跟Snowflake的设计非常类似吧.这个方案用一个PostgreSQL集群代替了Twitter Snowflake 集群,优点是利用了现成的PostgreSQL,容易
懂,维护方便.

因此,正确答案:分布式发号器(Distributed ID Generator),Flick, Twitter Snowflake 和 Instagram的方案都是不错的选择.

# 如何存储
如果存储短网址和长网址的对应关系?以短网址为 primary key, 长网址为value, 可以用传统的关系数据库存起来,例如MySQL,
PostgreSQL,也可以用任意一个分布式KV数据库,例如Redis, LevelDB.

# 301还是302重定向
这也是一个有意思的问题.这个问题主要是考察你对301和302的理解,以及浏览器缓存机制的理解.

301是永久重定向,302是临时重定向.短地址一经生成就不会变化,所以用301是符合http语义的.但是如果用了301, Google,百度等搜索引
擎,搜索的时候会直接展示真实地址,那我们就无法统计到短地址被点击的次数了,也无法收集用户的Cookie, User Agent 等信息,这些信
息可以用来做很多有意思的大数据分析,也是短网址服务商的主要盈利来源.

所以,正确答案是302重定向.

# 预防攻击
如果一些别有用心的黑客,短时间内向TinyURL服务器发送大量的请求,会迅速耗光ID,怎么办呢?

首先,限制IP的单日请求总数,超过阈值则直接拒绝服务.

光限制IP的请求数还不够,因为黑客一般手里有上百万台肉鸡的,IP地址大大的有,所以光限制IP作用不大.

可以用一台Redis作为缓存服务器,存储的不是 ID->长网址, 而是 长网址->ID, 仅存储一天以内的数据,用LRU机制进行淘汰.这样,如果黑
客大量发同一个长网址过来,直接从缓存服务器里返回短网址即可,他就无法耗光我们的ID了.

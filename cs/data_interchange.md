# Json
## Example
```json
{
	"name": "John Simith",
	"age": 25,
	"address": {
		"city": "New York",
		"state": "NY",
	},
	"phoneNumbers": [
		{"type": "home", "number": "212 555-1234"},
		{"type": "fax", "number": "646 555-4567"}
	]
}
```

## Format
1. 并列的数据之间用逗号(",")分隔
2. 映射用冒号(":")表示
3. 并列数据的集合(数组)用方括号("[]")表示
4. 映射的集合(对象)用大括号("{}")表示

comments in JSON are not allowed.
也就是说JSON中全是data(但是可以在json 中增加一个comment的字段来加)

## API
### python
import json

- `json.load(fp)`: Deserialize fp (fp.read()-supporting file-like object containing a JSON document) to a Python object(dict)
- `json.loads(s, encoding=None)`: Deserialize s(a str instance containing a JSON document) to a Python object

If the data being deserialized is not a valid JSON document, a JSONDecodeError will be raised.

- json.dump(obj, fp): Serialize obj as a JSON formatted stream to fp (fp.write()-supporting file-like object)
- json.dumps(obj): Serialize obj to a JSON formatted str


# Message Queue(MQ)
消息队列可驻留在内存或磁盘上, 队列存储消息直到它们被应用程序消费.

message queue的通讯模式

- 点对点通讯: 点对点方式是最为传统和常见的通讯方式,它支持一对一,一对多,多对多,多对一等多种配置方式,支持树状,网状等多种拓扑结构.
- 多点广播: 能够将消息发送到多个目标站点 (Destination List).可以使用一条 MQ 指令将单一消息发送到多个目标站点,并确保为每一站点可靠地提供信息.
- 发布/订阅(publish/subscribe)模式: 使消息按照特定的主题甚至内容进行分发,用户或应用程序可以根据主题或内容接收到所需要的消息
- 集群: 为了简化点对点通讯模式中的系统配置, MQ 提供 Cluster(群集)的解决方案.
	群集类似于一个域 (Domain),群集内部的队列管理器之间通讯时,不需要两两之间建立消息通道,而是采用群集 (Cluster) 通道与其它成员通讯,从而大大简化了系统配置.

- RabbitMQ是一个AMQP实现,传统的messaging queue系统实现,基于Erlang.老牌MQ产品了.AMQP协议更多用在企业系统内,对数据一致性,稳定性和可靠性要求很高的场景,对性能和吞吐量还在其次.
	不单单做到了简单的数据转发功能,还保证了单个队列上的数据有序,即便是有多个消费者和多个生产者.
	- RabbitMQ的消息应当尽可能的小,并且只用来处理实时且要高可靠性的消息.
	- 消费者和生产者的能力尽量对等,否则消息堆积会严重影响RabbitMQ的性能.
	- 集群部署,使用热备,保证消息的可靠性.
- Kafka是linkedin开源的MQ系统,主要特点是基于Pull的模式来处理消息消费,追求高吞吐量,一开始的目的就是用于日志收集和传输.
	0.8开始支持复制,不支持事务,适合产生大量数据的互联网服务的数据收集业务.
	- 应当有一个非常好的运维监控系统,不单单要监控Kafka本身,还要监控Zookeeper.
	- 对消息顺序不依赖,且不是那么实时的系统.
	- 对消息丢失并不那么敏感的系统.
- ZeroMQ只是一个网络编程的Pattern库, 将常见的网络请求形式(分组管理,链接管理,发布订阅等)模式化,组件化,简而言之socket之上,MQ之下.
	对于MQ来说,网络传输只是它的一部分,更多需要处理的是消息存储,路由,Broker服务发现和查找,事务,消费模式(ack,重投等),集群服务等.

## RabbitMQ
AMQP,即Advanced Message Queuing Protocol, 一个提供统一消息服务的应用层标准高级消息队列协议,是应用层协议的一个开放标准,为面向消息的中间件设计.
基于此协议的客户端与消息中间件可传递消息,并不受客户端/中间件不同产品,不同开发语言等条件的限制.
Erlang中的实现有 RabbitMQ等.

[rabbitmq-tutorials](https://github.com/rabbitmq/rabbitmq-tutorials)

- $ sudo rabbitmqctl list_queues name messages_ready messages_unacknowledged
- $ sudo rabbitmqctl list_exchanges
- $ sudo rabbitmqctl list_bindings

### work queue
The main idea behind Work Queues (aka: Task Queues) is to avoid doing a resource-intensive task immediately and having to wait for it to complete.
Instead we schedule the task to be done later. We **encapsulate a task as a message and send it to the queue**.
A worker process running in the background will pop the tasks and eventually execute the job. When you run many workers the tasks will be shared between them.

可是如果producer 需要得到这个计算任务的结果呢?怎么办, 怎么把结果取回来?

By default, RabbitMQ will send each message to the next consumer, in sequence. On average every consumer will get the same number of messages.
This way of distributing messages is called round-robin.

#### 持久化
##### consumer dies
Message acknowledgments are turned on by default. In previous examples we explicitly turned them off via the `no_ack=True` flag.
`channel.basic_consume(callback, queue='hello', no_ack=True)`

1. 默认开启ack模式 `channel.basic_consume(callback, queue = 'hello')`
1. 在callback 函数主动发送一个ack `channel.basic_ack(delivery_tag = method.delivery_tag)`

##### rabbitmq server dies
mark queue durable
```
channel.queue_declare(queue='hello',
					  durable=True)
```

mark messages durable
```
channel.basic_publish(exchange='',
                      routing_key="task_queue",
                      body=message,
                      properties=pika.BasicProperties(
                         delivery_mode = 2, # make message persistent
                      ))
```

#### Fair dispatch
For example in a situation with two workers, when all odd messages are heavy and even messages are light,
one worker will be constantly busy and the other one will do hardly any work.

`channel.basic_qos(prefetch_count=1)`:
This tells RabbitMQ not to give more than one message to a worker at a time.
Or, in other words, do not dispatch a new message to a worker until it has processed and acknowledged the previous one.
Instead, it will dispatch it to the next worker that is not still busy.

If all the workers are busy, your queue can fill up.
You will want to keep an eye on that, and maybe add more workers, or have some other strategy.

### Exchange
The core idea in the messaging model in RabbitMQ is that the producer never sends any messages directly to a queue.
Instead, the **producer can only send messages to an exchange**.
An exchange is a very simple thing. On one side it receives messages from producers and the other side it pushes them to queues.
The exchange must know exactly what to do with a message it receives.
Should it be appended to a particular queue? Should it be appended to many queues? Or should it get discarded.
The rules for that are defined by the exchange type.

There are a few exchange types available: direct, topic, headers and fanout.

- '': nameless exchange, the default exchange, messages are routed to the queue with the name specified by routing_key, if it exists.
- fanout: very simple, it just broadcasts all the messages it receives to all the queues it knows
	[log example using fanout](http://www.rabbitmq.com/tutorials/tutorial-three-python.html)
	every running copy of the receiver program will get the messages. run one receiver and direct the logs to disk and run another receiver and see the logs on the screen.
- direct
	[log example using direct](http://www.rabbitmq.com/tutorials/tutorial-four-python.html)
	direct only critical error messages to the log file(to save disk space), while still being able to print all of the log messages on the console.
- topic
	[log example using topic](http://www.rabbitmq.com/tutorials/tutorial-five-python.html)
	subscribe to logs based on both severity and the source which emitted the log. eg: listen to just critical errors coming from 'cron' but also all logs from 'kern'
- headers

### RPC
[rpc example](http://www.rabbitmq.com/tutorials/tutorial-six-python.html)

What if we need to run a function on a remote computer and wait for the result?
Well, that is a different story. This pattern is commonly known as Remote Procedure Call or RPC.

To illustrate how an RPC service could be used.
we are going to create a simple client class.
A client sends a request message and a server replies with a response message.
The client is going to expose a method named call which **sends an RPC request and blocks until the answer is received**.


Use the term node to refer to a RabbitMQ server instance.
In reality, what a node really describes is an Erlang node running an Erlang application.

```
root@dev:~/git# rabbitmqctl stop_app
Stopping node rabbit@dev ...
root@dev:~/git# rabbitmqctl start_app
Starting node rabbit@dev ...
root@dev:~/git# rabbitmqctl stop
Stopping and halting node rabbit@dev ...
```

rabbitmqctl add_user user passwd
rabbitmqctl delete_user user
rabbitmqctl list_users
rabbitmqctl change_password <username> <newpassword>

Mnesia的设计要求如下:
1.快速实时的键(key)/值(value)查找
2.主要用于运营和维护的非实时复杂查询
3.由于分布式应用导致的分布式数据
4.高容错
5.动态重配置
6.复杂对象

## Apache Kafka


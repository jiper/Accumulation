<https://www.rabbitmq.com/getstarted.html>

## RabbitMQ 6种工作模式

1. 最简单的点对点通信（略）

   ------



   ### Work Queues

   ![img](https://www.rabbitmq.com/img/tutorials/python-two.png)

   - 主要用途:

   ​	负载均衡，当producer源源不断产生许多耗资源的task时，可以让多个worker来分担。worker越多，处理效率越高，而且能够相互独立运行。

   ​	如图，P不断产生task，C1和C2竞争。结果是整体处理时间减半，C1和C2的工作量相同。如果再增加worker，平均的负荷就更小了。如果谁中途退出，也没有很大影响。

   - 重要概念：**message acknowledgment**

   ​	就是通常所说的ack信号，如果producer发出的task，没有收到ack，则认为该task没有得到正确执行，那么系统会自动重发该task，从而确保每一个task能够得到正确的处理。

   开启的方式是设置参数： `no_ack = False`

   - 重要概念：**message durability**

     进一步提升消息传输的可靠性，防止丢失。

     开启的方式

     A. 声明queue时设置参数： `queue_declare(queue='task_queue', durable=True)`

     注意：此处的durable设置必须在Producer和Consumer两端同步设置才有效！

     B. 发送消息时设置参数：把delevery_mode设置为**2**

     ``

     ```python
     channel.basic_publish(exchange='',
                           routing_key="task_queue",
                           body=message,
                           properties=pika.BasicProperties(
                              delivery_mode = 2, # make message persistent
                           ))
     ```

   - 重要概念：**Fair dispatch**

     多个worker同时工作的时候，可以控制其后面预取(排队)的个数。此参数会影响性能，取值为1可以免除烦恼。

     `channel.basic_qos(prefetch_count=1)`

     比如：task执行时间是5，1，5，1，5，1.。。。此时A/B两个worker均保留两个任务。则A后面总是5，5，5.而B后面总是1，1，1

   汇总示例代码：

   producer.py

   ```python
   import pika
   import sys
   
   connection = pika.BlockingConnection(pika.ConnectionParameters(host='localhost'))
   channel = connection.channel()
   
   channel.queue_declare(queue='task_queue', durable=True)
   
   message = ' '.join(sys.argv[1:]) or "Hello World!"
   channel.basic_publish(exchange='',
                         routing_key='task_queue',
                         body=message,
                         properties=pika.BasicProperties(
                            delivery_mode = 2, # make message persistent
                         ))
   print(" [x] Sent %r" % message)
   connection.close()
   ```

   worker.py

   ```python
   import pika
   import time
   
   connection = pika.BlockingConnection(pika.ConnectionParameters(host='localhost'))
   channel = connection.channel()
   
   channel.queue_declare(queue='task_queue', durable=True)
   print(' [*] Waiting for messages. To exit press CTRL+C')
   
   def callback(ch, method, properties, body):
       print(" [x] Received %r" % body)
       time.sleep(body.count(b'.'))
       print(" [x] Done")
       ch.basic_ack(delivery_tag = method.delivery_tag) #very important, don't forget it!!!
   
   channel.basic_qos(prefetch_count=1)
   channel.basic_consume(callback,
                         queue='task_queue')
   
   channel.start_consuming()
   ```

------



   ### Publish/Subscribe(fanout广播模式)

   - 主要用途：

   ​	同时向多个消费者发送消息

   举例：产生log信息时，AB进程都收到log信息，A将该信息存盘，B将log在控制台上打印显示

   原理：

   producer的消息传输到exchange就结束了，不管谁来consumer

   ![img](https://www.rabbitmq.com/img/tutorials/exchanges.png)

   exchange有4种模式（非常重要！）

- direct(routing_key一致才能收到)
- topic(routing_key一致才能收到，但是支持正则表达式)
- headers(headers格式一致才能收到)
- fanout(广播形式，所有的consumer都能接收到)

consumer声明queue的两个要点：

1. 申请一个随机名字的queue，以保证该queue处于完全clear的状态

   `result = channel.queue_declare()`

   此时的queue name 为： result.method.queue

2. 置标志位 exclusive，保证consumer删除时，该queue得到正确销毁

   `result = channel.queue_declare(exclusive=True)`

源码示例：

producer:

```python
import pika
import sys

connection = pika.BlockingConnection(pika.ConnectionParameters(host='localhost'))
channel = connection.channel()

channel.exchange_declare(exchange='logs',
                         exchange_type='fanout')

message = ' '.join(sys.argv[1:]) or "info: Hello World!"
channel.basic_publish(exchange='logs',
                      routing_key='',
                      body=message)
print(" [x] Sent %r" % message)
connection.close()
```

consumer:

```python
import pika

connection = pika.BlockingConnection(pika.ConnectionParameters(host='localhost'))
channel = connection.channel()

channel.exchange_declare(exchange='logs',
                         exchange_type='fanout')

result = channel.queue_declare(exclusive=True)
queue_name = result.method.queue

channel.queue_bind(exchange='logs',
                   queue=queue_name)

print(' [*] Waiting for logs. To exit press CTRL+C')

def callback(ch, method, properties, body):
    print(" [x] %r" % body)

channel.basic_consume(callback,
                      queue=queue_name,
                      no_ack=True)

channel.start_consuming()
```

------



### Publish/Subscribe(Routing 路由过滤)

**主要用途：**consumer只接收感兴趣的信息

举例：A仍然是获取log并存盘，B仍然是获取log信息并显示。但是A只有获取error的时候才存盘

原理：在原来广播模式基础上增加一个routing_key将message分发到特定的queue中去，只有当consumer的routing_key匹配时才能收到信息（广播模式则是routing_key=''，所有的queue都路由，所以对应的consumer也都能收到）

![img](https://www.rabbitmq.com/img/tutorials/direct-exchange.png)

**代码示例：**

producer.py

```python
import pika
import sys

connection = pika.BlockingConnection(pika.ConnectionParameters(host='localhost'))
channel = connection.channel()

channel.exchange_declare(exchange='direct_logs',
                         exchange_type='direct') #关键参数

routing_name = sys.argv[1] if len(sys.argv) > 2 else 'info' #默认'info'队列
message = ' '.join(sys.argv[2]) or 'hello, world'

channel.basic_publish(exchange='direct_logs',
                      routing_key=routing_name,
                      body=message)

print(" [x] Send %r:%r" % (routing_name, message))

channel.close()
```

consumer.py

```python
import pika
import sys

connection = pika.BlockingConnection(pika.ConnectionParameters(host='localhost'))
channel = connection.channel()

channel.exchange_declare(exchange='direct_logs',
                         exchange_type='direct')

result = channel.queue_declare(exclusive=True)
queue_name = result.method.queue

topic_name = sys.argv[1:] #选择想要接收的topic,可以是多个
if not topic_name:
    sys.stderr.write("Usage: %s [info] [warning] [error]\n" % sys.argv[0])
    sys.exit(1)

for s in topic_name: #此处可以同时收到多个topic消息
    channel.queue_bind(
        exchange='direct_logs',
        queue=queue_name,
        routing_key=s
    )
print(' [*] Waiting for logs. To exit press CTRL+C')

def callback(ch, method, properties, body):
    print(" [x] %r:%r" % (method.routing_key, body))

channel.basic_consume(callback,
                      queue=queue_name,
                      no_ack=True)    

channel.start_consuming()
```

**代码使用示例：**

假设producer可以产生A,B,C,D 四种topic，consumer1对A,B两个感兴趣，consumer2对所有感兴趣

1. consumer1监听A,B

   `python consumer.py A B`

2. consumer2监听A,B,C,D

   `python consumer.py A B C D`

3. producer产生数据

   `python producer.py A say something...`

------



### Publish/Subscribe(Topic主题)

应用场景：更高级更灵活的路由方式，通过正则表达式，而不是routing_key来匹配路由的queue

应用举例：同样希望收到系统的log信息，但是想接收来自

![img](https://www.rabbitmq.com/img/tutorials/python-five.png)

代码示例：

producer.py

```python
import pika
import sys

connection = pika.BlockingConnection(pika.ConnectionParameters(host='localhost'))
channel = connection.channel()

channel.exchange_declare(exchange='topic_logs',
                         exchange_type='topic')

routing_key = sys.argv[1] if len(sys.argv) > 2 else 'anonymous.info'
message = ' '.join(sys.argv[2:]) or 'Hello World!'
channel.basic_publish(exchange='topic_logs',
                      routing_key=routing_key,
                      body=message)
print(" [x] Sent %r:%r" % (routing_key, message))
connection.close()
```

consumer.py

```python
import pika
import sys

connection = pika.BlockingConnection(pika.ConnectionParameters(host='localhost'))
channel = connection.channel()

channel.exchange_declare(exchange='topic_logs',
                         exchange_type='topic')

result = channel.queue_declare(exclusive=True)
queue_name = result.method.queue  

binding_keys = sys.argv[1:] #定义并绑定关键词
if not binding_keys:
    sys.stderr.write("Usage: %s [binding_key]...\n" % sys.argv[0])
    sys.exit(1)
for binding_key in binding_keys:
    channel.queue_bind(exchange='topic_logs',
                       queue=queue_name,
                       routing_key=binding_key)

print(' [*] Waiting for logs. To exit press CTRL+C')

def callback(ch, method, properties, body):
    print(" [x] %r:%r" % (method.routing_key, body))

channel.basic_consume(callback,
                      queue=queue_name,
                      no_ack=True)

channel.start_consuming()
```

代码使用示例：

假设producer会产生/kernel/info.log的信息，也会产生/debug/debug.log信息

假设consumer1只关注debug信息，consumer2关注kernel所有信息，consumer3只关注kernel的error信息

1. consumer1监听关于debug的所有信息

   `python consumer.py *.log`

2. consumer2监听关于kernel的所有信息

   `python consumer.py kernel.*`

3. consumer3监听kernel的error信息

   `python consumer.py kernel.critial *.error`

4. producer产生相关测试信息

   `python producer.py 1.log create some debug info`

   `python producer.py kernel.error create some kernel info`

------

### RPC(remote procedure call)

应用场景：类似于http的post请求，发送请求消息给远端并获取其返回的数据

应用举例：

原理：

该模式==有坑==要格外小心：

​	RPC模式下，请求消息发送后会一直***阻塞***，直到收到应答为止。因此增加了调试和运行的复杂度，降低了系统的稳定性。所以能够用其他方式来取代，就尽量不要用这种模式。如果非用不可，请听从官方建议：

​	时刻清楚被调用的服务在本地还是远端

​	通信涉及到的各部分之间的相互依赖一定要相当清楚，最好用文档约定好

​	仔细处理错误分支，比如RPC server挂掉以后client怎么办？		

消息拓扑图：

![img](https://www.rabbitmq.com/img/tutorials/python-six.png)

示例代码：

server.py

```python
import pika
connection = pika.BlockingConnection(pika.ConnectionParameters(host='localhost'))
channel = connection.channel()

channel.queue_declare(queue='rpc_queue')

def dosomething(n):
    print('I have worked %d seconds...' % n)
    return n

def on_request(ch, method, props, body):
    n = int(body)
    response = dosomething(n)

    ch.basic_publish(
        exchange='',
        routing_key=props.reply_to,
        properties=pika.BasicProperties(
            correlation_id=props.correlation_id
        ),
        body=str(response)
    )
    ch.basic_ack(delivery_tag = method.delivery_tag)

channel.basic_qos(prefetch_count=1)
channel.basic_consume(on_request,queue='rpc_queue')

print("[x] Awating RPC requests")
channel.start_consuming()
```

client.py

```python
import pika
import uuid

class TestRpcClient(object):
    def __init__(self):
        self.connection = pika.BlockingConnection(pika.ConnectionParameters(host='localhost'))
        self.channel = self.connection.channel()

        result = self.channel.queue_declare(exclusive=True)
        self.callback_queue = result.method.queue

        self.channel.basic_consume(self.on_response, no_ack=True,queue=self.callback_queue)

    def on_response(self, ch, method, props, body): #此处只接收id匹配的消息
        if self.corr_id == props.correlation_id:
            self.response = body    

    def call(self, n):
        self.response = None
        self.corr_id = str(uuid.uuid4())
        self.channel.basic_publish(
            exchange='',
            routing_key='rpc_queue',
            properties=pika.BasicProperties(
                reply_to=self.callback_queue,
                correlation_id=self.corr_id
            ),
            body=str(n)
        )      
        while self.response is None:
            self.connection.process_data_events()
        return int(self.response)

test = TestRpcClient()
print('[x] request delay(10)')
response = test.call(10)
print('[.] got %r' % response)
```

使用说明：

先启动server

`python server.py`

然后启动client，观察发出的信号是否得到有效回复

`python client.py`

预期结果是：

`[x] request delay(10)`

`[.] got 10`




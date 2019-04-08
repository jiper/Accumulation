作者：尹超

日期：2018-10-24

RPC模型回顾：



背景：

​	需要用到Rabbit MQ实现控制中心到多个服务器之间的网络通信。RMQ官网提供的RPC通信模型下，消息通信是阻塞式的，影响性能。



RPC通信机制回顾：

client(控制中心)发送请求，然后阻塞式监听reply_to端口， server收到请求后，返回消息。client直到获取应答信息后才结束通信。

![img](https://www.rabbitmq.com/img/tutorials/python-six.png)



尝试解决方案：

http://www.cnblogs.com/lianzhilei/p/5983673.html

https://www.rabbitmq.com/reliability.html

https://blog.csdn.net/zhuisaozhang1292/article/details/80926099





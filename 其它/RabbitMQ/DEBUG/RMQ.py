import pika
import uuid
#import threading
#import queue

class Producer(object):
    '''
    @描述：RMQ生产者模型，默认提供topic消息发送模式
    @API：
            Init()初始化参数，建立连接
            SendMsg()发送消息
            DisConnection()断开连接
    '''    
    def __init__(self): #默认值在此处修改
        self.exchange = ''
        self.channel = ''
        self.routing_keys = ''
        self.mode = 'topic'
        
    def Init(self, ip, exchange_name, rout_keys, mode = 'topic'):
        '''
        @描述：初始化实现ip,exchange,routing_key,mode四个参数的配置，
              并建立连接和通道（默认topic模式）
        @输入：ip(str)->host地址
                exchange_name(str)->exhange名称
                rout_keys(str)->路由键
                mode(str)->暂时只提供两种配置'topic'和'rpc'(以后需要的话可以继续增加)
        @返回值：成功->0  失败->-1
        '''
        self.mode = mode
        self.exchange = exchange_name
        self.routing_keys = rout_keys
        
        self.connection = pika.BlockingConnection(pika.ConnectionParameters(host = ip))#建立一个阻塞连接
        self.channel = self.connection.channel()#在连接的基础上建立一个频道
        
        if mode == 'rpc':
            #不指定queue名字,exclusive=True会在使用此queue的消费者断开后,自动将queue删除
            result = self.channel.queue_declare(exclusive = True)
            self.callback_queue = result.method.queue #给频道自动分配一个队列名
            #设置从哪个queue接收消息，
            #处理消息的函数on_response,是否需要确认消息
            #默认情况下是要对消息进行确认的，以防止消息的丢失
            self.channel.basic_consume(self.on_response,
                                       no_ack=False,
                                       queue=self.callback_queue)           
            self.task_id = []
            
        elif mode == 'topic':
            self.channel.exchange_declare(exchange=self.exchange,
                                     exchange_type='topic')
        else:
            print('暂未提供该模式')
            return -1
        return 0
    
    def on_response(self,ch,method,props,body):
        if self.corr_id == props.correlation_id:
            self.response = body.decode()
    
    def call(self,cmd):
        #初始化response和corr_id属性
        self.response = None
        self.corr_id = str(uuid.uuid4())
        #使用默认exchange向server种定义的rpc_queue发送消息
        #在properties种指定replay_to属性和correlation_id属性用于告知远程server
        #correlation_id属性用于匹配request和response
        self.channel.basic_publish(exchange='',
                                   routing_key=self.routing_keys,
                                   properties=pika.BasicProperties(
                                       reply_to=self.callback_queue,
                                       correlation_id=self.corr_id),
                                   body = cmd)
        
        while self.response is None:
            self.connection.process_data_events()
        return str(self.response)
    
    def SendMsg(self, msg):
        if self.mode == 'rpc':
            return self.call(msg)
        elif self.mode == 'topic':
            if isinstance(self.routing_keys, list): #发到多个通道
                for keys in self.routing_keys:
                    self.channel.basic_publish(exchange=self.exchange,
                              routing_key=keys,
                              body=msg)     
            else: #发给单个通道        
                self.channel.basic_publish(exchange=self.exchange,
                          routing_key=self.routing_keys,
                          body=msg)
            print(" [x] Sent %r:%r" % (self.routing_keys, msg))
            return 0
            
    def Disconnection(self):
        self.connection.close()

    
class Consumer(object):
    '''
    @描述：RMQ消费者模型，默认提供topic消息接收模式，接收的消息都存入参数queue中，供外部
          直接使用
    @API：
            Init()初始化参数，建立连接
            Running()无限监听消息
            DisConnection()断开连接
    '''    
    def __init__(self, inqueue):
        self.exchange = ''
        self.binding_keys = ''
        self.channel = ''
        self.con = ''
        self.mode = 'topic'
        self.inqueue = inqueue
        self.DEBUG = False #实际使用时，请将该参数设置为 False
        self.test_rec_cnt = 0 #仅供测试使用
        
    def Init(self, ip, exchange_name, bind_keys, mode = 'topic'):
        '''
        @描述：初始化实现ip,exchange,bind_keys,mode四个参数的配置，
              并建立连接和通道（默认topic模式）
        @输入：ip(str)->host地址
                exchange_name(str)->exhange名称
                bind_keys(str)->路由键
                mode(str)->暂时只提供两种配置'topic'和'rpc'(以后需要的话可以继续增加)
        @返回值：成功->0  失败->-1
        '''
        self.mode = mode
        self.exchange = exchange_name
        self.binding_keys = bind_keys
                
        self.connection = pika.BlockingConnection(pika.ConnectionParameters(host=ip))
        self.channel = self.connection.channel()
        
        if mode == 'rpc':
            self.channel.queue_declare(queue=self.binding_keys)
            #result = self.channel.queue_declare(exclusive=True)
            #self.queue_name = result.method.queue 
            self.channel.basic_qos(prefetch_count=1) 
            print(self.binding_keys)
            self.channel.basic_consume(self.on_request,queue=self.binding_keys)
            
        elif mode == 'topic':
            self.channel.exchange_declare(exchange=self.exchange,
                         exchange_type='topic')

            result = self.channel.queue_declare(exclusive=True)
            queue_name = result.method.queue  
            
            if isinstance(self.binding_keys, list):#监听多个队列
                print('multi mode')
                for binding_key in self.binding_keys:
                    self.channel.queue_bind(exchange=self.exchange,
                                       queue=queue_name,
                                       routing_key=binding_key)
            else:#监听一个队列
                print('single mode')
                self.channel.queue_bind(exchange=self.exchange,
                                   queue=queue_name,
                                   routing_key=self.binding_keys)  
            print('topic mode has enabled, consumer is wating...')
            self.channel.basic_consume(self.topic_callback,
                                  queue=queue_name,
                                  no_ack=True)              
                       
        elif mode == 'fanout':
            print(1)
        elif mode == 'direct':
            self.channel = self.con.channel()
            self.channel.exchange_declare(exchange=self.exchange,
                                     exchange_type=self.mode)             
    
    def topic_callback(self, ch, method, properties, body):
        '''
        消息接收的回调函数：此处如果是多个队列一起监听，则要将消息put在不同的queue里
        '''
        self.inqueue.put(body)
        if self.DEBUG == True:
            print(" [x] %r:%r" % (method.routing_key, body))
            self.test_rec_cnt += 1
            if self.test_rec_cnt >= 5:
                self.Disconnection()
                
    def on_request(self,ch,method,props,body):
        receive_cmd = body.decode()
        callback_msg = '[ack] '+receive_cmd
        self.inqueue.put(callback_msg)
        ch.basic_publish(exchange='',
                         routing_key = props.reply_to,
                         properties = pika.BasicProperties(correlation_id=props.correlation_id),
                         body = str(callback_msg)
                        )
        ch.basic_ack(delivery_tag = method.delivery_tag)
        self.test_rec_cnt += 1
        if self.test_rec_cnt >= 5:
            self.Disconnection()
        
    def Running(self):
        '''
        用户调用AIP，启动监听状态
        '''
        self.channel.start_consuming()
    def Disconnection(self):
        '''
        用户调用AIP，断开连接
        '''
        self.connection.close()
        

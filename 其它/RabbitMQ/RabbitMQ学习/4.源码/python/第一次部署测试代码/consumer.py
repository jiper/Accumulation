# -*- coding: utf-8 -*-
"""
Created on Wed Oct 17 00:59:50 2018

@author: yinchao
"""


import pika

# connect to RabbitMQ server
connection = pika.BlockingConnection(pika.ConnectionParameters('localhost'))
channel = connection.channel()

# create message queue
channel.queue_declare(queue='message')

def callback(ch, method, properties, body):
    print(" [x] Received {}".format(body))
    
channel.basic_consume(callback, queue='message', no_ack=True)

# infinite running
print(' [*] Waiting for messages. To exit press CTRL+C')    
channel.start_consuming()
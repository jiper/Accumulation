# -*- coding: utf-8 -*-
"""
Created on Wed Oct 17 01:00:02 2018

@author: yinchao
"""

import pika

# connect to RabbitMQ server
connection = pika.BlockingConnection(pika.ConnectionParameters('localhost'))
channel = connection.channel()

# create message queue
channel.queue_declare(queue='message')

# send message to brocker
channel.basic_publish(exchange='',routing_key='message',body='hello,world!')

# close connect
connection.close()
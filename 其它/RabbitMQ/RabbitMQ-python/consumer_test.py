# -*- coding: utf-8 -*-
"""
Created on Mon Jan  7 10:04:19 2019

@author: yinchao
"""

import queue
import RMQ
import threading
import sys
if __name__ == '__main__':
    rmq_ip = 'localhost' 
    exchange_name = 'gd.sz'
    bind_keys = 'ack.007'
    
    if (len(sys.argv)>=2):
        mode = sys.argv[1]  #模式设置接受命令行参数
    else:
        mode = 'topic'  #默认模式
    fifo = queue.Queue()
    client = RMQ.Consumer(fifo)
    client.Init(rmq_ip, exchange_name, bind_keys,mode)
    th1 = threading.Thread(target=client.Running,args=[]) #子线程接收
    th1.start()
    
    while True: #主线程监控fifo中是否有数据
        if fifo.qsize() > 0:
            msg = fifo.get()
            print(msg)
# -*- coding: utf-8 -*-
"""
Created on Mon Jan  7 11:34:39 2019

@author: yinchao
"""

import RMQ
if __name__ == '__main__':
    rmq_ip = 'localhost' 
    exchange_name = 'gd.sz'
    rout_keys = 'ack.007'
    mode = 'topic'
    client = RMQ.Producer()
    client.Init(rmq_ip, exchange_name, rout_keys,mode)
    print(client.SendMsg('hello, world!'))
https://www.rabbitmq.com/queues.html

# Queues

## 命名规则：

- 少于255个utf-8字符
- 禁止使用“amq.”前缀（AMQP协议中保留的系统队列名），否则出现403(ACCESS_REFUSED)错误

## server-named queues技巧

队列名既可以指定，也可以由系统随机生成。

如果需要使用系统自动生成的功能，则**在queue定义的时候不指定其名称**，系统就会自动为其创建一个合法名称。不用担心exchange的对方不知道该名称，因为系统会自动记住上一个queue名称，所以**exchange的对方也使用相同方式创建队列**即可。

## queue的属性

Durable：一直存在

Exclusive：1.只供一个连接所使用，2.连接中断后队列自动删除

Auto-delete：供多个连接一起使用的exclusive版本，只有当最后一个人失去连接时才会删除该队列

Arguments：可选项，当使用插件时可以指定额外的参数

## queue的使用流程

必须先declared，再使用

declared过程中，如果先前没有该队列，则创建。如果已有该队列，且属性与本次声明一致，则忽略，如果不一致则报错：ERROR406（PERCONDITION_FAILED)

## 可选参数

- Message and queue TTL
- Queue length limit
- Mirroring settings
- Max number of priorities
- Consumer priorities


# 阿里云 ONS 消息队列服务 Node.js SDK

> **ONS**（开放消息服务）是基于阿里开源消息中间件MetaQ（RocketMQ）打造的一款云消息产品。

## 安装

```sh
$ npm install --save ons
```

## 使用方法

首先你需要开通 ONS 服务并且获取 `access key` 以及 `secret key`，然后创建一个消费者 ID 或者生产者 ID，还有就是话题（topic）。

详情可以参考[阿里云 ONS 帮助](https://help.aliyun.com/product/8315024_ons.html)或者[阿里云控制台](http://ons.console.aliyun.com/)。

### 样例

你可以参考两个样例文件 [consumer.js](example/consumer.js) 以及 [producer.js](example/producer.js).

### Consumer

通过下面的代码来创建一个 Consumer。

```javascript
var Consumer = require("ons").Consumer;
var consumer = new Consumer(CUSTOMER_ID, TOPIC, TAGS, ACCESS_KEY, SECRET_KEY);
```

然后创建一个获取消息的事件监听。

```javascript
consumer.on("message", function(msg) {
    // 做一些事情
    // 
    // 该函数会在收到消息之后被触发。
});
```

当你完成创建和设置监听函数之后，就可以初始化 Consumer 并开始监听消息了。

```javascript
consumer.init(function(err) {
    if(err) return console.log(err);
    consumer.listen();
});
```

以及，你也可以在你想要的时候停止它。

```javascript
consumer.stop();
```

### Producer

通过下面的代码来创建一个 Producer。

```javascript
var Producer = require("ons").Producer;
var producer = new Producer(PRODUCER_ID, ACCESS_KEY, SECRET_KEY);
```

创建完毕之后需要启动它才能发消息。

```javascript
producer.start(function(err) {
    if(err) return console.log(err);
    console.log("Started!");
});
```

然后你就可以通过 `send` 函数来发消息了。

```javascript
producer.send(KEY, TOPIC, TAGS, CONTENT, function(err, messageId) {
    console.log(arguments);
});

// `KEY` 参数并不是必选的，所以也可以如下调用

producer.send(TOPIC, TAGS, CONTENT, function(err, messageId) {
    console.log(arguments);
});
```

当然，你也可以在你想要的时候停止它。

```javascript
producer.stop();
```

## Contribute

快来快来 **Fxxk** 我！然后提交 PR 什么的最喜欢了🙈

「雖然我覺得不怎麼可能有人會關注我」
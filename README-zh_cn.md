# 阿里云 ONS 消息队列服务 Node.js SDK

[![Version](http://img.shields.io/npm/v/ons.svg)](https://www.npmjs.com/package/ons)
[![Downloads](http://img.shields.io/npm/dm/ons.svg)](https://www.npmjs.com/package/ons)
[![License](https://img.shields.io/npm/l/ons.svg?style=flat)](https://opensource.org/licenses/MIT)
[![AppVeypr](https://ci.appveyor.com/api/projects/status/9qdfjl7ig6s9d72s/branch/master?svg=true)](https://ci.appveyor.com/project/XadillaX/aliyun-ons)
[![TravisCI](https://travis-ci.org/XadillaX/aliyun-ons.svg)](https://travis-ci.org/XadillaX/aliyun-ons)
[![Dependency](https://david-dm.org/XadillaX/aliyun-ons.svg)](https://david-dm.org/XadillaX/aliyun-ons)

> **ONS**（开放消息服务）是基于阿里开源消息中间件 MetaQ（RocketMQ）打造的一款云消息产品。

## 安装

运行：
```sh
$ npm install --save ons
```
如果你是用root用户登录linux系统，运行
```sh
$ npm install --save ons
```


> **注意:** 因为阿里云 ONS 的官方 C++ SDK 是闭源的，且只提供了 Linux 和 Windows 下的链接库（libonsclient4cpp.a，ONSClientCPP.lib），所以该包目前为止只支持在 Linux 和 64 位 Windows 下安装。
>
> 如需开发环境，OSX 用户请移步 Linux 或者启动一个 Vagrant、Docker 等。
>


## 使用方法

首先你需要开通 ONS 服务并且获取 `access key` 以及 `secret key`，然后创建一个消费者 ID 或者生产者 ID，还有就是话题（topic）。

详情可以参考[阿里云 ONS 帮助](https://help.aliyun.com/product/8315024_ons.html)或者[阿里云控制台](http://ons.console.aliyun.com/)。

### 样例

你可以参考两个样例文件 [consumer.js](example/consumer.js) 以及 [producer.js](example/producer.js).

### Consumer

通过下面的代码来创建一个 Consumer。

```javascript
var Consumer = require("ons").Consumer;
var consumer = new Consumer(CUSTOMER_ID, TOPIC, TAGS, ACCESS_KEY, SECRET_KEY, OPTIONS);
```

> `OPTIONS` 及其内部的参数都是可选的。
>
> 例如
>
> ```javascript
> {
>     namesrvAddr: "112.124.141.191:80",
>     onsAddr: "http://onsaddr-internet.aliyun.com:80/rocketmq/nsaddr4client-internet",
>
>     threadNum: 3
> }
> ```
>
> + **namesrvAddr**：ONS 服务器地址
> + **onsAddr**：用于寻找 ONS 服务器地址
> + **threadNum**：工作线程数
> + **order:** 是否以 `OrderConsumer` 创建

然后创建一个获取消息的事件监听。

```javascript
consumer.on("message", function(msg, ack) {
    // 做一些事情
    // 
    // 该函数会在收到消息之后被触发。
    //
    // 在你做完事情之后别忘了调用 `ack.done(true)` 或是 `ack.done(false)`
    // 来告诉 ONS 你已处理消息成功或者失败，若失败则 ONS 会重试
    //
    // `ack.done()` 等价于 `ack.done(true)`
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
consumer.stop(function() {
    // 停止了
});
```

> **注意：**在调用 `stop()` 之前，你需要 `ack` 所有已接收到的消息（无论是 `done(true)` 或者 `done(false)`），否则回调函数不会被触发，Consumer 不会停止。
>
> **另外，你最好在你的程序退出之前停止 Consumer。比如说：**
>
> ```javascript
> process.on("SIGINT", function() {
>     consumer.stop(function() {
>         process.exit(0);
>     });
> });
> ```
>
> 不过这段代码要视你自己代码的情况而写，只是个例子而已。

### Producer

通过下面的代码来创建一个 Producer。

```javascript
var Producer = require("ons").Producer;
var producer = new Producer(PRODUCER_ID, ACCESS_KEY, SECRET_KEY);
```

> `OPTIONS` 及其内部的参数都是可选的。
>
> 例如
>
> ```javascript
> {
>     namesrvAddr: "112.124.141.191:80",
>     onsAddr: "http://onsaddr-internet.aliyun.com:80/rocketmq/nsaddr4client-internet",
>
>     sendTimeout: 1000
> }
> ```
>
> + **namesrvAddr**：ONS 服务器地址
> + **onsAddr**：用于寻找 ONS 服务器地址
> + **sendTimeout**：发送超时时间
> + **order**：是否以 `OrderProducer` 模式创建

创建完毕之后需要启动它才能发消息。

```javascript
producer.start(function(err) {
    if(err) return console.log(err);
    console.log("Started!");
});
```

然后你就可以通过 `send` 函数来发消息了。

```javascript
producer.send([KEY,] TOPIC, TAGS, CONTENT, [SHARDING_KEY,] [DELAY,] function(err, messageId) {
    console.log(arguments);
});

// `KEY` 参数并不是必选的，所以也可以如下调用
// `SHARDING_KEY` 在非 `OrderProducer` 不需要传，在 `OrderProducer` 模式下必传
// `DELAY` 延时参数也是可选的，默认不传为立即发送，否则会发一个指定毫秒延时的消息

producer.send(TOPIC, TAGS, CONTENT, function(err, messageId) {
    console.log(arguments);
});
```

> **注意壹：**`SHARDING_KEY` 仅在 `OrderProducer` 模式必传，否则不需要传。在 `OrderProducer` 模式下，同 `SHARDING_KEY`
> 的消息会顺序发送，不同 `SHARDING_KEY` 之间消息不能保证顺序。在 `OrderConsumer` 接受消息时，同 `SHARDING_KEY`
> 的消息会顺序接受，不同 `SHARDING_KEY` 之间消息不能保证顺序。
>
> **注意贰：**`callback` 在非 `OrderProducer` 模式下可选，若不传 `callback` 则该 `Producer` 将会以 **Oneway** 形式发送。

当然，你也可以在你想要的时候停止它。

```javascript
producer.stop(function() {
    // 停止了
});
```

> **注意：**你最好在你的程序退出之前停止 Producer。比如说：
>
> ```javascript
> process.on("SIGINT", function() {
>     producer.stop(function() {
>         process.exit(0);
>     });
> });
> ```
>
> 不过这段代码要视你自己代码的情况而写，只是个例子而已。

### 原始日志

> **该特性目前只在 Linux 下实现。**

C++ ONS SDK 会生成它的源日志，所以我们创建了一个 `tail stream` 来监控它。

```javascript
const logger = require("ons").logger;
logger.on("data", function(data) {
    console.log("[ORIG LOG]", data);
});

// [ORIG LOG] ... register sendMessageHook success,hookname is OnsSendMessageHook ...
// ...
// [ORIG LOG] ... register consumeMessageHook success,hookname is OnsConsumerMessageHook ...
// ...
// [ORIG LOG] ... shutdown producerl successfully ...
// ...
// [ORIG LOG] ... shutdown pushConsumer successfully ...
// ...
```

> **提示：**C++ ONS SDK 在一个进程中只会创建一个日志文件，所以 `logger` 是一个单件（Singleton）。

## C++ SDK 更改日志

这里是 [ONS 原始 C++ SDK 更改日志](src/third_party/CHANGELOG.md)。

> **注意：**这只是原始 C++ SDK 的更改日志，Node.js SDK 不一定用到所有的新特性。

## 贡献

快来快来 **Fxxk** 我！然后提交 PR 什么的最喜欢了🙈

「雖然我覺得不怎麼可能有人會關注我」

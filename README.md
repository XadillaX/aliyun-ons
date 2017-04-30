# Aliyun ONS SDK for Node.js

[![Version](http://img.shields.io/npm/v/ons.svg)](https://www.npmjs.com/package/ons)
[![Downloads](http://img.shields.io/npm/dm/ons.svg)](https://www.npmjs.com/package/ons)
[![License](https://img.shields.io/npm/l/ons.svg?style=flat)](https://opensource.org/licenses/MIT)
[![AppVeypr](https://ci.appveyor.com/api/projects/status/9qdfjl7ig6s9d72s/branch/master?svg=true)](https://ci.appveyor.com/project/XadillaX/aliyun-ons)
[![TravisCI](https://travis-ci.org/XadillaX/aliyun-ons.svg)](https://travis-ci.org/XadillaX/aliyun-ons)
[![Dependency](https://david-dm.org/XadillaX/aliyun-ons.svg)](https://david-dm.org/XadillaX/aliyun-ons)

SDK of Node.js for Aliyun ONS.

> **ONS** (Open Notification Service) is a message queue service of [aliyun](http://www.aliyun.com/product/ons/) based on MetaQ (RocketMQ).
>
> Maybe you want [中文文档](README-zh_cn.md)?

## Installation

Default ,run
```sh
$ npm install --save ons
```
If you login with root, run
```sh
$ npm install --save ons
```



> **NOTE:** Because of Aliyun ONS C++ SDK's closed source, it only provides Linux and Windows library file (libonsclient4cpp.a, ONSClientCPP.lib). So you can only install this package under Linux and Windows 64x so far.
>
> **If you need to develop under OSX, please run a vagrant or a docker.**

## Usage

You should turn on ONS first and then get an `access key` and a `secret key`. In next step you should create a consumer id or a producer id, and a topic.

You can do steps above by refering to [help desk](https://help.aliyun.com/product/8315024_ons.html) and [aliyun console](http://ons.console.aliyun.com/).

### Examples

Here's some examples for [consumer](example/consumer.js) and [producer](example/producer.js).

### Consumer

You can create a consumer by code following:

```javascript
var Consumer = require("ons").Consumer;
var consumer = new Consumer(CUSTOMER_ID, TOPIC, TAGS, ACCESS_KEY, SECRET_KEY, OPTIONS);
```

> `OPTIONS` is optional and any parameters in `OPTIONS` are optional too.
>
> eg.
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
> + **namesrvAddr:** the ONS server address
> + **onsAddr:** an address to fetch ONS server address
> + **threadNum:** worker thread count
> + **order:** `true` if you want it be `OrderConsumer`

Next step you should set one or more message receive function to that consumer.

```javascript
consumer.on("message", function(msg, ack) {
    // DO SOMETHING
    // 
    // this function will be emitted while receiving a message
    //
    // after finishing this, call `ack.done(true)` or `ack.done(false)` to tell
    // ONS whether you're successful.
    //
    // `ack.done()` equals to `ack.done(true)`
});
```

After creating a consumer and set listener, you should initialize for it and then listen.

```javascript
consumer.init(function(err) {
    if(err) return console.log(err);
    consumer.listen();
});
```

That's easy! And what's more, you can stop it when you want.

```javascript
consumer.stop(function() {
    // closed
});
```

> **CAUTION:** You should `ack` all received messages (whether `done(true)` or `done(false)`) before you call `consumer.stop()`, or you won't get callback function called in `stop` and consumer won't be stopped.
>
> **What's more, you'd better to stop consumer before your program exited. e.g.**
>
> ```javascript
> process.on("SIGINT", function() {
>     consumer.stop(function() {
>         process.exit(0);
>     });
> });
> ```
> You should write down your exit code in your own scene.

### Producer

You can create a producer by code following:

```javascript
var Producer = require("ons").Producer;
var producer = new Producer(PRODUCER_ID, ACCESS_KEY, SECRET_KEY, OPTIONS);
```

> `OPTIONS` is optional and any parameters in `OPTIONS` are optional too. e.g.
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
> + **namesrvAddr:** the ONS server address
> + **onsAddr:** an address to fetch ONS server address
> + **sendTimeout:** timeout for sending a message
> + **order:** `true` if you want it be `OrderProducer`

After creating a producer, you should start it.

```javascript
producer.start(function(err) {
    if(err) return console.log(err);
    console.log("Started!");
});
```

Now you can send message(s)!

```javascript
producer.send([KEY,] TOPIC, TAGS, CONTENT, [SHARDING_KEY,] [DELAY,] function(err, messageId) {
    console.log(arguments);
});

// or key / shardingKey / delay (ms) / callback are optional parameter

producer.send(TOPIC, TAGS, CONTENT, function(err, messageId) {
    console.log(arguments);
});
```

> **NOTICE 1:** `SHARDING_KEY` is only for `OrderProducer`, each message in same `SHARDING_KEY` will send one by one in
> order and `OrderConsumer` will receive messages in same `SHARDING_KEY` one by one in order.
>
> **NOTICE 2:** `callback` is optional when it's not `OrderProducer`. If no `callback` passed, message will be sent in
> `oneway` method.

That's easy! And what's more, you can stop it when you want.

```javascript
producer.stop(function() {
    // closed
});
```

> **CAUTION:** you'd better to stop producer before your program exited. e.g.
>
> ```javascript
> process.on("SIGINT", function() {
>     producer.stop(function() {
>         process.exit(0);
>     });
> });
> ```
>
> You should write down your exit code in your own scene.

### Original Logs

> **This feature is available under Linux so far.**

By default C++ ONS SDK will generate a log file. So we create a tail stream to watch it.

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

> **NOTICE:** C++ ONS SDK will create only one log file per process, so `logger` is a singleton.

## C++ SDK Changelog

Here's [original C++ ONS SDK changelog](src/third_party/CHANGELOG.md).

> **NOTICE:** It's only the changelog for the original C++ SDK. Node.js SDK may not use all new features of original SDK.

## Contribute

You're welcome to fork and make pull requests!

「雖然我覺得不怎麼可能有人會關注我」

# Aliyun ONS SDK for Node.js

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
$ npm install --save ons --unsafe-perm
```



> **NOTE:** Because of Aliyun ONS C++ SDK's closed source, it only provides linux static library file (libonsclient4cpp.a). So you can only install this package under linux so far.
>
> If you need to develop under OSX, please run a vagrant or a docker.
>
> **Microsoft Windows is supported now since version 1.2.0!**
>
> ~~And what's more, because of C++ SDK again, I only can consider every messge as successfully processed and commit `succeed` back to ONS service.~~ (**ACK feature is finished!**)
>
> You're welcome to provide some useful solution!

## Usage

You should turn on ONS first and then get an `access key` and a `secret key`. In next step you should create a consumer id or a producer id, and a topic.

You can do steps above by refering to [help desk](https://help.aliyun.com/product/8315024_ons.html) and [aliyun console](http://ons.console.aliyun.com/).

### Examples

Here's examples for [consumer](example/consumer.js) and [producer](example/producer.js).

### Consumer

You can create a consumer by code following:

```javascript
var Consumer = require("ons").Consumer;
var consumer = new Consumer(CUSTOMER_ID, TOPIC, TAGS, ACCESS_KEY, SECRET_KEY);
```

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
consumer.stop();
```

### Producer

You can create a producer by code following:

```javascript
var Producer = require("ons").Producer;
var producer = new Producer(PRODUCER_ID, ACCESS_KEY, SECRET_KEY);
```

After creating a producer, you should start it.

```javascript
producer.start(function(err) {
    if(err) return console.log(err);
    console.log("Started!");
});
```

Now you can send message(s)!

```javascript
producer.send(KEY, TOPIC, TAGS, CONTENT, function(err, messageId) {
    console.log(arguments);
});

// or key is an optional parameter

producer.send(TOPIC, TAGS, CONTENT, function(err, messageId) {
    console.log(arguments);
});
```

That's easy! And what's more, you can stop it when you want.

```javascript
producer.stop();
```

## Contribute

You're welcome to fork and make pull requests!

「雖然我覺得不怎麼可能有人會關注我」

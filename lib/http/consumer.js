/**
 * XadillaX <i@2333.moe> created at 2017-05-24 14:34:20 with ❤
 *
 * Copyright (c) 2017 xcoder.in, all rights reserved.
 */
"use strict";

var qs = require("querystring");
var util = require("util");

var Scarlet = require("scarlet-task");
var spidex = require("spidex");

var Base = require("./base");

var ONSConsumerClient = function(consumerId, topic, tag, accessKey, secretKey, options) {
    Base.call(this, consumerId, accessKey, secretKey, options);

    this.sendTimeout = this.options.sendTimeout || 35000;
    this.topic = topic;
    this.tag = tag;
    this.running = false;
    this.cache = [];
    this.scarlet = new Scarlet(options.threadNum || 1);

    this.retryTimeout = 1;

    const self = this;
    this.on("message", function(msg, ack) {
        if(typeof self.listener === "function") {
            self.listener(undefined, msg, ack);
        }
    });

    this.on("error", function(err) {
        if(typeof self.listener === "function") {
            self.listener(err);
        }
    });
};

util.inherits(ONSConsumerClient, Base);

ONSConsumerClient.prototype.fetch = function(callback) {
    const now = +new Date();
    const sign = this.calcSignature(`${this.topic}\n${this.id}\n${now}`);

    // headers
    const headers = {
        AccessKey: this.accessKey,
        Signature: sign,
        ConsumerId: this.id
    };

    // querystring
    const query = {
        topic: this.topic,
        time: now,
        num: 32,
        timeout: this.sendTimeout
    };

    spidex.get(`http://${this.domain}/message/?${qs.stringify(query)}`, {
        header: headers,
        charset: "utf8",
        timeout: 5000 + this.sendTimeout
    }, function(content, status) {
        if(status === 403) {
            return callback(new Error("鉴权失败。"));
        }

        let json;
        try {
            json = JSON.parse(content);
        } catch(e) {
            return callback(e);
        }

        if(Array.isArray(json)) return callback(undefined, json);
        const err = new Error(json.info || "未知错误。");
        if(json.code) err.code = json.code;
        return callback(err);
    }).on("error", function(err) {
        return callback(err);
    });
};

ONSConsumerClient.prototype.ack = function(msgHandle, callback) {
    const now = +new Date();
    const sign = this.calcSignature(`${this.topic}\n${this.id}\n${msgHandle}\n${now}`);

    // headers
    const headers = {
        AccessKey: this.accessKey,
        Signature: sign,
        ConsumerId: this.id,
        "Content-Type": "text/html;charset=UTF-8"
    };

    // querystring
    const query = {
        topic: this.topic,
        time: now,
        msgHandle: msgHandle
    };

    spidex.delete(`http://${this.domain}/message/?${qs.stringify(query)}`, {
        header: headers,
        charset: "utf8"
    }, function(content, status) {
        if(status === 403) {
            return callback(new Error("鉴权失败。"));
        }

        if(status >= 200 && status < 300) return callback();

        let json;
        try {
            json = JSON.parse(content);
        } catch(e) {
            return callback(e);
        }

        if(Array.isArray(json)) return callback(undefined, json);
        const err = new Error(json.info || "未知错误。");
        if(json.code) err.code = json.code;
        return callback(err);
    }).on("error", function(err) {
        return callback(err);
    });
};

ONSConsumerClient.prototype.processPer = function(taskObject) {
    if(!this.running) {
        this.cache.push(taskObject.task);
        return taskObject.done();
    }

    const msg = taskObject.task;
    const ret = {
        body: msg.body,
        msgId: msg.msgId,
        reconsumeTimes: msg.reconsumeTimes,
        startDeliverTime: msg.bornTime,
        topic: this.topic,
        tag: null,
        key: null
    };

    const self = this;
    function ackCb(err) {
        if(err) {
            self.emit("error", err);
            return setTimeout(function() {
                self.ack(msg.msgHandle, ackCb);
            }, 500);
        }

        taskObject.done();
    }

    this.emit("message", ret, {
        done: function() {
            self.ack(msg.msgHandle, ackCb);
        }
    });
};

ONSConsumerClient.prototype._cycle = function() {
    const self = this;
    if(!this.running) return;
    this.fetch(function(err, ret) {
        if(err) {
            self.emit("error", err);
            self.retryTimeout *= 2;
            if(self.retryTimeout > 4096) self.retryTimeout = 4096;
            return setTimeout(function() {
                self._cycle();
            }, self.retryTimeout);
        }

        self.retryTimeout = 1;
        if(!ret.length) {
            return setTimeout(function() {
                self._cycle();
            }, 500);
        }

        for(let i = 0; i < ret.length; i++) {
            self.scarlet.push(ret[i], self.processPer.bind(self));
        }

        self.scarlet.afterFinish(ret.length, self._cycle.bind(self), false);
    });
};

ONSConsumerClient.prototype.listen = function() {
    if(this.running) return;

    const self = this;
    this.running = true;
    process.nextTick(function() {
        if(self.cache && self.cache.length) {
            for(let i = 0; i < self.cache.length; i++) {
                self.scarlet.push(self.cache[i], self.processPer.bind(self));
            }
            self.scarlet.afterFinish(self.cache.length, self._cycle.bind(self), false);
            self.cache = [];
            return;
        }

        self._cycle();        
    });
};

ONSConsumerClient.prototype.stop = function() {
    this.running = false;
};

ONSConsumerClient.prototype.setListener = function(func) {
    this.listener = func;
};

module.exports = ONSConsumerClient;

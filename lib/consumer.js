/**
 * XadillaX created at 2015-12-18 15:38:58 With ♥
 *
 * Copyright (c) 2015 Souche.com, all rights
 * reserved.
 */
"use strict";

var EventEmitter = require("events").EventEmitter;
var os = require("os");
var util = require("util");

if(os.platform() === "darwin") {
    throw new Error("Aliyun ONS Node.js SDK doesn't support OSX yet.");
}
var _Consumer = require("../build/Release/ons").ONSConsumer;


/**
 * Consumer
 * @param {String} cid the consumer id
 * @param {String} topic the topic
 * @param {String} tag the tag
 * @param {String} accessKey the access key
 * @param {String} secretKey the secret key
 * @constructor
 */
var Consumer = function(cid, topic, tag, accessKey, secretKey) {
    EventEmitter.call(this);

    this.consumer = new _Consumer(cid, topic, tag, accessKey, secretKey);
    this.keepTimer = null;
};

util.inherits(Consumer, EventEmitter);

/**
 * init
 * @param {Function} callback the callback function
 */
Consumer.prototype.init = function(callback) {
    var self = this;
    this.consumer.init(function(err) {
        if(err) return callback(err);
        self.consumer.setListener(function(_, message, ack) {
            self.emit("message", message, ack);
        });

        callback();
    });
};

/**
 * listen
 */
Consumer.prototype.listen = function() {
    this.consumer.listen();

    if(this.keepTimer) {
        clearInterval(this.keepTimer);
    }

    this.keepTimer = setInterval(function() {}, 2147483647);
};

/**
 * stop
 */
Consumer.prototype.stop = function() {
    this.consumer.stop();

    if(this.keepTimer) {
        clearInterval(this.keepTimer);
    }
};

module.exports = Consumer;

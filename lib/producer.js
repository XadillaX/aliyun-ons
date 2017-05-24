/**
 * XadillaX created at 2015-12-19 23:21:56 With ♥
 *
 * Copyright (c) 2017 Souche.com, all rights
 * reserved.
 */
"use strict";

var os = require("os");

var _Producer;
var util = require("./util");

if(os.platform() === "darwin") {
    _Producer = require("./http").ONSProducer;
} else {
    require("./logger");
    _Producer = require("../build/Release/ons").ONSProducer;
}

var __PARSER = "parseSendArguments";
var __ORDER_PARSER = "parseOrderSendArguments";

/**
 * Producer
 * @param {String} producerId the producer id
 * @param {String} accessKey the access key
 * @param {String} secretKey the secret key
 * @param {Object} [options] the options
 * @param {String} [options.onsAddr] the fetching ONS server address
 * @param {String} [options.namesrvAddr] the ONS server address
 * @param {Boolean} [options.order] whether it will be `OrderProducer`
 * @constructor
 */
var Producer = function ONSProducer(producerId, accessKey, secretKey, options) {
    Object.defineProperties(this, {
        producer: {
            enumerable: false,
            writable: false,
            configurable: false,
            value: new _Producer(producerId, accessKey, secretKey, options) // jshint ignore: line
        },

        keepTimer: {
            enumerable: false,
            writable: true,
            configurable: true,
            value: null
        },

        producerId: {
            enumerable: true,
            writable: false,
            configurable: false,
            value: producerId
        },

        isOrder: {
            enumerable: true,
            writable: false,
            configurable: false,
            value: !!options.order
        }
    });
};

/**
 * Start the producer
 *
 * @param {Function} callback the callback function
 */
Producer.prototype.start = function(callback) {
    var self = this;
    this.producer.start(function(err) {
        if(err) return callback(err);

        if(self.keepTimer) {
            clearInterval(self.keepTimer);
        }
        self.keepTimer = setInterval(function() {}, 2147483647);
        callback();
    });
};

/**
 * Stop the producer
 *
 * @param {Function} callback the callback function
 */
Producer.prototype.stop = function(callback) {
    if(this.keepTimer) {
        clearInterval(this.keepTimer);
    }

    this.producer.stop(callback);
};

/**
 * send
 * @param {String} [key] the message key
 * @param {String} topic the topic
 * @param {String} tags the tags
 * @param {String} content the content
 * @param {String} [shardingKey] the sharding key in OrderProducer
 * @param {Number} [delay] send message after delay (ms)
 * @param {Function} callback the callback function
 */
Producer.prototype.send = function(key, topic, tags, content, shardingKey, delay, callback) { // jshint ignore: line
    var args = util[this.isOrder ? __ORDER_PARSER : __PARSER](Array.prototype.slice.call(arguments));
    this.producer.send.apply(this.producer, args);
};

module.exports = Producer;

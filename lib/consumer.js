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

require("./logger");
var _Consumer = require("../build/Release/ons").ONSConsumer;

/**
 * Consumer
 * @param {String} cid the consumer id
 * @param {String} topic the topic
 * @param {String} tag the tag
 * @param {String} accessKey the access key
 * @param {String} secretKey the secret key
 * @param {Object} [options] the options
 * @param {String} [options.onsAddr] the fetching ONS server address
 * @param {String} [options.namesrvAddr] the ONS server address
 * @param {Number} [options.threadNum] the thread count
 * @param {Number} [options.sendTimeout] the send timeout (ms)
 * @constructor
 */
var Consumer = function ONSConsumer(cid, topic, tag, accessKey, secretKey, options) {
    EventEmitter.call(this);

    Object.defineProperties(this, {
        consumer: {
            enumerable: false,
            writable: false,
            configurable: false,
            value: new _Consumer(cid, topic, tag, accessKey, secretKey, options) // jshint ignore: line
        },

        keepTimer: {
            enumerable: false,
            writable: true,
            configurable: true,
            value: null
        },

        consumerId: {
            enumerable: true,
            writable: false,
            configurable: false,
            value: cid
        },

        topic: {
            enumerable: true,
            writable: false,
            configurable: false,
            value: topic
        },

        tag: {
            enumerable: true,
            writable: false,
            configurable: false,
            value: tag
        }
    });
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

        // set the consumer listener
        self.consumer.setListener(function(_, message, ack) {
            if(process.env.NODE_ONS_LOG === "true") {
                var now = new Date();

                var done = ack.done.bind(ack);
                ack.done = function(result) {
                    done(result);
                    console.log("[" + message.msgId + "][------] message acknowledged in " + (new Date() - now) + " ms");
                };
            }

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
Consumer.prototype.stop = function(callback) {
    if(this.keepTimer) {
        clearInterval(this.keepTimer);
    }

    this.consumer.stop(callback);
};

module.exports = Consumer;

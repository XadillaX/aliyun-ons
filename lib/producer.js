/**
 * XadillaX created at 2015-12-19 23:21:56 With ♥
 *
 * Copyright (c) 2015 Souche.com, all rights
 * reserved.
 */
"use strict";

var _Producer = require("../build/Release/ons").ONSProducer;

/**
 * Producer
 * @param {String} producerId the producer id
 * @param {String} accessKey the access key
 * @param {String} secretKey the secret key
 * @constructor
 */
var Producer = function(producerId, accessKey, secretKey) {
    this.producer = new _Producer(producerId, accessKey, secretKey);
    this.keepTimer = null;
};

/**
 * start
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
 * stop
 */
Producer.prototype.stop = function() {
    this.producer.stop();

    if(this.keepTimer) {
        clearInterval(this.keepTimer);
    }
};

/**
 * send
 * @param {String} [key] the message key
 * @param {String} topic the topic
 * @param {String} tags the tags
 * @param {String} content the content
 * @param {Function} callback the callback function
 */
Producer.prototype.send = function(key, topic, tags, content, callback) {
    if(typeof content === "function") {
        callback = content;
        content = tags;
        tags = topic;
        topic = key;
        key = "";
    }

    this.producer.send(topic, tags, key, content, callback);
};

module.exports = Producer;

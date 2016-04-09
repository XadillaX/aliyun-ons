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
 * @param {Number} [delay] send message after delay (ms)
 * @param {Function} callback the callback function
 */
Producer.prototype.send = function(key, topic, tags, content, delay, callback) {
    var args = Array.prototype.slice.call(arguments);
    var pos = -1;
    for(var i = 0; i < args.length; i++) {
        if(typeof args[i] === "function") {
            pos = i;
            break;
        }
    }
    
    if(pos < 3 || pos > 5) {
        return process.nextTick(function() {
            callback(new Error("Broken arguments."));
        });
    }
    
    if(pos === 3) {
        // NO KEY; NO DELAY.
        key = "";
        topic = args[0];
        tags = args[1];
        content = args[2];
        delay = -1;
        callback = args[3];
    } else if(pos === 4) {
        if(typeof args[3] === "number") {
            // NO KEY; DELAY EXISTS.
            key = "";
            topic = args[0];
            tags = args[1];
            content = args[2];
            delay = args[3];
            callback = args[4];
        } else {
            // NO DELAY; KEY EXISTS.
            delay = -1;
            callback = args[4];
        }
    }
    
    delay = parseInt(delay) || -1;
    if(delay !== -1) {
        delay = delay + (+new Date());
    }

    console.log(topic, tags, key, content, delay);
    this.producer.send(topic, tags, key, content, delay, callback);
};

module.exports = Producer;

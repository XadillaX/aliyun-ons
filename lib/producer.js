/**
 * XadillaX created at 2015-12-19 23:21:56 With ♥
 *
 * Copyright (c) 2015 Souche.com, all rights
 * reserved.
 */
"use strict";

var fs = require("fs");
var os = require("os");

var tailStream = require("lei-stream").tailStream;

if(os.platform() === "darwin") {
    throw new Error("Aliyun ONS Node.js SDK doesn't support OSX yet.");
}

var _Producer = require("../build/Release/ons").ONSProducer;
var _stdout = require("./_stdout");
var origLog = require("./orig_log");
var util = require("./util");

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
 * @param {Boolean} [options.order] whether use order producer
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
 * start
 * @param {Function} callback the callback function
 */
Producer.prototype.start = function(callback) {
    var self = this;
    var _logInited = origLog.inited;

    if(!_logInited) _stdout.fxxkStdout();
    this.producer.start(function(err, uuid) {
        if(!_logInited) _stdout.restoreStdout();

        if(err) return callback(err);

        if(self.keepTimer) {
            clearInterval(self.keepTimer);
        }

        self.keepTimer = setInterval(function() {}, 2147483647);

        if(uuid) {
            // if log file detected

            var tempFilename = "./.ons-" + uuid + ".log";
            fs.readFile(tempFilename, { encoding: "utf8" }, function(err, lines) {
                if(err) return callback();

                // if log is not initialized, initialize it
                if(!origLog.inited) {
                    lines = lines.trim().split("\n");
                    lines.forEach(function(line) {
                        if(line.substr(0, 8) !== "logfile:") {
                            process.stdout.write(line + "\n");
                        } else {
                            origLog.filename = line.substr(8);
                            origLog.stream = tailStream(origLog.filename);

                            origLog.stream.on("data", function(data) {
                                data = data.toString();
                                data = data.trim().split("\n");

                                for(var i = 0; i < data.length; i++) {
                                    origLog.emit("data", data[i]);
                                }
                            });

                            origLog.inited = true;
                        }
                    });
                }

                fs.unlink(tempFilename, function() {
                    callback();
                });
            });
        } else {
            // no log file detected

            callback();
        } 
    }, _logInited);
};

/**
 * stop
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

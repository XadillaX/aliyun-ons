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

/**
 * Producer
 * @param {String} producerId the producer id
 * @param {String} accessKey the access key
 * @param {String} secretKey the secret key
 * @param {Object} [options] the options
 * @param {String} [options.onsAddr] the fetching ONS server address
 * @param {String} [options.namesrvAddr] the ONS server address
 * @constructor
 */
var Producer = function ONSProducer(producerId, accessKey, secretKey, options) {
    Object.defineProperties(this, {
        producer: {
            enumerable: false,
            writable: false,
            configurable: false,
            value: new _Producer(producerId, accessKey, secretKey, options)
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

    this.producer.send(topic, tags, key, content, delay, callback);
};

module.exports = Producer;

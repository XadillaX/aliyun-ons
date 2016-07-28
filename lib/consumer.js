/**
 * XadillaX created at 2015-12-18 15:38:58 With ♥
 *
 * Copyright (c) 2015 Souche.com, all rights
 * reserved.
 */
"use strict";

var EventEmitter = require("events").EventEmitter;
var fs = require("fs");
var os = require("os");
var util = require("util");

var tailStream = require("lei-stream").tailStream;

if(os.platform() === "darwin") {
    throw new Error("Aliyun ONS Node.js SDK doesn't support OSX yet.");
}
var _Consumer = require("../build/Release/ons").ONSConsumer;
var _stdout = require("./_stdout");
var origLog = require("./orig_log");

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
var Consumer = function(cid, topic, tag, accessKey, secretKey, options) {
    EventEmitter.call(this);

    this.consumer = new _Consumer(cid, topic, tag, accessKey, secretKey, options);
    this.keepTimer = null;

    var self = this;
    process.on("exit", function() {
        if(self.keepTimer) {
            self.stop();
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

    var _logInited = origLog.inited;

    if(!_logInited) _stdout.fxxkStdout();
    this.consumer.init(function(err, uuid) {
        if(!_logInited) _stdout.restoreStdout();

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

        if(uuid) {
            // if log file detected

            var tempFilename = "./.ons-" + uuid + ".log";
            fs.readFile(tempFilename, { encoding: "utf8" }, function(err, lines) {
                console.log(err, lines);
                 
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

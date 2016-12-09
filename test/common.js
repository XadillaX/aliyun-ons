/**
 * XadillaX created at 2016-06-07 17:25:06 With ♥
 *
 * Copyright (c) 2016 Souche.com, all rights
 * reserved.
 */
"use strict";

var Consumer = require("../").Consumer;
var Producer = require("../").Producer;
var OriginalLog = require("../").OriginalLog;

var config = require("./config");

module.exports = {
    consumer: new Consumer(config.consumerId, config.topic, "*", config.accessKey, config.secretKey, {
        threadNum: 1,
        onsAddr: "http://onsaddr-internet.aliyun.com:80/rocketmq/nsaddr4client-internet"
    }),
    producer: new Producer(config.producerId, config.accessKey, config.secretKey, {
        onsAddr: "http://onsaddr-internet.aliyun.com:80/rocketmq/nsaddr4client-internet"
    }),
    logs: []
};

if(process.platform !== "win32") {
    OriginalLog.on("data", function(data) {
        module.exports.logs.push(data);
    });
}

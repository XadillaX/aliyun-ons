/**
 * XadillaX created at 2016-06-07 17:25:06 With ♥
 *
 * Copyright (c) 2018 xcoder.in, all rights
 * reserved.
 */
"use strict";

var Consumer = require("../").Consumer;
var Producer = require("../").Producer;
var logger = require("../").logger;

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

logger.on("data", function(data) {
    module.exports.logs.push(data);
});

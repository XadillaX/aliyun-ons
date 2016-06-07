/**
 * XadillaX created at 2016-06-07 17:25:06 With ♥
 *
 * Copyright (c) 2016 Souche.com, all rights
 * reserved.
 */
"use strict";

var Consumer = require("../").Consumer;
var Producer = require("../").Producer;

var config = require("./config");

module.exports = {
    consumer: new Consumer(config.consumerId, config.topic, "*", config.accessKey, config.secretKey),
    producer: new Producer(config.producerId, config.accessKey, config.secretKey)
};

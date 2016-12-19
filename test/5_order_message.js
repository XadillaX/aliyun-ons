/**
 * XadillaX <i@2333.moe> created at 2016-12-19 14:50:10 with ❤
 *
 * Copyright (c) 2016 Souche.com, all rights reserved.
 */
"use strict";

const config = require("./config");

describe("#order message", function() {
    const Producer = require("../").Producer;
    const producer = new Producer(config.producerId, config.accessKey, config.secretKey, {
        onsAddr: "http://onsaddr-internet.aliyun.com:80/rocketmq/nsaddr4client-internet",
        order: true
    });

    it("should start an order producer", function(done) {
        producer.start(done);
    });

    it("should stop the order producer", function(done) {
        producer.stop(done);
    });
});

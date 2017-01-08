/**
 * XadillaX <i@2333.moe> created at 2016-12-19 14:50:10 with ❤
 *
 * Copyright (c) 2017 Souche.com, all rights reserved.
 */
"use strict";

const config = require("./config");

describe("#order message", function() {
    const Producer = require("../").Producer;
    const Consumer = require("../").Consumer;
    const producer = new Producer(config.producerId, config.accessKey, config.secretKey, {
        onsAddr: "http://onsaddr-internet.aliyun.com:80/rocketmq/nsaddr4client-internet",
        order: true
    });
    const consumer = new Consumer(config.consumerId, config.topic, "*", config.accessKey, config.secretKey, {
        onsAddr: "http://onsaddr-internet.aliyun.com:80/rocketmq/nsaddr4client-internet",
        order: true
    });

    const messages = [];
    consumer.on("message", function(msg, ack) {
        messages.push(msg);
        ack.done();
    });

    this.timeout(0);

    it("should start an order producer", function(done) {
        producer.start(done);
    });

    it("should start an order consumer", function(done) {
        consumer.init(function() {
            consumer.listen();
            done();
        });
    });

    it("should send two messages", function(done) {
        producer.send(config.topic, "tagOrder", "Aloha1", "key1", function() {
            producer.send(config.topic, "tagOrder", "Aloha2", "key1", function() {
                done();
            });
        });
    });

    it("should get two messages by order", function(done) {
        function wait() {
            if(messages.length === 2) {
                messages[0].body.should.equal("Aloha1");
                messages[1].body.should.equal("Aloha2");
                return done();
            }

            setTimeout(wait, 50);
        }

        wait();
    });

    it("should stop the order producer", function(done) {
        producer.stop(done);
    });

    it("should stop the order consumer", function(done) {
        consumer.stop(done);
    });
});

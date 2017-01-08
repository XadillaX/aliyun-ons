/**
 * XadillaX created at 2016-12-09 11:47:27 With ♥
 *
 * Copyright (c) 2017 Souche.com, all rights
 * reserved.
 */
"use strict";

const should = require("should");

const config = require("./config");

describe("#message", function() {
    const consumer = require("./common").consumer;
    const producer = require("./common").producer;
    const date = new Date();
    const DATE = "" + date.getUTCFullYear() + date.getUTCMonth() + date.getUTCDate();

    let message;
    let ack;
    function waitMessage(callback) {
        function wait() {
            if(!message || !ack) {
                return setTimeout(wait, 50);
            }

            const _message = message;
            const _ack = ack;
            message = undefined;
            ack = undefined;
            callback(undefined, _message, _ack);
        }

        setTimeout(wait, 50);
    }

    this.timeout(0);

    it("should start producer", function(done) {
        producer.start(done);
    });

    it("should start consumer", function(done) {
        consumer.init(function() {
            consumer.on("message", function(_message, _ack) {
                message = _message;
                ack = _ack;
            });

            consumer.listen();
            done();
        });
    });

    describe("#oneway", function() {
        it("should send oneway message", function() {
            producer.send(config.topic, "tagA", "单向!");
        });

        it("should get oneway message", function(done) {
            waitMessage(function(err, message, ack) {
                ack.done(true);

                message.topic.should.be.eql(config.topic);
                message.tag.should.be.eql("tagA");
                message.key.should.be.eql("");
                message.msgId.should.match(/^[0-9A-Z]{32}$/);
                message.body.should.be.eql("单向!");
                message.reconsumeTimes.should.be.eql(0);
                message.startDeliverTime.should.be.eql(0);

                done();
            });
        });
    });

    describe("#send & consume", function() {
        it("should send an immediate message", function(done) {
            producer.send(
                config.topic,
                "tagA",
                "Hello " + DATE,
                function(err, id) {
                    should.ifError(err);
                    id.should.match(/^[0-9A-Z]{32}$/);
                    done();
                });
        });

        it("should get one message", function(done) {
            waitMessage(function(err, message, ack) {
                ack.done(false);

                message.topic.should.be.eql(config.topic);
                message.tag.should.be.eql("tagA");
                message.key.should.be.eql("");
                message.msgId.should.match(/^[0-9A-Z]{32}$/);
                message.body.should.be.eql("Hello " + DATE);
                message.reconsumeTimes.should.be.eql(0);
                message.startDeliverTime.should.be.eql(0);

                done();
            });
        });

        it("should reconsume", function(done) {
            waitMessage(function(err, message, ack) {
                ack.done();

                message.topic.should.be.eql(config.topic);
                message.tag.should.be.eql("tagA");
                message.key.should.be.eql("");
                message.msgId.should.match(/^[0-9A-Z]{32}$/);

                message.body.should.be.eql("Hello " + DATE);
                message.reconsumeTimes.should.be.eql(1);
                message.startDeliverTime.should.be.eql(0);

                done();
            });
        });
    });

    describe("#timing", function() {
        it("should send a timing message", function(done) {
            producer.send(
                config.topic,
                "tagA",
                "World " + DATE,
                1000,
                function(err, id) {
                    should.ifError(err);
                    id.should.match(/^[0-9a-zA-Z]{32}$/);
                    done();
                });
        });

        it("should get the timing message", function(done) {
            waitMessage(function (err, message, ack) {
                ack.done(true);

                message.topic.should.be.eql(config.topic);
                message.tag.should.be.eql("tagA");
                message.key.should.be.eql("");
                message.msgId.should.match(/^[0-9A-Z]{32}$/);

                message.body.should.be.eql("World " + DATE);
                message.reconsumeTimes.should.be.eql(0);
                message.startDeliverTime.should.not.be.eql(0);

                done();
            });
        });
    });

    describe("#default", function() {
        it("should create consumer using default options", function(done) {
            const Consumer = require("../").Consumer;
            const consumer = new Consumer(
                config.consumerId,
                config.topic,
                "*",
                config.accessKey,
                config.secretKey);

            consumer.stop(function() {
                done();
            });
        });
    });
});

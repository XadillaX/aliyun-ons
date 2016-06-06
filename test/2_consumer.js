/**
 * XadillaX created at 2016-06-06 21:40:48 With ♥
 *
 * Copyright (c) 2016 Souche.com, all rights
 * reserved.
 */
"use strict";

require("should");

var config = require("./config");
var Consumer = require("../").Consumer;

describe("#consumer", function() {
    var consumer = new Consumer(config.consumerId, config.topic, "*", config.accessKey, config.secretKey);
    var date = new Date();
    var DATE = date.getUTCFullYear() + date.getUTCMonth() + date.getUTCDate();
    var _ack;

    this.timeout(0);

    it("should start consumer", function(done) {
        consumer.init(function() {
            consumer.listen();
            done();
        });
    });

    it("should get one message", function(done) {
        consumer.once("message", function(message, ack) {
            message.topic.should.be.eql(config.topic);
            message.tag.should.be.eql("tagA");
            message.key.should.be.eql("");
            message.msgId.should.match(/^[0-9A-Z]{32}$/);
            message.body.should.be.eql("Hello " + DATE);
            message.reconsumeTimes.should.be.eql(0);

            _ack = ack;
            done();
        });
    });

    it("should get another message", function(done) {
        consumer.once("message", function(message, ack) {
            _ack = ack;

            message.topic.should.be.eql(config.topic);
            message.tag.should.be.eql("tagA");
            message.key.should.be.eql("");
            message.msgId.should.match(/^[0-9A-Z]{32}$/);
            message.body.should.be.eql("World " + DATE);
            message.reconsumeTimes.should.be.eql(0);

            done();
        });

        _ack.done(false);
    });

    it("should reconsume", function(done) {
        consumer.once("message", function(message, ack) {
            message.topic.should.be.eql(config.topic);
            message.tag.should.be.eql("tagA");
            message.key.should.be.eql("");
            message.msgId.should.match(/^[0-9A-Z]{32}$/);
            message.body.should.be.eql("Hello " + DATE);
            message.reconsumeTimes.should.be.eql(1);

            ack.done();

            done();
        });

        _ack.done(true);
    });

    it("should stop producer", function(done) {
        consumer.stop();
        done();
    });
});

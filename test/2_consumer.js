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
    var DATE = "" + date.getUTCFullYear() + date.getUTCMonth() + date.getUTCDate();
    var ack = [];
    var message = [];

    this.timeout(0);

    function listener(_message, _ack) {
        message.push(_message);
        ack.push(_ack);
    }

    it("should start consumer", function(done) {
        consumer.on("message", listener);
        consumer.init(function() {
            consumer.listen();
            done();
        });
    });

    it("should get one message", function(done) {
        function verify() {
            if(ack.length < 1) return setTimeout(verify, 500);

            var _message = message[0];
            var _ack = ack[0];

            console.log(_message);

            _ack.done(false);

            _message.topic.should.be.eql(config.topic);
            _message.tag.should.be.eql("tagA");
            _message.key.should.be.eql("");
            _message.msgId.should.match(/^[0-9A-Z]{32}$/);
            _message.body.should.be.eql("Hello " + DATE);
            _message.reconsumeTimes.should.be.eql(0);
            _message.startDeliverTime.should.be.eql(0);

            done();
        }

        setTimeout(verify, 500);
    });

    it("should get another message", function(done) {
        function verify() {
            if(ack.length < 2) return setTimeout(verify, 500);

            var _message = message[1];
            var _ack = ack[1];

            console.log(_message);

            _ack.done(true);

            _message.topic.should.be.eql(config.topic);
            _message.tag.should.be.eql("tagA");
            _message.key.should.be.eql("");
            _message.msgId.should.match(/^[0-9A-Z]{32}$/);
            _message.body.should.be.eql("World " + DATE);
            _message.reconsumeTimes.should.be.eql(0);
            _message.startDeliverTime.should.not.be.eql(0);

            done();
        }

        setTimeout(verify, 500);
    });

    it("should reconsume", function(done) {
        function verify() {
            if(ack.length < 3) return setTimeout(verify, 500);

            var _message = message[2];
            var _ack = ack[2];

            console.log(_message);

            _ack.done();

            _message.topic.should.be.eql(config.topic);
            _message.tag.should.be.eql("tagA");
            _message.key.should.be.eql("");
            _message.msgId.should.match(/^[0-9A-Z]{32}$/);
            _message.body.should.be.eql("Hello " + DATE);
            _message.reconsumeTimes.should.be.eql(1);
            _message.startDeliverTime.should.be.eql(0);

            done();
        }

        setTimeout(verify, 500);
    });

    it("should stop consumer", function(done) {
        consumer.stop();
        done();
    });
});

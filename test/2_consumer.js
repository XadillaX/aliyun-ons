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
    var _ack = null;
    var _message = null;

    this.timeout(0);

    function listener(message, ack) {
        _message = message;
        _ack = ack;
    }

    it("should start consumer", function(done) {
        consumer.on("message", listener);
        consumer.init(function() {
            consumer.listen();
            done();
        });
    });

    it("should get one message", function(done) {
        // consumer.once("message", function(message, ack) {
        //     

        //     _ack = ack;
        //     done();
        // });
        function verify() {
            if(_ack === null) return setTimeout(verify, 500);

            _message.topic.should.be.eql(config.topic);
            _message.tag.should.be.eql("tagA");
            _message.key.should.be.eql("");
            _message.msgId.should.match(/^[0-9A-Z]{32}$/);
            _message.body.should.be.eql("Hello " + DATE);
            _message.reconsumeTimes.should.be.eql(0);

            _message = null;
            _ack.ack(false);
            _ack = null;

            done();
        }

        setTimeout(verify, 500);
    });

    it("should get another message", function(done) {
        function verify() {
            if(_ack === null) return setTimeout(verify, 500);

            _message.topic.should.be.eql(config.topic);
            _message.tag.should.be.eql("tagA");
            _message.key.should.be.eql("");
            _message.msgId.should.match(/^[0-9A-Z]{32}$/);
            _message.body.should.be.eql("World " + DATE);
            _message.reconsumeTimes.should.be.eql(0);

            _message = null;
            _ack.ack(true);
            _ack = null;

            done();
        }

        setTimeout(verify, 500);
    });

    it("should reconsume", function(done) {
        function verify() {
            if(_ack === null) return setTimeout(verify, 500);

            _message.topic.should.be.eql(config.topic);
            _message.tag.should.be.eql("tagA");
            _message.key.should.be.eql("");
            _message.msgId.should.match(/^[0-9A-Z]{32}$/);
            _message.body.should.be.eql("Hello " + DATE);
            _message.reconsumeTimes.should.be.eql(1);

            _ack.done(true);

            done();
        }

        setTimeout(verify, 500);
    });

    it("should stop consumer", function(done) {
        consumer.stop();
        done();
    });
});

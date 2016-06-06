/**
 * XadillaX created at 2015-12-19 23:29:10 With ♥
 *
 * Copyright (c) 2015 Souche.com, all rights
 * reserved.
 */
"use strict";

const should = require("should");

const config = require("./config");
const Producer = require("../").Producer;

describe("#producer", function() {
    const producer = new Producer(config.producerId, config.accessKey, config.secretKey);
    const date = new Date();

    it("should start producer", function(done) {
        this.timeout(0);
        producer.start(done);
    });

    it("should send an immediate message", function(done) {
        producer.send(
            config.topic,
            "tagA",
            `Hello ${date.getUTCFullYear()}${date.getUTCMonth()}${date.getUTCDate()}`,
            function(err, id) {
                should.ifError(err);
                id.should.match(/^[0-9A-Z]{32}$/);
                done();
            });
    });

    it("should send a timing message", function(done) {
        producer.send(
            config.topic,
            "tagA",
            `World ${date.getUTCFullYear()}${date.getUTCMonth()}${date.getUTCDate()}`,
            function(err, id) {
                should.ifError(err);
                id.should.match(/^[0-9A-Z]{32}$/);
                done();
            });
    });

    it("should stop producer", function(done) {
        this.timeout(0);
        producer.stop();
        done();
    });
});

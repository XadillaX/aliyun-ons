/**
 * XadillaX created at 2016-06-07 17:28:26 With ♥
 *
 * Copyright (c) 2018 xcoder.in, all rights
 * reserved.
 */
"use strict";

var common = require("./common");

describe("#thicken", function() {
    this.timeout(0);

    it("should stop producer", function(done) {
        common.producer.stop(function() {
            done();
        });
    });

    it("should stop consumer", function(done) {
        common.consumer.stop(function() {
            done();
        });
    });
});

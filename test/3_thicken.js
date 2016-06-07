/**
 * XadillaX created at 2016-06-07 17:28:26 With ♥
 *
 * Copyright (c) 2016 Souche.com, all rights
 * reserved.
 */
"use strict";

var common = require("./common");

describe("#thicken", function() {
    it("should stop producer", function() {
        common.producer.stop();
    });

    it("should stop consumer", function() {
        common.consumer.stop();
    });
});

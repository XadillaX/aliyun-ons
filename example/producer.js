/**
 * XadillaX created at 2015-12-19 23:37:40 With ♥
 *
 * Copyright (c) 2015 Souche.com, all rights
 * reserved.
 */
"use strict";

var Producer = require("../lib/producer");

var producer = new Producer(
    "PID",
    "access_key",
    "secret_key");

console.log("Connecting to Aliyun ONS...");
producer.start(function() {
    console.log("Started.");

    setInterval(function() {
        for(var i = 0; i < 10; i++) {
            producer.send("ons_subscriber_test", "tagA", "Hello " + i + "!", function(err, messageId) {
                console.log(err, messageId);
            }); /* jshint ignore: line */
        }
    }, 1000);
});

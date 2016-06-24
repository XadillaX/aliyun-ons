/**
 * XadillaX created at 2015-12-19 23:37:40 With ♥
 *
 * Copyright (c) 2015 Souche.com, all rights
 * reserved.
 */
"use strict";

var config = require("../test/_config");
var Producer = require("../lib/producer");

var producer = new Producer(
    config.producerId,
    config.accessKey,
    config.secretKey, {
        onsAddr: "http://onsaddr-internet.aliyun.com:80/rocketmq/nsaddr4client-internet"
    });

console.log("Connecting to Aliyun ONS...");
producer.start(function() {
    console.log("Started.");

    setInterval(function() {
        for(var i = 0; i < 10; i++) {
            producer.send(config.topic, "tagA", "Hello " + i + "!", 1000, function(err, messageId) {
                console.log(err, messageId);
            }); /* jshint ignore: line */
        }
    }, 1000);
});

process.on("SIGINT", function() {
    producer.stop();
    process.exit(0);
});

/**
 * XadillaX created at 2015-12-19 23:37:40 With ♥
 *
 * Copyright (c) 2015 Souche.com, all rights
 * reserved.
 */
"use strict";

var Scarlet = require("scarlet-task");

var config = require("../test/_config");
var Producer = require("../lib/producer");

var producer = new Producer(
    config.producerId,
    config.accessKey,
    config.secretKey, {
        onsAddr: "http://onsaddr-internet.aliyun.com:80/rocketmq/nsaddr4client-internet"
    });

function send() {
    var scarlet = new Scarlet(10);

    function p(taskObject) {
        var i = taskObject.task.i;
        producer.send(config.topic, "tagA", "Hello " + i + "!", 1000, function(err, messageId) {
            console.log(err, messageId);
            taskObject.done();
        });
        producer.send(config.topic, "tagA", "单向 " + i);
    }

    for(var i = 0; i < 10; i++) {
        scarlet.push({ i: i }, p);
    }

    scarlet.afterFinish(10, function() {
        setTimeout(send, 100);
    }, false);
}

console.log("Connecting to Aliyun ONS...");
producer.start(function() {
    console.log("Started.");
    setTimeout(send, 100);
});

process.on("SIGINT", function() {
    producer.stop(function() {
        process.exit(0);
    });
});

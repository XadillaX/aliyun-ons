/**
 * XadillaX created at 2015-12-18 17:24:31 With ♥
 *
 * Copyright (c) 2015 Souche.com, all rights
 * reserved.
 */
"use strict";

var Consumer = require("../lib/consumer");

var consumer = new Consumer(
    "CID",
    "topic",
    "tag|*",
    "access_key",
    "secret_key");

consumer.on("message", function(message) {
    console.log(message);
});

console.log("Connecting to Aliyun ONS...");
consumer.init(function() {
    console.log("Initialized.");
    consumer.listen();
    console.log("Listened.");
});

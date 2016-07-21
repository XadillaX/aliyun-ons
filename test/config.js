/**
 * XadillaX created at 2016-06-06 20:19:05 With ♥
 *
 * Copyright (c) 2016 Souche.com, all rights
 * reserved.
 */
"use strict";

try {
    module.exports = require("./_config");
} catch(e) {
    var env = process.env;
    
    module.exports = {
        accessKey: env.ONS_KEY,
        secretKey: env.ONS_SECRET,
        producerId: env.ONS_PRODUCER_ID,
        consumerId: env.ONS_CONSUMER_ID,
        topic: env.ONS_TOPIC
    };

    console.log("[Test val: " + env.TEST_VAL + "]");
}

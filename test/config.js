/**
 * XadillaX created at 2016-06-06 20:19:05 With ♥
 *
 * Copyright (c) 2018 xcoder.in, all rights
 * reserved.
 */
"use strict";

/**
 * CI Memo:
 *
 * + Topic: ons_subscriber_test
 * + Topic 4 Windows: ons_subscriber_test_windows
 * + Producer ID: PID_my_producer_test
 * + Consumer ID: CID_my_consumer_test
 *
 * Remember deleting all resource after CI finished.
 */

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

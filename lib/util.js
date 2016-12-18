/**
 * XadillaX <i@2333.moe> created at 2016-12-16 17:48:18 with ❤
 *
 * Copyright (c) 2016 xcoder.in, all rights reserved.
 */
"use strict";

exports.parseSendArguments = function(args) {
    var pos = -1;
    for(var i = 0; i < args.length; i++) {
        if(typeof args[i] === "function") {
            pos = i;
            break;
        }
    }

    if((pos < 3 || pos > 5) && pos !== -1) {
        throw new Error("Broken arguments.");
    }

    let key = args[0], topic = args[1], tags = args[2], content = args[3], delay = args[4], callback = args[5];
    if(pos === 3) {
        // NO KEY; NO DELAY.
        key = "";
        topic = args[0];
        tags = args[1];
        content = args[2];
        delay = -1;
        callback = args[3];
    } else if(pos === 4) {
        if(typeof args[3] === "number") {
            // NO KEY; DELAY EXISTS.
            key = "";
            topic = args[0];
            tags = args[1];
            content = args[2];
            delay = args[3];
            callback = args[4];
        } else {
            // NO DELAY; KEY EXISTS.
            delay = -1;
            callback = args[4];
        }
    } else if(pos === -1) {
        // NO CALLBACK
        if(args.length === 3) {
            // NO KEY; NO DELAY.
            key = "";
            topic = args[0];
            tags = args[1];
            content = args[2];
            delay = -1;
            callback = null;
        } else if(args.length === 4) {
            if(typeof args[3] === "number") {
                // NO KEY; DELAY EXISTS.
                key = "";
                topic = args[0];
                tags = args[1];
                content = args[2];
                delay = args[3];
                callback = null;
            } else {
                // NO DELAY; KEY EXISTS.
                delay = -1;
                callback = null;
            }
        }
    }
    
    delay = parseInt(delay) || -1;
    if(delay !== -1) {
        delay = delay + (+new Date());
    }

    return [ topic, tags, key, content, delay, "", callback ];
};

exports.parseOrderSendArguments = function(args) {
    var pos = -1;
    for(var i = 0; i < args.length; i++) {
        if(typeof args[i] === "function") {
            pos = i;
            break;
        }
    }

    if((pos < 4 || pos > 6) && pos !== -1) {
        throw new Error("Broken arguments.");
    }

    let key = args[0], topic = args[1], tags = args[2], content = args[3];
    let shardingKey = args[4], delay = args[5], callback = args[6];
    if(pos === 4) {
        // NO KEY; NO DELAY.
        key = "";
        topic = args[0];
        tags = args[1];
        content = args[2];
        shardingKey = args[3];
        delay = -1;
        callback = args[4];
    } else if(pos === 5) {
        if(typeof args[3] === "number") {
            // NO KEY; DELAY EXISTS.
            key = "";
            topic = args[0];
            tags = args[1];
            content = args[2];
            shardingKey = args[3];
            delay = args[4];
            callback = args[5];
        } else {
            // NO DELAY; KEY EXISTS.
            delay = -1;
            callback = args[5];
        }
    } else if(pos === -1) {
        // NO CALLBACK
        if(args.length === 4) {
            // NO KEY; NO DELAY.
            key = "";
            topic = args[0];
            tags = args[1];
            content = args[2];
            shardingKey = args[3];
            delay = -1;
            callback = null;
        } else if(args.length === 5) {
            if(typeof args[3] === "number") {
                // NO KEY; DELAY EXISTS.
                key = "";
                topic = args[0];
                tags = args[1];
                content = args[2];
                shardingKey = args[3];
                delay = args[4];
                callback = null;
            } else {
                // NO DELAY; KEY EXISTS.
                delay = -1;
                callback = null;
            }
        }
    }
    
    delay = parseInt(delay) || -1;
    if(delay !== -1) {
        delay = delay + (+new Date());
    }

    return [ topic, tags, key, content, delay, shardingKey, callback ];
};

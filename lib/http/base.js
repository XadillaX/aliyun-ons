/**
 * XadillaX <i@2333.moe> created at 2017-05-24 13:41:53 with ❤
 *
 * Copyright (c) 2017 xcoder.in, all rights reserved.
 */
"use strict";

var crypto = require("crypto");
var EventEmitter = require("eventemitter2").EventEmitter2;
var util = require("util");

var ONSBaseClient = function(id, accessKey, secretKey, options) {
    EventEmitter.call(this);

    Object.defineProperties(this, {
        options: {
            enumerable: false,
            writable: false,
            configurable: false,
            value: options || {}
        },

        accessKey: {
            enumerable: false,
            writable: false,
            configurable: false,
            value: accessKey
        },

        secretKey: {
            enumerable: false,
            writable: false,
            configurable: false,
            value: secretKey
        },

        id: {
            enumerable: true,
            writable: false,
            configurable: false,
            value: id
        },

        domain: {
            enumerable: true,
            writable: false,
            configurable: false,
            value: (options || {}).httpDomain || "publictest-rest.ons.aliyun.com"
        }
    });
};

util.inherits(ONSBaseClient, EventEmitter);

ONSBaseClient.prototype.start = function(callback) {
    return process.nextTick(callback);
};

ONSBaseClient.prototype.stop = function(callback) {
    return process.nextTick(callback);
};

ONSBaseClient.prototype.init = function(callback) {
    return process.nextTick(callback);
}

ONSBaseClient.prototype.calcSignature = function(str) {
    return crypto.createHmac("sha1", this.secretKey).update(str).digest().toString("base64");
};

module.exports = ONSBaseClient;

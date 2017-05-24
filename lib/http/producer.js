/**
 * XadillaX <i@2333.moe> created at 2017-05-24 13:35:32 with ❤
 *
 * Copyright (c) 2017 xcoder.in, all rights reserved.
 */
"use strict";

const qs = require("querystring");
const util = require("util");

const md5 = require("blueimp-md5");
const spidex = require("spidex");

const Base = require("./base");

const _emptyFunc = function() {};

/**
 * ONSProducerClient
 * @param {String} producerId the producer id
 * @param {String} accessKey the access key
 * @param {String} secretKey the secret key
 * @param {Object} [options] the options
 * @param {String} [options.namesrvAddr] the ONS server address
 * @param {Boolean} [options.order] whether it will be `OrderProducer`
 * @constructor
 */
var ONSProducerClient = function ONSProducer(id, accessKey, secretKey, options) {
    Base.call(this, id, accessKey, secretKey, options);
    this.order = !!this.options.order;
};

util.inherits(ONSProducerClient, Base);

ONSProducerClient.prototype.send = function(topic, tags, key, content, delay, shardingKey, callback) {
    const now = +new Date();
    const sign = this.calcSignature(`${topic}\n${this.id}\n${md5(encodeURIComponent(content))}\n${now}`);

    if(typeof callback !== "function") {
        callback = _emptyFunc;
    }

    // headers
    const headers = {
        AccessKey: this.accessKey,
        Signature: sign,
        ProducerId: this.id,
        isOrder: this.order.toString(),
        "Content-Type": "text/html;charset=UTF-8"
    };
    if(this.order) headers.shardingKey = shardingKey;

    // querystring
    const query = {
        topic: topic,
        time: now
    };
    if(tags) query.tag = tags;
    if(key) query.key = key;
    if(delay && delay !== -1) query.startdelivertime = delay;

    const uri = `http://${this.domain}/message/?${qs.stringify(query)}`;
    spidex.post(uri, {
        header: headers,
        data: encodeURIComponent(content),
        charset: "utf8"
    }, function(content, status) {
        if(status === 403) {
            return callback(new Error("鉴权失败。"));
        }

        let json;
        try {
            json = JSON.parse(content);
        } catch(e) {
            return callback(e);
        }

        if(json.msgId) return callback(undefined, json.msgId);
        const err = new Error(json.info || "未知错误。");
        if(json.code) err.code = json.code;
        return callback(err);
    }).on("error", function(err) {
        callback(err);
    });
};

module.exports = ONSProducerClient;

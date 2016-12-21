/**
 * XadillaX <i@2333.moe> created at 2016-12-20 11:41:04 with ❤
 *
 * Copyright (c) 2016 xcoder.in, all rights reserved.
 */
"use strict";

var EventEmitter = require("events").EventEmitter;
var fs = require("fs-extra");
var os = require("os");
var path = require("path");

var tailStream = require("lei-stream").tailStream;
var uuid = require("uuid/v1");
var watch = require("node-watch");

var _setLogPath = require("../build/Release/ons").setLogPath;

var logger = new EventEmitter();

var filenames = [];
var _pathname = path.resolve(os.tmpdir(), "/.@node-ons/log-" + uuid());
_setLogPath(_pathname);

try {
    fs.mkdirSync(_pathname);
    filenames = fs.readdirSync(_pathname);
} catch(e) {
    // ...
}

if(process.env.NODE_ONS_LOG === "true") {
    console.log("[.] Log pathname: " + _pathname);
}

var _stream;
watch(_pathname, { recursive: false, followSymLinks: false }, function(filename) {
    if(filenames.indexOf(filename) >= 0) {
        return;
    }

    if(_stream) {
        _stream.close();
        _stream = undefined;
    }

    filenames.push(filename);
    _stream = tailStream(path.resolve(_pathname, filename)).on("data", function(data) {
        data = data.toString().trim().split("\n");
        data.forEach(d => logger.emit("data", d));
    });
});

module.exports = logger;

process.on("exit", function() {
    if(process.env.NODE_ONS_LOG === "true") {
        console.log("[.] Delete path: " + _pathname);
    }

    try {
        fs.removeSync(_pathname);
    } catch(e) {
        // ...
    }
});

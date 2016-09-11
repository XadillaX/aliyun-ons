/**
 * XadillaX created at 2016-07-26 19:30:32 With ♥
 *
 * Copyright (c) 2016 Souche.com, all rights
 * reserved.
 */
"use strict";

var EventEmitter = require("events").EventEmitter;
var _emitter = new EventEmitter();
_emitter.inited = false;

module.exports = _emitter;

let fn = "";

_emitter.__defineGetter__("filename", function() {
    return fn;
});

_emitter.__defineSetter__("filename", function(_fn) {
    if(process.env.NODE_ONS_LOG === "true") {
        console.log("[.] Log filename: " + _fn);
    }
    fn = _fn;
});

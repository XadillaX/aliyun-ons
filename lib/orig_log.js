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

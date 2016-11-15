/**
 * XadillaX created at 2016-07-26 17:14:03 With ♥
 *
 * Copyright (c) 2016 Souche.com, all rights
 * reserved.
 */
"use strict";

var ref = 0;
var buf = [];

var origStdoutWrite = process.stdout.write.bind(process.stdout);

var newStdoutWrite = function() {
    buf.push(arguments);
};

exports.fxxkStdout = function() {
    if(ref) {
        return ref++;
    }

    buf = [];
    process.stdout.write = newStdoutWrite;
    ref = 1;
};

exports.restoreStdout = function() {
    if(!ref) return;

    ref--;
    if(!ref) {
        process.stdout.write = origStdoutWrite;

        buf.forEach(function(b) {
            process.stdout.write.apply(process.stdout, b);
        });
        buf = [];
    }
};

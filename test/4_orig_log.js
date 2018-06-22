/**
 * XadillaX created at 2016-07-26 20:08:54 With ♥
 *
 * Copyright (c) 2018 xcoder.in, all rights
 * reserved.
 */
"use strict";

var common = require("./common");

describe("#original log", function() {
    this.timeout(0);

    var preparedLogs = [
        "register sendMessageHook success,hookname is OnsSendMessageHook",
        "register consumeMessageHook success,hookname is OnsConsumerMessageHook",
        "shutdown producerl successfully",
        "shutdown pushConsumer successfully"
    ];

    it("should produce right logs", function(done) {
        // windows has no log event so far
        if(process.platform === "win32") return this.skip();

        var result = [];
        setTimeout(function() {
            var currentPrepareIdx = 0;
            common.logs.forEach(function(log) {
                if(currentPrepareIdx < preparedLogs.length && log.indexOf(preparedLogs[currentPrepareIdx]) >= 0) {
                    result.push(preparedLogs[currentPrepareIdx]);
                    currentPrepareIdx++;
                }
            });

            result.should.be.eql(preparedLogs);
            done();
        }, 1000);
    });
});

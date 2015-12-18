/**
 * XadillaX created at 2015-12-18 15:38:58 With ♥
 *
 * Copyright (c) 2015 Souche.com, all rights
 * reserved.
 */
"use strict";

var Consumer = require("../build/Release/ons.node").ONSConsumer;

Consumer.prototype.$init = Consumer.prototype.init;
Consumer.prototype.init = function(callback) {
    var self = this;
    this.$init(function(err) {
        self.setListener(function(err, message) {
            console.log(err, message);
        });

        callback(err);
    });
};

Consumer.prototype.$listen = Consumer.prototype.listen;
Consumer.prototype.listen = function() {
    this.$listen();

    if(this.keepTimer) {
        clearInterval(this.keepTimer);
    }

    this.keepTimer = setInterval(function() {}, 2147483647);
};

Consumer.prototype.$stop = Consumer.prototype.stop;
Consumer.prototype.stop = function() {
    this.$stop;

    if(this.keepTimer) {
        clearInterval(this.keepTimer);
    }
};

module.exports = Consumer;

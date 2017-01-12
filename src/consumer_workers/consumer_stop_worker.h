/**
 *            ___
 *      |     | |
 *     / \    | |
 *    |--o|===|-|
 *    |---|   |n|
 *   /     \  |o|
 *  | O     | |d|
 *  | N     |=|e|
 *  | S     | | |
 *  |_______| |_|
 *   |@| |@|  | |
 * ___________|_|_
 *
 * AliyunONS - Node.js SDK for Aliyun ONS (based on RocketMQ)
 *
 * Copyright (c) 2017 XadillaX <i@2333.moe>
 *
 * MIT LIcense <https://github.com/XadillaX/aliyun-ons/blob/master/LICENSE>
 */
#ifndef __CONSUMER_STOP_WORKER_H__
#define __CONSUMER_STOP_WORKER_H__
#include "../consumer.h"

class ConsumerStopWorker : public Nan::AsyncWorker {
public:
    ConsumerStopWorker(Nan::Callback* callback, ONSConsumerV8& ons) :
        AsyncWorker(callback),
        ons(ons)
    {
    }

    ~ConsumerStopWorker() {}

    void Execute()
    {
        ons.Stop();
    }

    void HandleOKCallback()
    {
        Nan::HandleScope scope;
        callback->Call(0, 0);
    }

private:
    ONSConsumerV8& ons;
};
#endif

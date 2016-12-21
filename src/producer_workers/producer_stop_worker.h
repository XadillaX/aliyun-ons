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
 * Copyright (c) 2016 XadillaX <i@2333.moe>
 *
 * MIT LIcense <https://github.com/XadillaX/aliyun-ons/blob/master/LICENSE>
 */
#ifndef __PRODUCER_STOP_WORKER_H__
#define __PRODUCER_STOP_WORKER_H__
#include "../producer.h"
using namespace std;

class ProducerStopWorker : public Nan::AsyncWorker {
public:
    ProducerStopWorker(Nan::Callback* callback, ONSProducerV8& _ons) :
        AsyncWorker(callback),
        ons(_ons)
    {
    }

    ~ProducerStopWorker() {}

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
    ONSProducerV8& ons;
};
#endif

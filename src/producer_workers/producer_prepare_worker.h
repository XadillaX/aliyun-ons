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
#ifndef __PRODUCER_PREPARE_WORKER_H__
#define __PRODUCER_PREPARE_WORKER_H__
#include "../log_util.h"
#include "../producer.h"
#include "../real_producer_wrapper.h"
using namespace std;

class ProducerPrepareWorker : public Nan::AsyncWorker {
public:
    ProducerPrepareWorker(Nan::Callback* callback, ONSProducerV8& ons, bool order) :
        AsyncWorker(callback),
        ons(ons),
        factory_info(ons.factory_info),
        is_order(order)
    {
    }

    ~ProducerPrepareWorker() {}

    void Execute()
    {
        real_producer = new ONSRealProducerWrapper(factory_info, is_order);
        real_producer->Start();
    }

    void HandleOKCallback()
    {
        Nan::HandleScope scope;

        ons.real_producer = real_producer;
        ons.initializing = false;
        ons.inited = true;
        ons.started = true;

        callback->Call(0, NULL);
    }

private:
    ONSProducerV8& ons;
    ONSFactoryProperty& factory_info;
    ONSRealProducerWrapper* real_producer;

    bool is_order;
};
#endif

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
#ifndef __ONS_REAL_PRODUCER_WRAPPER_H__
#define __ONS_REAL_PRODUCER_WRAPPER_H__
#include <ONSFactory.h>
#include "third_party/include/Producer.h"
#include <OrderProducer.h>

class ONSRealProducerWrapper
{
public:
    ONSRealProducerWrapper(ons::ONSFactoryProperty& factory_info, bool order);
    ~ONSRealProducerWrapper();

    void Start();
    void Shutdown();

    ons::SendResultONS Send(ons::Message& msg);
    ons::SendResultONS Send(ons::Message& msg, std::string shardingKey);
    void SendOneway(ons::Message& msg);

    bool IsOrder();

private:
    void* real_producer;
    bool order;
};
#endif

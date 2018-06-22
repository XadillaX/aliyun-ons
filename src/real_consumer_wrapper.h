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
 * Copyright (c) 2018 XadillaX <i@2333.moe>
 *
 * MIT LIcense <https://github.com/XadillaX/aliyun-ons/blob/master/LICENSE>
 */
#ifndef __REAL_CONSUMER_WRAPPER_H__
#define __REAL_CONSUMER_WRAPPER_H__
#include <ONSFactory.h>
#include <MessageListener.h>
#include <MessageOrderListener.h>

class ONSRealConsumerWrapper {
public:
    ONSRealConsumerWrapper(ons::ONSFactoryProperty& factory_info, bool order);
    ~ONSRealConsumerWrapper();

    void Start();
    void Shutdown();
    void Subscribe(const char* topic, const char* sub_expression, void* listener);

    bool IsOrder();

private:
    void* real_consumer;
    bool order;
};

#endif

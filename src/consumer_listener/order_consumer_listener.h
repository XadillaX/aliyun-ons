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
#ifndef __ORDER_CONSUMER_LISTENER_H__
#define __ORDER_CONSUMER_LISTENER_H__
#include <ConsumeOrderContext.h>
#include <MessageOrderListener.h>
#include "base_listener.h"
using namespace AliyunONS;

class ONSOrderConsumerListener : public MessageOrderListener, public ONSConsumerBaseListener {
public: 
    ONSOrderConsumerListener(ONSConsumerV8* parent);
    virtual ~ONSOrderConsumerListener();

    virtual OrderAction consume(Message& message, ConsumeOrderContext& context);
};

#endif

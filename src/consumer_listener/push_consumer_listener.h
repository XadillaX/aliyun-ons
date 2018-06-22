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
#ifndef __CONSUMER_LISTENER_H__
#define __CONSUMER_LISTENER_H__
#include <ConsumeContext.h>
#include <MessageListener.h>
#include "base_listener.h"
using namespace AliyunONS;

class ONSPushConsumerListener : public MessageListener, public ONSConsumerBaseListener {
public: 
    ONSPushConsumerListener(ONSConsumerV8* parent);
    virtual ~ONSPushConsumerListener();

    virtual Action consume(Message& message, ConsumeContext& context);
};

#endif

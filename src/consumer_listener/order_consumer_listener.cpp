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
#include "order_consumer_listener.h"

ONSOrderConsumerListener::ONSOrderConsumerListener(ONSConsumerV8* parent) :
    ONSConsumerBaseListener(parent)
{
}

ONSOrderConsumerListener::~ONSOrderConsumerListener()
{
}

OrderAction ONSOrderConsumerListener::consume(Message& message, ConsumeOrderContext& context)
{
    return ONSConsumerBaseListener::Consume(message) == COMMON_ACTION::SUCCESS ?
        OrderAction::Success :
        OrderAction::Suspend;
}

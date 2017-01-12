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
#include "push_consumer_listener.h"

ONSPushConsumerListener::ONSPushConsumerListener(ONSConsumerV8* parent) :
    ONSConsumerBaseListener(parent)
{
}

ONSPushConsumerListener::~ONSPushConsumerListener()
{
}

Action ONSPushConsumerListener::consume(Message& message, ConsumeContext& context)
{
    return ONSConsumerBaseListener::Consume(message) == COMMON_ACTION::SUCCESS ?
        Action::CommitMessage :
        Action::ReconsumeLater;
}

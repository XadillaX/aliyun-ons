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
#include "real_consumer_listener_wrapper.h"
#include "consumer_listener/push_consumer_listener.h"
#include "consumer_listener/order_consumer_listener.h"

ONSRealConsumerListenerWrapper::ONSRealConsumerListenerWrapper(ONSConsumerV8* parent, bool _order)
{
    order = _order;
    real_listener = order ?
        (void*)new ONSOrderConsumerListener(parent) :
        (void*)new ONSPushConsumerListener(parent);
}

ONSRealConsumerListenerWrapper::~ONSRealConsumerListenerWrapper()
{
    order ?
        delete (ONSOrderConsumerListener*)real_listener :
        delete (ONSPushConsumerListener*)real_listener;
}

bool ONSRealConsumerListenerWrapper::IsOrder()
{
    return order;
}

void* ONSRealConsumerListenerWrapper::GetListener()
{
    return real_listener;
}

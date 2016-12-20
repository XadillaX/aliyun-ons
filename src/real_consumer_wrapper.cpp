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
#include <PushConsumer.h>
#include <OrderConsumer.h>
#include "real_consumer_wrapper.h"
using namespace ons;

#define REAL_CONSUMER ((PushConsumer*)real_consumer)
#define REAL_ORDER_CONSUMER ((OrderConsumer*)real_consumer)

ONSRealConsumerWrapper::ONSRealConsumerWrapper(ONSFactoryProperty& factory_info, bool order)
    : order(order)
{
    real_consumer = order ?
            (void*)ONSFactory::getInstance()->createOrderConsumer(factory_info) :
            (void*)ONSFactory::getInstance()->createPushConsumer(factory_info);
}

ONSRealConsumerWrapper::~ONSRealConsumerWrapper()
{
}

void ONSRealConsumerWrapper::Start()
{
    return order ? REAL_ORDER_CONSUMER->start() : REAL_CONSUMER->start();
}

void ONSRealConsumerWrapper::Shutdown()
{
    return order ? REAL_ORDER_CONSUMER->shutdown() : REAL_CONSUMER->shutdown();
}

void ONSRealConsumerWrapper::Subscribe(const char* topic, const char* sub_expression, void* listener)
{
    return order ?
        REAL_ORDER_CONSUMER->subscribe(topic, sub_expression, (MessageOrderListener*)listener) :
        REAL_CONSUMER->subscribe(topic, sub_expression, (MessageListener*)listener);
}

bool ONSRealConsumerWrapper::IsOrder()
{
    return order;
}

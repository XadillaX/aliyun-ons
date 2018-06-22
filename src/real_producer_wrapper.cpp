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
#include "real_producer_wrapper.h"
using namespace ons;

#define REAL_PRODUCER ((Producer*)real_producer)
#define REAL_ORDER_PRODUCER ((OrderProducer*)real_producer)

ONSRealProducerWrapper::ONSRealProducerWrapper(ONSFactoryProperty& factory_info, bool order) :
    order(order)
{
    real_producer = order ?
            (void*)ONSFactory::getInstance()->createOrderProducer(factory_info) :
            (void*)ONSFactory::getInstance()->createProducer(factory_info);
}

ONSRealProducerWrapper::~ONSRealProducerWrapper()
{
}

void ONSRealProducerWrapper::Start()
{
    !order ? REAL_PRODUCER->start() : REAL_ORDER_PRODUCER->start();
}

void ONSRealProducerWrapper::Shutdown()
{
    !order ? REAL_PRODUCER->shutdown() : REAL_ORDER_PRODUCER->shutdown();

    // needn't delete real_producer
    // refer to document: https://help.aliyun.com/document_detail/29556.html
}

SendResultONS ONSRealProducerWrapper::Send(ons::Message& msg)
{
    if(order)
    {
        throw ONSClientException("OrderProducer should offer shardingKey.", 0x10000000);
    }

    return REAL_PRODUCER->send(msg);
}

SendResultONS ONSRealProducerWrapper::Send(Message& msg, std::string shardingKey)
{
    if(!order)
    {
        throw ONSClientException("Producer shouldn't offer shardingKey.", 0x10000000);
    }

    return REAL_ORDER_PRODUCER->send(msg, shardingKey);
}

void ONSRealProducerWrapper::SendOneway(Message& msg)
{
    if(order)
    {
        throw ONSClientException("OrderProducer don't support for sending oneway.", 0x10000000);
    }

    return REAL_PRODUCER->sendOneway(msg);
}

bool ONSRealProducerWrapper::IsOrder()
{
    return order;
}

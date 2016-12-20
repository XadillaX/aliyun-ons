/*
 * =====================================================================================
 *
 *       Filename:  real_producer_wrapper.h
 *
 *    Description:  ONS real producer wrapper for node.js
 *
 *        Version:  1.0
 *        Created:  2015/12/19 22时20分48秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  XadillaX (ZKD), zhukaidi@souche.com
 *   Organization:  Design & Development Center, Souche Car Service Co., Ltd, HANGZHOU
 *
 * =====================================================================================
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

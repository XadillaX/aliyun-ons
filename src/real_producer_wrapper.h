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

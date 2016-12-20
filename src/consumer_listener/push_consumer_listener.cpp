/*
 * =====================================================================================
 *
 *       Filename:  consumer_listener.cpp
 *
 *    Description:  Consumer listener class and function
 *
 *        Version:  1.0
 *        Created:  2016/07/13 15时15分32秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  XadillaX (zkd), i@2333.moe
 *   Organization:  Design & Development Center, Souche Inc. CHINA
 *
 * =====================================================================================
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
    return (Action)ONSConsumerBaseListener::Consume(message);
}

/*
 * =====================================================================================
 *
 *       Filename:  consumer_listener.h
 *
 *    Description:  Consumer listener class and function
 *
 *        Version:  1.0
 *        Created:  2016/07/13 15时14分09秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  XadillaX (zkd), i@2333.moe
 *   Organization:  Design & Development Center, Souche Inc. CHINA
 *
 * =====================================================================================
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

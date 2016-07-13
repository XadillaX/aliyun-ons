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
#include <nan.h>
#include <uv.h>
#include "ONSFactory.h"
using namespace std;
using namespace ons;

class ONSConsumerV8;
class ONSConsumerACKInner;

struct MessageHandlerParam
{
    ONSConsumerV8* ons;
    Message* message;
    ONSConsumerACKInner* ack_inner;
};

class ONSListenerV8 : public MessageListener {
public:
    ONSListenerV8(ONSConsumerV8* parent)
        : parent(parent)
    {
        async = parent->GetAsync();
    }

    virtual ~ONSListenerV8()
    {
    }
    virtual Action consume(Message& message, ConsumeContext& context);

private:
    ONSConsumerV8* parent;
    uv_async_t* async;
};
#endif

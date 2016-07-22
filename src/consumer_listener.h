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
#include <queue>
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
    ONSListenerV8(ONSConsumerV8*);
    virtual ~ONSListenerV8();

    virtual Action consume(Message&, ConsumeContext&);

    uv_async_t* GetAsync();
    void RestoreAsync(uv_async_t*);

private:
    int thread_count;
    ONSConsumerV8* parent;

    // pool for async
    unsigned int thread_num;
    uv_async_t* async_pool;
    queue<uv_async_t*> idle_async;
    uv_mutex_t mutex;
};
#endif

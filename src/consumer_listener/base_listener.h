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
#ifndef __BASE_LISTENER_H__
#define __BASE_LISTENER_H__
#include <uv.h>
#include <nan.h>
#include <queue>
#include <Message.h>
#include "../consumer_ack.h"
using namespace std;
using namespace ons;

class ONSConsumerV8;
class ONSConsumerACKInner;

namespace AliyunONS {

struct MessageHandlerParam
{
    ONSConsumerV8* ons;
    Message* message;
    ONSConsumerACKInner* ack_inner;
};

class ONSConsumerBaseListener {
public:
    ONSConsumerBaseListener(ONSConsumerV8* parent);
    virtual ~ONSConsumerBaseListener();

    uv_async_t* GetAsync();
    void RestoreAsync(uv_async_t* async);

    COMMON_ACTION Consume(Message& message);

private:
    int thread_count;
    ONSConsumerV8* parent;

    // pool for async
    unsigned int thread_num;
    uv_async_t* async_pool;
    queue<uv_async_t*> idle_async;
    uv_mutex_t mutex;
};

}

#endif

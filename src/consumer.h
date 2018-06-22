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
#ifndef __ONS_CONSUMER_H__
#define __ONS_CONSUMER_H__
#include "ons_options.h"
#include <nan.h>
#include <string>
#include <uv.h>
#include "ONSFactory.h"
#include "real_consumer_wrapper.h"
#include "real_consumer_listener_wrapper.h"
using namespace std;
using namespace ons;

namespace AliyunONS { class ONSConsumerBaseListener; }
class PrepareWorker;

class ONSConsumerV8 : public Nan::ObjectWrap {
public:
    static NAN_MODULE_INIT(Init);

private:
    explicit ONSConsumerV8(
            string _consumer_id,
            string _topics,
            string _tag,
            string _access_key,
            string _secret_key,
            ONSOptions _options);
    ~ONSConsumerV8();

    static NAN_METHOD(New);
    static NAN_METHOD(Prepare);
    static NAN_METHOD(Listen);
    static NAN_METHOD(Stop);
    static NAN_METHOD(SetListener);

    static Nan::Persistent<v8::Function> constructor;

    static void HandleMessage(uv_async_t* handle);

public:
    Nan::Callback* GetListenerFunc()
    {
        Nan::Callback* cb;
        cb = &listener_func;
        return cb;
    }

private:
    void Stop();

private:
    string consumer_id;
    string topics;
    string tag;
    string access_key;
    string secret_key;
    bool is_order;

    bool initializing;
    bool inited;
    bool started;

    ONSFactoryProperty factory_info;
    ONSRealConsumerWrapper* real_consumer;
    ONSRealConsumerListenerWrapper* listener;

    Nan::Callback listener_func;

public:
    friend class ConsumerPrepareWorker;
    friend class ConsumerStopWorker;
    friend class AliyunONS::ONSConsumerBaseListener;
};
#endif

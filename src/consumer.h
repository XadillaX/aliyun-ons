/*
 * =====================================================================================
 *
 *       Filename:  consumer.h
 *
 *    Description:  ONS consumer wrapper for node.js
 *
 *        Version:  1.0
 *        Created:  2015/12/18 13时25分31秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  XadillaX (ZKD), zhukaidi@souche.com
 *   Organization:  Design & Development Center, Souche Car Service Co., Ltd, HANGZHOU
 *
 * =====================================================================================
 */
#ifndef __ONS_CONSUMER_H__
#define __ONS_CONSUMER_H__
#include "ons_options.h"
#include <nan.h>
#include <string>
#include <uv.h>
#include "ONSFactory.h"
using namespace std;
using namespace ons;

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
        uv_mutex_lock(&mutex);
        cb = &listener_func;
        uv_mutex_unlock(&mutex);
        return cb;
    }

    uv_async_t* GetAsync()
    {
        return &async;
    }

private:
    void Stop();

private:
    string consumer_id;
    string topics;
    string tag;
    string access_key;
    string secret_key;

    bool initializing;
    bool inited;
    bool started;

    ONSFactoryProperty factory_info;
    PushConsumer* real_consumer;
    MessageListener* listener;

    Nan::Callback listener_func;

    uv_mutex_t mutex;
    uv_async_t async;
    uv_loop_t* loop;

public:
    friend class ConsumerPrepareWorker;
    friend class ConsumerStopWorker;
};
#endif

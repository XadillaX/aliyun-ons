/*
 * =====================================================================================
 *
 *       Filename:  ons.h
 *
 *    Description:  ONS wrapper for node.js
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
#ifndef __ONS_H__
#define __ONS_H__
#include <nan.h>
#include <string>
#include <uv.h>
#include "ONSFactory.h"
using namespace std;
using namespace ons;

class PrepareWorker;

class ONSConsumerV8 : public Nan::ObjectWrap {
public:
    static void Init(v8::Local<v8::Object> exports);

private:
    explicit ONSConsumerV8(
            string _consumer_id,
            string _topics,
            string _tag,
            string _access_key,
            string _secret_key);
    ~ONSConsumerV8();

    static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);
    static void Prepare(const Nan::FunctionCallbackInfo<v8::Value>& info);
    static void Listen(const Nan::FunctionCallbackInfo<v8::Value>& info);
    static void Stop(const Nan::FunctionCallbackInfo<v8::Value>& info);
    static void SetListener(const Nan::FunctionCallbackInfo<v8::Value>& info);

    static Nan::Persistent<v8::Function> constructor;

    static void HandleMessage(uv_async_t* handle);

public:
    Nan::Callback* GetListenerFunc()
    {
        Nan::Callback* cb;
        uv_mutex_lock(&mutex);
        cb = listener_func;
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

    Nan::Callback* listener_func;

    uv_mutex_t mutex;
    uv_async_t async;
    uv_loop_t* loop;

public:
    friend class PrepareWorker;
};

class ONSListenerV8 : public MessageListener {
public:
    ONSListenerV8(ONSConsumerV8* parent)
        : parent(parent)
    {
        async = parent->GetAsync();
    }

    virtual ~ONSListenerV8() {};
    virtual Action consume(Message message, ConsumeContext context);

private:
    ONSConsumerV8* parent;
    uv_async_t* async;
};

#endif

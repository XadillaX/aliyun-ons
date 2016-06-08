/*
 * =====================================================================================
 *
 *       Filename:  producer.h
 *
 *    Description:  ONS producer wrapper for node.js
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
#ifndef __ONS_PRODUCER_H__
#define __ONS_PRODUCER_H__
#include "ons_options.h"
#include <nan.h>
#include <string>
#include <uv.h>
#include "ONSFactory.h"
using namespace std;
using namespace ons;

class ONSProducerV8 : public Nan::ObjectWrap {
public:
    static void Init(v8::Local<v8::Object> exports);

private:
    explicit ONSProducerV8(
            string _producer_id,
            string _access_key,
            string _secret_key,
            ONSOptions _options);
    ~ONSProducerV8();

    static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);
    static void Start(const Nan::FunctionCallbackInfo<v8::Value>& info);
    static void Stop(const Nan::FunctionCallbackInfo<v8::Value>& info);
    static void Send(const Nan::FunctionCallbackInfo<v8::Value>& info);

    static Nan::Persistent<v8::Function> constructor;

private:
    void Stop();

private:
    string producer_id;
    string access_key;
    string secret_key;

    bool initializing;
    bool inited;
    bool started;

    ONSFactoryProperty factory_info;
    Producer* real_producer;

    uv_mutex_t mutex;

public:
    friend class ProducerPrepareWorker;
    friend class ProducerSendWorker;
};
#endif

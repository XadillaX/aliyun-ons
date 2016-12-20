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
#include "real_producer_wrapper.h"
using namespace std;
using namespace ons;

class ONSProducerV8 : public Nan::ObjectWrap {
public:
    static NAN_MODULE_INIT(Init);

private:
    explicit ONSProducerV8(
            string _producer_id,
            string _access_key,
            string _secret_key,
            ONSOptions _options);
    ~ONSProducerV8();

    static NAN_METHOD(New);
    static NAN_METHOD(Start);
    static NAN_METHOD(Stop);
    static NAN_METHOD(Send);

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
    bool is_order;

    ONSFactoryProperty factory_info;
    ONSRealProducerWrapper* real_producer;

    uv_mutex_t mutex;

public:
    friend class ProducerPrepareWorker;
    friend class ProducerSendWorker;
    friend class ProducerStopWorker;
    friend void PrdrSendingOneWay(uv_work_t* req);
};
#endif

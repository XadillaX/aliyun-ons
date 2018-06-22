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
#include "log_util.h"
#include "producer.h"
#include "ONSClientException.h"

#include "producer_workers/producer_prepare_worker.h"
#include "producer_workers/producer_send_worker.h"
#include "producer_workers/producer_oneway.h"
#include "producer_workers/producer_stop_worker.h"

std::string producer_env_v = std::getenv("NODE_ONS_LOG") == NULL ?
        "" : std::getenv("NODE_ONS_LOG");

Nan::Persistent<v8::Function> ONSProducerV8::constructor;

ONSProducerV8::ONSProducerV8(string _producer_id, string _access_key, string _secret_key, ONSOptions _options) :
    producer_id(_producer_id),
    access_key(_access_key),
    secret_key(_secret_key),

    initializing(false),
    inited(false),
    started(false),
    is_order(false)
{
    factory_info.setFactoryProperty(ONSFactoryProperty::ProducerId, producer_id.c_str());
    factory_info.setFactoryProperty(ONSFactoryProperty::AccessKey, access_key.c_str());
    factory_info.setFactoryProperty(ONSFactoryProperty::SecretKey, secret_key.c_str());

    if(_options.ons_addr != "")
    {
        factory_info.setFactoryProperty(ONSFactoryProperty::ONSAddr, _options.ons_addr.c_str());
    }

    if(_options.namesrv_addr != "")
    {
        factory_info.setFactoryProperty(ONSFactoryProperty::NAMESRV_ADDR, _options.namesrv_addr.c_str());
    }

    if(_options.send_msg_timeout_millis != -1)
    {
        factory_info.setFactoryProperty(
                ONSFactoryProperty::SendMsgTimeoutMillis,
                std::to_string(_options.send_msg_timeout_millis).c_str());
    }

    if(_options.order)
    {
        is_order = true;
    }

    // log file
    string log_filename = AliyunONS::GetLogPath();
    if(log_filename.size())
    {
        factory_info.setFactoryProperty(ONSFactoryProperty::LogPath, log_filename.c_str());
    }

    uv_mutex_init(&mutex);
}

ONSProducerV8::~ONSProducerV8()
{
    Stop();
}

NAN_MODULE_INIT(ONSProducerV8::Init)
{
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("ONSProducer").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(tpl, "start", Start);
    Nan::SetPrototypeMethod(tpl, "stop", Stop);
    Nan::SetPrototypeMethod(tpl, "send", Send);

    constructor.Reset(tpl->GetFunction());
    Nan::Set(target, Nan::New("ONSProducer").ToLocalChecked(), tpl->GetFunction());
}

NAN_METHOD(ONSProducerV8::New)
{
    if(!info.IsConstructCall())
    {
        const int argc = 4;
        v8::Local<v8::Value> argv[argc] = { info[0], info[1], info[2], info[3] };
        v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
        info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
        return;
    }

    v8::String::Utf8Value v8_producer_id(info[0]->ToString());
    v8::String::Utf8Value v8_access_key(info[1]->ToString());
    v8::String::Utf8Value v8_secret_key(info[2]->ToString());

    ONSProducerV8* obj = new ONSProducerV8(
            *v8_producer_id,
            *v8_access_key,
            *v8_secret_key,
            ONSOptions(info[3]));

    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(ONSProducerV8::Start)
{
    ONSProducerV8* ons = ObjectWrap::Unwrap<ONSProducerV8>(info.Holder());
    Nan::Callback* cb = new Nan::Callback(info[0].As<v8::Function>());

    // if it's initialized, it can't be started again
    if(ons->inited)
    {
        v8::Local<v8::Value> argv[1] = { Nan::Error("This ONS producer is initialized.") };
        cb->Call(1, argv);
        delete cb;
        return;
    }

    // if it's initializing, it can't be initialized again
    if(ons->initializing)
    {
        v8::Local<v8::Value> argv[1] = { Nan::Error("This ONS producer is initializing.") };
        cb->Call(1, argv);
        delete cb;
        return;
    }

    ons->initializing = true;

    AsyncQueueWorker(new ProducerPrepareWorker(cb, *ons, ons->is_order));
}

NAN_METHOD(ONSProducerV8::Stop)
{
    ONSProducerV8* ons = ObjectWrap::Unwrap<ONSProducerV8>(info.Holder());
    Nan::Callback* cb = new Nan::Callback(info[0].As<v8::Function>());
    AsyncQueueWorker(new ProducerStopWorker(cb, *ons));
}

NAN_METHOD(ONSProducerV8::Send)
{
    ONSProducerV8* ons = ObjectWrap::Unwrap<ONSProducerV8>(info.Holder());

    Nan::Callback* cb = (info[6]->IsUndefined() || info[6]->IsNull()) ?
            NULL :
            new Nan::Callback(info[6].As<v8::Function>());

    // if it's not initialized or not started, throw an error
    if(!ons->inited || !ons->started)
    {
        v8::Local<v8::Value> argv[1] = { Nan::Error("This ONS producer is not started.") };
        cb->Call(1, argv);
        delete cb;
        return;
    }

    v8::String::Utf8Value v8_topic(info[0]->ToString());
    v8::String::Utf8Value v8_tags(info[1]->ToString());
    v8::String::Utf8Value v8_key(info[2]->ToString());
    v8::String::Utf8Value v8_content(info[3]->ToString());
    int64_t send_at = info[4]->IntegerValue();
    v8::String::Utf8Value v8_sharding_key(info[5]->ToString());

    if(!cb && !ons->is_order)
    {
        // Send Oneway
        PrdrSendOneWay(*ons, *v8_topic, *v8_tags, *v8_key, *v8_content, send_at);
        return;
    }

    AsyncQueueWorker(new ProducerSendWorker(
                cb, *ons, *v8_topic, *v8_tags, *v8_key, *v8_content, send_at, *v8_sharding_key));
}

void ONSProducerV8::Stop()
{
    uv_mutex_lock(&mutex);

    if(!inited || !started) 
    {
        uv_mutex_unlock(&mutex);
        return;
    }

    if(!real_producer)
    {
        uv_mutex_unlock(&mutex);
        return;
    }

    if(producer_env_v == "true") printf("producer stopping...\n");

    real_producer->Shutdown();
    delete real_producer;
    real_producer = NULL;
 
    if(producer_env_v == "true") printf("producer stopped.\n");

    started = false;
    
    uv_mutex_unlock(&mutex);
}

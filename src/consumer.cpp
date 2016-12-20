/*
 * =====================================================================================
 *
 *       Filename:  consumer.cpp
 *
 *    Description:  ONS consumer wrapper for node.js
 *
 *        Version:  1.0
 *        Created:  2015/12/18 13时23分04秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  XadillaX (ZKD), zhukaidi@souche.com
 *   Organization:  Design & Development Center, Souche Car Service Co., Ltd, HANGZHOU
 *
 * =====================================================================================
 */
#include "log_util.h"
#include "consumer.h"
#include "consumer_ack.h"
#include "consumer_listener/base_listener.h"

#include "consumer_workers/consumer_prepare_worker.h"
#include "consumer_workers/consumer_stop_worker.h"

std::string consumer_env_v = std::getenv("NODE_ONS_LOG") == NULL ?
        "" : std::getenv("NODE_ONS_LOG");

Nan::Persistent<v8::Function> ONSConsumerV8::constructor;

ONSConsumerV8::ONSConsumerV8(
        string _consumer_id,
        string _topics,
        string _tag,
        string _access_key,
        string _secret_key,
        ONSOptions _options) :
    consumer_id(_consumer_id),
    topics(_topics),
    tag(_tag),
    access_key(_access_key),
    secret_key(_secret_key),

    initializing(false),
    inited(false),
    started(false),

    real_consumer(NULL),
    listener(NULL),

    listener_func()
{
    Nan::HandleScope scope;

    factory_info.setFactoryProperty(ONSFactoryProperty::ConsumerId, consumer_id.c_str());
    factory_info.setFactoryProperty(ONSFactoryProperty::PublishTopics, topics.c_str());
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

    if(_options.thread_num != -1)
    {
        factory_info.setFactoryProperty(
                ONSFactoryProperty::ConsumeThreadNums,
                std::to_string(_options.thread_num).c_str());
    }
    else
    {
        // to fix https://github.com/XadillaX/aliyun-ons/issues/17
        factory_info.setFactoryProperty(
                ONSFactoryProperty::ConsumeThreadNums, "1");
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

    if(consumer_env_v == "true")
    {
        printf("options: %s %d %d\n",
                _options.ons_addr.c_str(),
                _options.thread_num,
                _options.send_msg_timeout_millis);
    }

    listener = new ONSRealConsumerListenerWrapper(this, is_order);
}

ONSConsumerV8::~ONSConsumerV8()
{
    Stop();

    if(listener) {
        delete listener;
        listener = NULL;
    }
}

NAN_MODULE_INIT(ONSConsumerV8::Init)
{
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("ONSConsumer").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(tpl, "init", Prepare);
    Nan::SetPrototypeMethod(tpl, "listen", Listen);
    Nan::SetPrototypeMethod(tpl, "stop", Stop);
    Nan::SetPrototypeMethod(tpl, "setListener", SetListener);

    constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
    Nan::Set(target, Nan::New("ONSConsumer").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());

    // initialize ack v8 constructor
    ONSConsumerACKV8::Init();
}

NAN_METHOD(ONSConsumerV8::New)
{
    if(!info.IsConstructCall())
    {
        const int argc = 6;
        v8::Local<v8::Value> argv[argc] = { info[0], info[1], info[2], info[3], info[4], info[5] };
        v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
        info.GetReturnValue().Set(cons->NewInstance(argc, argv));
        return;
    }

    v8::String::Utf8Value v8_consumer_id(info[0]->ToString());
    v8::String::Utf8Value v8_topics(info[1]->ToString());
    v8::String::Utf8Value v8_tag(info[2]->ToString());
    v8::String::Utf8Value v8_access_key(info[3]->ToString());
    v8::String::Utf8Value v8_secret_key(info[4]->ToString());

    ONSConsumerV8* obj = new ONSConsumerV8(
            *v8_consumer_id,
            *v8_topics,
            *v8_tag,
            *v8_access_key,
            *v8_secret_key,
            ONSOptions(info[5]));

    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(ONSConsumerV8::Prepare)
{
    ONSConsumerV8* ons = ObjectWrap::Unwrap<ONSConsumerV8>(info.Holder());
    Nan::Callback* cb = new Nan::Callback(info[0].As<v8::Function>());

    // if it's initialized...
    if(ons->inited)
    {
        v8::Local<v8::Value> argv[1] = { 
            Nan::Error("This ONS consumer is initialized.")
        };
        cb->Call(1, argv);
        delete cb;
        return;
    }

    // if it's initializing...
    if(ons->initializing)
    {
        v8::Local<v8::Value> argv[1] = {
            Nan::Error("This ONS consumer is initializing.")
        };
        cb->Call(1, argv);
        delete cb;
        return;
    }

    ons->initializing = true;
    AsyncQueueWorker(new ConsumerPrepareWorker(cb, *ons, ons->is_order));
}

NAN_METHOD(ONSConsumerV8::Listen)
{
    ONSConsumerV8* ons = ObjectWrap::Unwrap<ONSConsumerV8>(info.Holder());

    if(!ons->inited)
    {
        Nan::ThrowError("This ONS consumer is not initialized yet.");
        return info.GetReturnValue().Set(Nan::Undefined());
    }

    if(ons->started)
    {
        Nan::ThrowError("This ONS consumer is already started.");
        return info.GetReturnValue().Set(Nan::Undefined());
    }

    if(!ons->real_consumer)
    {
        Nan::ThrowError("Broken ONS object.");
        return info.GetReturnValue().Set(Nan::Undefined());
    }

    ons->started = true;

    ons->real_consumer->Start();
}

NAN_METHOD(ONSConsumerV8::Stop)
{
    ONSConsumerV8* ons = ObjectWrap::Unwrap<ONSConsumerV8>(info.Holder());
    Nan::Callback* cb = new Nan::Callback(info[0].As<v8::Function>());
    AsyncQueueWorker(new ConsumerStopWorker(cb, *ons));
}

NAN_METHOD(ONSConsumerV8::SetListener)
{
    ONSConsumerV8* ons = ObjectWrap::Unwrap<ONSConsumerV8>(info.Holder());

    if(!ons->listener_func.IsEmpty())
    {
        ons->listener_func.Reset();
    }

    ons->listener_func.Reset(info[0].As<v8::Function>());
}

void ONSConsumerV8::HandleMessage(uv_async_t* handle)
{
    Nan::HandleScope scope;

    AliyunONS::MessageHandlerParam* param = (AliyunONS::MessageHandlerParam*)handle->data;
    Message* message = param->message;

    ONSConsumerV8* ons = param->ons;

    // create ack inner
    ONSConsumerACKInner* ack_inner = param->ack_inner;

    // create ack object and set inner
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(ONSConsumerACKV8::GetConstructor());
    v8::Local<v8::Object> ack_obj = cons->NewInstance(0, {});
    ONSConsumerACKV8* ack = ObjectWrap::Unwrap<ONSConsumerACKV8>(ack_obj);
    ack->SetInner(ack_inner);

    // create message object
    v8::Local<v8::Object> result = Nan::New<v8::Object>();
    result->Set(
            Nan::New<v8::String>("topic").ToLocalChecked(),
            Nan::New<v8::String>(message->getTopic()).ToLocalChecked());
    result->Set(
            Nan::New<v8::String>("tag").ToLocalChecked(),
            Nan::New<v8::String>(message->getTag()).ToLocalChecked());
    result->Set(
            Nan::New<v8::String>("key").ToLocalChecked(),
            Nan::New<v8::String>(message->getKey()).ToLocalChecked());
    result->Set(
            Nan::New<v8::String>("msgId").ToLocalChecked(),
            Nan::New<v8::String>(message->getMsgID()).ToLocalChecked());
    result->Set(
            Nan::New<v8::String>("startDeliverTime").ToLocalChecked(),
            Nan::New<v8::Number>(message->getStartDeliverTime()));
    result->Set(
            Nan::New<v8::String>("body").ToLocalChecked(),
            Nan::New<v8::String>(message->getBody()).ToLocalChecked());
    result->Set(
            Nan::New<v8::String>("reconsumeTimes").ToLocalChecked(),
            Nan::New<v8::Number>(message->getReconsumeTimes()));

    v8::Local<v8::Value> argv[3] = { Nan::Undefined(), result, ack_obj };
    Nan::Callback* callback = ons->GetListenerFunc();
    callback->Call(3, argv);
}

void ONSConsumerV8::Stop()
{
    if(!inited || !started)
    {
        return;
    }

    if(!real_consumer)
    {
        return;
    }

    if(consumer_env_v == "true") printf("consumer stopping!\n");

    real_consumer->Shutdown();
    delete real_consumer;
    real_consumer = NULL;

    if(consumer_env_v == "true") printf("consumer stopped!\n");

    started = false;
}

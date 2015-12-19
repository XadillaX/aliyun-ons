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
#include "consumer.h"

Nan::Persistent<v8::Function> ONSConsumerV8::constructor;

struct MessageHandlerParam
{
    ONSConsumerV8* ons;
    Message* message;
};

class ConsumerPrepareWorker : public Nan::AsyncWorker {
public:
    ConsumerPrepareWorker(Nan::Callback* callback, ONSConsumerV8& ons) :
        AsyncWorker(callback),
        ons(ons),
        factory_info(ons.factory_info)
    {
    }

    ~ConsumerPrepareWorker() {}

    void Execute()
    {
        real_consumer = ONSFactory::getInstance()->createPushConsumer(factory_info);
    }

    void HandleOKCallback()
    {
        Nan::HandleScope scope;

        // subscribe...
        real_consumer->subscribe(factory_info.getPublishTopics(), ons.tag.c_str(), ons.listener);

        ons.real_consumer = real_consumer;
        ons.initializing = false;
        ons.inited = true;
        callback->Call(0, 0);
    }

private:
    ONSConsumerV8& ons;
    ONSFactoryProperty& factory_info;
    PushConsumer* real_consumer;
};

ONSConsumerV8::ONSConsumerV8(
        string _consumer_id,
        string _topics,
        string _tag,
        string _access_key,
        string _secret_key) :
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

    listener_func(NULL)
{
    factory_info.setFactoryProperty(ONSFactoryProperty::ConsumerId, consumer_id.c_str());
    factory_info.setFactoryProperty(ONSFactoryProperty::PublishTopics, topics.c_str());
    factory_info.setFactoryProperty(ONSFactoryProperty::AccessKey, access_key.c_str());
    factory_info.setFactoryProperty(ONSFactoryProperty::SecretKey, secret_key.c_str());

    listener = new ONSListenerV8(this);

    uv_mutex_init(&mutex);
    loop = uv_default_loop();

    uv_async_init(loop, &async, HandleMessage);
}

ONSConsumerV8::~ONSConsumerV8()
{
    Stop();
    if(real_consumer) delete real_consumer;
    if(listener) delete listener;

    uv_mutex_destroy(&mutex);
    uv_close((uv_handle_t*)&async, NULL);
}

void ONSConsumerV8::Init(v8::Local<v8::Object> exports)
{
    Nan::HandleScope scope;

    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("ONSConsumer").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    Nan::SetPrototypeMethod(tpl, "init", Prepare);
    Nan::SetPrototypeMethod(tpl, "listen", Listen);
    Nan::SetPrototypeMethod(tpl, "stop", Stop);
    Nan::SetPrototypeMethod(tpl, "setListener", SetListener);

    constructor.Reset(tpl->GetFunction());
    exports->Set(Nan::New("ONSConsumer").ToLocalChecked(), tpl->GetFunction());
}

void ONSConsumerV8::New(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    if(!info.IsConstructCall())
    {
        const int argc = 5;
        v8::Local<v8::Value> argv[argc] = { info[0], info[1], info[2], info[3], info[4] };
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
            *v8_secret_key);

    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
}

void ONSConsumerV8::Prepare(const Nan::FunctionCallbackInfo<v8::Value>& info)
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
    AsyncQueueWorker(new ConsumerPrepareWorker(cb, *ons));
}

void ONSConsumerV8::Listen(const Nan::FunctionCallbackInfo<v8::Value>& info)
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
    ons->real_consumer->start();
}

void ONSConsumerV8::Stop(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    ONSConsumerV8* ons = ObjectWrap::Unwrap<ONSConsumerV8>(info.Holder());
    ons->Stop();
}

void ONSConsumerV8::SetListener(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    ONSConsumerV8* ons = ObjectWrap::Unwrap<ONSConsumerV8>(info.Holder());

    uv_mutex_lock(&ons->mutex);
    if(ons->listener_func)
    {
        delete ons->listener_func;
        ons->listener_func = NULL;
    }

    Nan::Callback* cb = new Nan::Callback(info[0].As<v8::Function>());
    ons->listener_func = cb;
    uv_mutex_unlock(&ons->mutex);
}

void ONSConsumerV8::Stop()
{
    if(!inited || !started) return;
    if(!real_consumer) return;

    real_consumer->shutdown();
    started = false;
}

void ONSConsumerV8::HandleMessage(uv_async_t* handle)
{
    Nan::HandleScope scope;

    MessageHandlerParam* param = (MessageHandlerParam*)handle->data;

    Message* message = param->message;
    ONSConsumerV8* ons = param->ons;

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

    v8::Local<v8::Value> argv[2] = { Nan::Undefined(), result };
    Nan::Callback* callback = ons->GetListenerFunc();
    callback->Call(2, argv);

    delete param->message;
    delete param;
}

Action ONSListenerV8::consume(Message message, ConsumeContext context)
{
    Message* m = new Message(message);
    MessageHandlerParam* param = new MessageHandlerParam();
    param->message = m;
    param->ons = parent;
    async->data = (void*)param;
    uv_async_send(async);

    return CommitMessage;
}

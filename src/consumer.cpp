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
#include "consumer_ack.h"

std::string consumer_env_v = std::getenv("NODE_ONS_LOG") == NULL ?
        "" : std::getenv("NODE_ONS_LOG");

Nan::Persistent<v8::Function> ONSConsumerV8::constructor;

struct MessageHandlerParam
{
    ONSConsumerV8* ons;
    Message* message;
    ONSConsumerACKInner* ack_inner;
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
        
        // subscribe
        real_consumer->subscribe(factory_info.getPublishTopics(), ons.tag.c_str(), ons.listener);
    }

    void HandleOKCallback()
    {
        Nan::HandleScope scope;

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

    listener_func(NULL)
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

    if(consumer_env_v == "true")
    {
        printf("options: %s %d %d\n",
                _options.ons_addr.c_str(),
                _options.thread_num,
                _options.send_msg_timeout_millis);
    }

    listener = new ONSListenerV8(this);

    uv_mutex_init(&mutex);
    loop = uv_default_loop();

    uv_async_init(loop, &async, ONSConsumerV8::HandleMessage);
}

ONSConsumerV8::~ONSConsumerV8()
{
    Stop();

    uv_mutex_lock(&mutex);
    if(real_consumer) delete real_consumer;
    if(listener) delete listener;
    uv_mutex_unlock(&mutex);

    uv_mutex_destroy(&mutex);
    uv_close((uv_handle_t*)&async, NULL);
}

NAN_MODULE_INIT(ONSConsumerV8::Init)
{
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("ONSConsumer").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
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
    AsyncQueueWorker(new ConsumerPrepareWorker(cb, *ons));
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

    uv_mutex_lock(&ons->mutex);
    ons->real_consumer->start();
    uv_mutex_unlock(&ons->mutex);
}

NAN_METHOD(ONSConsumerV8::Stop)
{
    ONSConsumerV8* ons = ObjectWrap::Unwrap<ONSConsumerV8>(info.Holder());
    ons->Stop();
}

NAN_METHOD(ONSConsumerV8::SetListener)
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
    uv_mutex_lock(&mutex);
    if(!inited || !started)
    {
        uv_mutex_unlock(&mutex);
        return;
    }
    if(!real_consumer)
    {
        uv_mutex_unlock(&mutex);
        return;
    }

    if(consumer_env_v == "true") printf("Stopping!\n");
    real_consumer->shutdown();
    if(consumer_env_v == "true") printf("Stopped!\n");
    started = false;

    uv_mutex_unlock(&mutex);
}

void ONSConsumerV8::HandleMessage(uv_async_t* handle)
{
    Nan::HandleScope scope;

    MessageHandlerParam* param = (MessageHandlerParam*)handle->data;
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

Action ONSListenerV8::consume(Message& message, ConsumeContext& context)
{
    ONSConsumerACKInner* ack_inner = new ONSConsumerACKInner();
    MessageHandlerParam* param = new MessageHandlerParam();
    param->message = &message;

    if(consumer_env_v == "true")
    {
        printf(">>> ACK Inner Created: 0x%lX\n", (unsigned long)ack_inner);
        printf(">>> Message Handler Param Created: 0x%lX\n", (unsigned long)param);
    }

    param->ons = parent;
    param->ack_inner = ack_inner;
    async->data = (void*)param;
    uv_async_send(async);

    Action result = ack_inner->WaitResult();

    delete ack_inner;
    delete param;

    if(consumer_env_v == "true")
    {
        printf(">>> ACK Inner Deleted: 0x%lX\n", (unsigned long)ack_inner);
        printf(">>> Message Handler Param Deleted: 0x%lX\n", (unsigned long)param);
    }

    return result;
}

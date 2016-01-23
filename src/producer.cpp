/*
 * =====================================================================================
 *
 *       Filename:  producer.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015/12/19 22时24分45秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  XadillaX (ZKD), zhukaidi@souche.com
 *   Organization:  Design & Development Center, Souche Car Service Co., Ltd, HANGZHOU
 *
 * =====================================================================================
 */
#include "producer.h"
#include "ONSClientException.h"

Nan::Persistent<v8::Function> ONSProducerV8::constructor;

class ProducerPrepareWorker : public Nan::AsyncWorker {
public:
    ProducerPrepareWorker(Nan::Callback* callback, ONSProducerV8& ons) :
        AsyncWorker(callback),
        ons(ons),
        factory_info(ons.factory_info)
    {
    }

    ~ProducerPrepareWorker() {}

    void Execute()
    {
        real_producer = ONSFactory::getInstance()->createProducer(factory_info);
        real_producer->start();
    }

    void HandleOKCallback()
    {
        Nan::HandleScope scope;

        ons.real_producer = real_producer;
        ons.initializing = false;
        ons.inited = true;
        ons.started = true;
        callback->Call(0, 0);
    }

private:
    ONSProducerV8& ons;
    ONSFactoryProperty& factory_info;
    Producer* real_producer;
};

class ProducerSendWorker : public Nan::AsyncWorker {
public:
    ProducerSendWorker(
            Nan::Callback* callback,
            ONSProducerV8& _ons,
            string _topic,
            string _tags,
            string _key,
            string _content) :
        AsyncWorker(callback),
        ons(_ons),
        topic(_topic),
        tags(_tags),
        key(_key),
        content(_content),

        errored(false),
        error_msg("")
    {
    }

    ~ProducerSendWorker() {}

    void Execute()
    {
        Message msg(topic, tags, content);
        if(key != "")
        {
            msg.setKey(key);
        }

        uv_mutex_lock(&ons.mutex);
        Producer* real_producer = ons.real_producer;

        try
        {
            send_result = real_producer->send(msg);
        }
        catch(const ONSClientException& e)
        {
            error_msg = e.GetMsg();
            errored = true;
            uv_mutex_unlock(&ons.mutex);
            return;
        }

        uv_mutex_unlock(&ons.mutex);
    }

    void HandleOKCallback()
    {
        Nan::HandleScope scope;

        if(errored)
        {
            v8::Local<v8::Value> argv[1] = {
                Nan::Error(error_msg.c_str())
            };
            callback->Call(1, argv);
            return;
        }

        v8::Local<v8::Value> argv[2] = {
            Nan::Undefined(),
            Nan::New<v8::String>(send_result.getMessageId()).ToLocalChecked()
        };
        callback->Call(2, argv);
    }

private:
    ONSProducerV8& ons;
    string topic;
    string tags;
    string key;
    string content;

    SendResultONS send_result;
    string error_msg;
    bool errored;
};

ONSProducerV8::ONSProducerV8(string _producer_id, string _access_key, string _secret_key) :
    producer_id(_producer_id),
    access_key(_access_key),
    secret_key(_secret_key),

    initializing(false),
    inited(false),
    started(false)
{
    factory_info.setFactoryProperty(ONSFactoryProperty::ProducerId, producer_id.c_str());
    factory_info.setFactoryProperty(ONSFactoryProperty::AccessKey, access_key.c_str());
    factory_info.setFactoryProperty(ONSFactoryProperty::SecretKey, secret_key.c_str());

    uv_mutex_init(&mutex);
}

ONSProducerV8::~ONSProducerV8()
{
    Stop();

    uv_mutex_lock(&mutex);
    if(real_producer) delete real_producer;
    uv_mutex_unlock(&mutex);

    uv_mutex_destroy(&mutex);
}

void ONSProducerV8::Init(v8::Local<v8::Object> exports)
{
    Nan::HandleScope scope;

    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("ONSProducer").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    Nan::SetPrototypeMethod(tpl, "start", Start);
    Nan::SetPrototypeMethod(tpl, "stop", Stop);
    Nan::SetPrototypeMethod(tpl, "send", Send);

    constructor.Reset(tpl->GetFunction());
    exports->Set(Nan::New("ONSProducer").ToLocalChecked(), tpl->GetFunction());
}

void ONSProducerV8::New(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    if(!info.IsConstructCall())
    {
        const int argc = 3;
        v8::Local<v8::Value> argv[argc] = { info[0], info[1], info[2] };
        v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
        info.GetReturnValue().Set(cons->NewInstance(argc, argv));
        return;
    }

    v8::String::Utf8Value v8_producer_id(info[0]->ToString());
    v8::String::Utf8Value v8_access_key(info[1]->ToString());
    v8::String::Utf8Value v8_secret_key(info[2]->ToString());

    ONSProducerV8* obj = new ONSProducerV8(
            *v8_producer_id,
            *v8_access_key,
            *v8_secret_key);

    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
}

void ONSProducerV8::Start(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    ONSProducerV8* ons = ObjectWrap::Unwrap<ONSProducerV8>(info.Holder());
    Nan::Callback* cb = new Nan::Callback(info[0].As<v8::Function>());

    // if it's initialized...
    if(ons->inited)
    {
        v8::Local<v8::Value> argv[1] = { 
            Nan::Error("This ONS producer is initialized.")
        };
        cb->Call(1, argv);
        delete cb;
        return;
    }

    // if it's initializing...
    if(ons->initializing)
    {
        v8::Local<v8::Value> argv[1] = {
            Nan::Error("This ONS producer is initializing.")
        };
        cb->Call(1, argv);
        delete cb;
        return;
    }

    ons->initializing = true;
    AsyncQueueWorker(new ProducerPrepareWorker(cb, *ons));
}

void ONSProducerV8::Stop(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    ONSProducerV8* ons = ObjectWrap::Unwrap<ONSProducerV8>(info.Holder());
    ons->Stop();
}

void ONSProducerV8::Send(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    ONSProducerV8* ons = ObjectWrap::Unwrap<ONSProducerV8>(info.Holder());

    Nan::Callback* cb = new Nan::Callback(info[4].As<v8::Function>());

    if(!ons->inited || !ons->started)
    {
        v8::Local<v8::Value> argv[1] = { 
            Nan::Error("This ONS producer is not started.")
        };
        cb->Call(1, argv);
        delete cb;
        return;
    }

    v8::String::Utf8Value v8_topic(info[0]->ToString());
    v8::String::Utf8Value v8_tags(info[1]->ToString());
    v8::String::Utf8Value v8_key(info[2]->ToString());
    v8::String::Utf8Value v8_content(info[3]->ToString());

    AsyncQueueWorker(new ProducerSendWorker(cb, *ons, *v8_topic, *v8_tags, *v8_key, *v8_content));
}

void ONSProducerV8::Stop()
{
    if(!inited || !started) return;
    if(!real_producer) return;

    real_producer->shutdown();
    started = false;
}

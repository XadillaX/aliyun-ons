/*
 * =====================================================================================
 *
 *       Filename:  producer_send_worker.h
 *
 *    Description:  Send worker for producer
 *
 *        Version:  1.0
 *        Created:  2016/07/13 15时02分32秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  XadillaX (zkd), i@2333.moe
 *   Organization:  Design & Development Center, Souche Inc. CHINA
 *
 * =====================================================================================
 */
#ifndef __PRODUCER_SEND_WORKER_H__
#define __PRODUCER_SEND_WORKER_H__
#include "../producer.h"

class ProducerSendWorker : public Nan::AsyncWorker {
public:
    ProducerSendWorker(
            Nan::Callback* callback,
            ONSProducerV8& _ons,
            string _topic,
            string _tags,
            string _key,
            string _content,
            int64_t _send_at) :
        AsyncWorker(callback),
        ons(_ons),
        topic(_topic),
        tags(_tags),
        key(_key),
        content(_content),
        send_at(_send_at),

        errored(false),
        error_msg("")
    {
    }

    ~ProducerSendWorker() {}

    void Execute()
    {
        Message msg(topic.c_str(), tags.c_str(), content.c_str());
        if(key != "")
        {
            msg.setKey(key.c_str());
        }
        
        // delay...
        if(send_at != -1)
        {
            msg.setStartDeliverTime(send_at);
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
    
    int64_t send_at;

    SendResultONS send_result;
    bool errored;
    string error_msg;
};
#endif

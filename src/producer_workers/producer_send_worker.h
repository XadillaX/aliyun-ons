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
#include "../real_producer_wrapper.h"
#include "../producer.h"
using namespace std;

class ProducerSendWorker : public Nan::AsyncWorker {
public:
    ProducerSendWorker(
            Nan::Callback* callback,
            ONSProducerV8& _ons,
            string _topic,
            string _tags,
            string _key,
            string _content,
            int64_t _send_at,
            string _sharding_key) :
        AsyncWorker(callback),
        ons(_ons),
        topic(_topic),
        tags(_tags),
        key(_key),
        content(_content),

        errored(false),
        error_msg("")
    {
        send_at = _send_at;
        sharding_key = _sharding_key;
    }

    ~ProducerSendWorker() {}

    void Execute()
    {
        uv_mutex_lock(&ons.mutex);
        if(!ons.inited || !ons.started || !ons.real_producer)
        {
            error_msg = "producer stopped.";
            errored = true;

            uv_mutex_unlock(&ons.mutex);
            return;
        }

        Message msg(topic.c_str(), tags.c_str(), content.c_str());

        if(key != "")
        {
            msg.setKey(key.c_str());
        }
        
        if(send_at != -1)
        {
            msg.setStartDeliverTime(send_at);
        }

        ONSRealProducerWrapper* real_producer = ons.real_producer;

        try
        {
            if(!real_producer->IsOrder())
            {
                send_result = real_producer->Send(msg);
            }
            else
            {
                send_result = real_producer->Send(msg, sharding_key);
            }
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
            v8::Local<v8::Value> argv[1] = { Nan::Error(error_msg.c_str()) };
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
    string sharding_key;
    
    int64_t send_at;

    SendResultONS send_result;
    bool errored;
    string error_msg;
};
#endif

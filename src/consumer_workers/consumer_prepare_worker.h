/*
 * =====================================================================================
 *
 *       Filename:  consumer_prepare_worker.h
 *
 *    Description:  Prepare worker for consumer
 *
 *        Version:  1.0
 *        Created:  2016/07/13 15时10分46秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  XadillaX (zkd), i@2333.moe
 *   Organization:  Design & Development Center, Souche Inc. CHINA
 *
 * =====================================================================================
 */
#ifndef __CONSUMER_PREPARE_WORKER_H__
#define __CONSUMER_PREPARE_WORKER_H__
#include "../consumer.h"

class ConsumerPrepareWorker : public Nan::AsyncWorker {
public:
    ConsumerPrepareWorker(Nan::Callback* callback, ONSConsumerV8& ons, string uuid, int stdout_fd) :
        AsyncWorker(callback),
        ons(ons),
        factory_info(ons.factory_info),
        uuid(uuid),
        stdout_fd(stdout_fd)
    {
    }

    ~ConsumerPrepareWorker() {}

    void Execute()
    {
        real_consumer = ONSFactory::getInstance()->createPushConsumer(factory_info);
        
        // subscribe
        real_consumer->subscribe(
                factory_info.getPublishTopics(),
                ons.tag.c_str(),
                ons.listener);
    }

    void HandleOKCallback()
    {
        Nan::HandleScope scope;

        if(uuid != "")
        {
            fclose(stdout);
            dup2(stdout_fd, STDOUT_FILENO);
            stdout = fdopen(STDOUT_FILENO, "w");
            close(stdout_fd);
        }

        ons.real_consumer = real_consumer;
        ons.initializing = false;
        ons.inited = true;

        v8::Local<v8::Value> argv[2] = { Nan::Undefined(), Nan::New<v8::String>(uuid).ToLocalChecked() };
        callback->Call(2, argv);
    }

private:
    ONSConsumerV8& ons;
    ONSFactoryProperty& factory_info;
    PushConsumer* real_consumer;
    string uuid;
    int stdout_fd;
};
#endif

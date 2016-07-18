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
        real_consumer->subscribe(
                factory_info.getPublishTopics(),
                ons.tag.c_str(),
                ons.listener);
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
#endif

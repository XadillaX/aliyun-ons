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
    ConsumerPrepareWorker(Nan::Callback* callback, ONSConsumerV8& ons, bool order) :
        AsyncWorker(callback),
        ons(ons),
        factory_info(ons.factory_info),
        is_order(order)
    {
    }

    ~ConsumerPrepareWorker() {}

    void Execute()
    {
        real_consumer = new ONSRealConsumerWrapper(factory_info, is_order);
        
        // subscribe
        real_consumer->Subscribe(
                factory_info.getPublishTopics(),
                ons.tag.c_str(),
                ons.listener->GetListener());
    }

    void HandleOKCallback()
    {
        Nan::HandleScope scope;

        ons.real_consumer = real_consumer;
        ons.initializing = false;
        ons.inited = true;

        callback->Call(0, NULL);
    }

private:
    ONSConsumerV8& ons;
    ONSFactoryProperty& factory_info;
    ONSRealConsumerWrapper* real_consumer;

    bool is_order;
};
#endif

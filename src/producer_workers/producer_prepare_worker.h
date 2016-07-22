/*
 * =====================================================================================
 *
 *       Filename:  producer_prepare_worker.h
 *
 *    Description:  Prepare worker for producer
 *
 *        Version:  1.0
 *        Created:  2016/07/13 10时48分34秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  XadillaX (ZKD), zhukaidi@souche.com
 *   Organization:  Design & Development Center, Souche Car Service Co., Ltd, HANGZHOU
 *
 * =====================================================================================
 */
#ifndef __PRODUCER_PREPARE_WORKER_H__
#define __PRODUCER_PREPARE_WORKER_H__
#include "../producer.h"

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
#endif

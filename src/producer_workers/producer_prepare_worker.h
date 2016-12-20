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
#include "../log_util.h"
#include "../producer.h"
#include "../real_producer_wrapper.h"
using namespace std;

class ProducerPrepareWorker : public Nan::AsyncWorker {
public:
    ProducerPrepareWorker(Nan::Callback* callback, ONSProducerV8& ons, bool order) :
        AsyncWorker(callback),
        ons(ons),
        factory_info(ons.factory_info),
        is_order(order)
    {
    }

    ~ProducerPrepareWorker() {}

    void Execute()
    {
        real_producer = new ONSRealProducerWrapper(factory_info, is_order);
        real_producer->Start();
    }

    void HandleOKCallback()
    {
        Nan::HandleScope scope;

        ons.real_producer = real_producer;
        ons.initializing = false;
        ons.inited = true;
        ons.started = true;

        callback->Call(0, NULL);
    }

private:
    ONSProducerV8& ons;
    ONSFactoryProperty& factory_info;
    ONSRealProducerWrapper* real_producer;

    bool is_order;
};
#endif

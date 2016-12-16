/*
 * =====================================================================================
 *
 *       Filename:  producer_stop_worker.h
 *
 *    Description:  Stop worker for producer
 *
 *        Version:  1.0
 *        Created:  2016/07/13 15时08分05秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  XadillaX (zkd), i@2333.moe
 *   Organization:  Design & Development Center, Souche Inc. CHINA
 *
 * =====================================================================================
 */
#ifndef __PRODUCER_STOP_WORKER_H__
#define __PRODUCER_STOP_WORKER_H__
#include "../producer.h"
using namespace std;

class ProducerStopWorker : public Nan::AsyncWorker {
public:
    ProducerStopWorker(Nan::Callback* callback, ONSProducerV8& _ons) :
        AsyncWorker(callback),
        ons(_ons)
    {
    }

    ~ProducerStopWorker() {}

    void Execute()
    {
        ons.Stop();
    }

    void HandleOKCallback()
    {
        Nan::HandleScope scope;
        callback->Call(0, 0);
    }

private:
    ONSProducerV8& ons;
};
#endif

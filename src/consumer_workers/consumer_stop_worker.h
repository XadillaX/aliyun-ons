/*
 * =====================================================================================
 *
 *       Filename:  consumer_stop_worker.h
 *
 *    Description:  Stop worker for consumer
 *
 *        Version:  1.0
 *        Created:  2016/07/13 15时12分35秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  XadillaX (zkd), i@2333.moe
 *   Organization:  Design & Development Center, Souche Inc. CHINA
 *
 * =====================================================================================
 */
#ifndef __CONSUMER_STOP_WORKER_H__
#define __CONSUMER_STOP_WORKER_H__
#include "../consumer.h"

class ConsumerStopWorker : public Nan::AsyncWorker {
public:
    ConsumerStopWorker(Nan::Callback* callback, ONSConsumerV8& ons) :
        AsyncWorker(callback),
        ons(ons)
    {
    }

    ~ConsumerStopWorker() {}

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
    ONSConsumerV8& ons;
};
#endif

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
    ProducerPrepareWorker(Nan::Callback* callback, ONSProducerV8& ons, string u4, int stdout_fd) :
        AsyncWorker(callback),
        ons(ons),
        factory_info(ons.factory_info),
        uuid(u4),
        stdout_fd(stdout_fd)
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

        if(uuid != "")
        {
#ifdef WIN32
            // windows has no log console
#else
            fclose(stdout);
            dup2(stdout_fd, STDOUT_FILENO);
            stdout = fdopen(STDOUT_FILENO, "w");
            close(stdout_fd);
#endif
        }

        ons.real_producer = real_producer;
        ons.initializing = false;
        ons.inited = true;
        ons.started = true;

        v8::Local<v8::Value> argv[2] = { Nan::Undefined(), Nan::New<v8::String>(uuid).ToLocalChecked() };
        callback->Call(2, argv);
    }

private:
    ONSProducerV8& ons;
    ONSFactoryProperty& factory_info;
    Producer* real_producer;

    int stdout_fd;
    string uuid;
};
#endif

/*
 * =====================================================================================
 *
 *       Filename:  consumer_ack.h
 *
 *    Description:  ONS consumer ACK wrapper for node.js
 *
 *        Version:  1.0
 *        Created:  2016/01/25 11时16分17秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  XadillaX (ZKD), zhukaidi@souche.com
 *   Organization:  Design & Development Center, Souche Car Service Co., Ltd, HANGZHOU
 *
 * =====================================================================================
 */
#ifndef __ONS_CONSUMER_ACK_H__
#define __ONS_CONSUMER_ACK_H__
#include <uv.h>
#include <nan.h>
#include <string>
#include <cstdlib>
#include "Action.h"
using namespace std;

extern std::string ack_env_v;

class ONSConsumerACKInner {
public:
    ONSConsumerACKInner() :
        acked(false)
    {
        uv_cond_init(&cond);
        uv_mutex_init(&mutex);
    }

    ~ONSConsumerACKInner()
    {
        uv_mutex_destroy(&mutex);
        uv_cond_destroy(&cond);
    }

public:
    void Ack(Action result = Action::CommitMessage)
    {
        uv_mutex_lock(&mutex);
        bool _acked = acked;
        uv_mutex_unlock(&mutex);

        // if acknowledged, DONOT acknowledge again
        if(_acked) return;

        ack_result = result;

        // write down some debug information
        // while `NODE_ONS_LOG=true`
        if(ack_env_v == "true") printf("[-----] ack: 0x%lX\n", (unsigned long)this);

        // tell `this->WaitResult()` to continue
        uv_mutex_lock(&mutex);
        uv_cond_signal(&cond);
        acked = true;
        uv_mutex_unlock(&mutex);
    }

    Action WaitResult()
    {
        // Wait for `this->Ack`
        //
        // and it will emit `uv_cond_signal` to let it stop wait
        uv_mutex_lock(&mutex);
        uv_cond_wait(&cond, &mutex);
        uv_mutex_unlock(&mutex);

        Action result = ack_result;

        // write down some debug information
        // while `NODE_ONS_LOG=true`
        if(ack_env_v == "true") printf("[-----] finish wait: 0x%lX\n", (unsigned long)this);

        return result;
    }


private:
    uv_mutex_t mutex;
    uv_cond_t cond;
    Action ack_result;
    bool acked;
};

class ONSListenerV8;
class ONSConsumerACKV8 : public Nan::ObjectWrap {
public:
    friend class ONSConsumerV8;

private:
    explicit ONSConsumerACKV8();
    ~ONSConsumerACKV8();

    static NAN_METHOD(New);
    static NAN_METHOD(Done);

    static Nan::Persistent<v8::Function> constructor;

public:
    static void Init();

    static Nan::Persistent<v8::Function>& GetConstructor()
    {
        return constructor;
    }

    void SetInner(ONSConsumerACKInner* _inner)
    {
        // set the real `Acker` in the main loop
        //
        // it's thread-safe
        inner = _inner;
    }

    void Ack(Action result = Action::CommitMessage)
    {
        if(inner)
        {
            // call true `inner->Ack` in the main loop
            //
            // because this function <ONSConsumerACKV8::Ack()> will called on
            // the main loop only
            inner->Ack(result);

            // write down some debug information
            // while `NODE_ONS_LOG=true`
            if(ack_env_v == "true") printf("[---] inner unrefed: 0x%lX\n", (unsigned long)inner);

            inner = NULL;
        }
    }

private:
    ONSConsumerACKInner* inner;
};
#endif

/**
 *            ___
 *      |     | |
 *     / \    | |
 *    |--o|===|-|
 *    |---|   |n|
 *   /     \  |o|
 *  | O     | |d|
 *  | N     |=|e|
 *  | S     | | |
 *  |_______| |_|
 *   |@| |@|  | |
 * ___________|_|_
 *
 * AliyunONS - Node.js SDK for Aliyun ONS (based on RocketMQ)
 *
 * Copyright (c) 2018 XadillaX <i@2333.moe>
 *
 * MIT LIcense <https://github.com/XadillaX/aliyun-ons/blob/master/LICENSE>
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

enum COMMON_ACTION {
    SUCCESS,
    LATER
};

class ONSListenerV8;
class ONSConsumerACKInner {
public:
    ONSConsumerACKInner(const char* msg_id) :
        acked(false),
        msg_id(msg_id)
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
    void Ack(COMMON_ACTION result = COMMON_ACTION::SUCCESS)
    {
        uv_mutex_lock(&mutex);
        bool _acked = acked;

        // if acknowledged, DONOT acknowledge again
        if(_acked)
        {
            uv_mutex_unlock(&mutex);
            return;
        }

        ack_result = result;

        // write down some debug information
        // while `NODE_ONS_LOG=true`
        if(ack_env_v == "true")
        {
            printf("[%s][-----] ack: 0x%lX\n", msg_id.c_str(), (unsigned long)this);
        }

        // tell `this->WaitResult()` to continue
        acked = true;
        uv_cond_signal(&cond);
        uv_mutex_unlock(&mutex);
    }

    COMMON_ACTION WaitResult()
    { 
        uv_mutex_lock(&mutex);

        // If `cond signal` sent before `WaitResult()`,
        // `uv_cond_wait` will blocked and will never continue
        //
        // So we have to return result directly without `uv_cond_wait`
        if(acked)
        {
            COMMON_ACTION result = ack_result;
            uv_mutex_unlock(&mutex);

            return result;
        }

        // Wait for `this->Ack`
        //
        // and it will emit `uv_cond_signal` to let it stop wait
        uv_cond_wait(&cond, &mutex);

        COMMON_ACTION result = ack_result;
        uv_mutex_unlock(&mutex);

        // write down some debug information
        // while `NODE_ONS_LOG=true`
        if(ack_env_v == "true")
        {
            printf("[%s][-----] finish wait: 0x%lX\n", msg_id.c_str(), (unsigned long)this);
        }

        return result;
    }


private:
    uv_mutex_t mutex;
    uv_cond_t cond;
    COMMON_ACTION ack_result;
    bool acked;

public:
    string msg_id;
};

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

        if(msg_id)
        {
            delete []msg_id;
            msg_id = NULL;
        }

        msg_id = new char[inner->msg_id.size() + 1];
        strcpy(msg_id, inner->msg_id.c_str());
    }

    void Ack(COMMON_ACTION result = COMMON_ACTION::SUCCESS)
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
            if(ack_env_v == "true")
            {
                printf("[%s][---] inner unrefed: 0x%lX\n", msg_id, (unsigned long)inner);
            }

            inner = NULL;
        }
    }

private:
    ONSConsumerACKInner* inner;
    char* msg_id;
};
#endif

/*
 * =====================================================================================
 *
 *       Filename:  consumer_ack.cpp
 *
 *    Description:  ONS consumer ACK wrapper for node.js
 *
 *        Version:  1.0
 *        Created:  2016/01/25 11时37分20秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  XadillaX (ZKD), zhukaidi@souche.com
 *   Organization:  Design & Development Center, Souche Car Service Co., Ltd, HANGZHOU
 *
 * =====================================================================================
 */
#include "consumer_ack.h"

Nan::Persistent<v8::Function> ONSConsumerACKV8::constructor;

std::string ack_env_v = std::getenv("NODE_ONS_LOG") == NULL ?
        "" : std::getenv("NODE_ONS_LOG");

ONSConsumerACKV8::ONSConsumerACKV8() : inner(NULL), msg_id(NULL)
{
}

ONSConsumerACKV8::~ONSConsumerACKV8()
{
    inner = NULL;

    if(msg_id)
    {
        delete []msg_id;
        msg_id = NULL;
    }
}

void ONSConsumerACKV8::Init()
{
    Nan::HandleScope scope;

    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("ONSConsumer").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // set the v8 ack object's prototype `done`
    Nan::SetPrototypeMethod(tpl, "done", Done);

    constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(ONSConsumerACKV8::New)
{
    ONSConsumerACKV8* obj = new ONSConsumerACKV8();
    obj->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(ONSConsumerACKV8::Done)
{
    ONSConsumerACKV8* ack = ObjectWrap::Unwrap<ONSConsumerACKV8>(info.Holder());

    bool succ = true;
    if(info.Length() >= 1)
    {
        succ = (info[0]->IsUndefined() || info[0]->IsNull() || info[0]->ToBoolean()->Value());
    }

    // call v8 ack object's `Ack` function to emit the true `Acker`'s
    // `Ack` function and finish waiting of consume thread
    Action act = succ ? Action::CommitMessage : Action::ReconsumeLater;
    ack->Ack(act);
}

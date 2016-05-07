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

std::string ack_env_v = std::getenv("NODE_ONS_LOG") == NULL ?
        "" : std::getenv("NODE_ONS_LOG");

ONSConsumerACKV8::ONSConsumerACKV8()
{
}

ONSConsumerACKV8::~ONSConsumerACKV8()
{
}

void ONSConsumerACKV8::Done(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    ONSConsumerACKV8* ack = ObjectWrap::Unwrap<ONSConsumerACKV8>(info.Holder());
    bool succ = true;
    if(info.Length() >= 1)
    {
        succ = info[0]->ToBoolean()->Value();
    }

    //printf("Done: %d\n", succ);
    Action act = succ ? Action::CommitMessage : Action::ReconsumeLater;
    ack->Ack(act);
}

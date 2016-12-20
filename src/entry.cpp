/*
 * =====================================================================================
 *
 *       Filename:  entry.cpp
 *
 *    Description:  Entry file for ONS Node client.
 *
 *        Version:  1.0
 *        Created:  2015/12/18 15时35分58秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  XadillaX (ZKD), zhukaidi@souche.com
 *   Organization:  Design & Development Center, Souche Car Service Co., Ltd, HANGZHOU
 *
 * =====================================================================================
 */
#include <nan.h>
#include "consumer.h"
#include "producer.h"
#include "log_util.h"

void InitAll(v8::Local<v8::Object> exports)
{
    ONSConsumerV8::Init(exports);
    ONSProducerV8::Init(exports);

    Nan::Set(exports, Nan::New<v8::String>("setLogPath").ToLocalChecked(),
            Nan::GetFunction(Nan::New<v8::FunctionTemplate>(AliyunONS::SetLogPath)).ToLocalChecked());
}

NODE_MODULE(ons, InitAll)

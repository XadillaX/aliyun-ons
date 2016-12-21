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
 * Copyright (c) 2016 XadillaX <i@2333.moe>
 *
 * MIT LIcense <https://github.com/XadillaX/aliyun-ons/blob/master/LICENSE>
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

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
#include "ons_options.h"
#include "nan.h"

ONSOptions::ONSOptions()
{
    thread_num = -1;
    send_msg_timeout_millis = -1;
    ons_addr = "";
    namesrv_addr = "";
    order = false;
}

ONSOptions::ONSOptions(v8::Local<v8::Value> value) : ONSOptions()
{
    if(value->IsObject())
    {
        Nan::HandleScope scope;

        v8::Local<v8::Object> v8_options = value->ToObject();

        v8::Local<v8::Value> v8_thread_num = Nan::Get(v8_options,
                Nan::New("threadNum").ToLocalChecked()).ToLocalChecked();
        v8::Local<v8::Value> v8_send_msg_timeout_millis = Nan::Get(v8_options,
                Nan::New("sendTimeout").ToLocalChecked()).ToLocalChecked();
        v8::Local<v8::Value> v8_ons_addr = Nan::Get(v8_options,
                Nan::New("onsAddr").ToLocalChecked()).ToLocalChecked();
        v8::Local<v8::Value> v8_namesrv_addr = Nan::Get(v8_options,
                Nan::New("namesrvAddr").ToLocalChecked()).ToLocalChecked();
        v8::Local<v8::Value> v8_order = Nan::Get(v8_options,
                Nan::New("order").ToLocalChecked()).ToLocalChecked();

        if(v8_thread_num->IsInt32())
        {
            thread_num = Nan::To<int32_t>(v8_thread_num).FromJust();
        }

        if(v8_send_msg_timeout_millis->IsInt32())
        {
            send_msg_timeout_millis = Nan::To<int32_t>(v8_send_msg_timeout_millis).FromJust();
        }

        if(v8_ons_addr->IsString())
        {
            ons_addr = *(v8::String::Utf8Value(v8_ons_addr));
        }

        if(v8_namesrv_addr->IsString())
        {
            namesrv_addr = *(v8::String::Utf8Value(v8_namesrv_addr));
        }

        order = Nan::To<bool>(v8_order).FromJust();
    }
}

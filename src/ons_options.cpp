/*
 * =====================================================================================
 *
 *       Filename:  ons_options.cpp
 *
 *    Description:  ons options
 *
 *        Version:  1.0
 *        Created:  2016/06/08 15时11分15秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  XadillaX (ZKD), zhukaidi@souche.com
 *   Organization:  Design & Development Center, Souche Car Service Co., Ltd, HANGZHOU
 *
 * =====================================================================================
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
            thread_num = v8_thread_num->ToInt32()->Value();
        }

        if(v8_send_msg_timeout_millis->IsInt32())
        {
            send_msg_timeout_millis = v8_send_msg_timeout_millis->ToInt32()->Value();
        }

        if(v8_ons_addr->IsString())
        {
            ons_addr = *(v8::String::Utf8Value(v8_ons_addr));
        }

        if(v8_namesrv_addr->IsString())
        {
            namesrv_addr = *(v8::String::Utf8Value(v8_namesrv_addr));
        }

        order = v8_order->ToBoolean()->IsTrue();
    }
}

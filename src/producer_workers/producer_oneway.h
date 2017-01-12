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
 * Copyright (c) 2017 XadillaX <i@2333.moe>
 *
 * MIT LIcense <https://github.com/XadillaX/aliyun-ons/blob/master/LICENSE>
 */
#ifndef __PRODUCER_ONEWAY_H__
#define __PRODUCER_ONEWAY_H__
#include "../real_producer_wrapper.h"
#include "../producer.h"
using namespace std;

struct PrdrOneWayParam
{
    ONSProducerV8* ons;
    string topic;
    string tags;
    string key;
    string content;
    int64_t send_at;

    PrdrOneWayParam(ONSProducerV8& _ons, string _topic, string _tags, string _key, string _content, int64_t _send_at) :
        ons(&_ons), topic(_topic), tags(_tags), key(_key), content(_content), send_at(_send_at)
    {
    }
};

inline void PrdrSendingOneWay(uv_work_t* req)
{
    PrdrOneWayParam* param = static_cast<PrdrOneWayParam*>(req->data);
    ONSProducerV8* ons = param->ons;

    uv_mutex_lock(&ons->mutex);
    if(!ons->inited || !ons->started || !ons->real_producer)
    {
        uv_mutex_unlock(&ons->mutex);
        return;
    }

    Message msg(param->topic.c_str(), param->tags.c_str(), param->content.c_str());
    if(param->key != "") msg.setKey(param->key.c_str());
    if(param->send_at != -1) msg.setStartDeliverTime(param->send_at);
    ONSRealProducerWrapper* real_producer = ons->real_producer;
    try
    {
        real_producer->SendOneway(msg);
    }
    catch(const ONSClientException& e)
    {
        uv_mutex_unlock(&ons->mutex);
        return;
    }

    uv_mutex_unlock(&ons->mutex);
}

inline void PrdrSentOneWay(uv_work_t* req)
{
    PrdrOneWayParam* param = static_cast<PrdrOneWayParam*>(req->data);
    delete param;
    delete req;
}

void PrdrSendOneWay(ONSProducerV8& _ons, string _topic, string _tags, string _key, string _content, int64_t _send_at)
{
    uv_work_t* request = new uv_work_t();
    request->data = new PrdrOneWayParam(_ons, _topic, _tags, _key, _content, _send_at);
    uv_queue_work(
            uv_default_loop(),
            request,
            PrdrSendingOneWay,
            reinterpret_cast<uv_after_work_cb>(PrdrSentOneWay));
}
#endif

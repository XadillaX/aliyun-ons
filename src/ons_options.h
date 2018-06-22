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
#ifndef __ONS_OPTIONS_H__
#define __ONS_OPTIONS_H__

#include <string>
#include <v8.h>
using namespace std;

class ONSOptions
{
public:
    int thread_num;
    int send_msg_timeout_millis;
    string ons_addr;
    string namesrv_addr;
    bool order;

    ONSOptions();
    ONSOptions(v8::Local<v8::Value> value); 
};
#endif

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
#include <ONSFactory.h>
#include "log_util.h"

namespace AliyunONS {

std::string log_path = "";

NAN_METHOD(SetLogPath)
{
    if(info.Length() < 1)
    {
        return Nan::ThrowError("You should specified a log path.");
    }

    v8::String::Utf8Value path(info[0]->ToString());
    log_path = *path;
}

std::string GetLogPath()
{
    return log_path;
}

}

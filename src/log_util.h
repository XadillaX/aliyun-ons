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
#ifndef __LOG_UTIL_H__
#define __LOG_UTIL_H__
#include <string>
#include <nan.h>

namespace AliyunONS {

NAN_METHOD(SetLogPath);
std::string GetLogPath();

}

#endif

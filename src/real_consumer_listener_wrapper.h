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
#ifndef __REAL_CONSUMER_LISTENER_WRAPPER_H__
#define __REAL_CONSUMER_LISTENER_WRAPPER_H__

class ONSConsumerV8;

class ONSRealConsumerListenerWrapper {
public:
    ONSRealConsumerListenerWrapper(ONSConsumerV8* parent, bool order);
    ~ONSRealConsumerListenerWrapper();

    bool IsOrder();
    void* GetListener();

private:
    void* real_listener;
    bool order;
};

#endif

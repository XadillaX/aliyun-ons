/*
 * =====================================================================================
 *
 *       Filename:  consumer_listener.cpp
 *
 *    Description:  Consumer listener class and function
 *
 *        Version:  1.0
 *        Created:  2016/07/13 15时15分32秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  XadillaX (zkd), i@2333.moe
 *   Organization:  Design & Development Center, Souche Inc. CHINA
 *
 * =====================================================================================
 */
#include "consumer.h"
#include "consumer_ack.h"
#include "consumer_listener.h"

std::string consumer_listener_env_v = std::getenv("NODE_ONS_LOG") == NULL ?
        "" : std::getenv("NODE_ONS_LOG");

Action ONSListenerV8::consume(Message& message, ConsumeContext& context)
{
    ONSConsumerACKInner* ack_inner = new ONSConsumerACKInner();
    MessageHandlerParam* param = new MessageHandlerParam();
    param->message = &message;

    if(consumer_listener_env_v == "true")
    {
        printf(">>> ACK Inner Created: 0x%lX\n", (unsigned long)ack_inner);
        printf(">>> Message Handler Param Created: 0x%lX\n", (unsigned long)param);
    }

    param->ons = parent;
    param->ack_inner = ack_inner;
    async->data = (void*)param;
    uv_async_send(async);

    Action result = ack_inner->WaitResult();

    delete ack_inner;
    delete param;

    if(consumer_listener_env_v == "true")
    {
        printf(">>> ACK Inner Deleted: 0x%lX\n", (unsigned long)ack_inner);
        printf(">>> Message Handler Param Deleted: 0x%lX\n", (unsigned long)param);
    }

    return result;
}

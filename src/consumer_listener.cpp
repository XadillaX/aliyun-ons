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

ONSListenerV8::ONSListenerV8(ONSConsumerV8* parent) : parent(parent)
{
    uv_mutex_init(&mutex);

    // create `uv_async_pool`
    thread_num = parent->factory_info.getConsumeThreadNums();
    if(consumer_listener_env_v == "true")
    {
        printf("[-] consume thread count: %d\n", thread_num);
    }

    async_pool = new uv_async_t[thread_num];
    for(unsigned int i = 0; i < thread_num; i++)
    {
        uv_async_init(uv_default_loop(), async_pool + i, parent->HandleMessage);
        idle_async.push(async_pool + i);
    }
}

ONSListenerV8::~ONSListenerV8()
{
    uv_mutex_lock(&mutex);
    while(!idle_async.empty())
    {
        idle_async.pop();
    }
    uv_mutex_unlock(&mutex);

    for(unsigned int i = 0; i < thread_num; i++)
    {
        uv_close((uv_handle_t*)(async_pool + i), NULL);
    }
    delete []async_pool;

    uv_mutex_destroy(&mutex);
}

uv_async_t* ONSListenerV8::GetAsync()
{
    uv_async_t* async;
    uv_mutex_lock(&mutex);

    if(idle_async.empty())
    {
        uv_mutex_unlock(&mutex);
        return NULL;
    }

    async = idle_async.front();
    idle_async.pop();

    uv_mutex_unlock(&mutex);

    return async;
}

void ONSListenerV8::RestoreAsync(uv_async_t* async)
{
    uv_mutex_lock(&mutex);
    idle_async.push(async);
    uv_mutex_unlock(&mutex);
}

Action ONSListenerV8::consume(Message& message, ConsumeContext& context)
{
    const char* msg_id = message.getMsgID();

    ONSConsumerACKInner* ack_inner = new ONSConsumerACKInner(msg_id);
    MessageHandlerParam* param = new MessageHandlerParam();
    param->message = &message;
    param->ons = parent;
    param->ack_inner = ack_inner;

    // log some debug information
    // if `NODE_ONS_LOG=true`
    if(consumer_listener_env_v == "true")
    {
        printf("[%s][---] ack inner created: 0x%lX\n", msg_id, (unsigned long)ack_inner);
        printf("[%s][---] message handler param created: 0x%lX\n", msg_id, (unsigned long)param);
    }

    // create a uv_async_t
    //
    // because of the unsafe feature of uv_async_t::data, we create a uv_async_t
    // for each call of this function and destroy it before function done.
    uv_async_t* async = GetAsync();
    if(!async)
    {
        delete ack_inner;
        delete param;

        if(consumer_listener_env_v == "true")
        {
            printf("[%s][----] failed to get async object\n", msg_id);
            printf("[%s][---] ack inner deleted: 0x%lX\n", msg_id, (unsigned long)ack_inner);
            printf("[%s][---] message handler parameter deleted: 0x%lX\n", msg_id, (unsigned long)param);
        }

        return Action::ReconsumeLater;
    }

    if(consumer_listener_env_v == "true")
    {
        printf("[%s][----] async object 0x%lX got\n", msg_id, (unsigned long)async);
    } 

    async->data = (void*)param;
    uv_async_send(async);

    Action result = ack_inner->WaitResult();

    async->data = NULL;
    param->ons = NULL;
    param->ack_inner = NULL;
    param->message = NULL;

    delete ack_inner;
    delete param;

    // log some debug information
    // if `NODE_ONS_LOG=true`
    if(consumer_listener_env_v == "true")
    {
        printf("[%s][---] ack inner deleted: 0x%lX\n", msg_id, (unsigned long)ack_inner);
        printf("[%s][---] message handler parameter deleted: 0x%lX\n", msg_id, (unsigned long)param);
    }

    // restore `uv_async_t` object
    RestoreAsync(async);

    return result;
}

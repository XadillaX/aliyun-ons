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
    ONSConsumerACKInner* ack_inner = new ONSConsumerACKInner();
    MessageHandlerParam* param = new MessageHandlerParam();

    // log some debug information
    // if `NODE_ONS_LOG=true`
    if(consumer_listener_env_v == "true")
    {
        printf("[---] ack inner created: 0x%lX\n", (unsigned long)ack_inner);
        printf("[---] message handler param created: 0x%lX\n", (unsigned long)param);
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
            printf("[----] failed to get async object\n");
            printf("[---] ack inner deleted: 0x%lX\n", (unsigned long)ack_inner);
            printf("[---] message handler parameter deleted: 0x%lX\n", (unsigned long)param);
        }

        return Action::ReconsumeLater;
    }

    if(consumer_listener_env_v == "true")
    {
        printf("[----] async object 0x%lX got\n", (unsigned long)async);
    }

    param->message = &message;
    param->ons = parent;
    param->ack_inner = ack_inner;

    async->data = (void*)param;
    uv_async_send(async);

    Action result = ack_inner->WaitResult();

    param->ons = NULL;
    param->ack_inner = NULL;
    param->message = NULL;

    async->data = NULL;

    delete ack_inner;
    delete param;

    // log some debug information
    // if `NODE_ONS_LOG=true`
    if(consumer_listener_env_v == "true")
    {
        printf("[---] ack inner deleted: 0x%lX\n", (unsigned long)ack_inner);
        printf("[---] message handler parameter deleted: 0x%lX\n", (unsigned long)param);
    }

    // restore `uv_async_t` object
    RestoreAsync(async);

    return result;
}

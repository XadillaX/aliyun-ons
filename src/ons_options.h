/*
 * =====================================================================================
 *
 *       Filename:  ons_options.h
 *
 *    Description:  ONS options
 *
 *        Version:  1.0
 *        Created:  2016/06/08 15时06分29秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  XadillaX (ZKD), zhukaidi@souche.com
 *   Organization:  Design & Development Center, Souche Car Service Co., Ltd, HANGZHOU
 *
 * =====================================================================================
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

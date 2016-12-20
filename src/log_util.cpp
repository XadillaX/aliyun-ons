/*
 * =====================================================================================
 *
 *       Filename:  log_util.cpp
 *
 *    Description:  log util body
 *
 *        Version:  1.0
 *        Created:  2016/09/10 18时38分44秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  XadillaX (zkd), i@2333.moe
 *   Organization:  Design & Development Center, Souche Inc. CHINA
 *
 * =====================================================================================
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

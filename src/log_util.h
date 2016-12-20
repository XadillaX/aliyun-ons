/*
 * =====================================================================================
 *
 *       Filename:  log_util.h
 *
 *    Description:  log util header
 *
 *        Version:  1.0
 *        Created:  2016/09/10 18时33分46秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  XadillaX (zkd), i@2333.moe
 *   Organization:  Design & Development Center, Souche Inc. CHINA
 *
 * =====================================================================================
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

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
#include <uv.h>
#include "log_util.h"

#ifndef WIN32
#include <unistd.h>
#include <sole.hpp>
#else
#include <io.h>
#endif

std::string log_util_env_v = std::getenv("NODE_ONS_LOG") == NULL ?
        "" : std::getenv("NODE_ONS_LOG");

uv_mutex_t m;
int ______ = uv_mutex_init(&m);
bool __log_redirected = false;
bool __log_resumed = false;

void ONSStartRedirectStd(int* fd, string* u4)
{
    uv_mutex_lock(&m);
    if(__log_redirected)
    {
        *fd = 0;
        *u4 = "";
        uv_mutex_unlock(&m);
        return;
    }

#ifdef WIN32
    // windows has no log console
    *u4 = "";
#else
    if(log_util_env_v == "true")
    {
        printf("[.] ./ons-%s.log redirected", u4->c_str());
    }
    *u4 = sole::uuid4().str();
    *fd = dup(STDOUT_FILENO);
    freopen(("./.ons-" + *u4 + ".log").c_str(), "w", stdout);
    __log_redirected = true;
#endif
    uv_mutex_unlock(&m);
}

void ONSStartResumeStd(int fd)
{
    uv_mutex_lock(&m);

    if(__log_resumed)
    {
        uv_mutex_unlock(&m);
        return;
    }

#ifdef WIN32
    // windows has no log console
#else
    if(log_util_env_v == "true")
    {
        printf("[.] stdout resumed");
    }

    fclose(stdout);
    dup2(fd, STDOUT_FILENO);
    stdout = fdopen(STDOUT_FILENO, "w");
    close(fd);
    __log_resumed = true;
#endif
    uv_mutex_unlock(&m);
}

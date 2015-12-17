/********************************************************************
author:     здао jianlin.yjl@alibaba-cn.com
*********************************************************************/
#ifndef __ONSCLIENT_H__
#define  __ONSCLIENT_H__

#include <string>

#ifdef WIN32
    typedef const char* CONSTSTRING;
    typedef char* STRING;
#else
    typedef const std::string CONSTSTRING;
    typedef std::string STRING;
#endif


#ifdef WIN32
#ifdef ONSCLIENT_EXPORTS
#define ONSCLIENT_API __declspec(dllexport)
#else
#define ONSCLIENT_API __declspec(dllimport)
#endif
#else
#define ONSCLIENT_API
#endif

#endif

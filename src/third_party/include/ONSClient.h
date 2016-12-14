#ifndef __ONSCLIENT_H__
#define __ONSCLIENT_H__

#ifdef WIN32
#ifdef ONSCLIENT_EXPORTS

#ifndef SWIG
#define ONSCLIENT_API __declspec(dllexport)
#else
#define ONSCLIENT_API
#endif

#else
#define ONSCLIENT_API __declspec(dllimport)
#endif
#else
#define ONSCLIENT_API
#endif

#endif

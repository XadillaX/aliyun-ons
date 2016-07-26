#ifndef __ONSCLIENTEXCEPTION_H__
#define __ONSCLIENTEXCEPTION_H__

#include <exception>
#include "ONSClient.h"

namespace ons{

class ONSCLIENT_API ONSClientException: public std::exception
{
    public:
        ONSClientException() throw();
        virtual ~ONSClientException() throw();
        ONSClientException(std::string msg, int error) throw();
        const char* GetMsg() const throw();
        const char* what() const throw();
        int GetError() const throw();
        
    private:
        std::string  m_msg;
        int             m_error;
};

}


#endif

#ifndef __PUSHCONSUMER_H__
#define __PUSHCONSUMER_H__

#include "MessageListener.h"

namespace ons {

class ONSCLIENT_API PushConsumer{
        
public:
    PushConsumer(){} 
    virtual ~PushConsumer(){}
  
    virtual void start()=0;
    virtual void shutdown()=0;
    virtual void subscribe(CONSTSTRING topic, CONSTSTRING subExpression, MessageListener* listener)=0;
    //virtual void setNamesrvAddr(const std::string& nameSrvAddr) = 0;
};

}
#endif


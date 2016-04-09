#ifndef __PRODUCER_H__
#define __PRODUCER_H__


#include "SendResultONS.h"
#include "Message.h"


namespace ons {

class ONSCLIENT_API Producer{
        
public:
  
    Producer(){}
    virtual ~Producer(){}
    
    //before send msg, start must be called to allocate resources.
    virtual void start()=0;
    //before exit ons, shutdown must be called to release all resources allocated by ons internally.
    virtual void shutdown()=0;
    //retry max 3 times if send failed. if no exception throwed, it sends success; 
    virtual SendResultONS send(Message& msg)=0;

    //virtual void setSendMsgTimeout(int value) = 0;
    //virtual void setNamesrvAddr(const std::string& nameSrvAddr) = 0;
};

}
#endif


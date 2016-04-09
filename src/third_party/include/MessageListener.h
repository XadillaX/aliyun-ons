#ifndef __MESSAGELISTENER_H__
#define __MESSAGELISTENER_H__

#include "Action.h"
#include "Message.h"
#include "ConsumeContext.h"

namespace ons{

class ONSCLIENT_API MessageListener {
public:
    MessageListener(){}
    virtual ~MessageListener(){}
    
    //interface of consuming message, should be realized by application
    virtual Action consume(Message message, ConsumeContext context) = 0;
};

}
#endif

#ifndef __SENDRESULTONS_H__
#define __SENDRESULTONS_H__

#include "ONSClient.h"

namespace ons{

class ONSCLIENT_API SendResultONS{
public:
    SendResultONS();
    virtual ~SendResultONS();
    void setMessageId(const std::string& msgId);
    CONSTSTRING getMessageId() const;


private:
    std::string messageId;

};

}
#endif

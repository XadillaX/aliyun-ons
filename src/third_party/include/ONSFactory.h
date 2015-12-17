#ifndef __ONSFACTORY_H_
#define __ONSFACTORY_H_

#include "Producer.h"
//#include "PullConsumer.h"
#include "PushConsumer.h"
#include "ONSChannel.h"

namespace ons {

class ONSCLIENT_API ONSFactoryProperty{
public:
    ONSFactoryProperty();
    virtual ~ONSFactoryProperty();
    bool checkValidityOfFactoryProperties(const std::string& key, const std::string& value);
    void setSendMsgTimeout(const int value);
    void setOnsChannel(ONSChannel onsChannel);
    void setFactoryProperty(CONSTSTRING& key, CONSTSTRING value);
    void setFactoryProperties(std::map<std::string, std::string> factoryProperties);
    std::map<std::string, std::string> getFactoryProperties() const;
    CONSTSTRING getProducerId() const;
    CONSTSTRING getConsumerId() const;
    CONSTSTRING getPublishTopics() const;
    CONSTSTRING getMessageModel() const;
    const int getMessageTimeout() const;
    const int getConsumeThreadNums() const;
    const ONSChannel  getOnsChannel() const;
    CONSTSTRING  getChannel() const;
    CONSTSTRING getMessageContent() const;
    CONSTSTRING getNameSrvAddr() const;
    CONSTSTRING getNameSrvDomain() const;
    CONSTSTRING getAccessKey() const;
    CONSTSTRING getSecretKey() const;
public:
    static CONSTSTRING ProducerId;
    static CONSTSTRING ConsumerId;
    static CONSTSTRING PublishTopics;
    static CONSTSTRING MsgContent;
    static CONSTSTRING ONSAddr;
    static CONSTSTRING AccessKey;
    static CONSTSTRING SecretKey;
    static CONSTSTRING MessageModel;
    static CONSTSTRING BROADCASTING;
    static CONSTSTRING CLUSTERING;
    static CONSTSTRING SendMsgTimeoutMillis;
    static CONSTSTRING NAMESRV_ADDR;
    static CONSTSTRING ConsumeThreadNums;
    static CONSTSTRING OnsChannel;
private:    
    std::map<std::string, std::string> m_onsFactoryProperties;    
};   

class ONSCLIENT_API ONSFactoryAPI{
public:
    ONSFactoryAPI();
    virtual ~ONSFactoryAPI();

    virtual Producer*     createProducer(ONSFactoryProperty factoryProperty);
    //virtual PullConsumer* createPullConsumer(ONSFactoryProperty factoryProperty);
    virtual PushConsumer* createPushConsumer(ONSFactoryProperty factoryProperty);
};

class ONSCLIENT_API ONSFactory{
public:
    virtual ~ONSFactory();
    static ONSFactoryAPI* getInstance(); 
private:
     ONSFactory();
     static ONSFactoryAPI *onsFactoryInstance;
};
}
#endif


#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <map>
#include <vector>
#include <sstream>
#include "ONSClient.h"

namespace ons {

    class ONSCLIENT_API SystemPropKey
    {
        public:
            SystemPropKey(){}
            ~SystemPropKey(){}
            static CONSTSTRING TAG;
            static CONSTSTRING KEY;
            static CONSTSTRING MSGID;
            static CONSTSTRING RECONSUMETIMES;
            static CONSTSTRING STARTDELIVERTIME;
    };

    //<!***************************************************************************
    class ONSCLIENT_API Message
    {        
    public:
        Message();
        Message(CONSTSTRING topic, CONSTSTRING tags, CONSTSTRING body);
        Message(CONSTSTRING topic, CONSTSTRING tags, CONSTSTRING keys, CONSTSTRING body);

        virtual ~Message();
        
        Message(const Message& other);
        Message& operator=(const Message& other);

        //userProperties was used to save user specific parameters which doesn't belong to SystemPropKey
        void   putUserProperties(CONSTSTRING key, CONSTSTRING value);
        CONSTSTRING getUserProperties(CONSTSTRING key) const;
        void   setUserProperties(std::map<std::string, std::string>& userProperty);
        std::map<std::string, std::string> getUserProperties() const;

       //systemProperties only save parameters defined in SystemPropKey, please do not add other parameters into systemProperties, else it was not saved.
        void   putSystemProperties(CONSTSTRING key,CONSTSTRING value);
        CONSTSTRING getSystemProperties(CONSTSTRING key) const;
        void   setSystemProperties(std::map<std::string, std::string>& systemProperty);
        std::map<std::string, std::string> getSystemProperties() const;
        
        CONSTSTRING  getTopic()const;
        void    setTopic(CONSTSTRING topic);
    
        CONSTSTRING  getTag() const;
        void    setTag(CONSTSTRING tags);
    
        CONSTSTRING  getKey() const;
        void    setKey(CONSTSTRING keys);

        CONSTSTRING getMsgID() const;
        void   setMsgID(CONSTSTRING msgId);
    
        const long   getStartDeliverTime() const;
        void   setStartDeliverTime(long level);
    
        CONSTSTRING getBody()const;
        void   setBody(CONSTSTRING msgbody);
    
        const int    getReconsumeTimes() const;
        void  setReconsumeTimes(int reconsumeTimes);
        
        const std::string toString() const;

        const std::string toSystemString() const;

        const std::string toUserString() const;
    
    protected:
        void Init(const std::string& topic,
                  const std::string& tags,
                  const std::string& keys,
                  const std::string& body);
    
    
    private:
        std::string  topic;
        std::string  body;
        std::map<std::string, std::string> systemProperties;
        std::map<std::string, std::string> userProperties;
    };
    //<!***************************************************************************
    

}//<!end namespace;
#endif


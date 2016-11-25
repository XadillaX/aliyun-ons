#ifndef __ONSFACTORY_H_
#define __ONSFACTORY_H_

#include "LocalTransactionChecker.h"
#include "ONSChannel.h"
#include "OrderConsumer.h"
#include "OrderProducer.h"
#include "Producer.h"
#include "PushConsumer.h"
#include "TransactionProducer.h"

namespace ons {
class ONSCLIENT_API ONSFactoryProperty {
 public:
  ONSFactoryProperty();
  virtual ~ONSFactoryProperty();
  bool checkValidityOfFactoryProperties(const std::string& key,
                                        const std::string& value);
  void setSendMsgTimeout(const int value);
  void setSendMsgRetryTimes(const int value);
  void setMaxMsgCacheSize(const int size);
  void setOnsTraceSwitch(bool onswitch);
  void setOnsChannel(ONSChannel onsChannel);
  void setFactoryProperty(CONSTSTRING& key, CONSTSTRING value);
  void setFactoryProperties(
      std::map<std::string, std::string> factoryProperties);
  std::map<std::string, std::string> getFactoryProperties() const;
  CONSTSTRING getProducerId() const;
  CONSTSTRING getConsumerId() const;
  CONSTSTRING getPublishTopics() const;
  CONSTSTRING getMessageModel() const;
  const int getSendMsgTimeout() const;
  const int getSendMsgRetryTimes() const;
  const int getConsumeThreadNums() const;
  const int getMaxMsgCacheSize() const;
  const ONSChannel getOnsChannel() const;
  CONSTSTRING getChannel() const;
  CONSTSTRING getMessageContent() const;
  CONSTSTRING getNameSrvAddr() const;
  CONSTSTRING getNameSrvDomain() const;
  CONSTSTRING getAccessKey() const;
  CONSTSTRING getSecretKey() const;
  bool getOnsTraceSwitch() const;
  
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
  static CONSTSTRING MaxMsgCacheSize;
  static CONSTSTRING OnsTraceSwitch;
  static CONSTSTRING SendMsgRetryTimes;

 private:
  std::map<std::string, std::string> m_onsFactoryProperties;
};

class ONSCLIENT_API ONSFactoryAPI {
 public:
  ONSFactoryAPI();
  virtual ~ONSFactoryAPI();

  virtual Producer* createProducer(ONSFactoryProperty factoryProperty);
  virtual OrderProducer* createOrderProducer(
      ONSFactoryProperty factoryProperty);
  virtual OrderConsumer* createOrderConsumer(
      ONSFactoryProperty factoryProperty);
  virtual TransactionProducer* createTransactionProducer(
      ONSFactoryProperty factoryProperty, LocalTransactionChecker* checker);
  virtual PushConsumer* createPushConsumer(ONSFactoryProperty factoryProperty);
};

class ONSCLIENT_API ONSFactory {
 public:
  virtual ~ONSFactory();
  static ONSFactoryAPI* getInstance();

 private:
  ONSFactory();
  static ONSFactoryAPI* onsFactoryInstance;
};
}
#endif

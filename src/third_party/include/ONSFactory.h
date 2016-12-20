#ifndef __ONSFACTORY_H_
#define __ONSFACTORY_H_

#include "LocalTransactionChecker.h"
#include "ONSChannel.h"
#include "OrderConsumer.h"
#include "OrderProducer.h"
#include "Producer.h"
#include "PushConsumer.h"
#include "TransactionProducer.h"
#include "ONSClientException.h"

namespace ons {
class  ONSCLIENT_API ONSFactoryProperty {
 public:
  ONSFactoryProperty();
  virtual ~ONSFactoryProperty();
  bool checkValidityOfFactoryProperties(const std::string& key,
                                        const std::string& value) throw(ons::ONSClientException);
  const char* getLogPath();
  void setSendMsgTimeout(const int value);
  void setSendMsgRetryTimes(const int value);
  void setMaxMsgCacheSize(const int size);
  void setOnsTraceSwitch(bool onswitch);
  void setOnsChannel(ONSChannel onsChannel) throw(ons::ONSClientException);
  void setFactoryProperty(const char* key, const char* value) throw(ons::ONSClientException);
  void setFactoryProperties(
      std::map<std::string, std::string> factoryProperties);
  std::map<std::string, std::string> getFactoryProperties() const;
	const char* getProducerId() const;
	const char* getConsumerId() const;
	const char* getPublishTopics() const;
	const char* getMessageModel() const;
  const int getSendMsgTimeout() const;
  const int getSendMsgRetryTimes() const;
  const int getConsumeThreadNums() const;
  const int getMaxMsgCacheSize() const;
  const ONSChannel getOnsChannel() const;
	const char* getChannel() const;
	const char* getMessageContent() const;
	const char* getNameSrvAddr() const;
	const char* getNameSrvDomain() const;
	const char* getAccessKey() const;
	const char* getSecretKey() const;
  bool getOnsTraceSwitch() const;
  
 public:
  static const char* LogPath;
  static const char* ProducerId;
  static const char* ConsumerId;
  static const char* PublishTopics;
  static const char* MsgContent;
  static const char* ONSAddr;
  static const char* AccessKey;
  static const char* SecretKey;
  static const char* MessageModel;
  static const char* BROADCASTING;
  static const char* CLUSTERING;
  static const char* SendMsgTimeoutMillis;
  static const char* NAMESRV_ADDR;
  static const char* ConsumeThreadNums;
  static const char* OnsChannel;
  static const char* MaxMsgCacheSize;
  static const char* OnsTraceSwitch;
  static const char* SendMsgRetryTimes;

 private:
  std::map<std::string, std::string> m_onsFactoryProperties;
};

class  ONSCLIENT_API ONSFactoryAPI {
 public:
  ONSFactoryAPI();
  virtual ~ONSFactoryAPI();

  virtual ons::Producer* createProducer(ons::ONSFactoryProperty factoryProperty) throw(ons::ONSClientException);
  virtual ons::OrderProducer* createOrderProducer(
      ons::ONSFactoryProperty factoryProperty) throw(ons::ONSClientException);
  virtual ons::OrderConsumer* createOrderConsumer(
      ons::ONSFactoryProperty factoryProperty) throw(ons::ONSClientException);
  virtual ons::TransactionProducer* createTransactionProducer(
      ons::ONSFactoryProperty factoryProperty, ons::LocalTransactionChecker* checker) throw(ons::ONSClientException);
  virtual ons::PushConsumer* createPushConsumer(ons::ONSFactoryProperty factoryProperty) throw(ons::ONSClientException);
};

class  ONSCLIENT_API ONSFactory {
 public:
  virtual ~ONSFactory();
  static ons::ONSFactoryAPI* getInstance();

 private:
  ONSFactory();
  static ons::ONSFactoryAPI* onsFactoryInstance;
};
}
#endif

#ifndef __PULLCONSUMER_H__
#define __PULLCONSUMER_H__

#include <string>
#include <vector>
#include "MQMessageQueue.h"
#include "PullResultONS.h"

namespace ons {

class ONSFactoryProperty;
class MQMessageQueue;

class ONSCLIENT_API PullConsumer {
 public:
  PullConsumer() {}
  virtual ~PullConsumer() {}

  virtual void start() = 0;
  virtual void shutdown() = 0;
  virtual void fetchSubscribeMessageQueues(
      const std::string& topic, std::vector<MQMessageQueue>& mqs) = 0;
  virtual PullResultONS pull(const MQMessageQueue& mq,
                             const std::string& subExpression, long long offset,
                             int maxNums) = 0;
};
}
#endif

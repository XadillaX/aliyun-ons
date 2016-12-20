#ifndef __ORDERCONSUMER_H__
#define __ORDERCONSUMER_H__

#include "MessageOrderListener.h"

namespace ons {

class  ONSCLIENT_API OrderConsumer {
 public:
  OrderConsumer() {}
  virtual ~OrderConsumer() {}

  virtual void start() = 0;
  virtual void shutdown() = 0;
  virtual void subscribe(const char* topic, const char* subExpression,
                         MessageOrderListener* listener) = 0;
};
}
#endif

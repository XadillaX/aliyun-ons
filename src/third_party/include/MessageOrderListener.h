#ifndef __MESSAGEORDERLYLISTENER_H__
#define __MESSAGEORDERLYLISTENER_H__

#include "ConsumeOrderContext.h"
#include "Message.h"
#include "OrderAction.h"

namespace ons {

class  MessageOrderListener {
 public:
  MessageOrderListener() {}
  virtual ~MessageOrderListener() {}

  // interface of consuming message, should be realized by application
  virtual OrderAction consume(Message& message,
                              ConsumeOrderContext& context) = 0;
};
}
#endif

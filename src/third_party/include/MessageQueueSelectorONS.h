#ifndef _MESSAGEQUEUESELECTOR_H_
#define _MESSAGEQUEUESELECTOR_H_

#include "Message.h"
#include "MessageQueueONS.h"

namespace ons {

class MessageQueueSelectorONS {
 public:
  virtual ~MessageQueueSelectorONS() {}
  virtual MessageQueueONS select(const vector<MessageQueueONS>& mqs,
                                 const Message& msg, void* arg) = 0;
};

}  //<!end namespace;
#endif  //<! _MQSELECTOR_H_

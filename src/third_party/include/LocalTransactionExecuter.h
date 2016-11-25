#ifndef __LOCALTRANSACTIONEXECUTER_H__
#define __LOCALTRANSACTIONEXECUTER_H__

#include "Message.h"
#include "TransactionStatus.h"

namespace ons {
class LocalTransactionExecuter {
 public:
  virtual TransactionStatus execute(Message& msg) = 0;
  virtual ~LocalTransactionExecuter() {}
};
}

#endif

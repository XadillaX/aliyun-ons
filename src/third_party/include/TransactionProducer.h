#ifndef __TRANSACTIONPRODUCER_H__
#define __TRANSACTIONPRODUCER_H__

#include "LocalTransactionExecuter.h"
#include "ONSClient.h"

namespace ons {
class ONSCLIENT_API TransactionProducer {
 public:
  TransactionProducer() {}
  virtual ~TransactionProducer() {}

  // before send msg, start must be called to allocate resources.
  virtual void start() = 0;
  // before exit ons, shutdown must be called to release all resources allocated
  // by ons internally.
  virtual void shutdown() = 0;
  // retry max 3 times if send failed. if no exception throwed, it sends
  // success;
  virtual SendResultONS send(Message& msg,
                             LocalTransactionExecuter* executer) = 0;
};
}

#endif

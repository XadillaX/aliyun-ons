#ifndef __TRANSACTIONSTATUS_H__
#define __TRANSACTIONSTATUS_H__

namespace ons {

enum TransactionStatus {
  CommitTransaction = 0,
  RollbackTransaction = 1,
  Unknow = 2,
};
}

#endif

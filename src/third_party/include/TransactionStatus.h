#ifndef __TRANSACTIONSTATUS_H__
#define __TRANSACTIONSTATUS_H__

namespace ons {

enum TransactionStatus  {
    CommitTransaction=0,//提交事务
    RollbackTransaction=1,//回滚事务
    Unknow=2,//未知状态，一般在用户无法确定事务是成功还是失败时使用
};


}

#endif

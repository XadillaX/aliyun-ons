#ifndef __TRANSACTIONSTATUS_H__
#define __TRANSACTIONSTATUS_H__

namespace ons {

enum TransactionStatus  {
    CommitTransaction=0,//Ìá½»ÊÂÎñ
    RollbackTransaction=1,//»Ø¹öÊÂÎñ
    Unknow=2,//Î´Öª×´Ì¬£¬Ò»°ãÔÚÓÃ»§ÎÞ·¨È·¶¨ÊÂÎñÊÇ³É¹¦»¹ÊÇÊ§°ÜÊ±Ê¹ÓÃ
};


}

#endif

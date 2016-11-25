#ifndef __ONSCALLBACK_H__
#define __ONSCALLBACK_H__

#include "ONSClientException.h"
#include "SendResultONS.h"

namespace ons {
class SendCallbackONS {
 public:
  virtual ~SendCallbackONS() {}
  virtual void onSuccess(SendResultONS& sendResult) = 0;
  virtual void onException(ONSClientException& e) = 0;
};

}  // end of namespace SendResultONS
#endif  // end of _SENDCALLBACK_H_

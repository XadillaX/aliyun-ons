#ifndef __SENDRESULTONS_H__
#define __SENDRESULTONS_H__
#include <string>
#include "ONSClient.h"

namespace ons {

class  ONSCLIENT_API SendResultONS {
 public:
  SendResultONS();
  virtual ~SendResultONS();
  void setMessageId(const std::string& msgId);
	const char* getMessageId() const;

 private:
  std::string messageId;
};
}
#endif

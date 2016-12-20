#ifndef __PULLRESULTONS_H__
#define __PULLRESULTONS_H__

#include <vector>
#include "Message.h"
#include "ONSClient.h"

namespace ons {

enum ONSPullStatus {
  ONS_FOUND,
  ONS_NO_NEW_MSG,
  ONS_NO_MATCHED_MSG,
  ONS_OFFSET_ILLEGAL,
  ONS_BROKER_TIMEOUT  // indicate pull request timeout or received NULL response
};

class ONSCLIENT_API PullResultONS {
 public:
  PullResultONS(ONSPullStatus status)
      : pullStatus(status), nextBeginOffset(0), minOffset(0), maxOffset(0) {}

  PullResultONS(ONSPullStatus pullStatus, long long nextBeginOffset,
                long long minOffset, long long maxOffset)
      : pullStatus(pullStatus),
        nextBeginOffset(nextBeginOffset),
        minOffset(minOffset),
        maxOffset(maxOffset) {}

  virtual ~PullResultONS() {}

 public:
  ONSPullStatus pullStatus;
  long long nextBeginOffset;
  long long minOffset;
  long long maxOffset;
  std::vector<Message> msgFoundList;
};
}
#endif

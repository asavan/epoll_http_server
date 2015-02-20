#ifndef __COMMON_SOCKET_TUNER_H__
#define __COMMON_SOCKET_TUNER_H__

#include "types.h"

namespace Network
{
  
  class SocketTuner
  {
  public:
    SocketTuner(SocketHandle handle);
    void SetReuseAddr(bool reuse);
    void SetCorking(bool useCorking);
    void SetNoDelay(bool noDelay);
    
  private:
    SocketHandle Handle;
    
    void CheckResult(int code, int invalidCode, char const *msg, int errNo) const;
  };

}

#endif  // !__COMMON_SOCKET_TUNER_H__

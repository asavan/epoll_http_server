#include "socket_tuner.h"

#include "exceptions.h"

#include <errno.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

namespace Network
{

  DECLARE_RUNTIME_EXCEPTION(SocketTuner)

  SocketTuner::SocketTuner(SocketHandle handle)
    : Handle(handle)
  {
  }
  
  void SocketTuner::SetReuseAddr(bool reuse)
  {
    int On = reuse ? 1 : 0;
    CheckResult(setsockopt(Handle, SOL_SOCKET, SO_REUSEADDR, &On, sizeof(On)),
                -1, "Failed to set SO_REUSEADDR", errno);
  }
  
  void SocketTuner::SetCorking(bool useCorking)
  {
    int On = useCorking ? 1 : 0;
    CheckResult(setsockopt(Handle, IPPROTO_TCP, TCP_CORK, &On, sizeof(On)),
                -1, "Failed to set TCP_CORK", errno);
  }
  
  void SocketTuner::SetNoDelay(bool noDelay)
  {
    int On = noDelay ? 1 : 0;
    CheckResult(setsockopt(Handle, IPPROTO_TCP, TCP_NODELAY, &On, sizeof(On)),
                -1, "Failed to set TCP_NODELAY", errno);
  }
  
  void SocketTuner::CheckResult(int code, int invalidCode, char const *msg, int errNo) const
  {
    if (code != invalidCode)
      return;
    throw SocketTunerException(msg, errNo);
  }

}

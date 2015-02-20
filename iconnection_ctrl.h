#ifndef __COMMON_ICONNECTION_CTRL_H__
#define __COMMON_ICONNECTION_CTRL_H__

#include "socket_tuner.h"
#include "inet_address.h"

namespace Network
{

  struct IConnectionCtrl
  {
    virtual ~IConnectionCtrl()
    {
    }
    virtual void MarkMeForClose() = 0;
    virtual void UpdateSessionTime() = 0;
    virtual bool SendData(void const *buf, unsigned *bytes) = 0;
    virtual bool SendFile(int fileHandle, unsigned offset, unsigned *bytes) = 0;
    virtual InetAddress const& GetAddress() const = 0;
    virtual SocketTuner GetSocketTuner() const = 0;
  };

}

#endif  // !__COMMON_ICONNECTION_CTRL_H__

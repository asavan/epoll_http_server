#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include "non_copyable.h"
#include "iuser_session.h"
#include "inet_address.h"
#include "idisposable.h"

#include <functional>
#include <memory>

#include <vector>

namespace Network
{  
  typedef std::function<IUserSessionPtr ()> UserSessionCreator;

  class TCPServer
    : private Common::NonCopyable
  {
  public:
    TCPServer(const InetAddress &locAddr, int backlog, int maxThreadsCount,
              int maxConnectionsCount, UserSessionCreator sessionCreator);
    
  private:
    typedef std::unique_ptr<Common::IDisposable> IDisposablePtr;
    typedef std::vector<IDisposablePtr> IDisposablePool;
    
    IDisposablePool Threads;
  };
  
}

#endif  // !__TCP_SERVER_H__

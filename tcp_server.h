#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include "non_copyable.h"
#include "iuser_session.h"
#include "inet_address.h"
#include "idisposable.h"

#include <memory>
#include <vector>
#include <string>

namespace Network
{  
  class TCPServer
    : private Common::NonCopyable
  {
  public:
    TCPServer(InetAddressPtr locAddr, int backlog, int maxThreadsCount,
		int maxConnectionsCount, const std::string& rootDir, const std::string& defaultPage, bool useCorking);
    
  private:
    typedef std::unique_ptr<Common::IStartable> IDisposablePtr;
    typedef std::vector<IDisposablePtr> IDisposablePool;
    
    IDisposablePool Threads;	
  };
  
}

#endif  // !__TCP_SERVER_H__

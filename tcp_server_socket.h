#ifndef __COMMON_TCP_SERVER_SOCKET_H__
#define __COMMON_TCP_SERVER_SOCKET_H__

#include "socket.h"
#include "inet_address.h"

namespace Network
{

  class TCPServerSocket
    : protected Socket
  {
  public:
    TCPServerSocket(const InetAddress &locAddr, int backlog);
  };

}

#endif  // !__COMMON_TCP_SERVER_SOCKET_H__

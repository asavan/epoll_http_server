#include "socket.h"
#include "inet_address.h"

#include "exceptions.h"

#include <errno.h>

namespace Network
{
  DECLARE_RUNTIME_EXCEPTION(Socket)
  
  Socket::Socket(Type type, bool nonBlocking)
    : SocketHolder(socket(AF_INET, GetSockType(type) | (nonBlocking ? SOCK_NONBLOCK : 0), 0))
  {
  }
  
  void Socket::Bind(const InetAddress &addr)
  {
    if (bind(GetHandle(), addr.GetAddr(), addr.GetSize()) == -1)
      throw SocketException("Failed to bind socket", errno);
  }
  
  void Socket::Listen(int backlog)
  {
    if (listen(GetHandle(), backlog) == -1)
      throw SocketException("Failed to listen socket", errno);
  }
  
  SocketHolderPtr Socket::Accept(bool nonBlocking,
                      sockaddr *newAddr, socklen_t *newAddrSize)
  {
    SocketHolderPtr NewSocket(new SocketHolder(accept4(GetHandle(), newAddr, newAddrSize, nonBlocking ? SOCK_NONBLOCK : 0)));
    return NewSocket;
  }
  
  int Socket::GetSockType(Type type)
  {
    switch (type)
    {
    case Tcp :
      return SOCK_STREAM;
    case Udp :
      return SOCK_DGRAM;
    default :
      break;
    }
    throw SocketException("Unknown socket type");
  }

}

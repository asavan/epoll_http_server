#include "inet_address_v4.h"

#include <arpa/inet.h>
#include <iostream>

namespace Network
{
  
  InetAddressPtr InetAddressV4::CreateFromString(const std::string &addr, unsigned short port)
  {
    sockaddr_in SockAddr = { 0 };
    SockAddr.sin_family = AF_INET;
    SockAddr.sin_addr.s_addr = inet_addr(addr.c_str());
    SockAddr.sin_port = htons(port);
    InetAddressPtr Addr(new InetAddressV4(reinterpret_cast<sockaddr const *>(&SockAddr), sizeof(SockAddr)));
    return Addr;
  }
    
  InetAddressV4::InetAddressV4(sockaddr const *addr, int size)
    : InetAddress(addr, size)
  {
  }

}

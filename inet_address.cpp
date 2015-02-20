#include "inet_address.h"

#include "exceptions.h"
#include <string.h>

namespace Network
{

  DECLARE_RUNTIME_EXCEPTION(InetAddress)

  InetAddress::InetAddress(sockaddr const *addr, int size)
    : Addr(0)
    , Size(0)
  {
    Init(addr, size);
  }
  
  InetAddress::~InetAddress()
  {
    ::operator delete (Addr);
  }
  
  sockaddr const* InetAddress::GetAddr() const
  {
    return Addr;
  }
  
  int InetAddress::GetSize() const
  {
    return Size;
  }
  
  
  
  InetAddressPtr InetAddress::CreateFromSockAddr(sockaddr *addr, int size)
  {
    InetAddressPtr Addr(new InetAddress(addr, size));
    return Addr;
  }
  
  
  
  void InetAddress::Init(sockaddr const *addr, int size)
  {
    if (!addr || !size)
      throw InetAddressException("Invalid address");
    Addr = reinterpret_cast<sockaddr *>(::operator new(size));
    Size = size;
    memcpy(Addr, addr, Size);
  }

}

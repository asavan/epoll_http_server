#include "socket_holder.h"

#include "exceptions.h"

#include <unistd.h>
#include <sys/socket.h>

namespace Network
{
  DECLARE_RUNTIME_EXCEPTION(SocketHolder)
  
  SocketHolder::SocketHolder()
    : Handle(InvalidSocket)
  {
  }
  
  SocketHolder::SocketHolder(SocketHandle handle)
    : Handle(handle)
  {
    if (Handle == InvalidSocket)
      throw SocketHolderException("Invalid socket");
  }
  
  SocketHolder::~SocketHolder()
  {
    if (IsValid())
    {
      shutdown(Handle, SHUT_RDWR);
      close(Handle);
    }
  }
  
  SocketHandle SocketHolder::GetHandle() const
  {
    return Handle;
  }
  
  bool SocketHolder::IsValid() const
  {
    return Handle != InvalidSocket;
  }

}

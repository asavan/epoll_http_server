#include "io_helper.h"

#include "exceptions.h"

#include <sys/socket.h>
#include <errno.h>
#include <sys/sendfile.h>

namespace Network
{

  DECLARE_RUNTIME_EXCEPTION(IOHelper)

  IOHelper::IOHelper(SocketHandle handle)
    : Handle(handle)
  {
  }
  
  bool IOHelper::Read(void *buf, unsigned *bytes)
  {
    if (!buf || !bytes || !*bytes)
      throw IOHelperException("Invalid buffer for read");
    int Res = recv(Handle, buf, *bytes, 0);
    if (Res == -1)
    {
      if (errno == EWOULDBLOCK)
        return false;
      throw IOHelperException("Failed to receive data", errno);
    }
    *bytes = Res;
    return true;
  }
  
  bool IOHelper::Write(void const *buf, unsigned *bytes)
  {
    if (!buf || !bytes || !*bytes)
      throw IOHelperException("Invalid  buffer for wtite");
    int Res = send(Handle, buf, *bytes, 0);
    if (Res == -1)
    {
      if (errno == EWOULDBLOCK)
        return false;
      throw IOHelperException("Failed to send data", errno);
    }
    *bytes = Res;
    return true;
  }
  
  bool IOHelper::SendFile(int fileHandle, unsigned offset, unsigned *bytes)
  {
    if (!bytes || !*bytes)
      throw IOHelperException("Invalid input argument");
    off_t Offset = offset;
    ssize_t Res = sendfile(Handle, fileHandle, &Offset, *bytes);
    if (Res == -1)
    {
      if (errno == EAGAIN)
      {
        *bytes = Offset - offset;
        return false;
      }
      throw IOHelperException("Failed to send file", errno);
    }
    if (static_cast<unsigned>(Res) < *bytes)
    {
      *bytes = static_cast<unsigned>(Res);
      return false;
    }
    return true;
  }

}

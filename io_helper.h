#ifndef __COMMON_IO_HELPER_H__
#define __COMMON_IO_HELPER_H__

#include "non_copyable.h"
#include "types.h"

namespace Network
{  
  
  class IOHelper
    : private Common::NonCopyable
  {
  public:
    IOHelper(SocketHandle handle);
    bool Read(void *buf, unsigned *bytes);
    bool Write(void const *buf, unsigned *bytes);
    bool SendFile(int fileHandle, unsigned offset, unsigned *bytes);
    
  private:
    SocketHandle Handle;
  };

}

#endif  // !__COMMON_IO_HELPER_H__

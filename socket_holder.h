#ifndef __COMMON_SOCKET_HOLDER_H__
#define __COMMON_SOCKET_HOLDER_H__

#include "non_copyable.h"
#include "types.h"

#include <memory>

namespace Network
{    
  
  class SocketHolder
    : private Common::NonCopyable
  {
  public:
    SocketHolder();
    explicit SocketHolder(SocketHandle handle);
    virtual ~SocketHolder();
    SocketHandle GetHandle() const;
    
    
  private:
	bool IsValid() const;    
  
    SocketHandle Handle;
  };
  
  typedef std::unique_ptr<SocketHolder> SocketHolderPtr;

}

#endif  // !__COMMON_SOCKET_HOLDER_H__

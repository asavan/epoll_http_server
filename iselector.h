#ifndef __COMMON_ISELECTOR_H__
#define __COMMON_ISELECTOR_H__

#include "types.h"

#include <functional>

namespace Network
{

  struct ISelector
  {
    enum SelectType
    {
      stRead = 1 << 0,
      stClose = 1 << 1
    };
    
    typedef std::function<void (SocketHandle, SelectType)> SelectFunction;
    
    virtual ~ISelector()
    {
    }
    virtual void AddSocket(SocketHandle handle, int selectType) = 0;
    virtual void DelSocket(SocketHandle handle) = 0;
    virtual void Select(SelectFunction *function, unsigned timeout) = 0;
  };

}

#endif  // !__COMMON_ISELECTOR_H__

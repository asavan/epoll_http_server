#ifndef __COMMON_ISELECTOR_H__
#define __COMMON_ISELECTOR_H__

#include "types.h"

#include <functional>

namespace Network
{
	enum SelectType
    {
      stRead = 1 << 0,
      stClose = 1 << 1
    };

	class ISelectable{
	public:
		virtual ~ISelectable() {};
		virtual void onSelect(SocketHandle, SelectType) = 0;
	};

  struct ISelector
  {
    
    
    // typedef std::function<void (SocketHandle, SelectType)> SelectFunction;
    
    virtual ~ISelector()
    {
    }
    virtual void AddSocket(SocketHandle handle, int selectType) = 0;
    virtual void DelSocket(SocketHandle handle) = 0;
    virtual void Select(ISelectable* function, unsigned timeout) = 0;
  };

  
}

#endif  // !__COMMON_ISELECTOR_H__

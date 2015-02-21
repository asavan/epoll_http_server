#ifndef __COMMON_EPOLL_SELECTOR_H__
#define __COMMON_EPOLL_SELECTOR_H__

#include "non_copyable.h"
#include "iselector.h"

#include <sys/epoll.h>

#include <vector>

namespace Network
{
  
  
  
  class EPollSelector
    : private Common::NonCopyable
    , public ISelector
  {
  public:
    explicit EPollSelector(int maxSocketCount);
    ~EPollSelector();
    virtual void AddSocket(SocketHandle handle, int selectType);
    virtual void DelSocket(SocketHandle handle);
	virtual void Select(ISelectable *function, unsigned timeout);
    
  private:
    typedef std::vector<epoll_event> EventPool;
    EventPool Events;
    int EPoll;
    
    static int GetSelectFlags(int selectType);
  };

}

#endif  // !__COMMON_EPOLL_SELECTOR_H__

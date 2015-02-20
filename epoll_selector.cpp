#include "epoll_selector.h"

#include "exceptions.h"

#include <errno.h>
#include <unistd.h>

namespace Network
{
  DECLARE_RUNTIME_EXCEPTION(EPollSelector)
  
  EPollSelector::EPollSelector(int maxSocketCount)
    : Events(maxSocketCount)
    , EPoll(-1)
  {
    if ((EPoll = epoll_create1(EPOLL_CLOEXEC))== -1)
      throw EPollSelectorException("Failed to create epoll", errno);
  }
  
  EPollSelector::~EPollSelector()
  {
    close(EPoll);
  }
  
  void EPollSelector::AddSocket(SocketHandle handle, int selectType)
  {
    epoll_event Event = { 0 };
    Event.data.fd = handle;
    Event.events = GetSelectFlags(selectType);
    if (epoll_ctl(EPoll, EPOLL_CTL_ADD, handle, &Event) == -1)
      throw EPollSelectorException("Failed to add socket to epoll", errno);
  }
  
  void EPollSelector::DelSocket(SocketHandle handle)
  {
    if (epoll_ctl(EPoll, EPOLL_CTL_DEL, handle, 0) == -1)
      throw EPollSelectorException("Failed to del socket from epoll", errno);
  }
  
  void EPollSelector::Select(SelectFunction *function, unsigned timeout)
  {
    int Count = epoll_wait(EPoll, &Events[0], Events.size(), timeout);
    if (Count == -1)
      throw EPollSelectorException("Failed to select epoll events", errno);
    for (int i = 0 ; i < Count ; ++i)
    {
      (*function)(Events[i].data.fd, Events[i].events & EPOLLRDHUP ?
                    ISelector::stClose : ISelector::stRead);
    }
  }
  
  int EPollSelector::GetSelectFlags(int selectType)
  {
    int Flags = 0;
    if (selectType & ISelector::stRead)
      Flags |= EPOLLIN/* | EPOLLET*/;
    if (selectType & ISelector::stClose)
      Flags |= EPOLLRDHUP;
    return Flags;
  }

}

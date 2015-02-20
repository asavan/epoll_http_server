#ifndef __COMMON_SELECTOR_THREAD_H__
#define __COMMON_SELECTOR_THREAD_H__

#include "epoll_selector.h"
#include "thread_loop.h"

#include <memory>

namespace Network
{

  class SelectorThread
    : private EPollSelector
    , private System::ThreadLoop
  {
  public:
    
    using EPollSelector::AddSocket;
    using EPollSelector::DelSocket;
    
    typedef System::Thread::ThreadFunction ThreadFunction;
    typedef std::shared_ptr<ThreadFunction> ThreadFunctionPtr;
    
    SelectorThread(int maxEventsCount, unsigned waitTimeout, ISelector::SelectFunction onSelectFunc,
                   ThreadFunctionPtr idleFunc = ThreadFunctionPtr());
    virtual ~SelectorThread();
    
  private:
    void SelectItems(ISelector::SelectFunction &func, unsigned waitTimeout, ThreadFunctionPtr idleFunc);
  };

}

#endif  // !__COMMON_SELECTOR_THREAD_H__

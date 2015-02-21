#ifndef __COMMON_SELECTOR_THREAD_H__
#define __COMMON_SELECTOR_THREAD_H__

#include "epoll_selector.h"
#include "thread_loop.h"
#include "idisposable.h"
#include <memory>

namespace Network
{

  class SelectorThread
    : private EPollSelector,
	public Common::IRunnable
  {
  public:
    
    using EPollSelector::AddSocket;
    using EPollSelector::DelSocket;
    
	typedef std::function<void ()> ThreadFunction;
    typedef std::unique_ptr<ThreadFunction> ThreadFunctionPtr;
    
    SelectorThread(int maxEventsCount, unsigned waitTimeout, ISelector::SelectFunction onSelectFunc,
                   ThreadFunctionPtr idleFunc = ThreadFunctionPtr());
    virtual ~SelectorThread();
    virtual void run();
  private:
	System::ThreadLoop threadLoop;
	ThreadFunctionPtr idleFunc_;
	ISelector::SelectFunction onSelectFunc_;
	int maxEventsCount_;
	unsigned waitTimeout_;
    void SelectItems(ISelector::SelectFunction &func, unsigned waitTimeout);
  };

}

#endif  // !__COMMON_SELECTOR_THREAD_H__

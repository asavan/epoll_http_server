#ifndef __COMMON_SELECTOR_THREAD_H__
#define __COMMON_SELECTOR_THREAD_H__

#include "epoll_selector.h"
#include "thread_loop.h"
#include "idisposable.h"
#include <memory>

namespace Network
{

  class SelectorThread
    :
	public Common::IRunnable,
	private Common::NonCopyable
  {
  public:
   
	SelectorThread(int maxEventsCount, unsigned waitTimeout, ISelector::SelectFunction onSelectFunc,
                   Common::IRunnable* task = NULL);
    virtual ~SelectorThread();
    virtual void run();

	// ISelector
	void AddSocket(SocketHandle handle, int selectType);
    void DelSocket(SocketHandle handle);
  private:
	System::ThreadLoop threadLoop;
	EPollSelector ePollSelector_;
	
	ISelector::SelectFunction onSelectFunc_;
	int maxEventsCount_;
	unsigned waitTimeout_;
	Common::IRunnable* task_;
	
  };

}

#endif  // !__COMMON_SELECTOR_THREAD_H__

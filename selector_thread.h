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
	public Common::IRunnable, Common::IStartable,
	private Common::NonCopyable
  {
  public:
   
	SelectorThread(int maxEventsCount, unsigned waitTimeout, ISelectable* selectTask,
                   Common::IRunnable* task = NULL);
    virtual ~SelectorThread();
    virtual void run();
	virtual void Start();

	// ISelector
	void AddSocket(SocketHandle handle, int selectType);
    void DelSocket(SocketHandle handle);
  private:
	
	EPollSelector ePollSelector_;
	
	// ISelector::SelectFunction onSelectFunc_;
	int maxEventsCount_;
	unsigned waitTimeout_;
	ISelectable* selectTask_;
	Common::IRunnable* task_;
	System::ThreadLoop threadLoop;
	
  };

}

#endif  // !__COMMON_SELECTOR_THREAD_H__

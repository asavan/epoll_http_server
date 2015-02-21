#include "selector_thread.h"
#include "logger.h"

namespace Network
{

  SelectorThread::SelectorThread(int maxEventsCount, unsigned waitTimeout,
                                 ISelectable* selectTask,
                                 Common::IRunnable* task)
    : 	
	ePollSelector_(maxEventsCount),
	maxEventsCount_(maxEventsCount),
	waitTimeout_(waitTimeout),
	selectTask_(selectTask),
	task_(task),
	threadLoop(this)
  {
  }
  
  SelectorThread::~SelectorThread()
  {
	  task_ = NULL;
	  selectTask_ = NULL;
	  // Common::Log::GetLogInst() << "Selector wait to die " << threadLoop.getId() << " " << ePollSelector_.getEpollId() << std::endl;
  }

  void SelectorThread::AddSocket(SocketHandle handle, int selectType)
  {
	ePollSelector_.AddSocket(handle, selectType);
  }

  void SelectorThread::DelSocket(SocketHandle handle)
  {
	ePollSelector_.DelSocket(handle);
  }

  void SelectorThread::run() 
  {
	try
    {
		if (selectTask_)
		{
			ePollSelector_.Select(selectTask_, waitTimeout_);
		}
		if (task_) 
		{
			task_->run();
		}
    }
    catch (const std::exception &e)
    {
		Common::Log::GetLogInst() << e.what() << std::endl;
    }
  }    
}

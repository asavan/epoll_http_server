#include "selector_thread.h"
#include "logger.h"

namespace Network
{

  SelectorThread::SelectorThread(int maxEventsCount, unsigned waitTimeout,
                                 ISelector::SelectFunction onSelectFunc,
                                 Common::IRunnable* task)
    : ePollSelector_(maxEventsCount),
	threadLoop(this),
	// idleFunc_(std::move(idleFunc)),
	onSelectFunc_(onSelectFunc),
	maxEventsCount_(maxEventsCount),
	waitTimeout_(waitTimeout),
	task_(task)
  {
  }
  
  SelectorThread::~SelectorThread()
  {
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
      ePollSelector_.Select(&onSelectFunc_, waitTimeout_);
      if (task_)
        task_->run();
    }
    catch (const std::exception &e)
    {
      Common::Log::GetLogInst() << e.what();
    }
  }    
}

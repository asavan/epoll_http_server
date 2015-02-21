#include "selector_thread.h"
#include "logger.h"

namespace Network
{

  SelectorThread::SelectorThread(int maxEventsCount, unsigned waitTimeout,
                                 ISelector::SelectFunction onSelectFunc,
                                 ThreadFunctionPtr idleFunc)
    : EPollSelector(maxEventsCount),
	threadLoop(this),
	idleFunc_(std::move(idleFunc)),
	onSelectFunc_(onSelectFunc),
	maxEventsCount_(maxEventsCount),
	waitTimeout_(waitTimeout)	
  {
  }
  
  SelectorThread::~SelectorThread()
  {
  }

  void SelectorThread::run() 
  {
	SelectItems(onSelectFunc_, waitTimeout_);
  }
    
  void SelectorThread::SelectItems(ISelector::SelectFunction &func, unsigned waitTimeout)
  {
    try
    {
      Select(&func, waitTimeout);
      if (idleFunc_)
        (*idleFunc_)();
    }
    catch (const std::exception &e)
    {
      Common::Log::GetLogInst() << e.what();
    }
  }

}

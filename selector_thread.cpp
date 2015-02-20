#include "selector_thread.h"
#include "logger.h"

namespace Network
{

  SelectorThread::SelectorThread(int maxEventsCount, unsigned waitTimeout,
                                 ISelector::SelectFunction onSelectFunc,
                                 ThreadFunctionPtr idleFunc)
    : EPollSelector(maxEventsCount)
    , System::ThreadLoop(std::bind(&SelectorThread::SelectItems, this, onSelectFunc, waitTimeout, idleFunc))
  {
  }
  
  SelectorThread::~SelectorThread()
  {
  }
    
  void SelectorThread::SelectItems(ISelector::SelectFunction &func, unsigned waitTimeout, ThreadFunctionPtr idleFunc)
  {
    try
    {
      Select(&func, waitTimeout);
      if (idleFunc.get())
        (*idleFunc.get())();
    }
    catch (const std::exception &e)
    {
      Common::Log::GetLogInst() << e.what();
    }
  }

}

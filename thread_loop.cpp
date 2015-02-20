#include "thread_loop.h"

namespace System
{

  ThreadLoop::ThreadLoop(const Thread::ThreadFunction &func)
    : IsRun(true)
    , Function(new Thread::ThreadFunction(func)),
	  worker(std::bind(&ThreadLoop::Work, this))
  {
    
  }

  ThreadLoop::~ThreadLoop()
  {
    IsRun = false;
	worker.join();
  }
    
  void ThreadLoop::Work()
  {
    while (IsRun)
      (*Function)();
  }
  
}

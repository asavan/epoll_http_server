#include "thread_loop.h"

namespace System
{

  ThreadLoop::ThreadLoop(Common::IRunnable* task)
    : IsRun(true)
    , task_(task),
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
      task_->run();
  }
  
}

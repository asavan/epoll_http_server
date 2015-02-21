#include "thread_loop.h"
#include "logger.h"

namespace System
{

  ThreadLoop::ThreadLoop(Common::IRunnable* task)
    : IsRun(true)
    , task_(task),
	  worker()
  {
    
  }

  void ThreadLoop::Start()
  {
        worker = std::thread(&ThreadLoop::Work,this);
  }

  std::thread::id  ThreadLoop::getId() const
  {
	  return worker.get_id();
  }

  ThreadLoop::~ThreadLoop()
  {
	// Common::Log::GetLogInst() << "thread wait to die " << getId() << std::endl;
    IsRun = false;
	worker.join();
  }
    
  void ThreadLoop::Work()
  {
    while (IsRun)
      task_->run();
  }
  
}

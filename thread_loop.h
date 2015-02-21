#ifndef __COMMON_THREAD_LOOP_H__
#define __COMMON_THREAD_LOOP_H__

#include "non_copyable.h"
#include "idisposable.h"

#include <thread>

namespace System
{

  class ThreadLoop
    : private Common::NonCopyable
  {
  public:
    ThreadLoop(Common::IRunnable* task);
    ~ThreadLoop();
	std::thread::id  getId() const;
    
  private:
    bool volatile IsRun;
	Common::IRunnable* task_;
    std::thread worker;
    
    void Work();
  };

}

#endif  // !__COMMON_THREAD_LOOP_H__

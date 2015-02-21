#ifndef __COMMON_THREAD_LOOP_H__
#define __COMMON_THREAD_LOOP_H__

#include "non_copyable.h"
#include "idisposable.h"

#include <functional>
#include <memory>
#include <thread>

namespace System
{

  class ThreadLoop
    : private Common::NonCopyable
  {
  public:
    ThreadLoop(Common::IRunnable* task);
    ~ThreadLoop();
    
  private:
    bool volatile IsRun;
    // Thread::ThreadFunctionPtr Function;
    std::thread worker;
    Common::IRunnable* task_;
    void Work();
  };

}

#endif  // !__COMMON_THREAD_LOOP_H__

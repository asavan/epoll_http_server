#ifndef __COMMON_THREAD_LOOP_H__
#define __COMMON_THREAD_LOOP_H__

#include "non_copyable.h"

#include <functional>
#include <memory>
#include <thread>

namespace System
{

	namespace Thread
	{
		typedef std::function<void ()> ThreadFunction;
		typedef std::unique_ptr<ThreadFunction> ThreadFunctionPtr;
	}

  class ThreadLoop
    : private Common::NonCopyable
  {
  public:
    ThreadLoop(const Thread::ThreadFunction& func);
    ~ThreadLoop();
    
  private:
    bool volatile IsRun;
    Thread::ThreadFunctionPtr Function;
    std::thread worker;
    
    void Work();
  };

}

#endif  // !__COMMON_THREAD_LOOP_H__

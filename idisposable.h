#ifndef __COMMON_IDISPOSABLE_H__
#define __COMMON_IDISPOSABLE_H__

namespace Common
{

  struct IDisposable
  {
    virtual ~IDisposable()
    {
    }
  };

  class IRunnable
  {
  public:
    virtual ~IRunnable()
    {
    }
	virtual void run() = 0;
  };


}

#endif  // !__COMMON_IDISPOSABLE_H__

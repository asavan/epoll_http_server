#ifndef __COMMON_IDISPOSABLE_H__
#define __COMMON_IDISPOSABLE_H__

namespace Common
{  

  class IRunnable
  {
  public:
    virtual ~IRunnable()
    {
    }
	virtual void run() = 0;
  };

  class IStartable
  {
  public:
    virtual ~IStartable()
    {
    }
	virtual void Start() = 0;
  };


}

#endif  // !__COMMON_IDISPOSABLE_H__

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

}

#endif  // !__COMMON_IDISPOSABLE_H__

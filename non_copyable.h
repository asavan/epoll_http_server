#ifndef __COMMON_NON_COPYABLE_H__
#define __COMMON_NON_COPYABLE_H__

namespace Common
{
  
  class NonCopyable
  {
  protected:
    NonCopyable()
    {
    }
    ~NonCopyable()
    {
    }
  private:
    NonCopyable(const NonCopyable &);
    NonCopyable& operator = (const NonCopyable &);
  };

}

#endif  // !__COMMON_NON_COPYABLE_H__

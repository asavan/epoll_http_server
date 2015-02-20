#ifndef __COMMON_UTILS_H__
#define __COMMON_UTILS_H__

namespace Common
{
  
  template <typename T, size_t size>
  size_t SizeOfArray(T (&)[size])
  {
    return size;
  }
  
}

#endif  // !__COMMON_UTILS_H__

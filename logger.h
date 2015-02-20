#ifndef __COMMON_LOGGER_H__
#define __COMMON_LOGGER_H__

#include <ostream>

namespace Common
{
  
  namespace Log
  {
    
    std::ostream& GetLogInst();
    
  }
  
}

#endif  // !__COMMON_LOGGER_H__

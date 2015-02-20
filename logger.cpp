#include "logger.h"

#include <iostream>

namespace Common
{
  
  namespace Log
  {
    
    std::ostream& GetLogInst()
    {
      return std::cerr;
    }
    
  }
  
}

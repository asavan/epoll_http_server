#ifndef __COMMON_HTTP_SRC_FILE_HOLDER_H__
#define __COMMON_HTTP_SRC_FILE_HOLDER_H__

#include "file_holder.h"

#include <string>

// #include <unistd.h>

namespace Network
{
  
  namespace Proto
  {
    
    namespace Http
    {
	
	  DECLARE_RUNTIME_EXCEPTION(HttpSrcFileHolder)
	  
      class HttpSrcFileHolder
        : private System::FileHolder
      {
      public:
        HttpSrcFileHolder(std::string const &fileName);
      
        unsigned GetFileSize() const;
	  // protected:
	    int GetHandle() const;
      };
      
    }
    
  }
  
}

#endif// !__COMMON_HTTP_SRC_FILE_HOLDER_H__

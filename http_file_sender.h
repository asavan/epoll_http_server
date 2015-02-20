#ifndef __COMMON_HTTP_FILE_SENDER_H__
#define __COMMON_HTTP_FILE_SENDER_H__


#include "http_response.h"
#include "http_src_file_holder.h"

#include <string>

namespace Network
{
  class IConnectionCtrl;
  namespace Proto
  {
    
    namespace Http
    {
      
    
      
      class HttpFileSender
        : public HttpResponse
      {
      public:
        HttpFileSender(IConnectionCtrl *ctrl, const std::string &resourceName);
        bool Send();
        
      private:
        HttpSrcFileHolder File;
        bool HeaderHasBeenSent;
        unsigned FileOffset;
      };
      
    }
    
  }
  
}

#endif  // !__COMMON_HTTP_FILE_SENDER_H__

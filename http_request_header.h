#ifndef __COMMON_HTTP_REQUEST_HEADER_H__
#define __COMMON_HTTP_REQUEST_HEADER_H__

#include "exceptions.h"
#include "http_status_code.h"
#include "non_copyable.h"

#include <string>
#include <map>

namespace Network
{
  
  namespace Proto
  {
    
    namespace Http
    {
      
      DECLARE_RUNTIME_EXCEPTION(HttpRequestHeader)
      
      class HttpRequestHeader: private Common::NonCopyable
      {
      public:
        enum Method { mtdGet, mtdHead };
        
        static char const ContentLengthPrm[];
        
        HttpRequestHeader(Method mtd, const std::string && resource);
        
        Method GetMethod() const;
        const std::string & GetResource() const;
        void AddParam(const std::string &name, const std::string &value);
        bool TryGetParameter(const std::string &name, std::string *value) const;
        bool ExistsParameter(const std::string &name) const;
        unsigned GetContentLength() const;
        
      private:
        Method Mtd;
        std::string Resource;
        typedef std::map<std::string, std::string> StringMap;
        StringMap Params;
        mutable long ContentLength;
      };

    }

  }

}

#endif  // !__COMMON_HTTP_REQUEST_HEADER_H__

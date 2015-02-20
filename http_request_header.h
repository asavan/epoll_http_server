#ifndef __COMMON_HTTP_REQUEST_HEADER_H__
#define __COMMON_HTTP_REQUEST_HEADER_H__

#include "exceptions.h"
#include "http_status_code.h"

#include <string>
#include <map>

namespace Network
{
  
  namespace Proto
  {
    
    namespace Http
    {
      
      DECLARE_RUNTIME_EXCEPTION(HttpRequestHeader)
      
      class HttpRequestHeader
      {
      public:
        enum Method { mtdGet, mtdHead };
        
        static char const ContentLengthPrm[];
        
        HttpRequestHeader(Method mtd, std::string const &resource);
        
        Method GetMethod() const;
        std::string const& GetResource() const;
        void AddParam(std::string const &name, std::string const &value);
        bool TryGetParameter(std::string const &name, std::string *value) const;
        bool ExistsParameter(std::string const &name) const;
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

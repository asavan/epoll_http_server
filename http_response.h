#ifndef __COMMON__HTTP_RESPONSE_H__
#define __COMMON__HTTP_RESPONSE_H__

#include "non_copyable.h"
#include "http_status_code.h"

#include <string>
#include <vector>

namespace Network
{
  class IConnectionCtrl;
  namespace Proto
  {
    
    namespace Http
    {
      
      class HttpResponse
        : private Common::NonCopyable
      {
      public:
        enum ConnectionType
        {
          conClose,
          conKeepAlive
        };
        
        HttpResponse(IConnectionCtrl *ctrl);
        virtual ~HttpResponse();
        IConnectionCtrl* GetCtrl();
        virtual bool Send();
        void CreateResponseHeader();
        void SetStatusCode(HttpStatusCode status);
        void SetContentLength(unsigned lenght);
        void SetConnectionType(ConnectionType connection);
        void SetResourceName(std::string const &resourceName);
        
      private:
        static char const HttpVersion[];
        static char const EndLine[];
        static char const EndHeader[];
        static char const *Days[];
        static char const *Months[];
        static char const ServerName[];    
        static char const DatePrm[];
        static char const ServerPrm[];
        static char const ContentLengthPrm[];
        static char const ConnectionPrm[];
        static char const ContentTypePrm[];
        
        struct ResouurceTypePair
        {
          const char *rcType;
          const char *rcHttpType;
        };
        
        static ResouurceTypePair const RcTypes[];
        static unsigned const RcTypesCount;
        
        IConnectionCtrl *Ctrl;
        HttpStatusCode Status;
        mutable unsigned ContentLength;
        ConnectionType Connection;
        mutable std::string ResourceName;
        
        typedef std::vector<char> CharBuffer;
        CharBuffer Buffer;
        
        void CreateResponseHeader(std::string *response) const;
        static char const* GetStatusDescription(HttpStatusCode status);
        static std::string GetDateTime();
        static char const* GetConnectionType(ConnectionType type);
        static char const* GetContentType(const std::string &fileName);
        static void CreateErrorPage(HttpStatusCode code, const std::string &description, std::string *page);
      };
      
    }
    
  }
  
}

#endif  // !__COMMON__HTTP_RESPONSE_H__

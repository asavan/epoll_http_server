#ifndef __COMMON_HTTP_REQUEST_READER_H__
#define __COMMON_HTTP_REQUEST_READER_H__

#include "non_copyable.h"
#include "http_request_header.h"
#include "common_utils.h"

#include <vector>
#include <memory>

#include <string.h>

namespace Network
{
  
  namespace Proto
  {
    
    namespace Http
    {
      
      
      
      class HttpRequestReader
        : private Common::NonCopyable
      {
      public:
        HttpRequestReader();
        void AssignData(void const *buf, unsigned bytes);
        
      private:
        static char const EndHeader[];
        static unsigned const EndHeaderLen;
        static char const EndLine[];
        static unsigned const EndLineLen;
        static char const HttpMtdGet[];
        static unsigned const HttpMtdGetLen;
        static char const HttpMtdHead[];
        static unsigned const HttpMtdHeadLen;
        static char const HttpVersion10[];
        static unsigned const HttpVersion10Len;
        static char const HttpVersion11[];
        static unsigned const HttpVersion11Len;
        
        typedef std::vector<char> CharBuffer;
        CharBuffer Buffer;
        unsigned LastEndHeaderPos;
        std::unique_ptr<HttpRequestHeader> Header;
        
        void ProcessHeader(char const *header, unsigned bytes);        
        bool GetHeaderLine(char const *str, unsigned strLen, unsigned *endOfLineIndex);        
        std::unique_ptr<HttpRequestHeader> ExtractRequestMethod(char const *str, unsigned len);        
        void ExtractRequestParams(char const *str, unsigned len, HttpRequestHeader *header);        
        void ProcessRequest();
        
      protected:
        virtual void ProcessRequest(HttpRequestHeader const &header, void const *buf, unsigned bytes);
      };
        
    }
    
  }
  
}

#endif  // !__COMMON_HTTP_REQUEST_READER_H__

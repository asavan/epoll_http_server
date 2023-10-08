#ifndef __COMMON_HTTP_REQUEST_READER_H__
#define __COMMON_HTTP_REQUEST_READER_H__

#include "non_copyable.h"
#include "http_request_header.h"
#include "common_utils.h"

#include <vector>
#include <memory>

// #include <string.h>

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
        static size_t const EndHeaderLen;
        static char const EndLine[];
        static size_t const EndLineLen;
        static char const HttpMtdGet[];
        static size_t const HttpMtdGetLen;
        static char const HttpMtdHead[];
        static size_t const HttpMtdHeadLen;
        static char const HttpVersion10[];
        static size_t const HttpVersion10Len;
        static char const HttpVersion11[];
        static size_t const HttpVersion11Len;
        
        typedef std::vector<char> CharBuffer;
        CharBuffer Buffer;
        size_t LastEndHeaderPos;
        std::unique_ptr<HttpRequestHeader> Header;
        
        void ProcessHeader(char const *header, size_t bytes);        
        bool GetHeaderLine(char const *str, size_t strLen, size_t *endOfLineIndex);
        std::unique_ptr<HttpRequestHeader> ExtractRequestMethod(char const *str, size_t len);
        void ExtractRequestParams(char const *str, size_t len, HttpRequestHeader *header);
        void ProcessRequest();
        
      protected:
        virtual void ProcessRequest(HttpRequestHeader const &header, void const *buf, size_t bytes) = 0;
      };
        
    }
    
  }
  
}

#endif  // !__COMMON_HTTP_REQUEST_READER_H__

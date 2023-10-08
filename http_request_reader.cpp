#include "http_request_reader.h"
#include "exceptions.h"
#include <iterator>
#include <memory>
#include <cstring>

namespace Network
{
  
  namespace Proto
  {
    
    namespace Http
    {
	
	  DECLARE_RUNTIME_EXCEPTION(HttpRequestReader)
      
      HttpRequestReader::HttpRequestReader()
        : LastEndHeaderPos(0)
      {
      }
      
      void HttpRequestReader::AssignData(void const *buf, unsigned bytes)
      {
        if (!buf || !bytes)
          return;
        
        char const *InputData = reinterpret_cast<char const *>(buf);
        std::copy(&InputData[0], &InputData[bytes], std::back_inserter(Buffer));
        
        if (Buffer.size() < EndHeaderLen)
          return;
        
        if (Header)
        {
          ProcessRequest();
          return;
        }
        
        size_t CurBufSize = Buffer.size();
        for (char const *p = &Buffer[LastEndHeaderPos] ; p != &Buffer[CurBufSize - EndHeaderLen + 1] ; ++p)
        {
          if (!strncmp(p, EndHeader, EndHeaderLen))
          {
            CharBuffer::iterator Begin = Buffer.begin();
            CharBuffer::iterator End = Buffer.begin() +
              std::distance<char const *>(&Buffer[0], p) + EndHeaderLen;
            ProcessHeader(&Buffer[0], std::distance(Begin, End));
            Buffer.erase(Begin, End);
            LastEndHeaderPos = 0;
            ProcessRequest();
            return;
          }
        }
        return;
        
        LastEndHeaderPos = Buffer.size() - EndHeaderLen;
      }
      
      void HttpRequestReader::ProcessHeader(char const *header, size_t bytes)
      {
        size_t index = 0, pos = 0;
        if (!GetHeaderLine(&header[pos], bytes - pos, &index))
          throw HttpRequestReaderException("Invalid http request", statBadRequest);
        std::unique_ptr<HttpRequestHeader> NewHeader = ExtractRequestMethod(&header[pos], index);
        for (pos = index ; GetHeaderLine(&header[pos], bytes - pos, &index) ; pos += index)
          ExtractRequestParams(&header[pos], index, NewHeader.get());
        Header = std::move(NewHeader);
        ProcessRequest();
      }
      
      bool HttpRequestReader::GetHeaderLine(char const *str, size_t strLen, size_t *endOfLineIndex)
      {
        if (strLen < EndLineLen)
          return false;
        for (char const *p = str; p + EndLineLen < str + strLen; ++p)
        {
          if (!strncmp(p, EndLine, EndLineLen))
          {
            *endOfLineIndex = (p - str) + EndLineLen;
            return true;
          }
        }
        return false;
      }
      
      std::unique_ptr<HttpRequestHeader> HttpRequestReader::ExtractRequestMethod(char const *str, size_t len)
      {
        struct
        {
          void operator () (const char *&str, size_t& len) const
          {
            for ( ; len && (*str == ' ' || *str == '\t') && --len ; ++str);
          }
        } SkipDelimiters;
        struct
        {
          bool operator () (const char *str, size_t len, size_t *eol) const
          {
            for (unsigned i = 0 ; i < len ; ++i)
            {
              if (i && (str[i] == ' ' || str[i] == '\t' || str[i] == '\r'))
              {
                *eol = i;
                return true;
              }
            }
            return false;
          }
        } GetWord;
        SkipDelimiters(str, len);
        size_t EoL = 0;
        if (!GetWord(str, len, &EoL))
          throw HttpRequestReaderException("Failed to extract request method", statMethodNotAllowed);
        HttpRequestHeader::Method Mtd;
        if (EoL == HttpMtdGetLen && !strncasecmp(str, HttpMtdGet, EoL))
          Mtd = HttpRequestHeader::mtdGet;
        else
        if (EoL == HttpMtdHeadLen && !strncasecmp(str, HttpMtdHead, EoL))
          Mtd = HttpRequestHeader::mtdHead;
        else
          throw HttpRequestReaderException("Unsupported http method", statMethodNotAllowed);
        str += EoL;
        len -= EoL;
        SkipDelimiters(str, len);
        if (!GetWord(str, len, &EoL))
          throw HttpRequestReaderException("Failed to extract request method resource", statBadRequest);
        std::string Resource(str, EoL);
        str += EoL;
        len -= EoL;
        SkipDelimiters(str, len);
        if (!GetWord(str, len, &EoL))
          throw HttpRequestReaderException("Failed to extract HTTP version", statBadRequest);
        if (!((EoL == HttpVersion10Len && !strncasecmp(str, HttpVersion10, HttpVersion10Len)) ||
            (EoL == HttpVersion11Len && !strncasecmp(str, HttpVersion11, HttpVersion11Len))))
        {
            throw HttpRequestReaderException("Unsupported http version", statHTTPVersionNotSupported);
        }
        std::unique_ptr<HttpRequestHeader> header(new HttpRequestHeader(Mtd, Resource));
        return header;
      }
      
      void HttpRequestReader::ExtractRequestParams(char const *str, size_t len, HttpRequestHeader *header)
      {
        size_t DelimiterPos = 0;
        
        for ( ; str[DelimiterPos] != ' ' && str[DelimiterPos] != '\t' &&
              DelimiterPos < len ; ++DelimiterPos);
        
        if (DelimiterPos + EndLineLen > len)
          throw HttpRequestReaderException("Failed to get http request parameter", statBadRequest);
        
        header->AddParam(std::string(str, DelimiterPos - 1),
          std::string(str + DelimiterPos, len - DelimiterPos - EndLineLen));
      }
      
      void HttpRequestReader::ProcessRequest()
      {
        if (!Header)
          return;
  
        if (Header->ExistsParameter(HttpRequestHeader::ContentLengthPrm))
        {
          unsigned ContentLength = Header->GetContentLength();
          if (ContentLength <= Buffer.size())
          {
            ProcessRequest(*Header.get(), &Buffer[0], ContentLength);
            Buffer.erase(Buffer.begin(), Buffer.begin() + ContentLength);
            Header.reset();
          }
        }
        else
        {
          ProcessRequest(*Header.get(), 0, 0);
          Header.reset();
        }
      }

      char const HttpRequestReader::EndHeader[] = "\r\n\r\n";
      size_t const HttpRequestReader::EndHeaderLen = Common::SizeOfArray(EndHeader) - 1;
      char const HttpRequestReader::EndLine[] = "\r\n";
      size_t const HttpRequestReader::EndLineLen = Common::SizeOfArray(EndLine) - 1;
      char const HttpRequestReader::HttpMtdGet[] = "GET";
      size_t const HttpRequestReader::HttpMtdGetLen = Common::SizeOfArray(HttpMtdGet) - 1;
      char const HttpRequestReader::HttpMtdHead[] = "HEAD";
      size_t const HttpRequestReader::HttpMtdHeadLen = Common::SizeOfArray(HttpMtdHead) - 1;
      char const HttpRequestReader::HttpVersion10[] = "HTTP/1.0";
      size_t const HttpRequestReader::HttpVersion10Len = Common::SizeOfArray(HttpVersion10) - 1;
      char const HttpRequestReader::HttpVersion11[] = "HTTP/1.1";
      size_t const HttpRequestReader::HttpVersion11Len = Common::SizeOfArray(HttpVersion11) - 1;
      
    }
    
  }
  
}

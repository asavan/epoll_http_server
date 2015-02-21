#include "http_request_reader.h"
#include "exceptions.h"
#include <memory>
#include <strings.h>

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
        
        unsigned CurBufSize = Buffer.size();
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
      
      void HttpRequestReader::ProcessHeader(char const *header, unsigned bytes)
      {
        unsigned index = 0, pos = 0;
        if (!GetHeaderLine(&header[pos], bytes - pos, &index))
          throw HttpRequestReaderException("Invalid http request", statBadRequest);
        std::unique_ptr<HttpRequestHeader> NewHeader = ExtractRequestMethod(&header[pos], index);
        for (pos = index ; GetHeaderLine(&header[pos], bytes - pos, &index) ; pos += index)
          ExtractRequestParams(&header[pos], index, NewHeader.get());
        Header = std::move(NewHeader);
        ProcessRequest();
      }
      
      bool HttpRequestReader::GetHeaderLine(char const *str, unsigned strLen, unsigned *endOfLineIndex)
      {
        if (strLen < EndLineLen)
          return false;
        for (char const *p = str ; static_cast<unsigned>(p - str) < strLen - EndLineLen ; ++p)
        {
          if (!strncmp(p, EndLine, EndLineLen))
          {
            *endOfLineIndex = (p - str) + EndLineLen;
            return true;
          }
        }
        return false;
      }
      
      std::unique_ptr<HttpRequestHeader> HttpRequestReader::ExtractRequestMethod(char const *str, unsigned len)
      {
        struct
        {
          void operator () (const char *&str, unsigned &len) const
          {
            for ( ; len && (*str == ' ' || *str == '\t') && --len ; ++str);
          }
        } SkipDelimiters;
        struct
        {
          bool operator () (const char *str, unsigned len, unsigned *eol) const
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
        unsigned EoL = 0;
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
      
      void HttpRequestReader::ExtractRequestParams(char const *str, unsigned len, HttpRequestHeader *header)
      {
        unsigned DelimiterPos = 0;
        
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
      
      void HttpRequestReader::ProcessRequest(HttpRequestHeader const &header, void const *buf, unsigned bytes)
      {
      }

      char const HttpRequestReader::EndHeader[] = "\r\n\r\n";
      unsigned const HttpRequestReader::EndHeaderLen = Common::SizeOfArray(EndHeader) - 1;
      char const HttpRequestReader::EndLine[] = "\r\n";
      unsigned const HttpRequestReader::EndLineLen = Common::SizeOfArray(EndLine) - 1;
      char const HttpRequestReader::HttpMtdGet[] = "GET";
      unsigned const HttpRequestReader::HttpMtdGetLen = Common::SizeOfArray(HttpMtdGet) - 1;
      char const HttpRequestReader::HttpMtdHead[] = "HEAD";
      unsigned const HttpRequestReader::HttpMtdHeadLen = Common::SizeOfArray(HttpMtdHead) - 1;
      char const HttpRequestReader::HttpVersion10[] = "HTTP/1.0";
      unsigned const HttpRequestReader::HttpVersion10Len = Common::SizeOfArray(HttpVersion10) - 1;
      char const HttpRequestReader::HttpVersion11[] = "HTTP/1.1";
      unsigned const HttpRequestReader::HttpVersion11Len = Common::SizeOfArray(HttpVersion11) - 1;
      
    }
    
  }
  
}

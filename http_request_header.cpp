#include "http_request_header.h"
#include "logger.h"
#include <stdlib.h>

namespace Network
{

  namespace Proto
  {

    namespace Http
    {

      char const HttpRequestHeader::ContentLengthPrm[] = "Content-Length";

      HttpRequestHeader::HttpRequestHeader(Method mtd, const std::string &&resource)
        : Mtd(mtd)
		, Resource(std::move(resource))
        , ContentLength(-1)
      {
		  Common::Log::GetLogInst() << "HttpRequestHeader " << resource << std::endl;
      }
      
      HttpRequestHeader::Method HttpRequestHeader::GetMethod() const
      {
        return Mtd;
      }
      
      std::string const& HttpRequestHeader::GetResource() const
      {
        return Resource;
      }
      
      void HttpRequestHeader::AddParam(std::string const &name, std::string const &value)
      {
        Params[name] = value;
      }
      
      bool HttpRequestHeader::TryGetParameter(std::string const &name, std::string *value) const
      {
        StringMap::const_iterator Iter = Params.find(name);
        if (Iter == Params.end())
          return false;
        *value = Iter->second;
        return true;
      }
      
      bool HttpRequestHeader::ExistsParameter(std::string const &name) const
      {
        return Params.find(name) != Params.end();
      }
      
      unsigned HttpRequestHeader::GetContentLength() const
      {
        if (ContentLength == -1)
        {
          std::string Param;
          if (TryGetParameter(ContentLengthPrm, &Param))
          {
            ContentLength = atol(Param.c_str());
            if (ContentLength < 0)
              throw HttpRequestHeaderException("Failed to get content length", statBadRequest);
          }
          else
            throw HttpRequestHeaderException("Failed to get content length", statBadRequest);
        }
        return static_cast<unsigned>(ContentLength);
      }

    }

  }

}

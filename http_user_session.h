#ifndef __COMMON_HTTP_USER_SESSION_H__
#define __COMMON_HTTP_USER_SESSION_H__

#include "iuser_session.h"
#include "http_request_reader.h"
#include "http_request_header.h"
#include "http_response.h"
#include "http_file_sender.h"

#include <string>
#include <memory>
#include <ctime>

namespace Network
{
  class IConnectionCtrl;
  namespace Proto
  {
    
    namespace Http
    {
      
      class HttpUserSession
        : public IUserSession
        , protected HttpRequestReader
      {
      public:
        HttpUserSession(const std::string &rootDir, const std::string &defaultPage, bool useCorking);
        virtual void Init(IConnectionCtrl *ctrl);
        
        virtual unsigned GetMaxBufSizeForRead() const;
        virtual void OnRecvData(void const *buf, unsigned bytes);
        virtual void OnIdle();
        virtual void ProcessRequest(const HttpRequestHeader &header, void const *buf, size_t bytes) override;
	  protected:
		virtual bool IsExpiredSession(std::time_t lastActionTime) const;
        
      private:
        enum { MaxBufSizeForRead = 4096 };
        const double SessionTimeout = 3.0; // 3s
        IConnectionCtrl *Ctrl;
        std::string RootDir;
        std::string DefaultPage;
        bool UseCorking;
        std::unique_ptr<HttpFileSender> FileSender;
        std::unique_ptr<HttpResponse> HeadResponse;
        
        std::string CreateResourceName(const std::string &resource) const;
      };
      
      IUserSessionPtrU CreateHttpUserSession(const std::string &rootDir,
        const std::string &defaultPage, bool useCorking);
      
    }
    
  }
  
}

#endif  // !__COMMON_HTTP_USER_SESSION_H__

#include "http_file_sender.h"
#include "iconnection_ctrl.h"

#include "exceptions.h"
namespace Network
{
  
  namespace Proto
  {
    
    namespace Http
    {
	  DECLARE_RUNTIME_EXCEPTION(HttpFileSender)

      HttpFileSender::HttpFileSender(IConnectionCtrl *ctrl, const std::string &resourceName)
        : HttpResponse(ctrl)
        , File(resourceName)
        , HeaderHasBeenSent(false)
        , FileOffset(0)
      {
        SetResourceName(resourceName);
        SetContentLength(File.GetFileSize());
      }
      
      bool HttpFileSender::Send()
      {
        if (!HeaderHasBeenSent)
          HeaderHasBeenSent = !HttpResponse::Send();
        unsigned Bytes = File.GetFileSize() - FileOffset;
        if (!Bytes)
          return false;
        if (GetCtrl()->SendFile(File.GetHandle(), FileOffset, &Bytes))
          return false;
        FileOffset += Bytes;
        if (FileOffset >= File.GetFileSize())
          return false;
        return true;
      }
      
    }
    
  }
  
}

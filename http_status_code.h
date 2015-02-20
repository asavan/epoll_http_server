#ifndef __COMMON_HTTP_STATUS_CODE_H__
#define __COMMON_HTTP_STATUS_CODE_H__

namespace Network
{

  namespace Proto
  {
    
    namespace Http
    {
      
      enum HttpStatusCode
      {
        statOK = 200,
        statBadRequest = 400,
        statForbidden = 403,
        statNotFound = 404,
        statMethodNotAllowed = 405,
        statNotAcceptable = 406,
        statRequestURITooLong = 414,
        statInternalServerError = 500,
        statHTTPVersionNotSupported = 505
      };
  
    }
    
  }
  
}


#endif  // !__COMMON_HTTP_STATUS_CODE_H__

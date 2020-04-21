#include "http_response.h"
#include "common_utils.h"

#include "iconnection_ctrl.h"
#include "logger.h"

#include <sstream>
#include <iomanip>

#include <string.h>
// #include <ctime>

namespace Network
{
  
  namespace Proto
  {
    
    namespace Http
    {
      
      HttpResponse::HttpResponse(IConnectionCtrl *ctrl)
        : Ctrl(ctrl)
        , Status(statOK)
        , ContentLength(0)
        , Connection(conClose)
      {
      }
      
      HttpResponse::~HttpResponse()
      {
      }
      
      IConnectionCtrl* HttpResponse::GetCtrl()
      {
        return Ctrl;
      }
      
      bool HttpResponse::Send()
      {
		  // Common::Log::GetLogInst() << "send begin" << std::endl;
        if (Buffer.empty()) {
            return false;
        }
        unsigned Bytes = Buffer.size();
        while (Bytes && Ctrl->SendData(&Buffer[0], &Bytes))
        {
          if (Bytes == Buffer.size())
          {
            Buffer.clear();
            return false;
          }
          Buffer.erase(Buffer.begin(), Buffer.begin() + Bytes);
          Bytes = Buffer.size();
        }
        return true;
      }
      
      void HttpResponse::CreateResponseHeader()
      {
        std::string ResponseHeader = CreateResponseHeader_();
        CharBuffer().swap(Buffer);
        Buffer.resize(ResponseHeader.length());
        memcpy(&Buffer[0], ResponseHeader.c_str(), ResponseHeader.length());
      }
      
      void HttpResponse::SetStatusCode(HttpStatusCode status)
      {
        Status = status;
      }
      
      void HttpResponse::SetContentLength(unsigned lenght)
      {
        ContentLength = lenght;
      }
      
      void HttpResponse::SetConnectionType(ConnectionType connection)
      {
        Connection = connection;
      }
      
      void HttpResponse::SetResourceName(const std::string& resourceName)
      {
        ResourceName = resourceName;
      }
      
      std::string HttpResponse::CreateResponseHeader_() const
      {

        std::string ErrorPage;
        if (Status != statOK)
        {
          ErrorPage = CreateErrorPage(Status, GetStatusDescription(Status));
          ContentLength = ErrorPage.length();
          ResourceName = "error.html";
        }
        std::stringstream Stream;
        Stream
          << HttpVersion << " " << Status << " " << GetStatusDescription(Status) << EndLine
          << DatePrm << " " << GetDateTime() << EndLine
          << ServerPrm << " " << ServerName << EndLine
          << ContentLengthPrm << " " << ContentLength << EndLine
          << ConnectionPrm << " " << GetConnectionType(Connection) << EndLine
          << ContentTypePrm << " " << GetContentType(ResourceName) << EndHeader;
        std::string response = Stream.str();
        if (!ErrorPage.empty()) {
            response += ErrorPage;
        }
        return response;
      }
      
      char const* HttpResponse::GetStatusDescription(HttpStatusCode status)
      {
        switch (status)
        {
        case statOK :
          return "OK";
        case statBadRequest :
          return "Bad Request";
        case statForbidden :
          return "Forbidden";
        case statNotFound :
          return "Not Found";
        case statMethodNotAllowed :
          return "Method Not Allowed";
        case statNotAcceptable :
          return "Not Acceptable";
        case statRequestURITooLong :
          return "Request-URI Too Long";
        case statInternalServerError :
          return "Internal Server Error";
        case statHTTPVersionNotSupported :
          return "HTTP Version Not Supported";
        default :
          break;
        }
        return "Unknown error";
      }
      
      std::string HttpResponse::GetDateTime()
      {
        tm CurDT = { 0 };
        time_t CurTime = time(0);
        gmtime_r(&CurTime, &CurDT);
        std::stringstream Stream;
        Stream
          << Days[CurDT.tm_wday]
          << ", "
          << std::setw(2) << std::setfill('0') << CurDT.tm_mday
          << " "
          << Months[CurDT.tm_mon]
          << " "
          << std::setw(4) << std::setfill('0') << CurDT.tm_year + 1900
          << " "
          << std::setw(2) << std::setfill('0') << CurDT.tm_hour
          << ":"
          << std::setw(2) << std::setfill('0') << CurDT.tm_min
          << ":"
          << std::setw(2) << std::setfill('0') << CurDT.tm_sec
          << " GMT";
        // Stream << std::put_time(&CurDT, "%c %Z");
        return Stream.str();
      }
      
      char const* HttpResponse::GetConnectionType(ConnectionType type)
      {
        switch(type)
        {
        case conClose :
          return "close";
        case conKeepAlive :
          return "keep-alive";
        default :
          break;
        }
        return "";
      }
      
      char const* HttpResponse::GetContentType(std::string const &fileName)
      {
        std::string::size_type Pos = fileName.rfind('.');
        if (Pos != std::string::npos || Pos == fileName.length())
        {
          std::string Type = fileName.substr(Pos + 1);
          for (unsigned i = 0 ; i < RcTypesCount ; ++i)
          {
            if (Type == RcTypes[i].rcType)
              return RcTypes[i].rcHttpType;
          }
        }
        return "*/*";
      }
      
      std::string HttpResponse::CreateErrorPage(HttpStatusCode code, std::string const &description)
      {
        std::stringstream Stream;
        Stream
            <<  "<html>"
            <<    "<title>"
            <<      ServerName
            <<    "</title>"
            <<    "<body>"
            <<      "<h1>"
            <<        static_cast<unsigned>(code) << " " << description
            <<      "</h1>"
            <<    "</body>"
            <<  "</html>";
        return Stream.str();
      }
      
      char const HttpResponse::HttpVersion[] = "HTTP/1.1";
      char const HttpResponse::EndLine[] = "\r\n";
      char const HttpResponse::EndHeader[] = "\r\n\r\n";
      char const *HttpResponse::Days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
      char const *HttpResponse::Months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Oct", "Nov", "Dec"};
      char const HttpResponse::ServerName[] = "MyTestHttpServer";
      char const HttpResponse::DatePrm[] = "Date:";
      char const HttpResponse::ServerPrm[] = "Server:";
      char const HttpResponse::ContentLengthPrm[] = "Content-Length:";
      char const HttpResponse::ConnectionPrm[] = "Connection:";
      char const HttpResponse::ContentTypePrm[] = "Content-Type:";
      
      HttpResponse::ResouurceTypePair const HttpResponse::RcTypes[] =
      {
        {"bin", "application/octet-stream"},
        {"oda", "application/oda"},
        {"pdf", "application/pdf"},
        {"ai", "application/postscript"},
        {"eps", "application/postscript"},
        {"ps", "application/postscript"},
        {"rtf", "application/rtf"},
        {"mif", "application/x-mif"},
        {"csh", "application/x-csh"},
        {"dvi", "application/x-dvi"},
        {"hdf", "application/x-hdf"},
        {"latex", "application/x-latex"},
        {"nc", "application/x-netcdf"},
        {"cdf", "application/x-netcdf"},
        {"sh", "application/x-sh"},
        {"tcl", "application/x-tcl"},
        {"tex", "application/x-tex"},
        {"texinfo", "application/x-texinfo"},
        {"texi", "application/x-texinfo"},
        {"roff", "application/x-troff"},
        {"man", "application/x-troff-man"},
        {"me", "application/x-troff-me"},
        {"ms", "application/x-troff-ms"},
        {"src", "application/x-wais-source"},
        {"zip", "application/zip"},
        {"bcpio", "application/x-bcpio"},
        {"cpio", "application/x-cpio"},
        {"gtar", "application/x-gtar"},
        {"shar", "application/x-shar"},
        {"sv4cpio", "application/x-sv4cpio"},
        {"sv4crc", "application/x-sv4crc"},
        {"tar", "application/x-tar"},
        {"ustar", "application/x-ustar"},
        {"au", "audio/basic"},
        {"snd", "audio/basic"},
        {"aif", "audio/x-aiff"},
        {"aiff", "audio/x-aiff"},
        {"aifc", "audio/x-aiff"},
        {"wav", "audio/x-wav"},
        {"gif", "image/gif"},
        {"ief", "image/ief"},
        {"jpeg", "image/jpeg"},
        {"jpg", "image/jpeg"},
        {"jpe", "image/jpeg"},
		{"png", "image/png"},
		{"svg", "image/svg+xml"},
        {"tiff", "image/tiff"},
        {"tif", "image/tiff"},
        {"ras", "image/x-cmu-raster"},
        {"pnm", "image/x-portable-anymap"},
        {"pbm", "image/x-portable-bitmap"},
        {"pgm", "image/x-portable-graymap"},
        {"ppm", "image/x-portable-pixmap"},
        {"rgb", "image/x-rgb"},
        {"xbm", "image/x-xbitmap"},
        {"xpm", "image/x-xpixmap"},
        {"xwd", "image/x-xwindowdump"},
        {"html", "text/html"},
        {"txt", "text/plain"},
        {"rtx", "text/richtext"},
        {"tsv", "text/tab-separated-values"},
        {"ext", "text/x-setext"},
        {"mpeg", "video/mpeg"},
        {"mpg", "video/mpeg"},
        {"mpe", "video/mpeg"},
        {"qt", "video/quicktime"},
        {"mov", "video/quicktime"},
        {"avi", "video/x-msvideo"},
        {"movie", "video/x-sgi-movie"}
      };
      
      unsigned const HttpResponse::RcTypesCount = Common::SizeOfArray(HttpResponse::RcTypes);
      
    }
    
  }
  
}

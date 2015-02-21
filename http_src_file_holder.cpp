#include "http_src_file_holder.h"
#include "http_status_code.h"
#include "logger.h"
namespace Network
{
  
  namespace Proto
  {
    
    namespace Http
    {	  
      
      HttpSrcFileHolder::HttpSrcFileHolder(const std::string &fileName)
      try
        : System::FileHolder(fileName)
      {
		  // Common::Log::GetLogInst() << "HttpSrcFileHolder " << fileName << std::endl;
      }
      catch (const System::FileHolderException & e)
      {
        throw HttpSrcFileHolderException(e.what(), statNotFound);
      }
      
      int HttpSrcFileHolder::GetHandle() const
      {
        return System::FileHolder::GetHandle();
      }
      
      unsigned HttpSrcFileHolder::GetFileSize() const
      {
        return static_cast<unsigned>(System::FileHolder::GetSize());
      }
      
    }
    
  }
  
}

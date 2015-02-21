#include "file_holder.h"
#include "logger.h"

#include <fcntl.h>
#include <unistd.h>

namespace System
{ 
  
  FileHolder::FileHolder(const std::string &fileName)
    : Handle(-1)
    , Size(0)
  {
    Open(fileName);
  }
  
  FileHolder::~FileHolder()
  {
    if (Handle != -1)
      close(Handle);
  }
  
  void FileHolder::Open(const std::string& fileName)
  {
	// Common::Log::GetLogInst() << "Open file " << fileName << std::endl;
    int NewHandle = open(fileName.c_str(), 0);
    if (NewHandle == -1)
      throw FileHolderException("Failed to open file " + fileName + "\n");
    int NewSize = lseek(NewHandle, 0, SEEK_SET);
    if (NewSize == -1 || (NewSize = lseek(NewHandle, 0, SEEK_END)) == -1 ||
        lseek(NewHandle, 0, SEEK_SET) == -1)
    {
      close(NewHandle);
      throw FileHolderException("Failed to set file position");
    }
    if (Handle != -1)
      close(Handle);
    Handle = NewHandle;
    Size = NewSize;
  }
  
  int FileHolder::GetHandle() const
  {
    return Handle;
  }
  
  int FileHolder::GetSize() const
  {
    return Size;
  }
  
  int FileHolder::Detach()
  {
    int Ret = Handle;
    Handle = -1;
    Size = -1;
    return Ret;
  }
  
}

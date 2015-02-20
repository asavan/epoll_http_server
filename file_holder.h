#ifndef __COMMON_FILE_HOLDER_H__
#define __COMMON_FILE_HOLDER_H__

#include "exceptions.h"
#include "non_copyable.h"

#include <string>

namespace System
{
  
  DECLARE_RUNTIME_EXCEPTION(FileHolder)
  
  class FileHolder
    : private Common::NonCopyable
  {
  public:
    explicit FileHolder(const std::string &fileName);
    virtual ~FileHolder();
    void Open(const std::string &fileName);
    int GetHandle() const;
    int GetSize() const;
    int Detach();
    
  private:
    int Handle;
    int Size;
  };
  
}

#endif  // !__COMMON_FILE_HOLDER_H__

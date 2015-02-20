#ifndef __COMMON_COMMAND_LINE_H__
#define __COMMON_COMMAND_LINE_H__

#include <string>
#include <unordered_map>
#include <sstream>

namespace Common
{

  
  class CommandLine
  {
  public:
    CommandLine(int argc, const char **argv);
    std::string GetProgramName() const;
    std::string GetStrParameter(const std::string& name) const;
    template <typename T>
    T GetParameter(const std::string& name) const
    {
      std::stringstream Stream;
      Stream << GetStrParameter(name);
      T Value = T();
      Stream >> Value;
      return Value;
    }
    
  private:
    std::string ProgramName;
    typedef std::unordered_map<std::string, std::string> ParamPool;
    ParamPool Params;
  };
  
}

#endif  // !__COMMON_COMMAND_LINE_H__

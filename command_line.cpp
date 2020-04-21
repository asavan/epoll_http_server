#include "command_line.h"
#include "exceptions.h"
#include <cstring>

namespace Common
{

  DECLARE_RUNTIME_EXCEPTION(CommandLine)
  
  CommandLine::CommandLine(int argc, char const **argv)
  {
    if (!argc || !argv)
      throw CommandLineException("Invalid input parameters");
    ProgramName = argv[0];
    for (int i = 1 ; i < argc ; ++i)
    {
      char const *Delimiter = strchr(argv[i], ':');
      if (!Delimiter)
        throw CommandLineException("Invalid parameter");
      Params[std::string(argv[i], Delimiter - argv[i])] = Delimiter + 1;
    }
  }
  
  std::string CommandLine::GetProgramName() const
  {
    return ProgramName;
  }
  
  std::string CommandLine::GetStrParameter(std::string const &name) const
  {
    ParamPool::const_iterator Iter = Params.find(name);
    if (Iter == Params.end())
      throw CommandLineException("Parameter not found");
    return Iter->second;
  }

}

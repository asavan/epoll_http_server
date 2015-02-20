#include <string>
#include <iostream>

#include <signal.h>

#include "command_line.h"
#include "inet_address_v4.h"
#include "tcp_server.h"
#include "http_user_session.h"

int main(int argc, const char **argv)
{
  if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
  {
    std::cerr << "Failed to call signal(SIGPIPE, SIG_IGN)" << std::endl;
    return 0;
  }
  try
  {
    char const ServerAddr[] = "Server";
    char const ServerPort[] = "Port";
    char const MaxBacklog[] = "Backlog";
    char const ThreadsCount[] = "Threads";
    char const MaxConnections[] = "Connections";
    char const RootDir[] = "Root";
    char const DefaultPage[] = "DefaultPage";
    char const UseCorking[] = "Corking";
    
    // Server:127.0.0.1 Port:5555 Backlog:1000 Threads:4 Connections:1000 Root:./ DefaultPage:index.html Corking:1
    Common::CommandLine CmdLine(argc, argv);
    
    Network::TCPServer Srv(*Network::InetAddressV4::CreateFromString(
        CmdLine.GetStrParameter(ServerAddr),
        CmdLine.GetParameter<unsigned short>(ServerPort)),
      CmdLine.GetParameter<unsigned>(MaxBacklog),
      CmdLine.GetParameter<unsigned>(ThreadsCount),
      CmdLine.GetParameter<unsigned>(MaxConnections),
      std::bind(&Network::Proto::Http::CreateHttpUserSession,
        CmdLine.GetStrParameter(RootDir),
        CmdLine.GetStrParameter(DefaultPage),
        !!CmdLine.GetParameter<unsigned>(UseCorking)));
    std::cin.get();
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  return 0;
}

#include "tcp_server.h"
#include "tcp_server_socket.h"
#include "selector_thread.h"
#include "logger.h"

#include "client_item.h"

#include "http_user_session.h"

#include <mutex>
#include <map>
#include <iostream>

#include "listen_thread.h"
#include "worker_thread.h"

namespace Network
{
  
  TCPServer::TCPServer(InetAddressPtr locAddr, int backlog, int maxThreadsCount,
	  int maxConnectionsCount, const std::string& rootDir, const std::string& defaultPage, bool useCorking)
  {
	ClientItemQueuePtr AcceptedItems(new ClientItemQueue(backlog));
    int EventsCount = maxConnectionsCount / maxThreadsCount;
    for (int i = 0 ; i < maxThreadsCount ; ++i)
    {
	  int maxEventsCount = EventsCount;
	  // last step
	  if (i == (maxThreadsCount - 1))
	  {
		maxEventsCount += maxConnectionsCount % maxThreadsCount;
	  }
      Threads.push_back(IDisposablePtr(new WorkerThread(maxEventsCount, AcceptedItems)));
    }
	Threads.push_back(IDisposablePtr(new ListenThread(std::move(locAddr), backlog, AcceptedItems, rootDir, defaultPage, useCorking)));
  }

}

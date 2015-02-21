#pragma once

// #include "queueBlock.h"
#include "queue.h"
#include "client_item.h"


#include "tcp_server_socket.h"
#include "selector_thread.h"

namespace Network
{

	// typedef Queue<std::unique_ptr<ClientItem>> ClientItemQueue;
	typedef Queue<ClientItem> ClientItemQueue;
    typedef std::shared_ptr<ClientItemQueue> ClientItemQueuePtr;
  
	class ListenThread:
      public Common::IDisposable, ISelectable
    {
    public:
      ListenThread(InetAddressPtr locAddr, int backlog,
                   ClientItemQueuePtr acceptedClients,
                   const std::string& rootDir, const std::string& defaultPage, bool useCorking);
	  virtual void onSelect(SocketHandle handle, SelectType selectType);      
    private:
		enum { WaitTimeout = 20 };
		TCPServerSocket TCPServerSocket_;
		ClientItemQueuePtr AcceptedClients;
		SelectorThread Selector;
		

		std::string RootDir;
		std::string DefaultPage;
		bool UseCorking;
    };
}
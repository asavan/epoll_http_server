#ifndef __LISTEN_THREAD_H__
#define __LISTEN_THREAD_H__

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
		public ISelectable, public Common::IStartable
    {
    public:
      ListenThread(InetAddressPtr locAddr, int backlog,
                   ClientItemQueuePtr acceptedClients,
                   const std::string& rootDir, const std::string& defaultPage, bool useCorking);
	  virtual void onSelect(SocketHandle handle, SelectType selectType);      
	  virtual void Start();	  
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
#endif // __LISTEN_THREAD_H__

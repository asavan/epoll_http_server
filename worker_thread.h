#pragma once
#include "listen_thread.h"
#include <map>
#include <memory>
namespace Network
{

class WorkerThread
      : private Common::NonCopyable
      , public Common::IDisposable, 
	  public Common::IRunnable,
	  public ISelectable
    {
    public:
      WorkerThread(int maxEventsCount, ClientItemQueuePtr acceptedClients);

	  virtual void onSelect(SocketHandle handle, SelectType selectType);
	  virtual void run();
    private:
      enum { WaitTimeout = 20 };
      
	  // g++ bug. replace shared_ptr to unique_ptr
      typedef std::map<SocketHandle, std::shared_ptr<ClientItem>> ClientPool;
      unsigned MaxConnections;
      ClientItemQueuePtr AcceptedClients;
      ClientPool Clients;
      SelectorThread Selector;
      
      void OnIdle();      
      void RemoveClient(ClientPool::iterator iter);
    };
}
#ifndef __WORKER_THREAD_H__
#define __WORKER_THREAD_H__

#include "listen_thread.h"
#include <map>
#include <memory>
namespace Network
{

class WorkerThread
      : private Common::NonCopyable,
	  public Common::IRunnable,
	  public ISelectable, 
	  public Common::IStartable
    {
    public:
      WorkerThread(int maxEventsCount, ClientItemQueuePtr acceptedClients);

	  virtual void onSelect(SocketHandle handle, SelectType selectType);
	  virtual void run();
	  virtual void Start() 
	  {
		Selector.Start();
	  }
    private:
      enum { WaitTimeout = 20 };
      
	  // g++ bug. replace shared_ptr to unique_ptr
      typedef std::map<SocketHandle, std::shared_ptr<ClientItem>> ClientPool;
      unsigned MaxConnections;
      ClientItemQueuePtr AcceptedClients;
      ClientPool Clients;
      SelectorThread Selector;
      
      void OnIdle();      
      void RemoveClient(ClientPool::const_iterator iter);
    };
}
#endif // __WORKER_THREAD_H__

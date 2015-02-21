#include "tcp_server.h"
#include "tcp_server_socket.h"
#include "selector_thread.h"
#include "logger.h"


#include "queueBlock.h"
#include "client_item.h"

#include "http_user_session.h"

#include <mutex>
#include <map>
#include <iostream>

namespace Network
{

  namespace Private
  {
  
	typedef Queue<std::unique_ptr<ClientItem>> ClientItemQueue;
    typedef std::shared_ptr<ClientItemQueue> ClientItemQueuePtr;
  
    class ListenThread
      : private TCPServerSocket
      , public Common::IDisposable
	  ,public ISelectable
    {
    public:
      ListenThread(InetAddressPtr locAddr, int backlog,
                   ClientItemQueuePtr acceptedClients,
                   const std::string& rootDir, const std::string& defaultPage, bool useCorking)
				   : TCPServerSocket(std::move(locAddr), backlog)
        , AcceptedClients(acceptedClients)
        // , SessionCreator(sessionCreator)
        , Selector(1, WaitTimeout, this),
			RootDir(rootDir), DefaultPage(defaultPage), UseCorking(useCorking)
      {
        Selector.AddSocket(GetHandle(), stRead);
      }

	  void onSelect(SocketHandle handle, SelectType selectType)
      {
        try
        {
          sockaddr Addr = { 0 };
          socklen_t AddrSize = sizeof(Addr);
                    
		  SocketHolderPtr holder = Accept(true, &Addr, &AddrSize);
          ClientItemPtr Client(new ClientItem(std::move(holder),
            Network::Proto::Http::CreateHttpUserSession(RootDir, DefaultPage, UseCorking)));
            
          AcceptedClients->push(std::move(Client));
        }
        catch (const std::exception &e)
        {
          Common::Log::GetLogInst() << e.what() << std::endl;
        }
      }
      
    private:
      enum { WaitTimeout = 20 };
      ClientItemQueuePtr AcceptedClients;
      // UserSessionCreator SessionCreator;
      SelectorThread Selector;

		std::string RootDir;
		std::string DefaultPage;
		bool UseCorking;
      
      
    };
  
    class WorkerThread
      : private Common::NonCopyable
      , public Common::IDisposable, 
	  public Common::IRunnable,
	  public ISelectable
    {
    public:
      WorkerThread(int maxEventsCount, ClientItemQueuePtr acceptedClients)
        : MaxConnections(maxEventsCount)
        , AcceptedClients(acceptedClients)
        , Selector(maxEventsCount, WaitTimeout, this, this)
      {
      }

	  void onSelect(SocketHandle handle, SelectType selectType)
      {
        try
        {
          auto Iter = Clients.find(handle);
		  if (Iter == Clients.end())
		  {
            return;
		  }
          switch (selectType)
          {
          case stRead :
            {
              try
              {
                Iter->second->RecvData();
              }
              catch (...)
              {
                RemoveClient(Iter);
                throw;
              }
            }
            break;
          case stClose :
              RemoveClient(Iter);
            break;
          default :
            break;
          }
        }
        catch (const std::exception &e)
        {
			Common::Log::GetLogInst() << e.what() << std::endl;
        }
      }
	  virtual void run() 
	  {
		OnIdle();
	  }
      
    private:
      enum { WaitTimeout = 20 };
      
	  // g++ bug. replace shared_ptr to unique_ptr
      typedef std::map<SocketHandle, std::shared_ptr<ClientItem>> ClientPool;
      unsigned MaxConnections;
      ClientItemQueuePtr AcceptedClients;
      ClientPool Clients;
      SelectorThread Selector;
      
      
      
      void OnIdle()
      {
        try
        {
          for (ClientPool::iterator i = Clients.begin() ; i != Clients.end() ; )
          {
            if (i->second->CanClose())
			{
              RemoveClient(i++);
			}
            else
            {
              try
              {
                i->second->OnIdle();
				if (i->second->CanClose())
				{
                  RemoveClient(i++);
				}
                else
				{
                  ++i;
				}
              }
              catch (const std::exception &e)
              {
				  Common::Log::GetLogInst() << e.what()<< std::endl;
                  Clients.erase(i++);
				
              }
            }
          }
          
          if (Clients.size() >= MaxConnections)
		  {
			Common::Log::GetLogInst() << "too many clients" << std::endl;
			return;
		  }
          		  	  
          std::unique_ptr<ClientItem> Client = AcceptedClients->pop();
          if (!Client)
		  {
			Common::Log::GetLogInst() << "wrong client" << std::endl;
            return;
		  }
          
          Selector.AddSocket(Client->GetHandle(), stRead | stClose);
		  SocketHandle h = Client->GetHandle();
		  // Clients.emplace(std::make_pair(h, std::move(Client)));
          // Clients[h].reset(Client.release());
		  Clients[h] = std::move(Client);
        }
        catch (const std::exception &e)
        {
			Common::Log::GetLogInst() << e.what() << std::endl;
        }
      }
      
      void RemoveClient(ClientPool::iterator iter)
      {
        try
        {
          Selector.DelSocket(iter->second->GetHandle());
        }
        catch (const std::exception &e)
        {
          Common::Log::GetLogInst() << e.what();
        }
        Clients.erase(iter);
      }
    };

  }
  
  TCPServer::TCPServer(InetAddressPtr locAddr, int backlog, int maxThreadsCount,
	  int maxConnectionsCount, const std::string& rootDir, const std::string& defaultPage, bool useCorking)
  {
	Private::ClientItemQueuePtr AcceptedItems(new Private::ClientItemQueue(backlog));
    int EventsCount = maxConnectionsCount / maxThreadsCount;
    for (int i = 0 ; i < maxThreadsCount ; ++i)
    {
	  int maxEventsCount = EventsCount;
	  // last step
	  if (i == (maxThreadsCount - 1))
	  {
		maxEventsCount += maxConnectionsCount % maxThreadsCount;
	  }
      Threads.push_back(IDisposablePtr(new Private::WorkerThread(maxEventsCount, AcceptedItems)));
    }
	Threads.push_back(IDisposablePtr(new Private::ListenThread(std::move(locAddr), backlog, AcceptedItems, rootDir, defaultPage, useCorking)));
  }

}

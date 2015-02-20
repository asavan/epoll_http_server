#include "tcp_server.h"
#include "tcp_server_socket.h"
#include "selector_thread.h"
#include "logger.h"


#include "queue.h"
#include "client_item.h"
#include <mutex>


#include <map>

// #include <assert.h>

namespace Network
{

  namespace Private
  {
  
	typedef Queue<ClientItem> ClientItemQueue;
    typedef std::shared_ptr<ClientItemQueue> ClientItemQueuePtr;
  
    class ListenThread
      : private TCPServerSocket
      , public Common::IDisposable
    {
    public:
      ListenThread(const InetAddress &locAddr, int backlog,
                   ClientItemQueuePtr acceptedClients,
                   UserSessionCreator sessionCreator)
        : TCPServerSocket(locAddr, backlog)
        , AcceptedClients(acceptedClients)
        , SessionCreator(sessionCreator)
        , Selector(1, WaitTimeout, std::bind(&ListenThread::OnSelect,
            this, std::placeholders::_1, std::placeholders::_2))
      {
        Selector.AddSocket(GetHandle(), Network::ISelector::stRead);
      }
      
    private:
      enum { WaitTimeout = 20 };
      ClientItemQueuePtr AcceptedClients;
      UserSessionCreator SessionCreator;
      SelectorThread Selector;
      
      void OnSelect(SocketHandle handle, Network::ISelector::SelectType selectType)
      {
        try
        {
          
          sockaddr Addr = { 0 };
          socklen_t AddrSize = sizeof(Addr);
                    
		  SocketHolderPtr holder = Accept(true, &Addr, &AddrSize);
          ClientItemPtr Client(new ClientItem(std::move(holder),
            InetAddress::CreateFromSockAddr(&Addr, AddrSize),
            SessionCreator()));
  
          if (!AcceptedClients)
            throw std::invalid_argument("Empty destQueue pointer");
          
          AcceptedClients->Push(std::move(Client));
        }
        catch (const std::exception &e)
        {
          Common::Log::GetLogInst() << e.what();
        }
      }
    };
  
    class WorkerThread
      : private Common::NonCopyable
      , public Common::IDisposable
    {
    public:
      WorkerThread(int maxEventsCount, ClientItemQueuePtr acceptedClients)
        : MaxConnections(maxEventsCount)
        , AcceptedClients(acceptedClients)
        , Selector(maxEventsCount, WaitTimeout, std::bind(&WorkerThread::OnSelect,
              this, std::placeholders::_1, std::placeholders::_2),
            SelectorThread::ThreadFunctionPtr(new SelectorThread::ThreadFunction(std::bind(
              &WorkerThread::OnIdle, this))))
      {
      }
      
    private:
      enum { WaitTimeout = 20 };
      
	  // g++ bug. replace shared_ptr to unique_ptr
      typedef std::map<SocketHandle, std::shared_ptr<ClientItem>> ClientPool;
      unsigned MaxConnections;
      ClientItemQueuePtr AcceptedClients;
      ClientPool Clients;
      SelectorThread Selector;
      
      void OnSelect(SocketHandle handle, Network::ISelector::SelectType selectType)
      {
        try
        {
          ClientPool::iterator Iter = Clients.find(handle);
          if (Iter == Clients.end())
            return;
          switch (selectType)
          {
          case Network::ISelector::stRead :
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
            return;
          case Network::ISelector::stClose :
              RemoveClient(Iter);
            return;
          default :
            break;
          }
        }
        catch (const std::exception &e)
        {
          Common::Log::GetLogInst() << e.what();
        }
      }
      
      void OnIdle()
      {
        try
        {
          for (ClientPool::iterator i = Clients.begin() ; i != Clients.end() ; )
          {
            if (i->second->CanClose())
              RemoveClient(i++);
            else
            {
              try
              {
                i->second->OnIdle();
                if (i->second->CanClose())
                  RemoveClient(i++);
                else
                  ++i;
              }
              catch (std::exception const &e)
              {
                Clients.erase(i++);
                Common::Log::GetLogInst() << e.what();
              }
            }
          }
          
          if (Clients.size() >= MaxConnections)
            return;
          
		  AcceptedClients->Pop();
		  
          std::unique_ptr<ClientItem> Client = AcceptedClients->Pop();
          if (!Client)
            return;
          
          Selector.AddSocket(Client->GetHandle(), Network::ISelector::stRead | Network::ISelector::stClose);
		  SocketHandle h = Client->GetHandle();
		  // Clients.emplace(std::make_pair(h, std::move(Client)));
          // Clients[h].reset(Client.release());
		  Clients[h] = std::move(Client);
        }
        catch (const std::exception &e)
        {
          Common::Log::GetLogInst() << e.what();
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
  
  TCPServer::TCPServer(const InetAddress &locAddr, int backlog, int maxThreadsCount,
            int maxConnectionsCount, UserSessionCreator sessionCreator)   
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
    Threads.push_back(IDisposablePtr(new Private::ListenThread(locAddr, backlog, AcceptedItems, sessionCreator)));
  }

}

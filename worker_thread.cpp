
#include "worker_thread.h"
#include "logger.h"
namespace Network
{

      WorkerThread::WorkerThread(int maxEventsCount, ClientItemQueuePtr acceptedClients)
        : MaxConnections(maxEventsCount)
        , AcceptedClients(acceptedClients)
        , Selector(maxEventsCount, WaitTimeout, this, this)
      {
      }

	  void WorkerThread::onSelect(SocketHandle handle, SelectType selectType)
      {
		  Common::Log::GetLogInst() << "onSelect WorkerThread " << std::this_thread::get_id() << std::endl;
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
			Common::Log::GetLogInst() << __LINE__ << e.what() << std::endl;
        }
      }
	  void WorkerThread::run() 
	  {
		OnIdle();
	  }
      void WorkerThread::OnIdle()
      {
		  //Common::Log::GetLogInst() << "OnIdle " << Clients.size() << " " << std::this_thread::get_id() << std::endl;
        try
        {
          for (ClientPool::iterator i = Clients.begin() ; i != Clients.end() ; )
          {
			  Common::Log::GetLogInst() << "iterator " << i->first << " "<<std::this_thread::get_id() << std::endl;
            if (i->second->CanClose())
			{
              RemoveClient(i++);
			}
            else
            {
              try
              {
				  Common::Log::GetLogInst() << "iterator idle" << i->first << " "<<std::this_thread::get_id() << std::endl;
                i->second->OnIdle();
				Common::Log::GetLogInst() << "iterator idle end" << i->first << " "<<std::this_thread::get_id() << std::endl;
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
				  Common::Log::GetLogInst() << __LINE__ << e.what()<< std::endl;
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
		  Common::Log::GetLogInst() << "pop " << Client->GetHandle()<< " " << std::this_thread::get_id() << std::endl;
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
		  Common::Log::GetLogInst() << "moved to list " << h << " "<<std::this_thread::get_id() << std::endl;
        }
        catch (const std::exception &e)
        {
			Common::Log::GetLogInst() << e.what() << std::endl;
        }
      }
      
      void WorkerThread::RemoveClient(ClientPool::iterator iter)
      {
        try
        {
          Selector.DelSocket(iter->second->GetHandle());
        }
        catch (const std::exception &e)
        {
			Common::Log::GetLogInst() << e.what() << std::endl;
        }
        Clients.erase(iter);
      }
}
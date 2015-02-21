#include "listen_thread.h"

#include "logger.h"
#include "http_user_session.h"


namespace Network
{

ListenThread::ListenThread(InetAddressPtr locAddr, int backlog,
                   ClientItemQueuePtr acceptedClients,
                   const std::string& rootDir, const std::string& defaultPage, bool useCorking)
				   : TCPServerSocket_(std::move(locAddr), backlog)
        , AcceptedClients(acceptedClients)
        , Selector(1, WaitTimeout, this),
			RootDir(rootDir), DefaultPage(defaultPage), UseCorking(useCorking)
      {
        Selector.AddSocket(TCPServerSocket_.GetHandle(), stRead);
      }

void ListenThread::Start() 
	  {
		Selector.Start();
	  }

void ListenThread::onSelect(SocketHandle /*handle*/, SelectType /*selectType*/)
{
	// Common::Log::GetLogInst() << "onSelect ListenThread " << std::this_thread::get_id() << std::endl;
	try
	{
		sockaddr Addr = { 0 };
		socklen_t AddrSize = sizeof(Addr);		        
		AcceptedClients->push(std::unique_ptr<ClientItem>(new ClientItem(TCPServerSocket_.Accept(true, &Addr, &AddrSize),
		Network::Proto::Http::CreateHttpUserSession(RootDir, DefaultPage, UseCorking))));

		// Common::Log::GetLogInst() << "push " << Client->GetHandle() << " " << std::this_thread::get_id() << std::endl;
		// std::move(Client));
		// Common::Log::GetLogInst() << "push " << std::this_thread::get_id() << std::endl;
	}
	catch (const std::exception &e)
	{
		Common::Log::GetLogInst() << __LINE__ << e.what() << std::endl;
	}
}
}
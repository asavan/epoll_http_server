#include "logger.h"
#include "client_item.h"
#include "io_helper.h"

namespace Network
{

  ClientItem::ClientItem(SocketHolderPtr holder, IUserSessionPtrU userSession)
    :
	  holder_(std::move(holder))
    , SessionLastActionTime(std::time(0))
    , MarkedForClose(false)
	, UserSession(std::move(userSession))
    , BufferSize(UserSession->GetMaxBufSizeForRead())
    , RecvBuffer(new char [BufferSize])
  {
    UserSession->Init(this);
  }    
  
  bool ClientItem::CanClose() const
  {
    return MarkedForClose;
  }
  
  void ClientItem::RecvData()
  {
    if (MarkedForClose)
      return;
    IOHelper IO(holder_->GetHandle());
    for (unsigned Bytes = BufferSize ; IO.Read(RecvBuffer.get(), &Bytes) ; Bytes = BufferSize)
      UserSession->OnRecvData(RecvBuffer.get(), Bytes);
  }
  
  void ClientItem::OnIdle()
  {
    if (!MarkedForClose)
	{
      MarkedForClose = UserSession->IsExpiredSession(SessionLastActionTime);
	}
    if (MarkedForClose)
	{
      return;
	}
    UserSession->OnIdle();
  }
  
  void ClientItem::MarkMeForClose()
  {
    MarkedForClose = true;
  }
  
  void ClientItem::UpdateSessionTime()
  {
    SessionLastActionTime = std::time(0);
  }
  
  bool ClientItem::SendData(void const *buf, unsigned *bytes)
  {
    IOHelper IO(GetHandle());
    return IO.Write(buf, bytes);
  }
  
  SocketHandle ClientItem::GetHandle() const
  {
	return holder_->GetHandle();
  }
  
  bool ClientItem::SendFile(int fileHandle, unsigned offset, unsigned *bytes)
  {
    IOHelper IO(GetHandle());
    return IO.SendFile(fileHandle, offset, bytes);
  }
  
  SocketTuner ClientItem::GetSocketTuner() const
  {
    return SocketTuner(GetHandle());
  }

}

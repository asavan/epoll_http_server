#include "client_item.h"
#include "io_helper.h"

namespace Network
{

  ClientItem::ClientItem(SocketHolderPtr holder, InetAddressPtr addr, IUserSessionPtr userSession)
    : addr_(std::move(addr)),
	  holder_(std::move(holder))
    , SessionLastActionTime(std::time(0))
    , MarkedForClose(false)
    , UserSession(userSession)
    , BufferSize(userSession->GetMaxBufSizeForRead())
    , RecvBuffer(new char [BufferSize])
  {
    UserSession->Init(this);
  }
  
  ClientItem::~ClientItem()
  {
    UserSession->Done();
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
      MarkedForClose = UserSession->IsExpiredSession(SessionLastActionTime);
    if (MarkedForClose)
      return;
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
  
  InetAddress const& ClientItem::GetAddress() const
  {
    return *addr_;
  }
  
  SocketTuner ClientItem::GetSocketTuner() const
  {
    return SocketTuner(GetHandle());
  }

}

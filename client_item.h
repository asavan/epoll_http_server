#ifndef __COMMON_CLIENT_ITEM_H__
#define __COMMON_CLIENT_ITEM_H__

#include "socket_holder.h"
#include "iconnection_ctrl.h"
#include "iuser_session.h"

#include "non_copyable.h"

#include <ctime>

#include <memory>

namespace Network
{

  class ClientItem
    : public IConnectionCtrl,
	private Common::NonCopyable
  {
  public:
    ClientItem(SocketHolderPtr holder, IUserSessionPtrU userSession);
    
    
    bool CanClose() const;
    void RecvData();
    void OnIdle();
	
	SocketHandle GetHandle() const;
    
    // IConnectionCtrl
    void MarkMeForClose() override;
    void UpdateSessionTime() override;
    bool SendData(void const *buf, unsigned *bytes) override;
    bool SendFile(int fileHandle, unsigned offset, unsigned *bytes) override;
    SocketTuner GetSocketTuner() const override;
    
  private:
	SocketHolderPtr holder_;
    std::time_t SessionLastActionTime;
    bool MarkedForClose;
    IUserSessionPtrU UserSession;
    unsigned BufferSize;
    std::unique_ptr<char> RecvBuffer;
  };
  
  typedef std::unique_ptr<ClientItem> ClientItemPtr;

}

#endif  // !__COMMON_CLIENT_ITEM_H__
